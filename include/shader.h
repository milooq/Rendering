#ifndef SHADER_H
#define SHADER_H
    //gl functions
    #include "glad/glad.h"
    //other stuff
    #include <iostream>
    #include <sstream>
    #include <fstream>
    #include <string>

    class shader {
        public:
            //program id
            GLuint program;
            //program use
            void use(void);
            //program delete
            void clear(){
                glDeleteProgram(this->program);
            }
            //shader constructor
            shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

        private:
            void checkCompileErrors(GLuint pShader, std::string type);
    };

    shader::shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "") {

        //1) Retrieve shaders sources

        //source code and streams variables
        std::string vertexSource, fragmentSource, geometrySource;
        std::ifstream vertexFile, fragmentFile, geometryFile;

        //ensure ifstream throw exceptions
        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


        try {
            //open files
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);

            //create string streams
            std::stringstream vertexStream, fragmentStream;

            //read data into string streams
            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            //close file handlers
            vertexFile.close();
            fragmentFile.close();

            //convert streams into strings
            vertexSource = vertexStream.str();
            fragmentSource = fragmentStream.str();

            //same for geometry shader, if it exists
            if(geometryPath != "") {
                geometryFile.open(geometryPath);
                std::stringstream geometryStream;
                geometryStream << geometryFile.rdbuf();
                geometryFile.close();
                geometrySource = geometryStream.str();
            };
        }
        catch(std::ifstream::failure& e) {
            std::cout<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"<< std::endl;
        };

        // 2) Compile shaders
        //c string sources
        const GLchar* vShaderCode = vertexSource.c_str();
        const GLchar* fShaderCode = fragmentSource.c_str();

        GLuint vertex, fragment;

        //Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex,"VERTEX");

        //Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment,"FRAGMENT");

        //Geometry shader, if it exists
        GLuint geometry = 0;
        if(geometryPath != "") {
            const GLchar* gShaderCode = geometrySource.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry,"GEOMETRY");
        }

        //shader program
        this->program = glCreateProgram();
        glAttachShader(this->program,vertex);
        glAttachShader(this->program,fragment);
        if(geometryPath != "") { glAttachShader(this->program,geometry); }

        glLinkProgram(this->program);
        checkCompileErrors(this->program,"PROGRAM");

        //delete shaders after they are linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryPath != "") { glDeleteShader(geometry); }

    }

    //error check function that ensure 
    //that shader was linked or compiled properly
    void shader::checkCompileErrors(GLuint target, std::string type) {
        GLint succsess;
        GLchar errorLog[512];
        if(type == "PROGRAM") {
             //check program linking status
            glGetProgramiv(target,GL_LINK_STATUS,&succsess);
            if(!succsess) {
                glGetProgramInfoLog(target, 512, NULL, errorLog);
                std::cout<< "ERROR::SHADER::LINKING_ERROR \n"
                << "Type: " << type
                << errorLog << std::endl;
            }
        } else {
              //check shader compile status
            glGetShaderiv(target,GL_COMPILE_STATUS,&succsess);
            if(!succsess) {
                glGetShaderInfoLog(target, 512, NULL, errorLog);
                std::cout<< "ERROR::SHADER::COMPILE_ERROR \n"
                << "Type: " << type
                << errorLog << std::endl;
            }
        }
    }

    void shader::use(void){ glUseProgram(this->program); }

#endif