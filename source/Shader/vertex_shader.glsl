#version 410 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 0) out vec3 fragmentColor;

uniform mat4 transform;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0) * transform;
    fragmentColor = vec3(1, 1, 0);
}