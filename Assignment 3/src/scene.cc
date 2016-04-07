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
#include "sphere.h"
#include "smf_parser.h"

namespace raychess {

//
// Function: SetUpDefaultScene
// ---------------------------
//
//   This function set up the default scene
//
//   Parameters:
//       manager: the render manager for ray tracing
//
//   Returns:
//       void
//

void SetUpDefaultScene(RenderManager *manager) {
    // Index
    int index = 0;

    // Set background color
    manager->set_background_color({0.50f, 0.05f, 0.80f});

    // Set up global ambient color
    manager->set_global_ambient({0.2f, 0.2f, 0.2f});

    // Set up light
    manager->set_light_position({-2.0f, 5.0f, 1.0f});
    manager->set_light_intensity({1.0f, 1.0f, 1.0f});

    // Set up decay parameters
    manager->set_decay_a(0.5f);
    manager->set_decay_b(0.3f);
    manager->set_decay_c(0.0f);

    // Sphere 1
    glm::vec3 sphere1_center = {1.5f, -0.2f, -3.2f};
    glm::vec3 sphere1_ambient = {0.7f, 0.7f, 0.7f};
    glm::vec3 sphere1_diffuse = {0.1f, 0.5f, 0.8f};
    glm::vec3 sphere1_specular = {1.0f, 1.0f, 1.0f};
    float sphere1_radius = 1.23f;
    float sphere1_shininess = 10;
    float sphere1_reflectance = 0.4f;
    float sphere1_refractance = 0.4f;
    float sphere1_reflect_ratio = 1.52f;
    Object *object_list = AddSphere(manager->get_scene_objects(), sphere1_center, sphere1_radius, sphere1_ambient,
                                   sphere1_diffuse, sphere1_specular, sphere1_shininess, sphere1_reflectance,
                                   sphere1_refractance, sphere1_reflect_ratio, ++index);
    manager->set_scene_objects(object_list);

    // Sphere 2
    glm::vec3 sphere2_center = {-1.5f, 0.0f, -3.5f};
    glm::vec3 sphere2_ambient = {0.6f, 0.6f, 0.6f};
    glm::vec3 sphere2_diffuse = {1.0f, 0.0f, 0.25f};
    glm::vec3 sphere2_specular = {1.0f, 1.0f, 1.0f};
    float sphere2_radius = 1.5f;
    float sphere2_shininess = 6;
    float sphere2_reflectance = 0.3f;
    float sphere2_refractance = 0.3f;
    float sphere2_reflect_ratio = 1.52f;
    object_list = AddSphere(manager->get_scene_objects(), sphere2_center, sphere2_radius, sphere2_ambient,
                           sphere2_diffuse, sphere2_specular, sphere2_shininess, sphere2_reflectance,
                           sphere2_refractance, sphere2_reflect_ratio, ++index);
    manager->set_scene_objects(object_list);

    // Sphere 3
    glm::vec3 sphere3_center = {-0.35f, 1.75f, -2.25f};
    glm::vec3 sphere3_ambient = {0.2f, 0.2f, 0.2f};
    glm::vec3 sphere3_diffuse = {0.0f, 1.0f, 0.25f};
    glm::vec3 sphere3_specular = {0.0f, 1.0f, 0.0f};
    float sphere3_radius = 0.5f;
    float sphere3_shininess = 30;
    float sphere3_reflectance = 0.3f;
    float sphere3_refractance = 0.3f;
    float sphere3_reflect_ratio = 1.52f;
    object_list = AddSphere(manager->get_scene_objects(), sphere3_center, sphere3_radius, sphere3_ambient,
                            sphere3_diffuse, sphere3_specular, sphere3_shininess, sphere3_reflectance,
                            sphere3_refractance, sphere3_reflect_ratio, ++index);
    manager->set_scene_objects(object_list);

    // Add chessboard
    if (manager->chessboard_on_) {
        bool infinite = false;
        bool chessboard = true;
        glm::vec3 v1, v2, v3, v4;
        float triangle_shininess = 30.0f;
        float triangle_reflectance = 1.0f;
        float triangle_refractance = 0.0f;
        float triangle_reflect_ratio = 1.00f;
        float width = libconsts::kChessBoardGridWidth;
        glm::vec3 offset = libconsts::kChessBoardOffset;
        glm::vec3 color = libconsts::kColorWhite;
        v1 = {0.0f, 0.0f, 0.0f};
        v2 = {width * 8.0f, 0.0f, 0.0f};
        v3 = {0.0f, 0.0f, width * 8.0f};
        v4 = {width * 8.0f, 0.0f, width * 8.0f};
        Object *object_list;
        object_list = AddTriangle(manager->get_scene_objects(), v1 + offset, v3 + offset, v2 + offset,
                                  color, color, color, triangle_shininess, triangle_reflectance, triangle_refractance,
                                  triangle_reflect_ratio, ++index, infinite, chessboard);
        manager->set_scene_objects(object_list);
        object_list = AddTriangle(manager->get_scene_objects(), v2 + offset, v3 + offset, v4 + offset,
                                  color, color, color, triangle_shininess, triangle_reflectance, triangle_refractance,
                                  triangle_reflect_ratio, ++index, infinite, chessboard);
        manager->set_scene_objects(object_list);
    }

    // Create octree with all the objects in scene
    if (manager->octree_on_) {
        Object *object = manager->get_scene_objects();
        while (object != nullptr) {
            manager->get_octree_root()->AddObject(object);
            object = object->get_next();
        }
        manager->get_octree_root()->SplitSpace(8);
    }
}

//
// Function: SetUpUserScene
// ---------------------------
//
//   This function set up my own scene
//
//   Parameters:
//       manager: the render manager for ray tracing
//
//   Returns:
//       void
//

void SetUpUserScene(RenderManager *manager) {
    // Index
    int index = 0;

    // Set background color
    manager->set_background_color({0.50f, 0.05f, 0.80f});

    // Set up global ambient color
    manager->set_global_ambient({0.2f, 0.2f, 0.2f});

    // Set up light
    manager->set_light_position({-2.0f, 5.0f, 1.0f});
    manager->set_light_intensity({1.0f, 1.0f, 1.0f});

    // Set up decay parameters
    manager->set_decay_a(0.5f);
    manager->set_decay_b(0.3f);
    manager->set_decay_c(0.0f);

    // Import from mesh file
    smfparser::ImportMeshFile(manager, "chess_piece.smf", 3.5f, 0, glm::vec3(-3.0f, -2.5f, -5.20f), index);
    smfparser::ImportMeshFile(manager, "bishop.smf", 45.0f, 0, glm::vec3(1.5f, -2.5f, -4.00f), index);
    //smfparser::ImportMeshFile(manager, "chess_hires.smf", 3.5f, 0, glm::vec3(-3.0f, -2.5f, -5.20f), index);
    //smfparser::ImportMeshFile(manager, "bishop_hires.smf", 45.0f, 0, glm::vec3(1.5f, -2.5f, -4.00f), index);

    // Add infinite chessboard
    if (manager->chessboard_on_) {
        bool infinite = true;
        bool chessboard = true;
        glm::vec3 v1, v2, v3;
        float triangle_shininess = 30.0f;
        float triangle_reflectance = 0.4f;
        float triangle_refractance = 0.0f;
        float triangle_reflect_ratio = 1.20f;
        float width = libconsts::kChessBoardGridWidth;
        glm::vec3 offset = libconsts::kChessBoardOffset;
        glm::vec3 color = libconsts::kColorWhite;
        v1 = {0.0f, 0.0f, 0.0f};
        v2 = {width, 0.0f, 0.0f};
        v3 = {0.0f, 0.0f, width};
        Object *object_list = AddTriangle(manager->get_scene_objects(), v1 + offset, v3 + offset, v2 + offset,
                                          color, color, color, triangle_shininess, triangle_reflectance, triangle_refractance,
                                          triangle_reflect_ratio, ++index, infinite, chessboard);
        manager->set_scene_objects(object_list);
    }

    // Create octree with all the objects in scene without chessboard
    if (manager->octree_on_) {
        Object *object = manager->get_scene_objects();
        while (object != nullptr) {
            manager->get_octree_root()->AddObject(object);
            object = object->get_next();
        }
        manager->get_octree_root()->SplitSpace(9);
    }
}

}  // namespace raychess
