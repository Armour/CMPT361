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
#include <sys/time.h>

#include "image_util.h"
#include "scene.h"
#include "trace.h"

// The array for the final image
GLfloat frame[libconsts::kWindowSizeHeight][libconsts::kWindowSizeWidth][3];

// Background color
glm::vec3 background_color;

// List of objects in the scene
raychess::Object *scene = nullptr;

// Light position and color
glm::vec3 light_position;
glm::vec3 light_intensity;

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
int refraction_on = 0;
int chessboard_on = 0;
int diffuse_on = 0;
int antialiasing_on = 0;

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

    // Optional arguments
    for(int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "+s") == 0) shadow_on = 1;
        if (strcmp(argv[i], "+l") == 0) reflection_on = 1;
        if (strcmp(argv[i], "+r") == 0) refraction_on = 1;
        if (strcmp(argv[i], "+c") == 0) chessboard_on = 1;
        if (strcmp(argv[i], "+f") == 0) diffuse_on = 1;
        if (strcmp(argv[i], "+p") == 0) antialiasing_on = 1;
    }

    // Check scene specification and max steps
    if (strcmp(argv[1], "-u") == 0) {   // User defined scene
        raychess::SetUpUserScene();
    } else {                            // Default scene
        raychess::SetUpDefaultScene();
    }
    step_max = atoi(argv[2]);           // Maximum level of recursions

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Ray trace and get time
    printf("Rendering scene using my fantastic ray tracer ...\n");
    struct timeval start, end;
    gettimeofday(&start, NULL);
    raychess::RayTrace(step_max);       // Do ray trace!
    gettimeofday(&end, NULL);
    auto delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("Done! Time used: %.2lfs\n", delta);

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

    // Set callback functions
    glutDisplayFunc(DisplayFunc);
    glutKeyboardFunc(KeyboardFunc);

    glutMainLoop();
    return 0;
}
