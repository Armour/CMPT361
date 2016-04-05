//////////////////////////////////////////////////////////////////////////////
//
//  octree.h
//
//  The source file for octree structure
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

#include "octree.h"
#include <iostream>

namespace raychess {

//
// Function: SetRange
// ---------------------------
//
//   This function set the range of the octree
//
//   Parameters:
//       min_pos: the minimum position of this octree space
//       max_pos: the maximum position of this octree space
//
//   Returns:
//       void
//

void Octree::SetRange(glm::vec3 min_pos, glm::vec3 max_pos) {
    set_min_pos(min_pos);
    set_max_pos(max_pos);
}

//
// Function: AddObject
// ---------------------------
//
//   This function add one object to the octree object array
//
//   Parameters:
//       object: the object that need added
//
//   Returns:
//       void
//

void Octree::AddObject(Object *object) {
    objects_.push_back(object);
}

//
// Function: SetRange
// ---------------------------
//
//   This function set the range of the octree
//
//   Parameters:
//       min_pos: the minimum position of this octree space
//       max_pos: the maximum position of this octree space
//
//   Returns:
//       void
//

void Octree::SplitSpace(int step) {
    if (step == 0 || objects_.size() < 3) return;

    // Inital state
    is_leaf_ = false;
    for (int i = 0; i < 8; i++) {
        sub_space_[i] = new Octree();
    }
    glm::vec3 mid_pos = (min_pos_ + max_pos_ ) / 2.0f;

    //std::cout << "(" << step << ")new split on X: " << mid_pos.x << std::endl;
    //std::cout << "(" << step << ")new split on Y: " << mid_pos.y << std::endl;
    //std::cout << "(" << step << ")new split on Z: " << mid_pos.z << std::endl;

    // Set sub space range
    sub_space_[0]->SetRange(glm::vec3(min_pos_.x, min_pos_.y, min_pos_.z), glm::vec3(mid_pos.x, mid_pos.y, mid_pos.z));
    sub_space_[1]->SetRange(glm::vec3(min_pos_.x, min_pos_.y, mid_pos.z), glm::vec3(mid_pos.x, mid_pos.y, max_pos_.z));
    sub_space_[2]->SetRange(glm::vec3(min_pos_.x, mid_pos.y, min_pos_.z), glm::vec3(mid_pos.x, max_pos_.y, mid_pos.z));
    sub_space_[3]->SetRange(glm::vec3(min_pos_.x, mid_pos.y, mid_pos.z), glm::vec3(mid_pos.x, max_pos_.y, max_pos_.z));
    sub_space_[4]->SetRange(glm::vec3(mid_pos.x, min_pos_.y, min_pos_.z), glm::vec3(max_pos_.x, mid_pos.y, mid_pos.z));
    sub_space_[5]->SetRange(glm::vec3(mid_pos.x, min_pos_.y, mid_pos.z), glm::vec3(max_pos_.x, mid_pos.y, max_pos_.z));
    sub_space_[6]->SetRange(glm::vec3(mid_pos.x, mid_pos.y, min_pos_.z), glm::vec3(max_pos_.x, max_pos_.y, mid_pos.z));
    sub_space_[7]->SetRange(glm::vec3(mid_pos.x, mid_pos.y, mid_pos.z), glm::vec3(max_pos_.x, max_pos_.y, max_pos_.z));

    // Put objects to sub space
    for (Object *object : objects_) {
        for (int i = 0; i < 8; i++) {
            glm::vec3 loose = glm::vec3(0.02f, 0.02f, 0.02f);
            if (((Triangle *)object)->InCubeRange(sub_space_[i]->min_pos_ - loose, sub_space_[i]->max_pos_ + loose)) {
                sub_space_[i]->AddObject(object);
            }
        }
    }

    //std::cout << "transfer " << objects_.size() << " node(s) into sub space\n";
    objects_.clear();

    // Recursive split space
    for (int i = 0; i < 8; i++) {
        //std::cout << "  " << sub_space_[i]->objects_.size() << " node(s) in tree with index: " << i << std::endl;
        sub_space_[i]->SplitSpace(step - 1);
    }
}

}  // namespace raychess

