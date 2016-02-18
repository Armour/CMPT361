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

// Window size
const int kWindowPositionX = 680;
const int kWindowPositionY = 178;

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
