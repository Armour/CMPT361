//////////////////////////////////////////////////////////////////////////////
//
//  smf_parser.h
//
//  The header file for function that import/export mesh data file
//
//  Project         : RayChess
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 2/11/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef RAYCHESS_SMF_PARSER_H_
#define RAYCHESS_SMF_PARSER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "lib_gl.h"
#include "lib_consts.h"
#include "object.h"
#include "triangle.h"
#include "raycast.h"

using namespace std;

namespace smfparser {

// Trim the leading space of a string
void TrimLeadingSpace(string &s);

// Trim the tailing space of a string
void TrimTailingSpace(string &s);

// Import mesh data from file to memory
void ImportMeshFile(raychess::RenderManager *manager, string file_path, float scale, float rotate, glm::vec3 offset, int &index);

}  // namespace smfparser

#endif  // SMFVIEW_SMF_PARSER_H_
