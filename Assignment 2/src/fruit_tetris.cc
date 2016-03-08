//////////////////////////////////////////////////////////////////////////////
//
//  fruit_tetris.cc
//
//  CMPT 361 Assignment 1 - FruitTetris implementation Sample Skeleton Code
//
//  Project         : FruitTetris
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Thomas Shermer
//  TA              : Luwei Yang
//
//  This code is extracted from Connor MacLeod's (crmacleo@sfu.ca) assignment submission
//  by Rui Ma (ruim@sfu.ca) on 2014-03-04.
//
//  Modified in Sep 2014 by Honghua Li (honghual@sfu.ca).
//
//////////////////////////////////////////////////////////////////////////////

#include "game_manager.h"
#include "robot_arm.h"
#include "init_shader.h"
#include <ctime>

using namespace std;

// The global game manager
GameManager manager;

// The global robot arm
RobotArm *robot_arm;

// The array containing the color of each vertices that make up the board
std::vector<glm::vec4> board_position;
std::vector<glm::vec4> board_colors;

// The array containing the color of each vertices that make up the grid
std::vector<glm::vec4> grid_position;
std::vector<glm::vec4> grid_colors;

// MVP matrix
glm::mat4 MVP;
glm::mat4 MV;

// Location of vertex attributes in the shader program
GLuint v_position;
GLuint v_color;
GLuint v_mvp;
GLuint v_mv;

// VAO and VBO
GLuint vao_IDs[4];  // One VAO for each object: the grid, the board, the current piece, the robot arm
GLuint vbo_IDs[8];  // Vertex Buffer Objects for each VAO (contain vertex positions and colours)

// The id of main window
int main_win;

// The size of map in z axis
int map_z = 1;

// Game start flag
int start_flag = 0;

// Game mode level
int level = 1;

// Variables in GLUI
GLUI *gluiTop;      // The GLUI on top
GLUI_StaticText *count_down_text;       // The text for count down display
GLUI_Button *pause_button;      // The button for pause and resume

// Camera rotation angle
float camera_rotation = 0.0f;

//
// Function: UpdateMVP
// ---------------------------
//
//   Update MVP matrix
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateMVP() {
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);  // Changes by GLUI rotation controller

    // Camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(30 * libconsts::kMapCubeSize * sin(camera_rotation),        // Camera position
                      libconsts::kMapCubeSize * (libconsts::kMapSizeHeight + 1),
                      30 * libconsts::kMapCubeSize * cos(camera_rotation)),
            glm::vec3(0.0, libconsts::kMapCubeSize * libconsts::kMapSizeHeight / 2, 0.0),   // Looks at the center of Y axis
            glm::vec3(0, 1, 0)          // Head is up (set to 0, -1, 0 to look upside-down)
    );

    // Projection matrix : 45 degree Field of View, 1:1 ratio, display range 0.1 unit - 5000 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1.0f / 1.0f, 0.1f, 5000.0f);

    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model;
    MV = View * Model;
}

//
// Function: UpdateTilePosition
// ---------------------------
//
//   When the current tile is moved or rotated (or created), update the VBO containing its vertex position data
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateTilePosition() {
    // Update tile position if in the robot arm
    if (manager.get_tile_current_state() == libconsts::kStateOnRobotArm) {
        manager.UpdateTilePosition();
    }

    // Bind the VBO containing current tile vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[4]);

    // For each of the 4 'cells' of the tile,
    for (int i = 0; i < libconsts::kCountCells; i++) {

        // Calculate the grid coordinates of the cell
        GLfloat x = manager.get_tile_current_position().x + manager.get_tile_current_cells()[i].x;
        GLfloat y = manager.get_tile_current_position().y + manager.get_tile_current_cells()[i].y;
        GLfloat z = manager.get_tile_current_position().z + manager.get_tile_current_cells()[i].z;

        // Create the 8 vertex of the cube - these vertices are using location in pixels
        glm::vec4 p[8];
        int map_size_z = (int)manager.get_map_size().z;
        for (int j = 0; j < 8; j++) {
            p[j] = glm::vec4(x * libconsts::kMapCubeSize, y * libconsts::kMapCubeSize,
                             (z - map_size_z / 2 + 0.5 * (1 - map_size_z % 2)) * libconsts::kMapCubeSize, 1)
                             + libconsts::kMapCubeOffset[j];
        }

        // Create triangles for 6 faces of new cube
        glm::vec4 new_cube[36];
        for (int j = 0; j < 36; j++) {
            new_cube[j] = p[libconsts::kCubeFaceIndex[j]];
        }

        // Put new data in the VBO
        glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(new_cube), sizeof(new_cube), new_cube);

    }
}

