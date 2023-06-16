#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTex;

// uniform float x;
// uniform float y;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main(){
    // aPos.x = x;
    // aPos.y = y;
    // vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z);
    // vec3 newPos = vec3(aPos.x, aPos.y, aPos.z);
	gl_Position = projection * view * transform * vec4(aPos, 1.0);
    texCoord = aTex;
}