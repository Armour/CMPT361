#version 130

in vec2 tex_coord;

out vec4 f_color;

uniform sampler2D texture;

void main() {
    f_color = texture2D(texture, tex_coord);
}

