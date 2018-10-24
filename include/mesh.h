#ifndef MESH_H
#define MESH_H
#include "glad/glad.h"
#include <vector>
class mesh{
    protected:
        //buffers
        GLuint VBO,VAO,EBI;
        //indicies array size
        GLuint size;
        //primitive type
        GLenum typeprim;
    public:
        //mesh constructor
        mesh(const std::vector<GLdouble>& pVertices,const std::vector<GLuint>& pIndices,const GLenum& pItypeprim);

        ~mesh(){};

        void clear(void){
            glDeleteBuffers(1,&EBI);
            glDeleteBuffers(1,&VBO);
            glDeleteVertexArrays(1,&VAO);
        }

        //draw function
        void draw(void);     
};

mesh::mesh(const std::vector<GLdouble>& pVertices, const std::vector<GLuint>& pIndices, const GLenum& pItypeprim): 
size(pIndices.size()), typeprim(pItypeprim) {

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, pVertices.size() * sizeof(GLdouble), &pVertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &EBI);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBI);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pIndices.size() * sizeof(GLuint), &pIndices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
};

void mesh::draw(void){
    glBindVertexArray(VAO);
    glDrawElements(typeprim, size, GL_UNSIGNED_INT, (void*)0);
}
#endif 