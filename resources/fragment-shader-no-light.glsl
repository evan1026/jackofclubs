#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 normal; // Not used but included to be compatible with vertex shader
in vec2 fragTexCoords; // Not used but included to be compatible with vertex shader

void main() {
    FragColor = vec4(ourColor, 1.0);
}
