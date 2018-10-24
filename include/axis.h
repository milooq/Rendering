#ifndef AXIS_H
#define AXIS_H
#include "mesh.h"
#include "string"

class axis{
    public:
        axis(float size):
        xAxis({0.0f,0.0f,0.0f,size,0.0f,0.0f}, {0,1}, GL_LINES),
        yAxis({0.0f,0.0f,0.0f,0.0f,size,0.0f}, {0,1}, GL_LINES),
        zAxis({0.0f,0.0f,0.0f,0.0f,0.0f,size}, {0,1}, GL_LINES){};
        
        void draw(GLuint program, std::string uniformName){
            GLuint ColorsID = glGetUniformLocation(program, uniformName.c_str());
            glUniform3f(ColorsID,1.0f,0.0f,0.0f);
            this->xAxis.draw();
            glUniform3f(ColorsID,0.0f,1.0f,0.0f);
            this->yAxis.draw();
            glUniform3f(ColorsID,0.0f,0.0f,1.0f);
            this->zAxis.draw();
        };

        void clear(void) {
            xAxis.clear();
            yAxis.clear();
            zAxis.clear();
        }
    protected:
        GLfloat verticies;
        GLbyte indicies;
        mesh xAxis,yAxis,zAxis;
};

#endif