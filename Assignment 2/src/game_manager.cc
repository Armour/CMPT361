//////////////////////////////////////////////////////////////////////////////
//
//  game_manager.cc
//
//  The source file of the game manager
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

#include "game_manager.h"
#include <iostream>

//
// Function: Init
// ---------------------------
//
//   Init game manager
//
//   Parameters:
//       width: the width of the window
//       height: the height of the window
//
//   Returns:
//       void
//

void GameManager::Init(int x, int y, int z) {
    map_size_.x = x;
    map_size_.y = y;
    map_size_.z = z;
    map_ = std::vector<std::vector<std::vector<int>>>(z, std::vector<std::vector<int>>(x, std::vector<int>(y, 0)));
    while (!game_states_.empty()) {
        game_states_.pop();
    }
    game_states_.push(GameState::GameStateNormal);
    tick_interval_ = libconsts::kTickNormalMode;
}

//
// Function: AddNewTile
// ---------------------------
//
//   Generate new tile
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::AddNewTile() {
    tile_current_position_.x = (int)spawn_point_.x;         // Spawn tile in spawn point
    tile_current_position_.y = (int)spawn_point_.y;
    tile_current_position_.z = (int)spawn_point_.z;

    tile_current_state_ = libconsts::kStateOnRobotArm;      // Set initial property
    tile_current_orient_Y = rand() % libconsts::kCountOrient;
    tile_current_orient_Z = rand() % libconsts::kCountOrient;
    tile_current_shape_ = rand() % libconsts::kCountShape;
    tile_count_down = 9;

    for (int i = 0; i < libconsts::kCountCells; i++) {
        tile_current_color_[i] = rand() % (libconsts::kCountColor - 2) + 2;    // Except black and white
    }
}

//
// Function: CalculateFitPosition
// ---------------------------
//
//   Get the block that fits the end point best
//
//   Parameters:
//       end_point: the point in global axis
//
//   Returns:
//       the block position in map axis
//

glm::vec3 GameManager::CalculateFitPosition(glm::vec4 end_point) {
    float x = end_point.x / libconsts::kMapCubeSize + libconsts::kMapSizeWidth / 2;
    float y = end_point.y / libconsts::kMapCubeSize;
    float z = end_point.z / libconsts::kMapCubeSize + map_size_.z / 2;
    return (glm::vec3(floorf(x), floorf(y), floorf(z)));
}

//
// Function: Tick
// ---------------------------
//
//   Tick function for timer callback
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Tick() {
    if (tile_current_state_ == libconsts::kStateOnAir) {
        if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
            int drop_state = DropOneBlock();
            if (drop_state == libconsts::kOutOfBoundaryDown || drop_state == libconsts::kCollision) {       // Check if can not drop
                FillTileToMap();
                CheckElimination();
                AddNewTile();
            }
        }
    } else {
        if (tile_count_down == 0) {
            tile_current_state_ = libconsts::kStateOnAir;
            if (CheckBoundary() != libconsts::kInBoundary || CheckCollision()) {
                game_states_.push(GameState::GameStateEnd);
            }
        } else {
            if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
                tile_count_down--;
            }
        }
    }
}

//
// Function: Easy
// ---------------------------
//
//   Change to easy mode
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Easy(){
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateEasy);
        tick_interval_ = libconsts::kTickEasyMode;
    }
}

//
// Function: Normal
// ---------------------------
//
//   Change to normal mode
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Normal() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateNormal);
        tick_interval_ = libconsts::kTickNormalMode;
    }
}

//
// Function: Hard
// ---------------------------
//
//   Change to hard mode
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Hard() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateHard);
        tick_interval_ = libconsts::kTickHardMode;
    }
}

//
// Function: Insane
// ---------------------------
//
//   Change to insane mode
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Insane() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateInsane);
        tick_interval_ = libconsts::kTickInsaneMode;
    }
}

//
// Function: Pause
// ---------------------------
//
//   Pause the game
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Pause() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        game_states_.push(GameState::GameStatePause);
    }
}

//
// Function: Resume
// ---------------------------
//
//   Resume the game
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Resume() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() == GameState::GameStatePause) {
        game_states_.pop();
        if (game_states_.empty()) {
            game_states_.push(GameState::GameStateNormal);
        }
    }
}

//
// Function: ChangeGameMode
// ---------------------------
//
//   Change game mode to specific state
//
//   Parameters:
//       state: the state that we want to change to
//
//   Returns:
//       void
//

