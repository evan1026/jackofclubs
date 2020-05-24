#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 texCoords;

uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec3 normal;
out vec2 fragTexCoords;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);

    ourColor = aColor;
    normal = aNormal;
    fragTexCoords = texCoords;
}
