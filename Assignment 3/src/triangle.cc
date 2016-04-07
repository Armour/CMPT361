//////////////////////////////////////////////////////////////////////////////
//
//  triangle.h
//
//  The source file for triangle class
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 3/24/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "triangle.h"

#define FIND_MIN_MAX(x0, x1, x2, min, max) \
                     min = max = x0; \
                     if (x1 < min) min = x1; \
                     if (x1 > max) max = x1; \
                     if (x2 < min) min = x2; \
                     if (x2 > max) max = x2;

#define AXIS_TEST_X12(a, b, fa, fb)	\
                      p1 = a * v1.y - b * v1.z; \
                      p3 = a * v3.y - b * v3.z; \
                      if (p1 < p3) { min = p1; max = p3; } \
                          else { min = p3; max = p1; } \
                      rad = fa * half_size.y + fb * half_size.z; \
                      if (min > rad || max < -rad) return 0;

#define AXIS_TEST_X3(a, b, fa, fb) \
                     p1 = a * v1.y - b * v1.z; \
                     p2 = a * v2.y - b * v2.z; \
                     if (p1 < p2) { min = p1; max = p2; } \
                         else { min = p2; max = p1; } \
                     rad = fa * half_size.y + fb * half_size.z; \
                     if (min > rad || max < -rad) return 0;

#define AXIS_TEST_Y13(a, b, fa, fb) \
                      p1 = -a * v1.x + b * v1.z; \
                      p3 = -a * v3.x + b * v3.z; \
                      if (p1 < p3) { min = p1; max = p3; } \
                          else { min = p3; max = p1; } \
                      rad = fa * half_size.x + fb * half_size.z; \
                      if (min > rad || max < -rad) return 0;

#define AXIS_TEST_Y2(a, b, fa, fb) \
                     p1 = -a * v1.x + b * v1.z; \
                     p2 = -a * v2.x + b * v2.z; \
                     if (p1 < p2) { min = p1; max = p2; } \
                         else { min = p2; max = p1; } \
                     rad = fa * half_size.x + fb * half_size.z; \
                     if (min > rad || max < -rad) return 0;

#define AXIS_TEST_Z23(a, b, fa, fb) \
                      p2 = a * v2.x - b * v2.y; \
                      p3 = a * v3.x - b * v3.y; \
                      if (p3 < p2) { min = p3; max = p2; } \
                          else { min = p2; max = p3; } \
                      rad = fa * half_size.x + fb * half_size.y; \
                      if (min > rad || max < -rad) return 0;

#define AXIS_TEST_Z1(a, b, fa, fb) \
                     p1 = a * v1.x - b * v1.y; \
                     p2 = a * v2.x - b * v2.y; \
                     if (p1 < p2) { min = p1; max = p2; } \
                         else { min = p2; max = p1; } \
                     rad = fa * half_size.x + fb * half_size.y; \
                     if (min > rad || max < -rad) return 0;