//
// Function: UpdateTileColor
// ---------------------------
//
//   When the current tile is moved or rotated (or created), update the VBO containing its vertex color data
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateTileColor() {
    // Get color from color category;
    glm::vec4 cube_colors[36 * 4];
    for (int i = 0; i < 36 * 4; i++) {
        if (manager.IsDroppable())
            cube_colors[i] = libconsts::kColorCategory[manager.get_tile_current_color()[i / 36]];
        else
            cube_colors[i] = libconsts::kColorGray;
    }

    // Update the color VBO of current tile
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[5]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_colors), cube_colors);
}

//
// Function: UpdateTileDisplay
// ---------------------------
//
//   When the current tile is moved or rotated (or created), update all of its VBO
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateTileDisplay() {
    // Update the geometry VBO of current tile
    UpdateTilePosition();

    // Update the color VBO of current tile
    UpdateTileColor();

    // Post redisplay
    glutPostRedisplay();
}

//
// Function: UpdateBoard
// ---------------------------
//
//   Update the VBO containing current board information
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateBoard() {
    // Get current map color data
    std::vector<std::vector<std::vector<int>>> &map_data = manager.get_map_data();
    board_colors.clear();
    board_position.clear();

    // Update board content
    for (int i = 0; i < (int)manager.get_map_size().x; i++) {
        for (int j = 0; j < (int)manager.get_map_size().y; j++) {
            for (int k = 0; k < (int)manager.get_map_size().z; k++) {
                // Create the 8 vertex of the cube - these vertices are using location in pixels
                glm::vec4 p[8];
                int map_size_z = (int)manager.get_map_size().z;
                for (int l = 0; l < 8; l++) {
                    p[l] = glm::vec4(i * libconsts::kMapCubeSize, j * libconsts::kMapCubeSize,
                                     (k - map_size_z / 2 + 0.5 * (1 - map_size_z % 2)) * libconsts::kMapCubeSize, 1)
                                    + libconsts::kMapCubeOffset[l];
                }

                // Create triangles for 6 faces of new cube
                glm::vec4 new_cube[36];
                for (int l = 0; l < 36; l++) {
                    new_cube[l] = p[libconsts::kCubeFaceIndex[l]];
                }

                // Push position and color into board_position
                for (int l = 0; l < 36; l++) {
                    if (map_data[k][i][j] != libconsts::kColorBlack) {
                        board_colors.push_back(libconsts::kColorCategory[map_data[k][i][j]]);      // Tile color
                    } else {
                        board_colors.push_back(glm::vec4(0.0, 0.0, 0.0, 0.0));      // Clear color
                    }
                    board_position.push_back(new_cube[l]);
                }
            }
        }
    }

    // Update grid cell vertex position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[2]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, board_position.size() * sizeof(glm::vec4), &board_position.front());

    // Update grid cell vertex color
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[3]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, board_colors.size() * sizeof(glm::vec4), &board_colors.front());
}

