//////////////////////////////////////////////////////////////////////////////
//
//  robot_arm.cc
//
//  The source file for robot arm manager
//
//  Project         : FruitTetris
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  Created by Armour on 3/5/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "robot_arm.h"

//
// Function: ResetAngle
// ---------------------------
//
//   Reset all the angle parameters
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void RobotArm::ResetAngle() {
    base_angle_ = 45.0;
    upper_arm_angle_ = -35.0;
    lower_arm_angle_ = 0.0;
}

//
// Function: RotateBase
// ---------------------------
//
//   Rotate the base part
//
//   Parameters:
//       direction: the direction you want to rotate
//
//   Returns:
//       void
//

void RobotArm::RotateBase(int direction) {
    base_angle_ += 2.0 * (GLfloat)direction;
    base_angle_ = RobotArm::ClampBaseAngle(base_angle_);
}

//
// Function: RotateLowerArm
// ---------------------------
//
//   Rotate the lower arm part
//
//   Parameters:
//       direction: the direction you want to rotate
//
//   Returns:
//       void
//

void RobotArm::RotateLowerArm(int direction) {
    lower_arm_angle_ += (GLfloat)direction;
    lower_arm_angle_ = RobotArm::ClampLowerArmAngle(lower_arm_angle_);
}

//
// Function: RotateUpperArm
// ---------------------------
//
//   Rotate the upper arm part
//
//   Parameters:
//       direction: the direction you want to rotate
//
//   Returns:
//       void
//

void RobotArm::RotateUpperArm(int direction) {
    upper_arm_angle_ += (GLfloat)direction;
    upper_arm_angle_ = RobotArm::ClampUpperArmAngle(upper_arm_angle_);
}

//
// Function: ClampBaseAngle
// ---------------------------
//
//   Clamp the angle of the base part
//
//   Parameters:
//       angle: the angle that we need to clamp
//
//   Returns:
//       void
//

GLfloat RobotArm::ClampBaseAngle(GLfloat angle) const {
    if (angle > 90.0) return 90.0;
    if (angle < 0.0) return 0.0;
    return angle;
}

//
// Function: ClampLowerArmAngle
// ---------------------------
//
//   Clamp the angle of the lower arm part
//
//   Parameters:
//       angle: the angle that we need to clamp
//
//   Returns:
//       void
//

GLfloat RobotArm::ClampLowerArmAngle(GLfloat angle) const {
    if (angle > 45.0) return 45.0;
    if (angle < -20.0) return -20.0;
    return angle;
}

//
// Function: ClampUpperArmAngle
// ---------------------------
//
//   Clamp the angle of the upper arm part
//
//   Parameters:
//       angle: the angle that we need to clamp
//
//   Returns:
//       void
//

GLfloat RobotArm::ClampUpperArmAngle(GLfloat angle) const {
    if (angle > 80.0) return 80.0;
    if (angle < -90.0) return -90.0;
    return angle;
}

//
// Function: GetBaseRotationMatrix
// ---------------------------
//
//   Get the rotation matrix of base part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::mat4x4 RobotArm::get_base_rotation_matrix() const {
    GLfloat delta = (base_angle_ - 45) * libconsts::kDegreeToRadians;
    glm::mat4x4 rotationY = glm::mat4x4( cos(delta), 0.0, sin(delta), 0.0,
                                            0.0    , 1.0,    0.0    , 0.0,
                                        -sin(delta), 0.0, cos(delta), 0.0,
                                            0.0    , 0.0,    0.0    , 1.0);
    return rotationY;
}

//
// Function: GetLowerRotationMatrix
// ---------------------------
//
//   Get the rotation matrix of lower part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::mat4x4 RobotArm::get_lower_rotation_matrix() const {
    GLfloat theta = lower_arm_angle_ * libconsts::kDegreeToRadians;
    glm::mat4x4 rotationZ = glm::mat4x4(cos(theta), -sin(theta), 0.0, 0.0,
                                        sin(theta),  cos(theta), 0.0, 0.0,
                                            0.0   ,     0.0    , 1.0, 0.0,
                                            0.0   ,     0.0    , 0.0, 1.0);
    return rotationZ;
}

//
// Function: GetUpperRotationMatrix
// ---------------------------
//
//   Get the rotation matrix of upper part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::mat4x4 RobotArm::get_upper_rotation_matrix() const {
    GLfloat phi = upper_arm_angle_ * libconsts::kDegreeToRadians;
    glm::mat4x4 rotationZ = glm::mat4x4(cos(phi), -sin(phi), 0.0, 0.0,
                                        sin(phi),  cos(phi), 0.0, 0.0,
                                          0.0   ,     0.0  , 1.0, 0.0,
                                          0.0   ,     0.0  , 0.0, 1.0);
    return rotationZ;
}

