#version 130

in vec4 v_position;
in vec4 v_color;
out vec4 color;

uniform int size_x;
uniform int size_y;

void main() {
    mat4 scale = mat4(2.0/size_x, 0.0, 0.0, 0.0,
                      0.0, 2.0/size_y, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0 );

    // First, center the image by translating each vertex by half of the original window size
    // Then, multiply by the scale matrix to maintain size after the window is reshaped
    vec4 new_pos = v_position + vec4(-200, -360, 0, 0);
    gl_Position = scale * new_pos;

    color = v_color;
}
