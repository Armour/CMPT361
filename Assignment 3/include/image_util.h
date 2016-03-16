//////////////////////////////////////////////////////////////////////////////
//
//  image_util.h
//
//  The header file for image util functions
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

#ifndef RAYCHESS_IMAGE_UTIL_H_
#define RAYCHESS_IMAGE_UTIL_H_

#include "lib_consts.h"

namespace image {

// This function saves the current image to a .bmp file
void SaveImage(void);

// This function normalizes the frame resulting from ray tracing so that the maximum R, G, or B value is 1.0
void HistogramNormalization(void);

}  // namespace image

#endif  // RAYCHESS_IMAGE_UTIL_H_
