//////////////////////////////////////////////////////////////////////////////
//
//  scene.h
//
//  The header file for scene setup
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

#ifndef RAYCHESS_SCENE_H_
#define RAYCHESS_SCENE_H_

#include "raycast.h"

namespace raychess {

// This function set up the default scene
void SetUpDefaultScene(RenderManager *manager);

// This function set up my own scene
void SetUpUserScene(RenderManager *manager);

}  // namespace raychess

#endif  // RAYCHESS_SCENE_H_
