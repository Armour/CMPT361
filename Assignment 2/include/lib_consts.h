//////////////////////////////////////////////////////////////////////////////
//
//  lib_consts.h
//
//  The header file for all global constants
//
//  Project         : FruitTetris
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 2/3/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FRUITTETRIS_LIB_CONSTANTS_H_
#define FRUITTETRIS_LIB_CONSTANTS_H_

#include "lib_gl.h"

namespace libconsts {

// Math
const float kDegreeToRadians = M_PI / 180.0;

// Window size
const int kWindowPositionX = 400;
const int kWindowPositionY = 85;
const int kWindowSizeWidth = 800;
const int kWindowSizeHeight = 720;

// Map size
const int kMapSizeWidth = 10;
const int kMapSizeHeight = 20;
const float kMapCubeSize = 33.0f;

// Map block offset
const glm::vec3 kMapCubeOffset[8] = {
    glm::vec3( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, 0.0,                      libconsts::kMapCubeSize / 2),
    glm::vec3((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, 0.0,                      libconsts::kMapCubeSize / 2),
    glm::vec3((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, libconsts::kMapCubeSize,  libconsts::kMapCubeSize / 2),
    glm::vec3( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, libconsts::kMapCubeSize,  libconsts::kMapCubeSize / 2),
    glm::vec3( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, 0.0,                     -libconsts::kMapCubeSize / 2),
    glm::vec3((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, 0.0,                     -libconsts::kMapCubeSize / 2),
    glm::vec3((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, libconsts::kMapCubeSize, -libconsts::kMapCubeSize / 2),
    glm::vec3( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, libconsts::kMapCubeSize, -libconsts::kMapCubeSize / 2)
};

// Cube face
const int kCubeFaceIndex[36] = {1, 0, 3, 1, 3, 2,
                                2, 3, 7, 2, 7, 6,
                                3, 0, 4, 3, 4, 7,
                                6, 5, 1, 6, 1, 2,
                                4, 5, 6, 4, 6, 7,
                                5, 4, 0, 5, 0, 1};

// State
const int kStateOnRobotArm = 0;
const int kStateOnAir      = 1;

// Count
const int kCountShape  = 6;
const int kCountColor  = 7;
const int kCountOrient = 4;
const int kCountCells  = 4;

// Tick
const int kTickEasyMode   = 2000;
const int kTickNormalMode = 1000;
const int kTickHardMode   = 500;
const int kTickInsaneMode = 250;

// Movement
const glm::vec2 kMoveDown(0, -1);
const glm::vec2 kMoveLeft(-1, 0);
const glm::vec2 kMoveRight(1, 0);

// Direction
const int kClockWise      =  1;
const int kAntiClockWise  = -1;

// Boundary & Collision
const int kInBoundary         =  0;
const int kOutOfBoundaryUp    = -1;
const int kOutOfBoundaryDown  = -2;
const int kOutOfBoundaryLeft  = -3;
const int kOutOfBoundaryRight = -4;
const int kCollision          = -5;

// Black and white
const int kColorBlack  = 0;
const int kColorWhite  = 1;

// Color category
const glm::vec4 kColorCategory[kCountColor] = {

    glm::vec4(0.0, 0.0, 0.0, 1.0),  // Black
    glm::vec4(1.0, 1.0, 1.0, 1.0),  // White
    glm::vec4(1.0, 0.0, 0.0, 1.0),  // Red
    glm::vec4(0.0, 1.0, 0.0, 1.0),  // Green
    glm::vec4(1.0, 0.5, 0.0, 1.0),  // Orange
    glm::vec4(1.0, 1.0, 0.0, 1.0),  // Yellow
    glm::vec4(0.5, 0.0, 0.5, 1.0)   // Purple

};

// Shape category
const glm::vec2 kShapeCategory[kCountShape][kCountOrient][kCountCells] = {

    // I Shape
    {
        {glm::vec2(-2,  0), glm::vec2(-1,  0), glm::vec2( 0,  0), glm::vec2( 1,  0)},
        {glm::vec2( 0, -2), glm::vec2( 0, -1), glm::vec2( 0,  0), glm::vec2( 0,  1)},
        {glm::vec2( 1,  0), glm::vec2( 0,  0), glm::vec2(-1,  0), glm::vec2(-2,  0)},
        {glm::vec2( 0,  1), glm::vec2( 0,  0), glm::vec2( 0, -1), glm::vec2( 0, -2)}
    },

    // Left S Shape
    {
        {glm::vec2(-1, -1), glm::vec2(0, -1), glm::vec2(0,  0), glm::vec2( 1,  0)},
        {glm::vec2( 1, -1), glm::vec2(1,  0), glm::vec2(0,  0), glm::vec2( 0,  1)},
        {glm::vec2( 1,  0), glm::vec2(0,  0), glm::vec2(0, -1), glm::vec2(-1, -1)},
        {glm::vec2( 0,  1), glm::vec2(0,  0), glm::vec2(1,  0), glm::vec2( 1, -1)}
    },

    // Right S Shape
    {
        {glm::vec2( 1, -1), glm::vec2(0, -1), glm::vec2(0,  0), glm::vec2(-1,  0)},
        {glm::vec2( 1,  1), glm::vec2(1,  0), glm::vec2(0,  0), glm::vec2( 0, -1)},
        {glm::vec2(-1,  0), glm::vec2(0,  0), glm::vec2(0, -1), glm::vec2( 1, -1)},
        {glm::vec2( 0, -1), glm::vec2(0,  0), glm::vec2(1,  0), glm::vec2( 1,  1)}
    },

    // Left L Shape
    {
        {glm::vec2(-1, -1), glm::vec2(-1,  0), glm::vec2(0, 0), glm::vec2( 1,  0)},
        {glm::vec2( 1, -1), glm::vec2( 0, -1), glm::vec2(0, 0), glm::vec2( 0,  1)},
        {glm::vec2( 1,  1), glm::vec2( 1,  0), glm::vec2(0, 0), glm::vec2(-1,  0)},
        {glm::vec2(-1,  1), glm::vec2( 0,  1), glm::vec2(0, 0), glm::vec2( 0, -1)}
    },

    // Right L Shape
    {
        {glm::vec2(-1,  1), glm::vec2(-1,  0), glm::vec2(0, 0), glm::vec2( 1,  0)},
        {glm::vec2(-1, -1), glm::vec2( 0, -1), glm::vec2(0, 0), glm::vec2( 0,  1)},
        {glm::vec2( 1, -1), glm::vec2( 1,  0), glm::vec2(0, 0), glm::vec2(-1,  0)},
        {glm::vec2( 1,  1), glm::vec2( 0,  1), glm::vec2(0, 0), glm::vec2( 0, -1)}
    },

    // T Shape
    {
        {glm::vec2( 0, -1), glm::vec2(-1,  0), glm::vec2(0, 0), glm::vec2( 1,  0)},
        {glm::vec2( 1,  0), glm::vec2( 0, -1), glm::vec2(0, 0), glm::vec2( 0,  1)},
        {glm::vec2( 0,  1), glm::vec2( 1,  0), glm::vec2(0, 0), glm::vec2(-1,  0)},
        {glm::vec2(-1,  0), glm::vec2( 0,  1), glm::vec2(0, 0), glm::vec2( 0, -1)}
    }

};

} // namespace libconsts

#endif //FRUITTETRIS_LIB_CONSTANTS_H_
