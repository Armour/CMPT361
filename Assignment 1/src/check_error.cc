//////////////////////////////////////////////////////////////////////////////
//
//  check_error.cc
//
//  The source file for error checking function
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

#include "check_error.h"
#include <iostream>

namespace angel {

//
// Function: ErrorString
// ---------------------------
//
//   Return error message string according to error code
//
//   Parameters:
//       error: the error code
//
//   Returns:
//       the error message string
//

const char *ErrorString(GLenum error) {
    const char *msg;
    switch (error) {
#define Case(Token) case Token: msg = #Token; break;
        Case(GL_NO_ERROR);
        Case(GL_INVALID_VALUE);
        Case(GL_INVALID_ENUM);
        Case(GL_INVALID_OPERATION);
        Case(GL_STACK_OVERFLOW);
        Case(GL_STACK_UNDERFLOW);
        Case(GL_OUT_OF_MEMORY);
#undef Case
    }
    return msg;
}

//
// Function: CheckError
// -------------------
//
//   Check and return the error message
//
//   Parameters:
//      file: the name of the file that need to check
//      line: the line number of that file
//
//   Returns:
//      void
//

void CheckError(const char *file, int line) {
    GLenum error = glGetError();
    do {
        fprintf(stderr, "[%s:%d] %s\n", file, line, ErrorString(error));
    } while ((error = glGetError()) != GL_NO_ERROR);
}

} // namespace angel