void GameManager::ChangeGameMode(GameState state){
    if (get_game_state() != GameState::GameStateEnd) {
        GameState last_state = GameState::GameStateEmpty;

        if (!game_states_.empty()) {        // Get last state
            last_state = game_states_.top();
            game_states_.pop();
        }

        game_states_.push(state);       // Push new state

        if (last_state == GameState::GameStatePause) {      // If last state is pause, we still need pause
            game_states_.push(last_state);
        }
    }
}

//
// Function: MoveTile
// ---------------------------
//
//   Move tile with direction
//
//   Parameters:
//       direction: the direction that we want to move the tile
//
//   Returns:
//       void
//

int GameManager::MoveTile(glm::vec2 direction) {
    if (get_game_state() != GameState::GameStatePause) {
        tile_current_position_ += glm::vec3(direction, 0.0);
        if (CheckCollision()) {         // Check collision
            tile_current_position_ -= glm::vec3(direction, 0.0);
            return libconsts::kCollision;
        }
        int boundary_state = CheckBoundary();       // Check boundary
        if (boundary_state != libconsts::kInBoundary) {
            tile_current_position_ -= glm::vec3(direction, 0.0);
            return boundary_state;
        }
    }
    return libconsts::kInBoundary;
}

//
// Function: RotateTile
// ---------------------------
//
//   Rotate tile with direction
//
//   Parameters:
//       direction: the direction that we want to rotate the tile
//
//   Returns:
//       void
//

int GameManager::RotateTile(int rotation_axis, int direction) {
    int *tile_current_orient;
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        if (rotation_axis == libconsts::kRotationAxisY) {
            tile_current_orient = &tile_current_orient_Y;
        } else {
            tile_current_orient = &tile_current_orient_Z;
        }
        *tile_current_orient += direction;
        *tile_current_orient %= libconsts::kCountOrient;
        if (tile_current_state_ == libconsts::kStateOnRobotArm)
            return libconsts::kInBoundary;
        if (CheckCollision()) {                     // Check collision
            *tile_current_orient -= direction;
            if (*tile_current_orient < 0)
                *tile_current_orient = libconsts::kCountOrient - 1;
            *tile_current_orient %= libconsts::kCountOrient;
            return libconsts::kCollision;
        }
        int boundary_state = CheckBoundary();       // Check boundary
        if (boundary_state != libconsts::kInBoundary) {
            *tile_current_orient -= direction;
            if (*tile_current_orient < 0)
                *tile_current_orient = libconsts::kCountOrient - 1;
            *tile_current_orient %= libconsts::kCountOrient;
            return boundary_state;
        }
    }
    return libconsts::kInBoundary;
}

//
// Function: UpdateTilePosition
// ---------------------------
//
//   Update current tile position if in robot arm
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::UpdateTilePosition() {
    tile_current_position_.x = spawn_point_.x;
    tile_current_position_.y = spawn_point_.y;
    tile_current_position_.z = spawn_point_.z;
}

//
// Function: FillTileToMap
// ---------------------------
//
//   Fill current tile color to map
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::FillTileToMap(){
    for (int i = 0; i < libconsts::kCountCells; i++) {
        int x = tile_current_position_.x + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].x;
        int y = tile_current_position_.y + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].y;
        int z = tile_current_position_.z + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].z;
        if (x >= 0 && x < map_size_.x && y >= 0 && y < map_size_.y && z >= 0 && z < map_size_.z) {
            map_[z][x][y] = tile_current_color_[i];
        }
    }
}

