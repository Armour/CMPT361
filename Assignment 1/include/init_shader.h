//////////////////////////////////////////////////////////////////////////////
//
//  init_shader.h
//
//  The header file for offering an easy way to init vertex and fragment shaders
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

#ifndef FRUITTETRIS_INIT_SHADER_H_
#define FRUITTETRIS_INIT_SHADER_H_

#include "lib_gl.h"

namespace angel {

// Helper function to read a shader source file and return its content
char *ReadShaderSource(const char *shader_file);

// Helper function to load vertex and fragment shader files
GLuint InitShader(const char* vertex_shader_file, const char* fragment_shader_file);

} // namespace angle

#endif  // FRUITTETRIS_INIT_SHADER_H_