//
// Function: UpdateRobotArmPosition
// ---------------------------
//
//   When the robot arm is moved or rotated (or created), update the VBO containing its vertex position data
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateRobotArmPosition() {
    std::vector<glm::vec4> robot_base_position = robot_arm->get_base_render_data();
    std::vector<glm::vec4> robot_lower_arm_position = robot_arm->get_lower_arm_render_data();
    std::vector<glm::vec4> robot_upper_arm_position = robot_arm->get_upper_arm_render_data();

    // Update robot base position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[6]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 0,
                    robot_base_position.size() * sizeof(glm::vec4), &robot_base_position.front());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * robot_base_position.size(),
                    robot_lower_arm_position.size() * sizeof(glm::vec4), &robot_lower_arm_position.front());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * (robot_base_position.size() + robot_lower_arm_position.size()),
                    robot_upper_arm_position.size() * sizeof(glm::vec4), &robot_upper_arm_position.front());

    // Update spawn point in game manager
    glm::vec4 end_point = robot_arm->get_upper_arm_end_point();
    glm::vec3 spawn_point = manager.CalculateFitPosition(end_point);
    manager.set_spawn_point(spawn_point);
    UpdateTileDisplay();

    // Post redisplay
    glutPostRedisplay();
}

//
// Function: InitGrid
// ---------------------------
//
//   Init grid line
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void InitGrid() {
    // Init grid buffer data
    int z = (int)manager.get_map_size().z;
    grid_colors.clear();
    grid_position.clear();

    // Horizontal lines X
    for (int i = 0; i < libconsts::kMapSizeHeight + 1; i++) {
        for (int j = 0; j < z + 1; j++) {
            grid_position.push_back(glm::vec4(-libconsts::kMapCubeSize * libconsts::kMapSizeWidth / 2.0,
                                               libconsts::kMapCubeSize * i,
                                              -libconsts::kMapCubeSize * z / 2.0 + libconsts::kMapCubeSize * j, 1));
            grid_position.push_back(glm::vec4(libconsts::kMapCubeSize * libconsts::kMapSizeWidth / 2.0,
                                              libconsts::kMapCubeSize * i,
                                              -libconsts::kMapCubeSize * z / 2.0 + libconsts::kMapCubeSize * j, 1));
            grid_colors.push_back(libconsts::kColorCategory[libconsts::kColorWhite]);
            grid_colors.push_back(libconsts::kColorCategory[libconsts::kColorWhite]);
        }
    }

    // Vertical lines Y
    for (int i = 0; i < libconsts::kMapSizeWidth + 1; i++) {
        for (int j = 0; j < z + 1; j++) {
            grid_position.push_back(glm::vec4(-libconsts::kMapCubeSize * libconsts::kMapSizeWidth / 2.0 + libconsts::kMapCubeSize * i,
                                               libconsts::kMapCubeSize * 0,
                                              -libconsts::kMapCubeSize * z / 2.0 + libconsts::kMapCubeSize * j, 1));
            grid_position.push_back(glm::vec4(-libconsts::kMapCubeSize * libconsts::kMapSizeWidth / 2.0 + libconsts::kMapCubeSize * i,
                                               libconsts::kMapCubeSize * libconsts::kMapSizeHeight,
                                              -libconsts::kMapCubeSize * z / 2.0 + libconsts::kMapCubeSize * j, 1));
            grid_colors.push_back(libconsts::kColorCategory[libconsts::kColorWhite]);
            grid_colors.push_back(libconsts::kColorCategory[libconsts::kColorWhite]);
        }
    }

    // Horizontal lines Z
    for (int i = 0; i < libconsts::kMapSizeWidth + 1; i++) {
        for (int j = 0; j < libconsts::kMapSizeHeight + 1; j++) {
            grid_position.push_back(glm::vec4(-libconsts::kMapCubeSize * libconsts::kMapSizeWidth / 2.0 + libconsts::kMapCubeSize * i,
                                               libconsts::kMapCubeSize * j,
                                              -libconsts::kMapCubeSize * z / 2.0, 1));
            grid_position.push_back(glm::vec4(-libconsts::kMapCubeSize * libconsts::kMapSizeWidth / 2.0 + libconsts::kMapCubeSize * i,
                                               libconsts::kMapCubeSize * j,
                                               libconsts::kMapCubeSize * z / 2.0, 1));
            grid_colors.push_back(libconsts::kColorCategory[libconsts::kColorWhite]);
            grid_colors.push_back(libconsts::kColorCategory[libconsts::kColorWhite]);
        }
    }

    // Set up first VAO that represent grid lines
    glBindVertexArray(vao_IDs[0]);      // Bind the first VAO
    glGenBuffers(2, &vbo_IDs[0]);       // Create two Vertex Buffer Objects for this VAO (positions, colours)

    // Grid vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[0]);      // Bind the first grid VBO (vertex positions)
    glBufferData(GL_ARRAY_BUFFER, grid_position.size() * sizeof(glm::vec4), &grid_position.front(), GL_STATIC_DRAW);     // Put the grid points in the VBO
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);

    // Grid vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[1]);      // Bind the second grid VBO (vertex colours)
    glBufferData(GL_ARRAY_BUFFER, grid_colors.size() * sizeof(glm::vec4), &grid_colors.front(), GL_STATIC_DRAW);     // Put the grid colours in the VBO
    glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_color);
}

