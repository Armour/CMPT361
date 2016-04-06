//////////////////////////////////////////////////////////////////////////////
//
//  trace.cc
//
//  The source file for ray trace function
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

#include "trace.h"

using namespace std;

extern GLfloat frame[libconsts::kWindowSizeHeight][libconsts::kWindowSizeWidth][3];
extern raychess::Object *scene;
extern glm::vec3 background_color;
extern glm::vec3 light_position;
extern glm::vec3 light_intensity;
extern glm::vec3 global_ambient;
extern float decay_a;
extern float decay_b;
extern float decay_c;
extern int shadow_on;
extern int reflection_on;
extern int refraction_on;
extern int chessboard_on;
extern int diffuse_on;
extern int antialiasing_on;
extern int step_max;

namespace raychess {

//
// Function: PhongIllumination
// ---------------------------
//
//   The phong illumination implementation
//
//   Parameters:
//       object: the object that we need to calculate phong illumination
//       hit: the hit point
//       surf_norm: surface normal vector
//       shadow_on: whether the shadow is on
//
//   Returns:
//       the light intensity
//

glm::vec3 PhongIllumination(Object *object, glm::vec3 hit, glm::vec3 surf_norm, bool shadow_on) {
    glm::vec3 intensity(0.0f, 0.0f, 0.0f);
    glm::vec3 kd = object->get_diffuse();
    glm::vec3 ks = object->get_specular();
    glm::vec3 ka = object->get_ambient();
    if (object->get_infinite()) {       // If is infinite chessboard
        float width = libconsts::kInfiniteChessBoardGridWidth;
        glm::vec3 grid_color = ((int) round((hit.x - width / 2.0f) / width) + (int) round(hit.z / width)) % 2 == 0?
                               libconsts::kColorBlack: libconsts::kColorWhite;
        kd = ks = ka = grid_color;
    }
    glm::vec3 *dummy = new glm::vec3();
    glm::vec3 l = glm::normalize(light_position - hit);
    if (!shadow_on || IntersectScene(hit + l * libconsts::kErrorEpsilon, l, scene, dummy, object->get_index()) == nullptr) {
        glm::vec3 v = glm::normalize(libconsts::kEyePosition - hit);
        glm::vec3 r = 2.0f * glm::dot(l, surf_norm) * surf_norm - l;
        float diffuse = fmaxf(glm::dot(surf_norm, l), 0.0f);
        float specular = powf(fmaxf(glm::dot(r, v), 0.0f), object->get_shininess());
        float delta = glm::length(light_position - hit);
        float div = decay_a + decay_b * delta + decay_c * delta * delta;
        intensity = light_intensity * (kd * diffuse + ks * specular) / div;         // Calculate diffuse and specular intensity
    }
    intensity += ka * global_ambient;           // Global ambient intensity
    delete dummy;
    return intensity;
}

//
// Function: RecursiveRayTrace
// ---------------------------
//
//   This is the recursive ray tracer
//
//   Parameters:
//       origin: the origin point of ray
//       direction: the direction of ray
//       object_ignore: the object we should ignore for intersection test (used for shadow)
//
//   Returns:
//       the light intensity
//

glm::vec3 RecursiveRayTrace(glm::vec3 origin, glm::vec3 direction, int depth, int object_ignore) {
    if (depth > step_max) {             // Exceed the max step, return global ambient
        return global_ambient;
    }

    glm::vec3 *hit = new glm::vec3();           // Find the object in scene which first intersect with the ray
    Object *object = IntersectScene(origin, direction, scene, hit, object_ignore);
    
    if (object != nullptr) {
        glm::vec3 surf_norm;

        surf_norm = object->GetNormal(*hit);        // Get surface normal

        bool in_object = false;
        if (glm::dot(surf_norm, direction) > 0.0f) {        // If is in the object, we reverse normal vector
            surf_norm = -surf_norm;
            in_object = true;
        }

        glm::vec3 color = PhongIllumination(object, *hit, surf_norm, shadow_on && !in_object);      // Calculate phong illumination

        if (depth < step_max && reflection_on) {            // Reflection
            glm::vec3 reflect_ray = 2.0f * surf_norm * (glm::dot(surf_norm, -direction)) + direction;
            color += object->get_reflectance() * RecursiveRayTrace(*hit + reflect_ray * libconsts::kErrorEpsilon, reflect_ray,
                                                                   depth + 1, object_ignore);
        }

        if (depth < step_max && refraction_on && object->get_refractance() > 0.001f) {          // Refraction
            float refract_ratio = object->get_refract_ratio();
            float ratio = in_object? refract_ratio: (1.0f / refract_ratio);
            float c1 = -glm::dot(surf_norm, direction);
            float c2 = 1.0f - ratio * ratio * (1.0f - c1 * c1);
            if (c2 > 0.0f) {
                glm::vec3 refract_ray = glm::normalize((ratio * c1 - sqrtf(c2)) * surf_norm + c1 * direction);
                color += object->get_refractance() * RecursiveRayTrace(*hit + refract_ray * libconsts::kErrorEpsilon, refract_ray,
                                                                       depth + 1, object_ignore);
            }
        }

        if (depth < 2 && !in_object && diffuse_on) {        // Diffuse
        	glm::vec3 diffuse_color = glm::vec3(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < libconsts::kDiffuseReflectNumber; i++) {
                float degree = (float)(rand() % 161) - 80.0f;				// Random -80 to 80 degree
                glm::vec3 rotate_normal = glm::normalize(glm::cross(surf_norm, -direction));
                glm::vec3 diffuse_ray = glm::rotate(surf_norm, degree * libconsts::kDegreeToRadians, rotate_normal);
                diffuse_color += RecursiveRayTrace(*hit + diffuse_ray * libconsts::kErrorEpsilon, diffuse_ray,
                                                   depth + 1, object_ignore);
            }
            color += object->get_diffuse() * diffuse_color / (float)libconsts::kDiffuseReflectNumber * 0.2f;
        }

        delete hit;
        return color;
    }

    delete hit;

    if (depth == 0)     // If depth is 0 then return background color else return gobal ambient
    	return background_color;
    else
    	return global_ambient;
}

//
// Function: RayTrace
// ---------------------------
//
//   This function traverses all the pixels and cast rays. It calls the recursive ray tracer and assign return color to frame
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void RayTrace(void) {
    int i, j;
    float x_grid_size = libconsts::kImageWidth / float(libconsts::kWindowSizeWidth);
    float y_grid_size = libconsts::kImageHeight / float(libconsts::kWindowSizeHeight);
    float x_start = -0.5f * libconsts::kImageWidth;
    float y_start = -0.5f * libconsts::kImageHeight;
    glm::vec3 ray;
    glm::vec3 ret_color;
    glm::vec3 cur_pixel_pos;

    for (i = 0; i < libconsts::kWindowSizeHeight; i++) {
        for (j = 0; j < libconsts::kWindowSizeWidth; j++) {

            // Ray is cast through center of pixel
            cur_pixel_pos.x = x_start + 0.5f * x_grid_size + j * x_grid_size;
            cur_pixel_pos.y = y_start + 0.5f * y_grid_size + i * y_grid_size;
            cur_pixel_pos.z = libconsts::kImagePlanePosZ;

            // Get ray
            ray = glm::normalize(cur_pixel_pos - libconsts::kEyePosition);

            // Calculate ray trace light intensity
            ret_color = RecursiveRayTrace(cur_pixel_pos, ray, 0, 0);

            // Antialiasing
            if (antialiasing_on) {
                for (int k = 0; k < 4; k++) {
                    glm::vec3 new_pixel_pos = cur_pixel_pos + libconsts::kAntialiasingOffset[k] * x_grid_size;
                    ray = glm::normalize(new_pixel_pos - libconsts::kEyePosition);
                    ret_color += RecursiveRayTrace(new_pixel_pos, ray, 0, 0);
                }
                ret_color = ret_color / 5.0f;
            }

            // Get pixel color
            frame[i][j][0] = ret_color.r;
            frame[i][j][1] = ret_color.g;
            frame[i][j][2] = ret_color.b;

            cur_pixel_pos.x += x_grid_size;
        }
        cur_pixel_pos.y += y_grid_size;
        cur_pixel_pos.x = x_start;
    }
}

}  // namespace raychess
