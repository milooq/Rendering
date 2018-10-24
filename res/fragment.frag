#version 330 core
layout(location = 0) out vec4 color; 
in vec3 myClr;  
void main()
{
color = vec4(myClr, 1.0);
//color = vec4(1.0,1.0,1.0,1.0);
}