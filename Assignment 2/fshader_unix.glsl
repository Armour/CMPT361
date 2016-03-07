#version 130

in  vec4 color;
out vec4 f_color;

void main() {
    if (color.a < 1.0) {
        discard;
    }
    f_color = color;
}