//
// Function: GetBaseEndPoint
// ---------------------------
//
//   Get the end point of base part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::vec4 RobotArm::get_base_end_point() const {
    return base_center_ + glm::vec4(0.0, base_height_ / 2, 0.0, 0.0);
}

//
// Function: GetLowerArmEndPoint
// ---------------------------
//
//   Get the end point of lower part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::vec4 RobotArm::get_lower_arm_end_point() const {
    GLfloat theta = lower_arm_angle_ * libconsts::kDegreeToRadians;
    glm::mat4x4 rotationY = get_base_rotation_matrix();
    return get_base_end_point() + rotationY * glm::vec4(lower_arm_height_ * sin(theta), lower_arm_height_ * cos(theta), 0, 0.0);
}

//
// Function: GetUpperArmEndPoint
// ---------------------------
//
//   Get the end point of upper part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::vec4 RobotArm::get_upper_arm_end_point() const {
    GLfloat phi = -upper_arm_angle_ * libconsts::kDegreeToRadians;
    glm::mat4x4 rotationZ = get_lower_rotation_matrix();
    glm::mat4x4 rotationY = get_base_rotation_matrix();
    return get_lower_arm_end_point() + rotationY * rotationZ * glm::vec4(upper_arm_height_ * cos(phi), upper_arm_height_ * sin(phi), 0, 0.0);
}

//
// Function: GetLowerArmCenterPoint
// ---------------------------
//
//   Get the center point of lower part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::vec4 RobotArm::get_lower_arm_center_point() const {
    GLfloat theta = lower_arm_angle_ * libconsts::kDegreeToRadians;
    glm::mat4x4 rotationY = get_base_rotation_matrix();
    return get_base_end_point() + rotationY * glm::vec4(lower_arm_height_ / 2 * sin(theta), lower_arm_height_ / 2 * cos(theta), 0, 0.0);
}

//
// Function: GetUpperArmCenterPoint
// ---------------------------
//
//   Get the center point of upper part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

glm::vec4 RobotArm::get_upper_arm_center_point() const {
    GLfloat phi = -upper_arm_angle_ * libconsts::kDegreeToRadians;
    glm::mat4x4 rotationZ = get_lower_rotation_matrix();
    glm::mat4x4 rotationY = get_base_rotation_matrix();
    return get_lower_arm_end_point() + rotationY * rotationZ * glm::vec4(upper_arm_height_ / 2 * cos(phi), upper_arm_height_ / 2 * sin(phi), 0, 0.0);
}

//
// Function: GetBaseRenderData
// ---------------------------
//
//   Get the render data of base part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

std::vector<glm::vec4> RobotArm::get_base_render_data() const {
    std::vector<glm::vec4> base_render_data;
    std::vector<glm::vec4> base_position;
    CalculateBasePosition(base_position, base_center_, base_width_, base_height_, base_angle_);
    for (int i = 0; i < 36; i++) {
        base_render_data.push_back(base_position[libconsts::kCubeFaceIndex[i]]);
    }
    return base_render_data;
}

//
// Function: GetLowerArmRenderData
// ---------------------------
//
//   Get the render data of lower part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

std::vector<glm::vec4> RobotArm::get_lower_arm_render_data() const {
    std::vector<glm::vec4> lower_arm_render_data;
    std::vector<glm::vec4> lower_arm_position;
    glm::vec4 lower_arm_center = get_lower_arm_center_point();
    CalculateLowerArmPosition(lower_arm_position, lower_arm_center, lower_arm_width_, lower_arm_height_, lower_arm_angle_);
    for (int i = 0; i < 36; i++) {
        lower_arm_render_data.push_back(lower_arm_position[libconsts::kCubeFaceIndex[i]]);
    }
    return lower_arm_render_data;
}

//
// Function: GetUpperArmRenderData
// ---------------------------
//
//   Get the render data of upper part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

std::vector<glm::vec4> RobotArm::get_upper_arm_render_data() const {
    std::vector<glm::vec4> upper_arm_render_data;
    std::vector<glm::vec4> upper_arm_position;
    glm::vec4 upper_arm_center = get_upper_arm_center_point();
    CalculateUpperArmPosition(upper_arm_position, upper_arm_center, upper_arm_width_, upper_arm_height_, upper_arm_angle_);
    for (int i = 0; i < 36; i++) {
        upper_arm_render_data.push_back(upper_arm_position[libconsts::kCubeFaceIndex[i]]);
    }
    return upper_arm_render_data;
}

