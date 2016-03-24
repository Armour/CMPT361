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
    int type_;                      // The type of the object (sphere, planer...)
    glm::vec3 mat_ambient_;         // Material property used in Phong model
    glm::vec3 mat_diffuse_;
    glm::vec3 mat_specular_;
    float mat_shininess_;
    float mat_reflectance_;         // Determines how much reflected light contributes to the color of a pixel
    class Object *next_;
public:
    inline int get_index(void) const { return index_; };
    inline int get_type(void) const { return type_; };
    inline glm::vec3 get_ambient(void) const { return mat_ambient_; };
    inline glm::vec3 get_diffuse(void) const { return mat_diffuse_; };
    inline glm::vec3 get_specular(void) const { return mat_specular_; };
    inline float get_shininess(void) const { return mat_shininess_; };
    inline float get_reflectance(void) const { return mat_reflectance_; };
    inline Object *get_next(void) const { return next_; };

    inline void set_index(float index) { this->index_ = index; };
    inline void set_type(float type) { this->type_ = type; };
    inline void set_ambient(glm::vec3 ambient) { this->mat_ambient_ = ambient; };
    inline void set_diffuse(glm::vec3 diffuse) { this->mat_diffuse_ = diffuse; };
    inline void set_specular(glm::vec3 specular) { this->mat_specular_ = specular; };
    inline void set_shininess(float shininess) { this->mat_shininess_ = shininess; };
    inline void set_reflectance(float reflectance) { this->mat_reflectance_ = reflectance; };
    inline void set_next(Object *next) { this->next_ = next; };
};

}  // namespace raychess

#endif //RAYCHESS_OBJECT_H_
