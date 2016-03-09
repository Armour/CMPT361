//////////////////////////////////////////////////////////////////////////////
//
//  robot_arm.h
//
//  The header file for robot arm manager
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

#ifndef FRUITTETRIS_ROBOT_ARM_H_
#define FRUITTETRIS_ROBOT_ARM_H_

#include "lib_gl.h"
#include "lib_consts.h"

class RobotArm{
private:
    glm::vec4 base_center_;         // The center of the base of the robot arm

    GLfloat base_width_;
    GLfloat base_height_;
    GLfloat lower_arm_width_;       // The width of the lower arm
    GLfloat lower_arm_height_;      // The height of the lower arm
    GLfloat upper_arm_width_;       // The width of the upper arm
    GLfloat upper_arm_height_;      // The height of the upper arm

    GLfloat base_angle_;            // Let's call it delta
    GLfloat lower_arm_angle_;       // Let's call it theta
    GLfloat upper_arm_angle_;       // Let's call it phi

private:
    GLfloat ClampBaseAngle(GLfloat angle) const;            // Clamp the angle of three part
    GLfloat ClampLowerArmAngle(GLfloat angle) const;
    GLfloat ClampUpperArmAngle(GLfloat angle) const;
    void CalculateBasePosition(std::vector<glm::vec4> & base_position, glm::vec4 center_points,
                               GLfloat width, GLfloat height, GLfloat angle) const;
    void CalculateLowerArmPosition(std::vector<glm::vec4> & lower_arm_position, glm::vec4 center_points,
                                   GLfloat width, GLfloat height, GLfloat angle) const;
    void CalculateUpperArmPosition(std::vector<glm::vec4> & upper_arm_position, glm::vec4 center_points,
                                   GLfloat width, GLfloat height, GLfloat angle) const;

public:
    RobotArm(glm::vec4 center): base_center_(center) {      // Constructor
        base_height_ = 30.0;
        base_width_ = 100.0;
        lower_arm_height_ = 380.0;
        lower_arm_width_ = 10.0;
        upper_arm_height_ = 380.0;
        upper_arm_width_ = 6.0;
        base_angle_ = 45.0;
        upper_arm_angle_ = -35.0;
        lower_arm_angle_ = 0.0;
    };

    void ResetAngle();
    void RotateBase(int direction);             // Rotate three part of robot arm
    void RotateLowerArm(int direction);
    void RotateUpperArm(int direction);

    std::vector<glm::vec4> get_base_render_data() const;
    std::vector<glm::vec4> get_lower_arm_render_data() const;
    std::vector<glm::vec4> get_upper_arm_render_data() const;

    glm::mat4x4 get_base_rotation_matrix() const;
    glm::mat4x4 get_lower_rotation_matrix() const;
    glm::mat4x4 get_upper_rotation_matrix() const;

    glm::vec4 get_base_end_point() const;
    glm::vec4 get_lower_arm_end_point() const;
    glm::vec4 get_upper_arm_end_point() const;

    glm::vec4 get_lower_arm_center_point() const;
    glm::vec4 get_upper_arm_center_point() const;
};

#endif //FRUITTETRIS_ROBOT_ARM_H_
