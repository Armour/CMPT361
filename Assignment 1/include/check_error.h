//////////////////////////////////////////////////////////////////////////////
//
//  check_error.h
//
//  The header file for error checking function
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

#ifndef FRUITTETRIS_CHECK_ERROR_H_
#define FRUITTETRIS_CHECK_ERROR_H_

#include "lib_gl.h"

namespace angel {

// Return error message string according to error code
const char *ErrorString(GLenum error);

// Check and return the error message
void CheckError(const char *file, int line);

} // namespace angle

#endif  // FRUITTETRIS_CHECK_ERROR_H_
