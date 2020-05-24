#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 normal;
in vec2 fragTexCoords;

uniform vec4 ambientLight;
uniform vec4 diffuseLight;
uniform vec3 light1Pos;
uniform vec3 light2Pos;

uniform sampler2D ourTexture;

void main() {
    vec3 norm = normalize(normal);
    vec3 normLight1Pos = normalize(light1Pos);
    vec3 normLight2Pos = normalize(light2Pos);

    float diff1 = max(dot(norm, normLight1Pos), 0.0);
    float diff2 = max(dot(norm, normLight2Pos), 0.0);

    vec4 diffuse1 = diff1 * diffuseLight;
    vec4 diffuse2 = diff2 * diffuseLight;

    FragColor = texture(ourTexture, fragTexCoords) * vec4(ourColor, 1.0) * (ambientLight + diffuse1 + diffuse2);
}