//
// Function: CalculateBasePosition
// ---------------------------
//
//   Calculate the cube points position of the base part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void RobotArm::CalculateBasePosition(std::vector<glm::vec4> &base_position, glm::vec4 center_points,
                                     GLfloat width, GLfloat height, GLfloat angle) const {
    GLfloat r = width / 2 * (GLfloat)sqrt(2.0);
    GLfloat delta = angle * libconsts::kDegreeToRadians;
    base_position.push_back(center_points + glm::vec4(r * -cos(delta), -height / 2, r * -sin(delta), 0.0));
    base_position.push_back(center_points + glm::vec4(r *  sin(delta), -height / 2, r * -cos(delta), 0.0));
    base_position.push_back(center_points + glm::vec4(r *  cos(delta), -height / 2, r *  sin(delta), 0.0));
    base_position.push_back(center_points + glm::vec4(r * -sin(delta), -height / 2, r *  cos(delta), 0.0));
    base_position.push_back(center_points + glm::vec4(r * -cos(delta),  height / 2, r * -sin(delta), 0.0));
    base_position.push_back(center_points + glm::vec4(r *  sin(delta),  height / 2, r * -cos(delta), 0.0));
    base_position.push_back(center_points + glm::vec4(r *  cos(delta),  height / 2, r *  sin(delta), 0.0));
    base_position.push_back(center_points + glm::vec4(r * -sin(delta),  height / 2, r *  cos(delta), 0.0));
}

//
// Function: CalculateLowerArmPosition
// ---------------------------
//
//   Calculate the cube points position of the lower part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void RobotArm::CalculateLowerArmPosition(std::vector<glm::vec4> &lower_arm_position, glm::vec4 center_points,
                                         GLfloat width, GLfloat height, GLfloat angle) const {
    GLfloat h = height / 2;
    GLfloat w = width / 2;
    GLfloat r = sqrt(h * h + w * w);
    GLfloat reletive_theta = w / h * 45 * libconsts::kDegreeToRadians;

    glm::mat4x4 rotationY = get_base_rotation_matrix();
    glm::mat4x4 rotationZ = get_lower_rotation_matrix();

    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r * -sin(reletive_theta), r * -cos(reletive_theta),  width / 2, 0.0));
    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r *  sin(reletive_theta), r * -cos(reletive_theta),  width / 2, 0.0));
    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r *  sin(reletive_theta), r *  cos(reletive_theta),  width / 2, 0.0));
    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r * -sin(reletive_theta), r *  cos(reletive_theta),  width / 2, 0.0));
    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r * -sin(reletive_theta), r * -cos(reletive_theta), -width / 2, 0.0));
    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r *  sin(reletive_theta), r * -cos(reletive_theta), -width / 2, 0.0));
    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r *  sin(reletive_theta), r *  cos(reletive_theta), -width / 2, 0.0));
    lower_arm_position.push_back(center_points + rotationY * rotationZ *
                                 glm::vec4(r * -sin(reletive_theta), r *  cos(reletive_theta), -width / 2, 0.0));
}

//
// Function: CalculateUpperArmPosition
// ---------------------------
//
//   Calculate the cube points position of the upper part
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void RobotArm::CalculateUpperArmPosition(std::vector<glm::vec4> &upper_arm_position, glm::vec4 center_points,
                                         GLfloat width, GLfloat height, GLfloat angle) const {
    GLfloat h = height / 2;
    GLfloat w = width / 2;
    GLfloat r = sqrt(h * h + w * w);
    GLfloat reletive_theta = w / h * 45 * libconsts::kDegreeToRadians;

    glm::mat4x4 rotationY = get_base_rotation_matrix();
    glm::mat4x4 rotationZ1 = get_lower_rotation_matrix();
    glm::mat4x4 rotationZ2 = get_upper_rotation_matrix();

    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r * -cos(reletive_theta), r * -sin(reletive_theta),  width / 2, 0.0));
    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r * cos(reletive_theta), r * -sin(reletive_theta),  width / 2, 0.0));
    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r *  cos(reletive_theta), r *  sin(reletive_theta),  width / 2, 0.0));
    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r * -cos(reletive_theta), r *  sin(reletive_theta),  width / 2, 0.0));
    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r * -cos(reletive_theta), r * -sin(reletive_theta), -width / 2, 0.0));
    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r *  cos(reletive_theta), r * -sin(reletive_theta), -width / 2, 0.0));
    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r *  cos(reletive_theta), r *  sin(reletive_theta), -width / 2, 0.0));
    upper_arm_position.push_back(center_points + rotationY * rotationZ1 * rotationZ2 *
                                 glm::vec4(r * -cos(reletive_theta), r *  sin(reletive_theta), -width / 2, 0.0));
}