//
// Function: InitBoard
// ---------------------------
//
//   Init board
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void InitBoard() {
    // Set up buffer objects
    glBindVertexArray(vao_IDs[1]);
    glGenBuffers(2, &vbo_IDs[2]);

    // Board vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[2]);
    glBufferData(GL_ARRAY_BUFFER, manager.get_map_size().z * 7200 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);

    // Board vertex color
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[3]);
    glBufferData(GL_ARRAY_BUFFER, manager.get_map_size().z * 7200 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_color);
}

//
// Function: InitCurrentTile
// ---------------------------
//
//   No geometry for current tile initially
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void InitCurrentTile() {
    // Bind to current tile buffer vertex array
    glBindVertexArray(vao_IDs[2]);
    glGenBuffers(2, &vbo_IDs[4]);

    // Current tile vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[4]);
    glBufferData(GL_ARRAY_BUFFER, 144 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);

    // Current tile vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[5]);
    glBufferData(GL_ARRAY_BUFFER, 144 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_color);
}

//
// Function: InitRobotArm
// ---------------------------
//
//   Init the robot arm
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void InitRobotArm() {
    // Init robot arm
    robot_arm = new RobotArm(glm::vec4(-libconsts::kMapCubeSize * (libconsts::kMapSizeWidth / 2 + 4), 10.0, 0.0, 1.0));

    // Init robot arm color
    std::vector<glm::vec4> robot_arm_color;
    for (int i = 0; i < 108; i++) {
        int index = rand() % 7;
        robot_arm_color.push_back(libconsts::kColorCategory[index]);
    }

    // Bind to current tile buffer vertex array
    glBindVertexArray(vao_IDs[3]);
    glGenBuffers(2, &vbo_IDs[6]);

    // Robot arm vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[6]);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);

    // Robot arm vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[7]);
    glBufferData(GL_ARRAY_BUFFER, robot_arm_color.size() * sizeof(glm::vec4), &robot_arm_color.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_color);
}

//
// Function: NewTile
// ---------------------------
//
//   Called at the start of play and every time a tile is placed
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void NewTile(void) {
    // Update spawn point in game manager
    glm::vec4 end_point = robot_arm->get_upper_arm_end_point();
    glm::vec3 spawn_point = manager.CalculateFitPosition(end_point);
    manager.set_spawn_point(spawn_point);

    // Add a new tile
    manager.AddNewTile();

    // Update tile display
    UpdateTileDisplay();
}

//
// Function: PauseOrResume
// ---------------------------
//
//   Pause or resume the game
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void PauseOrResume(void) {
    if (!start_flag) return;
    if (manager.get_game_state() == GameState::GameStatePause) {
        manager.Resume();
        pause_button->set_name("Pause");
    } else {
        manager.Pause();
        pause_button->set_name("Resume");
    }
    gluiTop->sync_live();
}

