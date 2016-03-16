//////////////////////////////////////////////////////////////////////////////
//
//  scene.cc
//
//  The source file for scene setup
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 3/15/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "scene.h"

extern raychess::Sphere *scene;
extern glm::vec3 background_color;
extern glm::vec3 light;
extern float light_intensity[3];
extern float global_ambient[3];
extern float decay_a;
extern float decay_b;
extern float decay_c;

namespace raychess {

//
// Function: SetUpDefaultScene
// ---------------------------
//
//   This function set up the default scene
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void SetUpDefaultScene(void) {

    // Set background color
    background_color.r = 0.5f;
    background_color.g = 0.05f;
    background_color.b = 0.8f;

    // Set up global ambient color
    global_ambient[0] = 0.2f;
    global_ambient[1] = 0.2f;
    global_ambient[2] = 0.2f;

    // Set up light
    light.x = -2.0f;
    light.y = 5.0f;
    light.z = 1.0f;
    light_intensity[0] = 1.0f;
    light_intensity[1] = 1.0f;
    light_intensity[2] = 1.0f;

    // Set up decay parameters
    decay_a = 0.5f;
    decay_b = 0.3f;
    decay_c = 0.0f;

    // Sphere 1
    glm::vec3 sphere1_ctr = {1.5f, -0.2f, -3.2f};
    float sphere1_radius = 1.23f;
    float sphere1_ambient[] = {0.7f, 0.7f, 0.7f};
    float sphere1_diffuse[] = {0.1f, 0.5f, 0.8f};
    float sphere1_specular[] = {1.0f, 1.0f, 1.0f};
    float sphere1_shineness = 10;
    float sphere1_reflectance = 0.4f;
    scene = AddSphere(scene, sphere1_ctr, sphere1_radius, sphere1_ambient,
                      sphere1_diffuse, sphere1_specular, sphere1_shineness,
                      sphere1_reflectance, 1);

    // Sphere 2
    glm::vec3 sphere2_ctr = {-1.5f, 0.0f, -3.5f};
    float sphere2_radius = 1.5f;
    float sphere2_ambient[] = {0.6f, 0.6f, 0.6f};
    float sphere2_diffuse[] = {1.0f, 0.0f, 0.25f};
    float sphere2_specular[] = {1.0f, 1.0f, 1.0f};
    float sphere2_shineness = 6;
    float sphere2_reflectance = 0.3f;
    scene = AddSphere(scene, sphere2_ctr, sphere2_radius, sphere2_ambient,
                      sphere2_diffuse, sphere2_specular, sphere2_shineness,
                      sphere2_reflectance, 2);

    // Sphere 3
    glm::vec3 sphere3_ctr = {-0.35f, 1.75f, -2.25f};
    float sphere3_radius = 0.5f;
    float sphere3_ambient[] = {0.2f, 0.2f, 0.2f};
    float sphere3_diffuse[] = {0.0f, 1.0f, 0.25f};
    float sphere3_specular[] = {0.0f, 1.0f, 0.0f};
    float sphere3_shineness = 30;
    float sphere3_reflectance = 0.3f;
    scene = AddSphere(scene, sphere3_ctr, sphere3_radius, sphere3_ambient,
                      sphere3_diffuse, sphere3_specular, sphere3_shineness,
                      sphere3_reflectance, 3);
}

//
// Function: SetUpUserScene
// ---------------------------
//
//   This function set up my own scene
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void SetUpUserScene(void) {
    //TODO: add my own scene
}

}  // namespace raychess
