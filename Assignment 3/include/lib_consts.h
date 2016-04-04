//////////////////////////////////////////////////////////////////////////////
//
//  lib_consts.h
//
//  The header file for all global constants
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

#ifndef RAYCHESS_LIB_CONSTANTS_H_
#define RAYCHESS_LIB_CONSTANTS_H_

#include "lib_gl.h"

namespace libconsts {

// Math
const float kDegreeToRadians = (float)M_PI / 180.0f;

// Window size
const int kWindowPositionX  = 400;
const int kWindowPositionY  = 120;
const int kWindowSizeWidth  = 512;
const int kWindowSizeHeight = 512;

// Image size
const int kImageWidth = 5;
const int kImageHeight = kWindowSizeHeight / kWindowSizeWidth * kImageWidth;

// Image plane position
const float kImagePlanePosZ = -1.5f;

// Object type
const int kTypeSphere = 1;
const int kTypeTriangle = 2;

// Max distance
const int kMaxDistance = 10000;

// Flag indicate input is vertex or face data
const int kFlagVertex = 0;
const int kFlagFace   = 1;

// Error epsilon
const float kErrorEpsilon = 0.001f;

// The eye position
const glm::vec3 kEyePosition = {0.0, 0.0, 0.0};

// Color category
const glm::vec3 kColorBlack = {0.0, 0.0, 0.0};
const glm::vec3 kColorWhite = {1.0, 1.0, 1.0};

// Chessboard property
const int kChessBoardWidth = 8;
const int kChessBoardHeight = 8;
const float kChessBoardGridWidth = 2.0f;
const glm::vec3 kChessBoardOffset = {-8.0f, -2.5f, -16.0f};

// Diffuse reflected light number
const int kDiffuseReflectNumber = 5;

// Antialiasing offset
const glm::vec3 kAntialiasingOffset[4] = {
    { 0.25f,  0.25f, 0.0f},
    {-0.25f,  0.25f, 0.0f},
    { 0.25f, -0.25f, 0.0f},
    {-0.25f, -0.25f, 0.0f}
};

}  // namespace libconsts

#endif  //RAYCHESS_LIB_CONSTANTS_H_