//
// Function: Restart
// ---------------------------
//
//   Restart game
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void Restart(void) {
    if (!start_flag) return;
    manager.Restart();
    robot_arm->ResetAngle();
    UpdateBoard();
    UpdateTileDisplay();
    UpdateRobotArmPosition();
    gluiTop->sync_live();
}

//
// Function: ChangeMode
// ---------------------------
//
//   Change game mode according to level varible
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void ChangeMode(void) {
    if (!start_flag) return;
    switch (level) {
        case 1: manager.Easy();
            break;
        case 2: manager.Normal();
            break;
        case 3: manager.Hard();
            break;
        case 4: manager.Insane();
            break;
    }
}

//
// Function: Init
// ---------------------------
//
//   Init all things, including shader
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void Init() {
    // Initialize the time seed
    srand(time(0));

    // Init game manager
    manager.Init(libconsts::kMapSizeWidth, libconsts::kMapSizeHeight, map_z);

    // Enable Z-buffering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Load shaders and use the shader program
#ifdef __APPLE__
    GLuint program = angel::InitShader("vshader_mac.glsl", "fshader_mac.glsl");
#else
    GLuint program = angel::InitShader("vshader_unix.glsl", "fshader_unix.glsl");
#endif
    glUseProgram(program);

    // Get the attribute and uniform variable location in shader
    v_position = glGetAttribLocation(program, "v_position");
    v_color = glGetAttribLocation(program, "v_color");
    v_mvp = glGetUniformLocation(program, "v_mvp");
    v_mv = glGetUniformLocation(program, "v_mv");

    // Create 3 Vertex Array Objects, each representing one 'object'. Store the names in array vao_IDs
    glGenVertexArrays(4, vao_IDs);

    // Initialize the grid, the board, and the current tile
    InitGrid();
    InitBoard();
    InitCurrentTile();
    InitRobotArm();
    UpdateRobotArmPosition();

    // Game initialization
    NewTile(); // create new next tile

    // Update start flag
    start_flag = 1;
}

//
// Function: Display
// ---------------------------
//
//   Display callback function
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void Display() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UpdateMVP();    // Update MVP matrix

    gluiTop->sync_live();       // Sync live variables in GLUI

    glUniformMatrix4fv(v_mv, 1, GL_FALSE, glm::value_ptr(MV));          // Send MV matrix to uniform v_mv in shader
    glUniformMatrix4fv(v_mvp, 1, GL_FALSE, glm::value_ptr(MVP));        // Send MVP matrix to uniform v_mvp in shader

    glEnable(GL_POLYGON_OFFSET_FILL);   // Draw all the block with offset, so that it seems to be in the grid
    glPolygonOffset(1.0f, 1.0f);

    if (manager.get_game_state() != GameState::GameStateEnd) {
        glBindVertexArray(vao_IDs[2]);          // Bind the VAO representing the current tile (to be drawn on top of the board)
        glDrawArrays(GL_TRIANGLES, 0, 144);      // Draw the current tile (8 triangles)
    }

    glBindVertexArray(vao_IDs[1]);      // Bind the VAO representing the grid cells (to be drawn first)
    glDrawArrays(GL_TRIANGLES, 0, board_position.size() * sizeof(glm::vec4));        // Draw the board (10 * 20 * 2 = 400 triangles)

    glDisable(GL_POLYGON_OFFSET_FILL);  // Disable the offset and begin to draw grid lines

    glBindVertexArray(vao_IDs[0]);      // Bind the VAO representing the grid lines (to be drawn on top of everything else)
    glDrawArrays(GL_LINES, 0, grid_position.size() * sizeof(glm::vec4));      // Draw the grid lines (21+11 = 32 lines)

    glBindVertexArray(vao_IDs[3]);
    glDrawArrays(GL_TRIANGLES, 0, 108);      // Draw the robot arm

    glutSwapBuffers();
}

//
// Function: Reshape
// ---------------------------
//
//   Reshape callback function
//
//   Parameters:
//       w: the width of the window
//       h: the height of the window
//
//   Returns:
//       void
//

