#version 430 core

layout(location = 0) in vec3 vp;

out vec3 vPosition;

void main () {
    vPosition = vp;
}