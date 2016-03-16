#version 150

in vec2 tex_coord;

out vec4 f_color;

uniform sampler2D texture;

void main() {
    f_color = texture(texture, tex_coord);
}

