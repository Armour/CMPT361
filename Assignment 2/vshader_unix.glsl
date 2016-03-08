#version 130

in vec4 v_position;
in vec4 v_color;

uniform mat4 v_mv;
uniform mat4 v_mvp;

out vec4 color;

void main() {
    gl_Position = v_mvp * v_position;
    color = v_color;
}
