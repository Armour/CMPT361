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

void OctreeNode::SetRange(glm::vec3 min_pos, glm::vec3 max_pos) {
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

void OctreeNode::AddObject(Object *object) {
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

void OctreeNode::SplitSpace(int step) {
    if (step == 0 || objects_.size() < 10) return;

    // Inital state
    set_leaf(false);
    for (int i = 0; i < MAX_NODE_COUNT; i++) {
        sub_space_[i] = new OctreeNode(glm::vec3(0.0f), glm::vec3(0.0f));
    }
    glm::vec3 mid_pos = (min_pos_ + max_pos_) / 2.0f;

    // Set sub space range
    sub_space_[FAR_BOTTOM_LEFT]->SetRange(glm::vec3(min_pos_.x, min_pos_.y, min_pos_.z), glm::vec3(mid_pos.x, mid_pos.y, mid_pos.z));
    sub_space_[NEAR_BOTTOM_LEFT]->SetRange(glm::vec3(min_pos_.x, min_pos_.y, mid_pos.z), glm::vec3(mid_pos.x, mid_pos.y, max_pos_.z));
    sub_space_[FAR_TOP_LEFT]->SetRange(glm::vec3(min_pos_.x, mid_pos.y, min_pos_.z), glm::vec3(mid_pos.x, max_pos_.y, mid_pos.z));
    sub_space_[NEAR_TOP_LEFT]->SetRange(glm::vec3(min_pos_.x, mid_pos.y, mid_pos.z), glm::vec3(mid_pos.x, max_pos_.y, max_pos_.z));
    sub_space_[FAR_BOTTOM_RIGHT]->SetRange(glm::vec3(mid_pos.x, min_pos_.y, min_pos_.z), glm::vec3(max_pos_.x, mid_pos.y, mid_pos.z));
    sub_space_[NEAR_BOTTOM_RIGHT]->SetRange(glm::vec3(mid_pos.x, min_pos_.y, mid_pos.z), glm::vec3(max_pos_.x, mid_pos.y, max_pos_.z));
    sub_space_[FAR_TOP_RIGHT]->SetRange(glm::vec3(mid_pos.x, mid_pos.y, min_pos_.z), glm::vec3(max_pos_.x, max_pos_.y, mid_pos.z));
    sub_space_[NEAR_TOP_RIGHT]->SetRange(glm::vec3(mid_pos.x, mid_pos.y, mid_pos.z), glm::vec3(max_pos_.x, max_pos_.y, max_pos_.z));

    // Put objects to sub space
    for (Object *object : objects_) {
        for (int i = 0; i < MAX_NODE_COUNT; i++) {
            glm::vec3 loose = glm::vec3(0.01f, 0.01f, 0.01f);
            if (object->get_type() == libconsts::kTypeSphere) {
                if (((Sphere *)object)->InCubeRange(sub_space_[i]->min_pos_ - loose, sub_space_[i]->max_pos_ + loose))
                    sub_space_[i]->AddObject(object);
            } else if (object->get_type() == libconsts::kTypeTriangle) {
                if (((Triangle *)object)->InCubeRange(sub_space_[i]->min_pos_ - loose, sub_space_[i]->max_pos_ + loose))
                    sub_space_[i]->AddObject(object);
            }
        }
    }
    objects_.clear();

    // Recursive split space
    for (int i = 0; i < MAX_NODE_COUNT; i++) {
        sub_space_[i]->SplitSpace(step - 1);
    }
}

//
// Function: GetFirstNode
// ---------------------------
//
//   This function used to get the first node to be entered by the ray
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

unsigned int GetFirstNode(glm::vec3 t0, glm::vec3 tm) {
    unsigned int entryPlane = 0;                // Initial first node

    if (t0.x > t0.y) {
        if (t0.x > t0.z) {                      // YZ plane
            if (tm.y < t0.x) entryPlane |= 2;
            if (tm.z < t0.x) entryPlane |= 1;
            return entryPlane;
        }
    } else if (t0.y > t0.z) {                   // XZ plane
        if (tm.x < t0.y) entryPlane |= 4;
        if (tm.z < t0.y) entryPlane |= 1;
        return entryPlane;
    }

    if (tm.x < t0.z) entryPlane |= 4;           // XY plane
    if (tm.y < t0.z) entryPlane |= 2;

    return entryPlane;                  // Returns the first node
}

//
// Function: GetNextNode
// ---------------------------
//
//   This function used for calculating the exit plane and the next node, once the current parent node is exited
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

unsigned int GetNextNode(glm::vec3 tm, unsigned int x, unsigned int y, unsigned int z) {
    if (tm.x < tm.y) {
        if (tm.x < tm.z) {          // YZ plane
            return x;
        }
    } else {
        if (tm.y < tm.z) {          // XZ plane
            return y;
        }
    }
    return z;                       // XY plane;
}

//
// Function: ProcessSubNode
// ---------------------------
//
//   This function used to process ray casting in sub node of one node
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void ProcessSubNode(glm::vec3 origin, OctreeNode* node, glm::vec3 t0, glm::vec3 t1, std::vector<OctreeNode*> &nodes, unsigned int a) {
    if (t1.x < 0 || t1.y < 0 || t1.z < 0) return;

    //std::cout << "!!!" << node->get_min_pos().x << " " << node->get_min_pos().y << " " << node->get_min_pos().z << " --- "
    //                   << node->get_max_pos().x << " " << node->get_max_pos().y << " " << node->get_max_pos().z << std::endl;

    if (node->get_leaf()) {
        nodes.push_back(node);
        return;
    }

    glm::vec3 tm = 0.5f * (t0 + t1);
    if (glm::isnan(tm.x)) tm.x = origin.x < 0.5f * (node->get_min_pos().x + node->get_max_pos().x)? INFINITY: -INFINITY;
    if (glm::isnan(tm.y)) tm.y = origin.y < 0.5f * (node->get_min_pos().y + node->get_max_pos().y)? INFINITY: -INFINITY;
    if (glm::isnan(tm.z)) tm.z = origin.z < 0.5f * (node->get_min_pos().z + node->get_max_pos().z)? INFINITY: -INFINITY;

    unsigned int current_node = GetFirstNode(t0, tm);

    do {
        switch (current_node) {
            case 0: ProcessSubNode(origin, node->sub_space_[a], glm::vec3(t0.x, t0.y, t0.z), glm::vec3(tm.x, tm.y, tm.z), nodes, a);
                current_node = GetNextNode(glm::vec3(tm.x, tm.y, tm.z), 4, 2, 1);
                break;

            case 1:	ProcessSubNode(origin, node->sub_space_[a^1], glm::vec3(t0.x, t0.y, tm.z), glm::vec3(tm.x, tm.y, t1.z), nodes, a);
                current_node = GetNextNode(glm::vec3(tm.x, tm.y, t1.z), 5, 3, 8);
                break;

            case 2:	ProcessSubNode(origin, node->sub_space_[a^2], glm::vec3(t0.x, tm.y, t0.z), glm::vec3(tm.x, t1.y, tm.z), nodes, a);
                current_node = GetNextNode(glm::vec3(tm.x, t1.y, tm.z), 6, 8, 3);
                break;

            case 3:	ProcessSubNode(origin, node->sub_space_[a^3], glm::vec3(t0.x, tm.y, tm.z), glm::vec3(tm.x, t1.y, t1.z), nodes, a);
                current_node = GetNextNode(glm::vec3(tm.x, t1.y, t1.z), 7, 8, 8);
                break;

            case 4:	ProcessSubNode(origin, node->sub_space_[a^4], glm::vec3(tm.x, t0.y, t0.z), glm::vec3(t1.x, tm.y, tm.z), nodes, a);
                current_node = GetNextNode(glm::vec3(t1.x, tm.y, tm.z), 8, 6, 5);
                break;

            case 5: ProcessSubNode(origin, node->sub_space_[a^5], glm::vec3(tm.x, t0.y, tm.z), glm::vec3(t1.x, tm.y, t1.z), nodes, a);
                current_node = GetNextNode(glm::vec3(t1.x, tm.y, t1.z), 8, 7, 8);
                break;

            case 6:	ProcessSubNode(origin, node->sub_space_[a^6], glm::vec3(tm.x, tm.y, t0.z), glm::vec3(t1.x, t1.y, tm.z), nodes, a);
                current_node = GetNextNode(glm::vec3(t1.x, t1.y, tm.z), 8, 8, 7);
                break;

            case 7: ProcessSubNode(origin, node->sub_space_[a^7], glm::vec3(tm.x, tm.y, tm.z), glm::vec3(t1.x, t1.y, t1.z), nodes, a);
                current_node = 8;
                break;

            default:
                return;
        }

    } while (current_node < MAX_NODE_COUNT);
}

//
// Function: RayTraverse
// ---------------------------
//
//   Run ray traverse in octree space
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void RayTraverse(OctreeNode* root, glm::vec3 origin, glm::vec3 direction, std::vector<OctreeNode*> &nodes) {
    if (root == nullptr) return;

    // Flag for negative ray components.
    unsigned int a = 0;

    // Calculate the initial ray parameters.
    glm::vec3 min_pos = root->get_min_pos();
    glm::vec3 max_pos = root->get_max_pos();

    // Check ray direction
    if (direction.x < 0.0f){
        origin.x = -origin.x;
        direction.x = -direction.x;
        a |= 4;
    }

    if (direction.y < 0.0f){
        origin.y = -origin.y;
        direction.y = -direction.y;
        a |= 2;
    }

    if (direction.z < 0.0f){
        origin.z = -origin.z;
        direction.z = -direction.z;
        a |= 1;
    }

    glm::vec3 t0 = (min_pos - origin) / direction;
    glm::vec3 t1 = (max_pos - origin) / direction;

    float tmin = glm::max(glm::max(t0.x, t0.y), t0.z);
    float tmax = glm::min(glm::min(t1.x, t1.y), t1.z);

    // Visiting current node, perform actual work here
    if (tmin < tmax) {
        ProcessSubNode(origin, root, t0, t1, nodes, a);
    }
}

}  // namespace raychess