//
// Function: CheckElimination
// ---------------------------
//
//   Check if there needs elimination
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::CheckElimination() {
    int should_eliminate;

    do {
        should_eliminate = false;

        int z = tile_current_position_.z;

        // Row elimination checking
        for (int i = 0; i < map_size_.y; i++) {
            bool row_flag = true;
            for (int j = 0; j < map_size_.x; j++) {
                if (map_[z][j][i] == libconsts::kColorBlack)
                    row_flag = false;
            }
            if (row_flag) {
                should_eliminate = true;
                EliminateRow(i);
                break;
            }
        }

        // Same fruits elimination checking
        std::vector<std::vector<bool>> eliminate_flag(map_size_.x, std::vector<bool>(map_size_.y, false));
        bool same_fruits_flag = false;
        for (int i = 0; i < map_size_.x; i++) {
            for (int j = 0; j < map_size_.y; j++) {
                if (i > 0 && i < map_size_.x - 1 && map_[z][i][j] != libconsts::kColorBlack &&
                    map_[z][i][j] == map_[z][i + 1][j] && map_[z][i][j] == map_[z][i - 1][j]) {         // "一"
                    same_fruits_flag = true;
                    eliminate_flag[i][j] = true;
                    eliminate_flag[i + 1][j] = true;
                    eliminate_flag[i - 1][j] = true;
                }
                if (j > 0 && j < map_size_.y - 1 && map_[z][i][j] != libconsts::kColorBlack &&
                    map_[z][i][j] == map_[z][i][j + 1] && map_[z][i][j] == map_[z][i][j - 1]) {         // "|"
                    same_fruits_flag = true;
                    eliminate_flag[i][j] = true;
                    eliminate_flag[i][j + 1] = true;
                    eliminate_flag[i][j - 1] = true;
                }
                if (i > 0 && i < map_size_.x - 1 && map_[z][i][j] != libconsts::kColorBlack &&         // "/"
                    j > 0 && j < map_size_.y - 1 && map_[z][i][j] == map_[z][i + 1][j + 1] && map_[z][i][j] == map_[z][i - 1][j - 1]) {
                    same_fruits_flag = true;
                    eliminate_flag[i][j] = true;
                    eliminate_flag[i + 1][j + 1] = true;
                    eliminate_flag[i - 1][j - 1] = true;
                }
                if (i > 0 && i < map_size_.x - 1 && map_[z][i][j] != libconsts::kColorBlack &&         // "\"
                    j > 0 && j < map_size_.y - 1 && map_[z][i][j] == map_[z][i - 1][j + 1] && map_[z][i][j] == map_[z][i + 1][j - 1]) {
                    same_fruits_flag = true;
                    eliminate_flag[i][j] = true;
                    eliminate_flag[i - 1][j + 1] = true;
                    eliminate_flag[i + 1][j - 1] = true;
                }
            }
        }
        if (same_fruits_flag) {         // Need eliminate
            should_eliminate = true;
            for (int i = 0; i < map_size_.x; i++) {
                for (int j = map_size_.y - 1; j >= 0; j--) {
                    if (eliminate_flag[i][j]) {
                        std::vector<int>::iterator it = map_[z][i].begin();
                        map_[z][i].erase(it + j, it + j + 1);
                        map_[z][i].push_back(libconsts::kColorBlack);
                    }
                }
            }
        }

    } while (should_eliminate);
}

//
// Function: EliminateRow
// ---------------------------
//
//   Eliminate one row
//
//   Parameters:
//       row: the row number that we want to eliminate
//
//   Returns:
//       void
//

void GameManager::EliminateRow(int row) {
    std::vector<int>::iterator it;
    for (int i = 0; i < map_size_.x; i++) {
        it = map_[tile_current_position_.z][i].begin();
        map_[tile_current_position_.z][i].erase(it + row, it + row + 1);
        map_[tile_current_position_.z][i].push_back(libconsts::kColorBlack);
    }
}

//
// Function: DropOneBlock
// ---------------------------
//
//   Drop current tile by one block
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

int GameManager::DropOneBlock() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        return MoveTile(libconsts::kMoveDown);
    }
    return libconsts::kInBoundary;
}

//
// Function: CheckBoundary
// ---------------------------
//
//   Check if current tile within the boundary
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

int GameManager::CheckBoundary() {
    for (int i = 0; i < libconsts::kCountCells; i++) {
        int x = tile_current_position_.x + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].x;
        int y = tile_current_position_.y + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].y;
        int z = tile_current_position_.z + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].z;
        if (y < 0)
            return libconsts::kOutOfBoundaryDown;
        if (y >= map_size_.y)
            return libconsts::kOutOfBoundaryUp;
        if (x < 0)
            return libconsts::kOutOfBoundaryLeft;
        if (x >= map_size_.x)
            return libconsts::kOutOfBoundaryRight;
        if (z < 0)
            return libconsts::kOutOfBoundaryBack;
        if (z >= map_size_.z)
            return libconsts::kOutOfBoundaryFront;
    }
    return libconsts::kInBoundary;
}

//
// Function: CheckCollision
// ---------------------------
//
//   Check if current tile collide with other tile
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

bool GameManager::CheckCollision() {
    for (int i = 0; i < libconsts::kCountCells; i++) {
        int x = tile_current_position_.x + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].x;
        int y = tile_current_position_.y + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].y;
        int z = tile_current_position_.z + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_Y][tile_current_orient_Z][i].z;
        if (x >= 0 && x < map_size_.x && y >= 0 && y < map_size_.y && z >= 0 && z < map_size_.z && map_[z][x][y])
            return true;
    }
    return false;
}

//
// Function: Restart
// ---------------------------
//
//   Restart the game
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void GameManager::Restart() {
    Init((int)map_size_.x, (int)map_size_.y, (int)map_size_.z);
    AddNewTile();
}
