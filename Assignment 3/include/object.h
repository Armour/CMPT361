//////////////////////////////////////////////////////////////////////////////
//
//  object.h
//
//  The header file for object class
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 3/23/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef RAYCHESS_OBJECT_H_
#define RAYCHESS_OBJECT_H_

#include "lib_consts.h"
#include "lib_gl.h"

namespace raychess {

class Object {
private:
    int index_;                     // Identifies an object, must be greater than 0
    bool infinite;                  // Whether the object is infinitely expand or not
    glm::vec3 mat_ambient_;         // Material property used in Phong model
    glm::vec3 mat_diffuse_;
    glm::vec3 mat_specular_;
    float mat_shininess_;
    float mat_reflectance_;         // Determines how much reflected light contributes to the color of a pixel
    float mat_refractance_;         // Determines how much refracted light contributes to the color of a pixel
    float mat_refract_ratio_;       // The refract ratio for this material (1.0 for vacuum)
    Object *next_;

public:
    Object() {};
    virtual ~Object() {};

    inline int get_index(void) const { return index_; };
    inline bool get_infinite(void) { return infinite; };
    inline glm::vec3 get_ambient(void) const { return mat_ambient_; };
    inline glm::vec3 get_diffuse(void) const { return mat_diffuse_; };
    inline glm::vec3 get_specular(void) const { return mat_specular_; };
    inline float get_shininess(void) const { return mat_shininess_; };
    inline float get_reflectance(void) const { return mat_reflectance_; };
    inline float get_refractance(void) const { return mat_refractance_; };
    inline float get_refract_ratio(void) const { return mat_refract_ratio_; };
    inline Object *get_next(void) const { return next_; };

    inline void set_index(int index) { this->index_ = index; };
    inline void set_infinite(bool infinite) { this->infinite = infinite; };
    inline void set_ambient(glm::vec3 ambient) { this->mat_ambient_ = ambient; };
    inline void set_diffuse(glm::vec3 diffuse) { this->mat_diffuse_ = diffuse; };
    inline void set_specular(glm::vec3 specular) { this->mat_specular_ = specular; };
    inline void set_shininess(float shininess) { this->mat_shininess_ = shininess; };
    inline void set_reflectance(float reflectance) { this->mat_reflectance_ = reflectance; };
    inline void set_refractance(float refractance) { this->mat_refractance_ = refractance; };
    inline void set_refract_ratio(float refract_ratio) { this->mat_refract_ratio_ = refract_ratio; };
    inline void set_next(Object *next) { this->next_ = next; };

    virtual glm::vec3 GetNormal(glm::vec3 surf_point) = 0;              // Get object surface normal
    virtual float IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit) = 0;      // Intersect with ray
    virtual bool InCubeRange(glm::vec3 min_pos, glm::vec3 max_pos) = 0;         // Check if a triangle is in a specific cube range
};

// Intersect ray with objects in scene
Object *IntersectScene(glm::vec3 origin, glm::vec3 direction, Object *objects, glm::vec3 *hit, int object_ignore);

}  // namespace raychess

#endif  // RAYCHESS_OBJECT_H_
