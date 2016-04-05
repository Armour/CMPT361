//////////////////////////////////////////////////////////////////////////////
//
//  octree.h
//
//  The header file for octree structure
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 4/3/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef RAYCHESS_OCTREE_H_
#define RAYCHESS_OCTREE_H_

#include "lib_consts.h"
#include "lib_gl.h"
#include "triangle.h"

namespace raychess {

class Octree {
private:
    bool is_leaf_;
    glm::vec3 min_pos_;
    glm::vec3 max_pos_;

public:
    Octree() {
        Octree(glm::vec3(0.0f), glm::vec3(0.0f));
    };
    Octree(glm::vec3 min, glm::vec3 max): is_leaf_(true), min_pos_(min), max_pos_(max) {
        for (int i = 0; i < 8; i++) {
            sub_space_[i] = nullptr;
        }
        objects_.clear();
    };
    ~Octree() {};

    Octree *sub_space_[8];
    std::vector<Object *> objects_;

    inline void set_leaf(bool is_leaf) { this->is_leaf_ = is_leaf; };
    inline void set_min_pos(glm::vec3 min_pos) { this->min_pos_ = min_pos; };
    inline void set_max_pos(glm::vec3 max_pos) { this->max_pos_ = max_pos; };
    inline bool get_leaf(void) { return is_leaf_; };
    inline glm::vec3 get_min_pos(void) { return min_pos_; };
    inline glm::vec3 get_max_pos(void) { return max_pos_; };

    void SetRange(glm::vec3 min_pos, glm::vec3 max_pos);
    void AddObject(Object *object);
    void SplitSpace(int step);
};

}  // namespace raychess

#endif  // RAYCHESS_OCTREE_H_
