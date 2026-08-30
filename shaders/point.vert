#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aSize;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool glow;

out vec3 vertexColor;

void main(){
    vec4 viewPosition = view * model * vec4(aPos, 1.0);

    gl_Position = projection * viewPosition;

    vertexColor = aColor;

    float pointSize = - aSize / viewPosition.z * 500.0;

    if(glow){
        pointSize *= 3;
    }

    gl_PointSize = pointSize;
}
