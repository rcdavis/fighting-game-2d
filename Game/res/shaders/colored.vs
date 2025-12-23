#version 330 core

in vec3 a_Position;
in vec4 a_Color;
in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main() {
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