namespace raychess {

//
// Function: IntersectRay
// ---------------------------
//
//   This function intersects a ray with a triangle
//
//   Parameters:
//       origin: the origin point of the ray
//       direction: the direction of the ray
//       hit: the intersection point, nullptr if not intersect
//
//   Returns:
//       void
//

float Triangle::IntersectRay(glm::vec3 origin, glm::vec3 direction, glm::vec3 *hit) {
    glm::vec3 norm = GetNormal(glm::vec3(0.0f));
    float t = -(glm::dot(norm, origin) - glm::dot(norm, v1)) / glm::dot(norm, direction);
    if (t < 0) return -1;
    *hit = origin + t * direction;
    if (!get_infinite() && !InTriangle(*hit)) return -1;
    return t;
}

//
// Function: GetNormal
// ---------------------------
//
//   Return the unit normal at a point on the triangle
//
//   Parameters:
//       dummy: useless point
//
//   Returns:
//       triangle normal vector
//

glm::vec3 Triangle::GetNormal(glm::vec3 dummy) {
    glm::vec3 u = v1 - v3;
    glm::vec3 v = v2 - v3;
    return glm::normalize(glm::cross(u, v));
}

//
// Function: AddTriangle
// ---------------------------
//
//   This function adds a triangle into the object list
//
//   Parameters:
//       the parameters for the sphere
//
//   Returns:
//       the pointer to the object list
//

Object *AddTriangle(Object *objects, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                    float shininess, float reflectance, float refractance, float refract_ratio, int index, bool infinite, bool chessboard) {
    Triangle *new_triangle = new Triangle();
    new_triangle->set_index(index);
    new_triangle->set_v1(v1);
    new_triangle->set_v2(v2);
    new_triangle->set_v3(v3);
    new_triangle->set_ambient(ambient);
    new_triangle->set_diffuse(diffuse);
    new_triangle->set_specular(specular);
    new_triangle->set_shininess(shininess);
    new_triangle->set_reflectance(reflectance);
    new_triangle->set_refractance(refractance);
    new_triangle->set_refract_ratio(refract_ratio);
    new_triangle->set_next(nullptr);
    new_triangle->set_infinite(infinite);
    new_triangle->set_chessboard(chessboard);

    Object *object = new_triangle;
    if (objects == nullptr) {           // First object
        objects = object;
    } else {                            // Insert at the beginning
        object->set_next(objects);
        objects = object;
    }

    return objects;
}

//
// Function: InTriangle
// ---------------------------
//
//   Check if the hit point in the triangle area
//
//   Parameters:
//       hit: the hit point
//
//   Returns:
//       if the hit point in the tirangle
//

bool Triangle::InTriangle(glm::vec3 hit) {
    glm::vec3 t1 = glm::cross(v1 - v3, v2 - v3);
    glm::vec3 t2 = glm::cross(v1 - hit, v2 - hit);

    if (t1.x * t2.x < 0 || t1.y * t2.y < 0 || t1.z * t2.z < 0) {
        return false;
    }

    t2 = glm::cross(v2 - hit, v3 - hit);

    if (t1.x * t2.x < 0 || t1.y * t2.y < 0 || t1.z * t2.z < 0) {
        return false;
    }

    t2 = glm::cross(v3 - hit, v1 - hit);

    if (t1.x * t2.x < 0 || t1.y * t2.y < 0 || t1.z * t2.z < 0) {
        return false;
    }
    return true;
}

//
// Function: InCubeRange
// ---------------------------
//   Check if a triangle is in a specific cube range
//
//   Parameters:
//       min_pos: the minimum position of the cube
//       max_pos: the maximum position of the cube
//
//   Returns:
//       if the sphere is in the cube
//

bool Triangle::InCubeRange(glm::vec3 min_pos, glm::vec3 max_pos) {
    if (get_infinite()) {
        if (v1.y <= max_pos.y && v1.y >= min_pos.y) return true;
        return false;
    }

    glm::vec3 half_size = (max_pos - min_pos) / 2.0f;
    glm::vec3 center = (max_pos + min_pos) / 2.0f;
    glm::vec3 v1, v2, v3;
    int min, max;
    float rad, p1, p2, p3;

    // Move everything so that the box center is in (0,0,0)
    v1 = this->v1 - center;
    v2 = this->v2 - center;
    v3 = this->v3 - center;

    // Compute triangle edges
    glm::vec3 e1 = v2 - v1;
    glm::vec3 e2 = v3 - v2;
    glm::vec3 e3 = v1 - v3;

    // Test the 9 tests first (this was faster)
    float fex = fabsf(e1.x);
    float fey = fabsf(e1.y);
    float fez = fabsf(e1.z);
    AXIS_TEST_X12(e1.z, e1.y, fez, fey);
    AXIS_TEST_Y13(e1.z, e1.x, fez, fex);
    AXIS_TEST_Z23(e1.y, e1.x, fey, fex);

    fex = fabsf(e2.x);
    fey = fabsf(e2.y);
    fez = fabsf(e2.z);
    AXIS_TEST_X12(e2.z, e2.y, fez, fey);
    AXIS_TEST_Y13(e2.z, e2.x, fez, fex);
    AXIS_TEST_Z1(e2.y, e2.x, fey, fex);

    fex = fabsf(e3.x);
    fey = fabsf(e3.y);
    fez = fabsf(e3.z);
    AXIS_TEST_X3(e3.z, e3.y, fez, fey);
    AXIS_TEST_Y2(e3.z, e3.x, fez, fex);
    AXIS_TEST_Z23(e3.y, e3.x, fey, fex);

    // Test in X-direction
    FIND_MIN_MAX(v1.x, v2.x, v3.x, min, max);
    if (min > half_size.x || max < -half_size.x) return false;

    // Test in Y-direction
    FIND_MIN_MAX(v1.y, v2.y, v3.y, min, max);
    if (min > half_size.y || max < -half_size.y) return false;

    // Test in Z-direction
    FIND_MIN_MAX(v1.z, v2.z, v3.z, min, max);
    if (min > half_size.z || max < -half_size.y) return false;

    // Test if the box intersects the plane of the triangle
    glm::vec3 normal = glm::normalize(glm::cross(e1, e2));
    if (!PlaneBoxOverlap(normal, v1, half_size)) return false;

    return true;
}

//
// Function: PlaneBoxOverlap
// ---------------------------
//   Test if a plane overlap with a AABB box
//
//   Parameters:
//       normal: the normal vector of he plane
//       vertex: a vertex in the plane
//       box_size: the size of the box
//
//   Returns:
//       if the plane is overlap with the box
//

bool PlaneBoxOverlap(glm::vec3 normal, glm::vec3 vertex, glm::vec3 box_size) {
    glm::vec3 vmin, vmax;
    for (int i = 0; i <= 2; i++) {
        if (normal[i] > 0.0f) {
            vmin[i] = -box_size[i] - vertex[i];
            vmax[i] =  box_size[i] - vertex[i];
        } else {
            vmin[i] =  box_size[i] - vertex[i];
            vmax[i] = -box_size[i] - vertex[i];
        }
    }
    if (glm::dot(normal, vmin) > 0.0f) return false;
    if (glm::dot(normal, vmax) >= 0.0f) return true;
    return false;
}

}  // namespace raychess

