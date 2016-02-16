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

// Init game manager
void GameManager::Init(int width, int height) {
    map_size_.x = width;
    map_size_.y = height;
    map_ = std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
    while (!game_states_.empty()) {
        game_states_.pop();
    }
    game_states_.push(GameState::GameStateNormal);
    tick_interval_ = libconsts::kTickNormalMode;
}

// Generate new tile
void GameManager::AddNewTile() {
    tile_current_position_.x = rand() % (int)map_size_.x;
    tile_current_position_.y = map_size_.y - 1; // Put the tile at the top of the board

    tile_current_orient_ = rand() % libconsts::kCountOrient;
    tile_current_shape_ = rand() % libconsts::kCountShape;

    for (int i = 0; i < libconsts::kCountCells; i++) {
        tile_current_color_[i] = rand() % (libconsts::kCountColor - 2) + 2;    // Except black and white
    }

    int boundary_state = CheckBoundary();
    while (boundary_state != libconsts::kInBoundary) {      // Adjust tile according to boundary state
        switch (boundary_state) {
            case libconsts::kOutOfBoundaryUp:
                tile_current_position_ += libconsts::kMoveDown;
                break;
            case libconsts::kOutOfBoundaryLeft:
                tile_current_position_ += libconsts::kMoveRight;
                break;
            case libconsts::kOutOfBoundaryRight:
                tile_current_position_ += libconsts::kMoveLeft;
                break;
        }
        boundary_state = CheckBoundary();
    }

    if (CheckCollision())
        game_states_.push(GameState::GameStateEnd);
}

// Tick
void GameManager::Tick() {
    if (get_game_state() != GameState::GameStateEnd || get_game_state() != GameState::GameStatePause) {
        int drop_state = DropOneBlock();
        if (drop_state == libconsts::kOutOfBoundaryDown || drop_state == libconsts::kCollision) {       // Check if it can drop
            FillTileToMap();
            CheckElimination();
            AddNewTile();
        }
    }
}

// Easy mode
void GameManager::Easy(){
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateEasy);
        tick_interval_ = libconsts::kTickEasyMode;
    }
}

// Normal mode
void GameManager::Normal() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateNormal);
        tick_interval_ = libconsts::kTickNormalMode;
    }
}

// Hard mode
void GameManager::Hard() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateHard);
        tick_interval_ = libconsts::kTickHardMode;
    }
}

// Insane mode
void GameManager::Insane() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        ChangeGameMode(GameState::GameStateInsane);
        tick_interval_ = libconsts::kTickInsaneMode;
    }
}

// Pause game
void GameManager::Pause() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        game_states_.push(GameState::GameStatePause);
    }
}

// Resume game
void GameManager::Resume() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() == GameState::GameStatePause) {
        game_states_.pop();
        if (game_states_.empty()) {
            game_states_.push(GameState::GameStateNormal);
        }
    }
}

// Change game mode to specific state
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

// Move tile with direction
int GameManager::MoveTile(glm::vec2 direction) {
    if (get_game_state() != GameState::GameStatePause) {
        tile_current_position_ += direction;
        if (CheckCollision()) {         // Check collision
            tile_current_position_ -= direction;
            return libconsts::kCollision;
        }
        int boundary_state = CheckBoundary();       // Check boundary
        if (boundary_state != libconsts::kInBoundary) {
            tile_current_position_ -= direction;
            return boundary_state;
        }
    }
    return libconsts::kInBoundary;
}

// Rotate tile with direction
int GameManager::RotateTile(int direction) {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        tile_current_orient_ += direction;
        tile_current_orient_ %= libconsts::kCountOrient;
        if (CheckCollision()) {         // Check collision
            tile_current_orient_ -= direction;
            if (tile_current_orient_ < 0)
                tile_current_orient_ = libconsts::kCountOrient - 1;
            tile_current_orient_ %= libconsts::kCountOrient;
            return libconsts::kCollision;
        }
        int boundary_state = CheckBoundary();       // Check boundary
        if (boundary_state != libconsts::kInBoundary) {
            tile_current_orient_ -= direction;
            if (tile_current_orient_ < 0)
                tile_current_orient_ = libconsts::kCountOrient - 1;
            tile_current_orient_ %= libconsts::kCountOrient;
            return boundary_state;
        }
    }
    return libconsts::kInBoundary;
}

