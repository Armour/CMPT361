//////////////////////////////////////////////////////////////////////////////
//
//  angel.h
//
//  The main header file for all examples from Angel 6th Edition
//
//  Project         : FruitTetris
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 2/1/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef FRUITTETRIS_ANGEL_H_
#define FRUITTETRIS_ANGEL_H_

// Include system headers
#include <cmath>
#include <iostream>
#include <cstdio>

// Include OpenGL header
#include "lib_gl.h"

// Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

// Create new namespace angel
namespace angel {

// Defined constant for when numbers are too small to be used in the denominator of a division operation
const GLfloat kDivideByZeroTolerance = GLfloat(1.0e-07);

// Degrees-to-radians constant
const GLfloat kDegreesToRadians = GLfloat(M_PI / 180.0);

} // namespace angel

// Include other headers
#include "init_shader.h"
#include "check_error.h"
#include "lib_consts.h"

#endif // FRUITTETRIS_ANGEL_H_
