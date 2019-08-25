#version 450 core

layout (location = 0) in vec4 vertexCoord;
layout (location = 1) in vec4 vertexColor;
//layout (location = 2) in vec4 vertexNormal;
//layout (location = 3) in vec4 texCoord;

layout(set = 0, binding = 0) uniform Transforms {
    mat4 modelTransform;
    mat4 viewTransform;
    mat4 projectionTransform;
};


layout (location = 0) out vec4 fragmentColor;

void main(){
    gl_Position = vertexCoord;
    fragmentColor = vertexColor;
}