void Reshape(GLsizei w, GLsizei h) {
    GLUI_Master.auto_set_viewport();        // GLUI auto set the view port, cheers!
    glutPostRedisplay();
}

//
// Function: Tick
// ---------------------------
//
//   Timer callback function
//
//   Parameters:
//       w: the width of the window
//       h: the height of the window
//
//   Returns:
//       void
//

void Tick(int value) {
    manager.Tick();

    int count_down = manager.get_tile_count_down();
    string s = "Drop Count Down:    " + std::to_string(count_down);
    count_down_text->set_text(s.c_str());

    UpdateBoard();
    UpdateTileDisplay();

    GLUI_Master.set_glutTimerFunc(manager.get_tick_interval(), Tick, 0);
}

//
// Function: Special
// ---------------------------
//
//   Special input callback function
//
//   Parameters:
//       key: the key that user pressed
//
//   Returns:
//       void
//

void Special(int key, int, int) {
    int modifiers = glutGetModifiers();
    switch (key) {
        case GLUT_KEY_LEFT:         // Move left
            if (modifiers & GLUT_ACTIVE_CTRL) {
                camera_rotation -= 0.1f;
            } else {
                manager.MoveTile(libconsts::kMoveLeft);
                UpdateTileDisplay();
            }
            break;
        case GLUT_KEY_RIGHT:        // Move right
            if (modifiers & GLUT_ACTIVE_CTRL) {
                camera_rotation += 0.1f;
            } else {
                manager.MoveTile(libconsts::kMoveRight);
                UpdateTileDisplay();
            }
            break;
        case GLUT_KEY_UP:           // Rotate
            if (modifiers & GLUT_ACTIVE_SHIFT) {
                manager.RotateTile(libconsts::kRotationAxisY);
            } else {
                manager.RotateTile(libconsts::kRotationAxisZ);
            }
            UpdateTileDisplay();
            break;
        case GLUT_KEY_DOWN:         // Drop down
            manager.MoveTile(libconsts::kMoveDown);
            UpdateTileDisplay();
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

//
// Function: Keyboard
// ---------------------------
//
//   Keyboard input callback function
//
//   Parameters:
//       key: the key that user pressed
//
//   Returns:
//       void
//

void Keyboard(unsigned char key, int, int) {
    switch (key) {
        case 033:   // Both escape key and 'q' cause the game to exit
            exit(EXIT_SUCCESS);
        case 'q':   // Both escape key and 'q' cause the game to exit
            exit (EXIT_SUCCESS);
        case '1':   // '1' key enter easy mode
            manager.Easy();
            level = 1;
            gluiTop->sync_live();
            break;
        case '2':   // '2' key enter normal mode
            manager.Normal();
            level = 2;
            gluiTop->sync_live();
            break;
        case '3':   // '3' key enter hard mode
            manager.Hard();
            level = 3;
            gluiTop->sync_live();
            break;
        case '4':   // '4' key enter insane mode
            manager.Insane();
            level = 4;
            gluiTop->sync_live();
            break;
        case 'a':
            robot_arm->RotateLowerArm(libconsts::kAntiClockWise);
            UpdateRobotArmPosition();
            break;
        case 'd':
            robot_arm->RotateLowerArm(libconsts::kClockWise);
            UpdateRobotArmPosition();
            break;
        case 'w':
            robot_arm->RotateUpperArm(libconsts::kAntiClockWise);
            UpdateRobotArmPosition();
            break;
        case 's':
            robot_arm->RotateUpperArm(libconsts::kClockWise);
            UpdateRobotArmPosition();
            break;
        case '<':
            robot_arm->RotateBase(libconsts::kAntiClockWise);
            UpdateRobotArmPosition();
            break;
        case '>':
            robot_arm->RotateBase(libconsts::kClockWise);
            UpdateRobotArmPosition();
            break;
        case ' ':
            if (manager.IsDroppable()) {
                manager.set_tile_state_on_air();
                manager.set_tile_count_down(0);
            }
            break;
        case 'r':   // 'r' key restarts the game
            Restart();
            break;
        case 'p':   // 'p' key pause or resume the game
            PauseOrResume();
            break;
    }
    glutPostRedisplay();
}

//
// Function: Idle
// ---------------------------
//
//   Idle callback function
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void Idle(void) {
    glutSetWindow(main_win);
    glutPostRedisplay();
}

//
// Function: InitGLUI
// ---------------------------
//
//   Init GLUI window
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void InitGLUI(void) {
    // Temp GLUI to fix MAC display bug
#ifdef __APPLE__
    GLUI *tmp = GLUI_Master.create_glui("Temp", 0, 0, 0);
    tmp->hide();
#endif

    // Setup top subwindow GUI
    gluiTop = GLUI_Master.create_glui_subwindow(main_win, GLUI_SUBWINDOW_TOP);

    // Add start panel
    GLUI_Panel *start_panel = gluiTop->add_panel("Game Option");
    GLUI_Spinner *spinner = gluiTop->add_spinner_to_panel(start_panel, "Input Z:", GLUI_SPINNER_INT, &map_z);
    spinner->set_int_limits(1, 10, GLUI_LIMIT_CLAMP);
    gluiTop->add_button_to_panel(start_panel, "(Re)Start", 0, (GLUI_Update_CB)Init);

    // Add static text
    gluiTop->add_column(false);
    GLUI_Panel *text_panel = gluiTop->add_panel("Timer");
    count_down_text = gluiTop->add_statictext_to_panel(text_panel, "Drop Count Down:    0");
    gluiTop->add_statictext_to_panel(text_panel, "");
    gluiTop->add_statictext_to_panel(text_panel, "Rotate robot base:  '<'  '>'");

    // Add game mode panel
    gluiTop->add_column(false);
    GLUI_Panel *mod_panel = gluiTop->add_panel("Game Level");
    GLUI_Spinner *level_spinner = gluiTop->add_spinner_to_panel(mod_panel, "Level:", GLUI_SPINNER_INT, &level);
    level_spinner->set_int_limits(1, 4, GLUI_LIMIT_CLAMP);
    gluiTop->add_button_to_panel(mod_panel, "Change", 0, (GLUI_Update_CB)ChangeMode);

    // Add control panel
    gluiTop->add_column(false);
    GLUI_Panel *quit_panel = gluiTop->add_panel("Game State");
    pause_button = gluiTop->add_button_to_panel(quit_panel, "Pause", 0, (GLUI_Update_CB)PauseOrResume);
    gluiTop->add_button_to_panel(quit_panel, "Quit", 0, (GLUI_Update_CB)exit);

    // Set main gfx windows
    gluiTop->set_main_gfx_window(main_win);
}

//
// Function: Main
// ---------------------------
//
//   The main function
//
//   Parameters:
//       argc: the number of parameters in main function
//       argv[]: the array of parameters in main function
//
//   Returns:
//       void
//

int main(int argc, char **argv) {
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

    // Init glut window
    glutInitWindowSize(libconsts::kWindowSizeWidth, libconsts::kWindowSizeHeight);
    glutInitWindowPosition(libconsts::kWindowPositionX, libconsts::kWindowPositionY);

    // Setup callback function
    main_win = glutCreateWindow("Fruit Tetris");
    GLUI_Master.set_glutReshapeFunc(Reshape);
    GLUI_Master.set_glutSpecialFunc(Special);
    GLUI_Master.set_glutKeyboardFunc(Keyboard);
    GLUI_Master.set_glutIdleFunc(Idle);
    GLUI_Master.set_glutTimerFunc(libconsts::kTickNormalMode, Tick, 0);
    glutDisplayFunc(Display);

    // Init
#ifndef __APPLE__
    glewInit();
#endif
    InitGLUI();

    // Start main loop
    glutMainLoop();
    return 0;
}
