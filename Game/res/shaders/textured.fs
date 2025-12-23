#version 330 core

in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;

uniform sampler2D u_Textures[32];

out vec4 finalColor;

void main() {
    vec4 color = texture(u_Textures[v_TexIndex], v_TexCoord) * v_Color;
    if (color.a < 0.1)
        discard;

    finalColor = color;
}
