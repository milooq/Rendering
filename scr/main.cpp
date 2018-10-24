// STD headers
#include <iostream>
#include <string>
#include <vector>

#include <experimental/filesystem>
#include <algorithm>

// glad
#include <glad/glad.h>

//  GLFw
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// self-written headers
#include "mesh.h"
#include "csvparse.h"
#include "shader.h"
#include "camera.h"
#include "axis.h"



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//error handling
void GLAPIENTRY
MessageCallback( GLenum source,GLenum type,GLuint id,GLenum severity,
GLsizei length,const GLchar* message,const void* userParam );

const GLuint WIDTH = 900, HEIGHT = 900;
//make camera global to init it in mouse callback
camera myCamera;

//time between frames
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

    //array of keys, 1 if pressed, 0 otherwise, allows to process arbitrary number of keys at the same time
    bool keys[1024];
    //last mouse positions
    glm::vec2 mouseScreenPos;

int main()
{   
    /* Collect any mandatory info about objects to draw, essencially creating verticies and indicies arrays.
    Objects themselves are declared and instantiated later after OpenGL content is loaded */

    std::string path;
    std::vector<std::string> filenames;

    std::cout<<"Enter folder name"<<std::endl;
    getline(std::cin,path);

    if(!std::experimental::filesystem::exists(path)) {
        std::cout << "Iterator cannot open directory: No such file or directory"
        << "[" << path << "]" << std::endl;
    }else{
        for (auto & p : std::experimental::filesystem::directory_iterator(path)) {
            filenames.push_back(p.path());
        }
    }
    // sort file list
    std::sort(filenames.begin(),filenames.end());

    // open the first file to deduce number of objects to draw
    CSVfile file(filenames[0]);
    
    // create a vector of vectors and then allocate memory for each sub-vector
    std::vector< std::vector<GLdouble> > verticesBuffer(file.height(),std::vector<GLdouble>());
    std::vector<GLuint> indicesBuffer;
 
    // find the largest coorinate(by abs value) to squeeze the scale
    float max = 0;

    //  loop through all files using earlier created CSVfile file
    //  fill verticesBuffer
    for(unsigned int i=0; i<filenames.size(); i++) {
        file.open(filenames[i]);
        // rows
        for(unsigned int r=0; r<file.height(); r++) {
            // for the first loop we pre-reserve space for our coordinate to increase performance
            // size is based on number of csv files - frames therefore
            if(i == 0) {
                verticesBuffer[i].reserve(filenames.size());
            };

            // columns
            for(unsigned int c=0; c<file.width(r); c++) {
                // deducing the maximum
                if(abs(std::stof(file[r][c])) > max) {
                    max = abs(std::stof(file[r][c]));
                }
                verticesBuffer[r].push_back(std::stod(file[r][c])); 
            }
        }
    }
    // fill indicesBuffer simply with natural number till a third of verticesBuffer size
    for(GLuint i = 0; i<verticesBuffer[0].size()/3; i++){
        indicesBuffer.push_back(i);
    };

    // init GLFW
    glfwInit();
    //Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "STARS ༼ つ ◕_◕ ༽つ", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    // mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // initialize glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0 );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE); 

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    std::vector<mesh> meshes;
    // squeeze the scale untill it's inbetween -10^3 and 10e3
    for(unsigned int it=0; it<verticesBuffer.size();it++){
        for(unsigned int ij=0; ij<verticesBuffer[it].size();ij++){
            verticesBuffer[it][ij]/=pow(10,(int)log10(max)-2);
        }
        // fill meshes vector
        meshes.emplace_back(mesh(verticesBuffer[it],indicesBuffer,GL_POINTS));
    }

    std::vector<GLdouble> vert = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    std::vector<GLuint> ind;
    GLuint indC = 0;
    for(auto x:vert){ 
    ind.push_back(indC);
    ++indC;
    }

    mesh cube(vert,ind,GL_TRIANGLES);

    axis axises(1000.0f);

    shader myShader("../res/vertex.vs","../res/fragment.frag");
    myShader.use();

    glm::mat4 projection = glm::perspective( glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.01f, 10000.0f);

    GLuint MVPID = glGetUniformLocation(myShader.program, "MVP");
    GLuint FlagID = glGetUniformLocation(myShader.program, "clr");

// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<float> colors;

    for(auto it: meshes){
        for(int j = 0; j<3; j++)
        colors.push_back(0.5+sin(rand())/2);
    }
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {    
        //  check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        // doMove();
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        myCamera.move(keys,deltaTime);
        myCamera.rotate(mouseScreenPos.x,mouseScreenPos.y);

        glClearColor(0.15f,0.15f,0.15f,0.15f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glm::mat4 MVP = projection*myCamera.view();

        glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
        
        for(unsigned int i=0; i<meshes.size(); i++) {
            glUniform3f(FlagID,colors[3*i],colors[3*i+1],colors[3*i+2]);
            meshes[i].draw();
        }
        glUniform3f(FlagID,1.0f,1.0f,1.0f);
        cube.draw();
        axises.draw(myShader.program,"clr");
        glfwSwapBuffers(window);

    }

    // free resources
    myShader.clear();

    cube.clear();
    axises.clear();
    for(auto x: meshes){
        x.clear();
    };
    
    glfwTerminate();
    return 0;
}

//error callback
void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity,
 GLsizei length, const GLchar* message, const void* userParam ) {
    if(type != 0x8251)
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, GL_TRUE);

    //set key status pressed if it is
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    myCamera.rotateInit(xpos,ypos);
    mouseScreenPos.x = xpos;
    mouseScreenPos.y = ypos;
}
