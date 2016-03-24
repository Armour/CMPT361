//////////////////////////////////////////////////////////////////////////////
//
//  raycast.cc
//
//  CMPT 361 Assignment 3 - Render a simple scene using ray tracing
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Some portions of the code was originally written by
//  M. vandePanne - and then modified by R. Zhang & H. Li
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_util.h"
#include "init_shader.h"
#include "scene.h"
#include "trace.h"

// The array for the final image
GLfloat frame[libconsts::kWindowSizeHeight][libconsts::kWindowSizeWidth][3];

// Background color
glm::vec3 background_color;

// List of spheres in the scene
raychess::Sphere *scene = NULL;

// Light position and color
glm::vec3 light;
glm::vec3 light_ambient;
glm::vec3 light_diffuse;
glm::vec3 light_specular;

// Global ambient term
glm::vec3 global_ambient;

// Light decay parameters
float decay_a;
float decay_b;
float decay_c;

// Maximum level of recursions of reflection
int step_max = 1;

// The option flags
int shadow_on = 0;
int reflection_on = 0;
int chess_board_on = 0;
int refraction_on = 0;
int diffuse_on = 0;
int antialiasing_on = 0;

//
// Function: Init
// ---------------------------
//
//   Init OpenGL lighting, materials, shading and VAO, VBO
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void init() {
    // Vertices of a square
    double ext = 1.0;
    glm::vec4 points[6] = {
        glm::vec4(-ext, -ext,  0, 1.0), // V1
        glm::vec4( ext, -ext,  0, 1.0), // V2
        glm::vec4(-ext,  ext,  0, 1.0), // V3
        glm::vec4(-ext,  ext,  0, 1.0), // V3
        glm::vec4( ext, -ext,  0, 1.0), // V2
        glm::vec4( ext,  ext,  0, 1.0)  // V4
    };

    // Texture coordinates
    glm::vec2 tex_coords[6] = {
        glm::vec2(0.0, 0.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(1.0, 1.0)
    };

    // Initialize texture objects
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, libconsts::kWindowSizeWidth, libconsts::kWindowSizeHeight, 0, GL_RGB, GL_FLOAT, frame);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glActiveTexture(GL_TEXTURE0);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(tex_coords), tex_coords);

    // Load shaders and use the shader program
#ifdef __APPLE__
    GLuint program = angel::InitShader("vshader_mac.glsl", "fshader_mac.glsl");
#else
    GLuint program = angel::InitShader("vshader_unix.glsl", "fshader_unix.glsl");
#endif
    glUseProgram(program);

    // set up vertex arrays
    GLuint v_position = (GLuint)glGetAttribLocation(program, "v_position");
    glEnableVertexAttribArray(v_position);
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint v_tex_coord = (GLuint)glGetAttribLocation(program, "v_tex_coord");
    glEnableVertexAttribArray(v_tex_coord);
    glVertexAttribPointer(v_tex_coord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(points));

    glUniform1i(glGetUniformLocation(program, "texture"), 0);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}

//
// Function: DisplayFunc
// ---------------------------
//
//   The display callback function
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void DisplayFunc(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glutSwapBuffers();
}

//
// Function: KeyboardFunc
// ---------------------------
//
//   The keyboard callback function
//
//   Parameters:
//       key: the key that pressed by user
//
//   Returns:
//       void
//

void KeyboardFunc(unsigned char key, int, int) {
    switch (key) {
    case 'q':       // 'q' and 'Q' keys for quit function
    case 'Q':
        free(scene);
        exit(0);
    case 's':       // 's' and 'S' keys for save image function
    case 'S':
        image::SaveImage();
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

//
// Function: Main
// ---------------------------
//
//   The main function
//
//   Parameters:
//       argc: the number of parameters in main function
//       argv[]: the array of parameters in main function
//
//   Returns:
//       void
//

int main(int argc, char **argv) {
    // Parse the arguments
    if (argc < 3) {
        printf("Missing arguments ... use:\n");
        printf("./raycast [-u | -d] step_max <options>\n");
        return -1;
    }

    // Check scene specification and max steps
    if (strcmp(argv[1], "-u") == 0) {   // user defined scene
        raychess::SetUpUserScene();
    } else {                            // default scene
        raychess::SetUpDefaultScene();
    }
    step_max = atoi(argv[2]);           // maximum level of recursions

    // Optional arguments
    for(int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) shadow_on = 1;
        if (strcmp(argv[i], "-l") == 0) reflection_on = 1;
        if (strcmp(argv[i], "-r") == 0) refraction_on = 1;
        if (strcmp(argv[i], "-c") == 0) chess_board_on = 1;
        if (strcmp(argv[i], "-f") == 0) diffuse_on = 1;
        if (strcmp(argv[i], "-p") == 0) antialiasing_on = 1;
    }

    // Ray trace the scene now
    printf("Rendering scene using my fantastic ray tracer ...\n");
    raychess::RayTrace(step_max);
    printf("Done!\n");

    // Make sure that intensity values are normalized
    image::HistogramNormalization();

    // Show the result in glut via texture mapping
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

    // Init glut window
    glutInitWindowSize(libconsts::kWindowSizeWidth, libconsts::kWindowSizeHeight);
    glutInitWindowPosition(libconsts::kWindowPositionX, libconsts::kWindowPositionY);
    glutCreateWindow("Ray tracing");

    // Init
#ifndef __APPLE__
    glewInit();
#endif
    init();

    // Set callback functions
    glutDisplayFunc(DisplayFunc);
    glutKeyboardFunc(KeyboardFunc);

    glutMainLoop();
    return 0;
}
