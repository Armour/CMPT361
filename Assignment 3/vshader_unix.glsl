#version 130

in vec4 v_position;
in vec2 v_tex_coord;

out vec2 tex_coord;

void main() {
    gl_Position = v_position;
    tex_coord = v_tex_coord;
}
