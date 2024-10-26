#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform float time;
uniform sampler2D ourTexture;

void main() {
    // Multiply by vec2(1, -1) to flip the image the right way up
    vec2 texColor = TexCoord * vec2(1, -1);
    vec2 tanArg = texColor * texColor * texColor;
    FragColor = texture(ourTexture, sin(tan(tanArg)));
}