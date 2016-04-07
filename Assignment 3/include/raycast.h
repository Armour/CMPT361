//////////////////////////////////////////////////////////////////////////////
//
//  raycast.h
//
//  The header file for scene setup
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 4/6/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef RAYCHESS_RAYCAST_H_
#define RAYCHESS_RAYCAST_H_

#include "octree.h"

namespace raychess {

class RenderManager {
private:
    Object *objects_;         // List of objects in the scene
    OctreeNode *octree_;      // The octree for ray casting

    glm::vec3 light_position_;          // Light position
    glm::vec3 light_intensity_;         // Light color
    glm::vec3 global_ambient_;          // Global ambient term
    glm::vec3 background_color_;        // Background color

    float decay_a_;             // Light decay parameters
    float decay_b_;
    float decay_c_;

    int step_max_;              // Maximum level of recursions of reflection

public:
    RenderManager() {           // Constructor
        objects_ = nullptr;
        octree_ = new OctreeNode(glm::vec3(-20.0f, -20.0f, -20.0f), glm::vec3(20.0f, 20.0f, 20.0f));
        step_max_ = 0;
        shadow_on_ = 0;
        reflection_on_ = 0;
        refraction_on_ = 0;
        chessboard_on_ = 0;
        diffuse_on_ = 0;
        antialiasing_on_ = 0;
        octree_on_ = 0;
    };
    ~RenderManager() {};

    int shadow_on_;             // The option flags
    int reflection_on_;
    int refraction_on_;
    int chessboard_on_;
    int diffuse_on_;
    int antialiasing_on_;
    int octree_on_;
    int infinite_on_;           // Infinite chessboard for bonus

    inline glm::vec3 get_light_position(void) const { return light_position_; };
    inline glm::vec3 get_light_intensity(void) const { return light_intensity_; };
    inline glm::vec3 get_global_ambient(void) const { return global_ambient_; };
    inline glm::vec3 get_background_color(void) const { return background_color_; };
    inline float get_decay_a(void) const { return decay_a_; };
    inline float get_decay_b(void) const { return decay_b_; };
    inline float get_decay_c(void) const { return decay_c_; };
    inline int get_step_max(void) const { return step_max_; };
    inline Object *get_scene_objects(void) const { return objects_; };
    inline OctreeNode *get_octree_root(void) const { return octree_; };

    inline void set_light_position(glm::vec3 light_position) { this->light_position_ = light_position; };
    inline void set_light_intensity(glm::vec3 light_intensity) { this->light_intensity_ = light_intensity; };
    inline void set_global_ambient(glm::vec3 global_ambient) { this->global_ambient_ = global_ambient; };
    inline void set_background_color(glm::vec3 background_color) { this->background_color_ = background_color; };
    inline void set_decay_a(float decay_a) { this->decay_a_ = decay_a; };
    inline void set_decay_b(float decay_a) { this->decay_b_ = decay_a; };
    inline void set_decay_c(float decay_a) { this->decay_c_ = decay_a; };
    inline void set_step_max(int step_max) { this->step_max_ = step_max; };
    inline void set_scene_objects(Object *object) { this->objects_ = object; };
    inline void set_octree_root(OctreeNode *node) { this->octree_ = node; };
};

} // namespace raychess

#endif // RAYCHESS_RAYCAST_H_
