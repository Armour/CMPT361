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
const int kWindowSizeWidth = 650;
const int kWindowSizeHeight = 800;

// Map size
const int kMapSizeWidth = 10;
const int kMapSizeHeight = 20;
const float kMapCubeSize = 33.0f;

// Map block offset
const glm::vec4 kMapCubeOffset[8] = {
    glm::vec4( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, 0.0,                      libconsts::kMapCubeSize / 2, 0.0),
    glm::vec4((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, 0.0,                      libconsts::kMapCubeSize / 2, 0.0),
    glm::vec4((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, libconsts::kMapCubeSize,  libconsts::kMapCubeSize / 2, 0.0),
    glm::vec4( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, libconsts::kMapCubeSize,  libconsts::kMapCubeSize / 2, 0.0),
    glm::vec4( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, 0.0,                     -libconsts::kMapCubeSize / 2, 0.0),
    glm::vec4((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, 0.0,                     -libconsts::kMapCubeSize / 2, 0.0),
    glm::vec4((-libconsts::kMapSizeWidth / 2 + 1) * libconsts::kMapCubeSize, libconsts::kMapCubeSize, -libconsts::kMapCubeSize / 2, 0.0),
    glm::vec4( -libconsts::kMapSizeWidth / 2      * libconsts::kMapCubeSize, libconsts::kMapCubeSize, -libconsts::kMapCubeSize / 2, 0.0)
};

// Cube face
const int kCubeFaceIndex[36] = {1, 3, 0, 1, 2, 3,
                                2, 7, 3, 2, 6, 7,
                                3, 4, 0, 3, 7, 4,
                                6, 1, 5, 6, 2, 1,
                                4, 6, 5, 4, 7, 6,
                                5, 0, 4, 5, 1, 0};

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
const int kOutOfBoundaryFront = -5;
const int kOutOfBoundaryBack  = -6;
const int kCollision          = -7;

// Black and white index
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

// Special color : gray
const glm::vec4 kColorGray = glm::vec4(0.5, 0.5, 0.5, 1.0);     // Used indicate tile can not be placed

// Shape category
const glm::vec3 kShapeCategory[kCountShape][kCountOrient][kCountCells] = {

    // I Shape
    {
        {glm::vec3(-2,  0, 0), glm::vec3(-1,  0, 0), glm::vec3( 0,  0, 0), glm::vec3( 1,  0, 0)},
        {glm::vec3( 0, -2, 0), glm::vec3( 0, -1, 0), glm::vec3( 0,  0, 0), glm::vec3( 0,  1, 0)},
        {glm::vec3( 1,  0, 0), glm::vec3( 0,  0, 0), glm::vec3(-1,  0, 0), glm::vec3(-2,  0, 0)},
        {glm::vec3( 0,  1, 0), glm::vec3( 0,  0, 0), glm::vec3( 0, -1, 0), glm::vec3( 0, -2, 0)}
    },

    // Left S Shape
    {
        {glm::vec3(-1, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0,  0, 0), glm::vec3( 1,  0, 0)},
        {glm::vec3( 1, -1, 0), glm::vec3(1,  0, 0), glm::vec3(0,  0, 0), glm::vec3( 0,  1, 0)},
        {glm::vec3( 1,  0, 0), glm::vec3(0,  0, 0), glm::vec3(0, -1, 0), glm::vec3(-1, -1, 0)},
        {glm::vec3( 0,  1, 0), glm::vec3(0,  0, 0), glm::vec3(1,  0, 0), glm::vec3( 1, -1, 0)}
    },

    // Right S Shape
    {
        {glm::vec3( 1, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0,  0, 0), glm::vec3(-1,  0, 0)},
        {glm::vec3( 1,  1, 0), glm::vec3(1,  0, 0), glm::vec3(0,  0, 0), glm::vec3( 0, -1, 0)},
        {glm::vec3(-1,  0, 0), glm::vec3(0,  0, 0), glm::vec3(0, -1, 0), glm::vec3( 1, -1, 0)},
        {glm::vec3( 0, -1, 0), glm::vec3(0,  0, 0), glm::vec3(1,  0, 0), glm::vec3( 1,  1, 0)}
    },

    // Left L Shape
    {
        {glm::vec3(-1, -1, 0), glm::vec3(-1,  0, 0), glm::vec3(0, 0, 0), glm::vec3( 1,  0, 0)},
        {glm::vec3( 1, -1, 0), glm::vec3( 0, -1, 0), glm::vec3(0, 0, 0), glm::vec3( 0,  1, 0)},
        {glm::vec3( 1,  1, 0), glm::vec3( 1,  0, 0), glm::vec3(0, 0, 0), glm::vec3(-1,  0, 0)},
        {glm::vec3(-1,  1, 0), glm::vec3( 0,  1, 0), glm::vec3(0, 0, 0), glm::vec3( 0, -1, 0)}
    },

    // Right L Shape
    {
        {glm::vec3(-1,  1, 0), glm::vec3(-1,  0, 0), glm::vec3(0, 0, 0), glm::vec3( 1,  0, 0)},
        {glm::vec3(-1, -1, 0), glm::vec3( 0, -1, 0), glm::vec3(0, 0, 0), glm::vec3( 0,  1, 0)},
        {glm::vec3( 1, -1, 0), glm::vec3( 1,  0, 0), glm::vec3(0, 0, 0), glm::vec3(-1,  0, 0)},
        {glm::vec3( 1,  1, 0), glm::vec3( 0,  1, 0), glm::vec3(0, 0, 0), glm::vec3( 0, -1, 0)}
    },

    // T Shape
    {
        {glm::vec3( 0, -1, 0), glm::vec3(-1,  0, 0), glm::vec3(0, 0, 0), glm::vec3( 1,  0, 0)},
        {glm::vec3( 1,  0, 0), glm::vec3( 0, -1, 0), glm::vec3(0, 0, 0), glm::vec3( 0,  1, 0)},
        {glm::vec3( 0,  1, 0), glm::vec3( 1,  0, 0), glm::vec3(0, 0, 0), glm::vec3(-1,  0, 0)},
        {glm::vec3(-1,  0, 0), glm::vec3( 0,  1, 0), glm::vec3(0, 0, 0), glm::vec3( 0, -1, 0)}
    }

};

} // namespace libconsts

#endif //FRUITTETRIS_LIB_CONSTANTS_H_
