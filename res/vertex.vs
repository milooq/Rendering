#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 MVP;
uniform vec3 clr;
out vec3 myClr;

void main()
{   //MVP*transform*
    gl_Position = MVP*vec4(position, 1.0);
    //gl_Position = vec4(position, 1.0);
    myClr = vec3(0.5)+clr;
}