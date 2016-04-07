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
#include "raycast.h"

GLfloat frame[libconsts::kWindowSizeHeight][libconsts::kWindowSizeWidth][3];    // The array for the final image

raychess::RenderManager *manager;       // The render manager

long long intersection_count = 0;       // The number of intersection test of ray and polygon

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
        exit(EXIT_SUCCESS);
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

    manager = new raychess::RenderManager();

    // Optional arguments
    for(int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "+s") == 0) manager->shadow_on_ = 1;
        if (strcmp(argv[i], "+l") == 0) manager->reflection_on_ = 1;
        if (strcmp(argv[i], "+r") == 0) manager->refraction_on_ = 1;
        if (strcmp(argv[i], "+c") == 0) manager->chessboard_on_ = 1;
        if (strcmp(argv[i], "+f") == 0) manager->diffuse_on_ = 1;
        if (strcmp(argv[i], "+p") == 0) manager->antialiasing_on_ = 1;
        if (strcmp(argv[i], "+o") == 0) manager->octree_on_ = 1;
    }

    // Check scene specification and max steps
    if (strcmp(argv[1], "-u") == 0) {   // User defined scene
        manager->infinite_on_ = 1;
        raychess::SetUpUserScene(manager);
    } else {                            // Default scene
        manager->infinite_on_ = 0;
        raychess::SetUpDefaultScene(manager);
    }
    manager->set_step_max(atoi(argv[2]));           // Maximum level of recursions

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Ray trace and get time
    printf("Rendering scene using my fantastic ray tracer ...\n");
    struct timeval start, end;
    gettimeofday(&start, NULL);
    raychess::RayTrace(manager);       // Do ray trace!
    gettimeofday(&end, NULL);
    auto delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
    printf("Done! Time used: %.2lfs, intersection test number: %lld\n", delta, intersection_count);

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
