#version 150

in vec4 v_position;
in vec4 v_color;

uniform mat4 v_mv;
uniform mat4 v_mvp;
uniform int is_text;

mat4 scale = mat4(0.08, 0.00, 0.00, 0.00,
                  0.00, -0.08, 0.00, 0.00,
                  0.00, 0.00, 1.00, 0.00,
                  0.00, 0.00, 0.00, 1.00);

mat4 transition = mat4(1.00, 0.00, 0.00, 20.00,
                      0.00, 1.00, 0.00, 20.00,
                      0.00, 0.00, 1.00, 0.00,
                      0.00, 0.00, 0.00, 1.00);

out vec4 color;

void main() {
    if (is_text == 1) {
        gl_Position = v_mvp * transition * scale * v_position;
        color = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        gl_Position = v_mvp * v_position;
        color = v_color;
    }
}
