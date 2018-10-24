#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class camera{
    //camera movement
    private:
        //default position and view and up vectors
        glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  1.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
        bool firstMove = true;
    public:
        GLfloat speed = 25.0f;
        void move(const bool* keys, GLfloat deltaTime);

    //camera rotation
    private:
        //  Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction
        //  vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
        GLfloat yaw   = -90.0f;	
        GLfloat pitch =   0.0f;
        GLfloat lastMouseX, lastMouseY;
    public:
        GLfloat sensitivity = 0.05;
        void rotate(const double& currentMouseX,const double& currentMouseY);
        //when it's first move we wnat to avoid harsh camera move so we set out last positions to current
        //call it in the mouse callback fuction, remember it's called once automatically, so you don't have to wrap it with 'if'
        void rotateInit(const double& currentMouseX,const double& currentMouseY){
            if(firstMove) {
                lastMouseX = currentMouseX;
                lastMouseY = currentMouseY;
                firstMove = false;
            }
        }
        //get complete view matrix
        const glm::mat4 view(){return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);}
        
};

void camera::move(const bool* keys, GLfloat deltaTime){
    //set default speed
    GLfloat cameraSpeed = speed*deltaTime;
    //twice it if shift is pressed
    if(keys[GLFW_KEY_LEFT_SHIFT]){
        cameraSpeed = 2*speed*deltaTime;
    }

    //increase/decrease speed
    if(keys[GLFW_KEY_KP_ADD]){
        speed+=0.1f;
        std::cout<<"Speed is: "<<speed<<std::endl;
    }
    if(keys[GLFW_KEY_KP_SUBTRACT]){
        speed-=0.1f;
        std::cout<<"Speed is: "<<speed<<std::endl;
    }

    //change position
    if(keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if(keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if(keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void camera::rotate(const double& currentMouseX,const double& currentMouseY){
    GLfloat xoffset = currentMouseX - lastMouseX;
    GLfloat yoffset = lastMouseY - currentMouseY;

    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    //Euler angles
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
#endif