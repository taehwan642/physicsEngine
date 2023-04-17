#version 410 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout(location=0) out vec2 texcoord;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0);
    texcoord=vec2(vertexPosition.x,vertexPosition.y)*0.5+0.5;
}