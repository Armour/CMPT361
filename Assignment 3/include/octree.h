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
#include "sphere.h"

namespace raychess {

enum NodePosition {             // The octree node position
    FAR_BOTTOM_LEFT = 0,
    NEAR_BOTTOM_LEFT = 1,
    FAR_TOP_LEFT = 2,
    NEAR_TOP_LEFT = 3,
    FAR_BOTTOM_RIGHT = 4,
    NEAR_BOTTOM_RIGHT = 5,
    FAR_TOP_RIGHT = 6,
    NEAR_TOP_RIGHT = 7,
    MAX_NODE_COUNT = 8          // Any defined node should be < 8 && > 0
};

class OctreeNode {
private:
    bool is_leaf_;              // If the octree node is leaf (termination)
    glm::vec3 min_pos_;         // The minimum position of the space
    glm::vec3 max_pos_;         // The maximum position of the space

public:
    OctreeNode() {};
    OctreeNode(glm::vec3 min, glm::vec3 max);
    ~OctreeNode() {};

    OctreeNode *sub_space_[MAX_NODE_COUNT];
    std::vector<Object *> objects_;

    inline bool get_leaf(void) const { return is_leaf_; };
    inline glm::vec3 get_min_pos(void) const { return min_pos_; };
    inline glm::vec3 get_max_pos(void) const { return max_pos_; };

    void SetRange(glm::vec3 min_pos, glm::vec3 max_pos);        // The the range of the node
    void AddObject(Object *object);             // Add one object into the objects_
    void SplitSpace(int step);          // Split the node into eight sub spaces
};

// This function used to get the first node to be entered by the ray
unsigned int GetFirstNode(glm::vec3 t0, glm::vec3 tm);

// This function used for calculating the exit plane and the next node, once the current parent node is exited
unsigned int GetNextNode(glm::vec3 tm, unsigned int x, unsigned int y, unsigned int z);

// This function used to process ray casting in sub node of one node
void ProcessSubNode(glm::vec3 origin, OctreeNode* node, glm::vec3 t0, glm::vec3 t1, std::vector<OctreeNode*> &nodes, unsigned int a);

// Run ray traverse in octree space
void RayTraverse(OctreeNode* root, glm::vec3 origin, glm::vec3 direction, std::vector<OctreeNode*> &nodes);

}  // namespace raychess

#endif  // RAYCHESS_OCTREE_H_
