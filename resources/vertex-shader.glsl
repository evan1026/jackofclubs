#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal; // idk what to do with this yet but hey it makes things easier

uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    ourColor = aColor;
}