// Fill current tile color to map
void GameManager::FillTileToMap(){
    for (int i = 0; i < libconsts::kCountCells; i++) {
        int x = tile_current_position_.x + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_][i].x;
        int y = tile_current_position_.y + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_][i].y;
        if (x >= 0 && x < map_size_.x && y >= 0 && y < map_size_.y) {
           map_[x][y] = tile_current_color_[i];
        }
    }
}

// Check elimination
void GameManager::CheckElimination() {
    int should_eliminate;

    do {
        should_eliminate = false;

        // Row elimination checking
        for (int i = 0; i < map_size_.y; i++) {
            bool row_flag = true;
            for (int j = 0; j < map_size_.x; j++) {
                if (map_[j][i] == libconsts::kColorBlack)
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
                if (i > 0 && i < map_size_.x - 1 && map_[i][j] != libconsts::kColorBlack &&
                    map_[i][j] == map_[i + 1][j] && map_[i][j] == map_[i - 1][j]) {         // "一"
                    same_fruits_flag = true;
                    eliminate_flag[i][j] = true;
                    eliminate_flag[i + 1][j] = true;
                    eliminate_flag[i - 1][j] = true;
                }
                if (j > 0 && j < map_size_.y - 1 && map_[i][j] != libconsts::kColorBlack &&
                    map_[i][j] == map_[i][j + 1] && map_[i][j] == map_[i][j - 1]) {         // "|"
                    same_fruits_flag = true;
                    eliminate_flag[i][j] = true;
                    eliminate_flag[i][j + 1] = true;
                    eliminate_flag[i][j - 1] = true;
                }
                if (i > 0 && i < map_size_.x - 1 && map_[i][j] != libconsts::kColorBlack &&         // "/"
                    j > 0 && j < map_size_.y - 1 && map_[i][j] == map_[i + 1][j + 1] && map_[i][j] == map_[i - 1][j - 1]) {
                    same_fruits_flag = true;
                    eliminate_flag[i][j] = true;
                    eliminate_flag[i + 1][j + 1] = true;
                    eliminate_flag[i - 1][j - 1] = true;
                }
                if (i > 0 && i < map_size_.x - 1 && map_[i][j] != libconsts::kColorBlack &&         // "\"
                    j > 0 && j < map_size_.y - 1 && map_[i][j] == map_[i - 1][j + 1] && map_[i][j] == map_[i + 1][j - 1]) {
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
                        std::vector<int>::iterator it = map_[i].begin();
                        map_[i].erase(it + j, it + j + 1);
                        map_[i].push_back(libconsts::kColorBlack);
                    }
                }
            }
        }

    } while (should_eliminate);
}

// Eliminate one row
void GameManager::EliminateRow(int row) {
    std::vector<int>::iterator it;
    for (int i = 0; i < map_size_.x; i++) {
        it = map_[i].begin();
        map_[i].erase(it + row, it + row + 1);
        map_[i].push_back(libconsts::kColorBlack);
    }
}

// Drop one block
int GameManager::DropOneBlock() {
    if (get_game_state() != GameState::GameStateEnd && get_game_state() != GameState::GameStatePause) {
        return MoveTile(libconsts::kMoveDown);
    }
    return libconsts::kInBoundary;
}

// Check boundary
int GameManager::CheckBoundary() {
    for (int i = 0; i < libconsts::kCountCells; i++) {
        int x = tile_current_position_.x + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_][i].x;
        int y = tile_current_position_.y + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_][i].y;
        if (y < 0)
            return libconsts::kOutOfBoundaryDown;
        if (y >= map_size_.y)
            return libconsts::kOutOfBoundaryUp;
        if (x < 0)
            return libconsts::kOutOfBoundaryLeft;
        if (x >= map_size_.x)
            return libconsts::kOutOfBoundaryRight;
    }
    return libconsts::kInBoundary;
}

// Check collision
bool GameManager::CheckCollision() {
    for (int i = 0; i < libconsts::kCountCells; i++) {
        int x = tile_current_position_.x + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_][i].x;
        int y = tile_current_position_.y + libconsts::kShapeCategory[tile_current_shape_][tile_current_orient_][i].y;
        if (x >= 0 && x < map_size_.x && y >= 0 && y < map_size_.y && map_[x][y])
            return true;
    }
    return false;
}

// Restart game
void GameManager::Restart() {
    Init((int)map_size_.x, (int)map_size_.y);
    AddNewTile();
}
