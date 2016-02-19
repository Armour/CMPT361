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
#include "init_shader.h"
#include <ctime>
#include <cstdlib>

using namespace std;

// The global game manager
GameManager manager;

// Represent the window size - updated if window is reshaped to prevent stretching of the game
int size_x = 400;
int size_y = 720;

// An array containing the color of each of the 10*20*2*3 vertices that make up the board
glm::vec4 board_colors[1200];

// Location of vertex attributes in the shader program
GLuint v_position;
GLuint v_color;

// Locations of uniform variables in shader program
GLuint loc_size_x;
GLuint loc_size_y;

// VAO and VBO
GLuint vao_IDs[3]; // One VAO for each object: the grid, the board, the current piece
GLuint vbo_IDs[6]; // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colours, respectively)

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
    // Bind the VBO containing current tile vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[4]);

    // For each of the 4 'cells' of the tile,
    for (int i = 0; i < libconsts::kCountCells; i++) {

        // Calculate the grid coordinates of the cell
        GLfloat x = manager.get_tile_current_position().x + libconsts::kShapeCategory[manager.get_tile_current_shape()][manager.get_tile_current_orient()][i].x;
        GLfloat y = manager.get_tile_current_position().y + libconsts::kShapeCategory[manager.get_tile_current_shape()][manager.get_tile_current_orient()][i].y;

        // Create the 4 corners of the square - these vertices are using location in pixels
        glm::vec4 p1 = glm::vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
        glm::vec4 p2 = glm::vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
        glm::vec4 p3 = glm::vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
        glm::vec4 p4 = glm::vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

        // Two points are used by two triangles each
        glm::vec4 new_points[6] = {p1, p2, p3, p2, p3, p4};

        // Put new data in the VBO
        glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(glm::vec4), 6 * sizeof(glm::vec4), new_points);

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
    glm::vec4 new_colors[24];
    for (int i = 0; i < 24; i++) {
        new_colors[i] = libconsts::kColorCategory[manager.get_tile_current_color()[i / 6]];
    }

    // Update the color VBO of current tile
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[5]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(new_colors), new_colors);
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
    std::vector<std::vector<int>> &map_data = manager.get_map_data();

    // Let the empty cells on the board be black
    for (int i = 0; i < 1200; i++) {
        int x = i / 6 / (int)manager.get_map_size().x;
        int y = i / 6 % (int)manager.get_map_size().x;
        board_colors[i] = libconsts::kColorCategory[map_data[y][x]];
    }

    // Update grid cell vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[3]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 1200 * sizeof(glm::vec4), board_colors);
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

void NewTile() {
    // Add a new tile
    manager.AddNewTile();

    // Update tile display
    UpdateTileDisplay();
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
    glm::vec4 grid_points[64]; // Array containing the 64 points of the 32 total lines to be later put in the VBO
    glm::vec4 grid_colors[64]; // One color per vertex

    // Vertical lines
    for (int i = 0; i < 11; i++){
        grid_points[2 * i] = glm::vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
        grid_points[2 * i + 1] = glm::vec4((33.0 + (33.0 * i)), 693.0, 0, 1);
    }

    // Horizontal lines
    for (int i = 0; i < 21; i++){
        grid_points[22 + 2 * i] = glm::vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
        grid_points[22 + 2 * i + 1] = glm::vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
    }

    // Make all grid lines white
    for (int i = 0; i < 64; i++)
        grid_colors[i] = libconsts::kColorCategory[libconsts::kColorWhite];

    // Set up first VAO that represent grid lines
    glBindVertexArray(vao_IDs[0]);      // Bind the first VAO
    glGenBuffers(2, &vbo_IDs[0]);       // Create two Vertex Buffer Objects for this VAO (positions, colours)

    // Grid vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[0]);      // Bind the first grid VBO (vertex positions)
    glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(glm::vec4), grid_points, GL_STATIC_DRAW);     // Put the grid points in the VBO
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);

    // Grid vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[1]);      // Bind the second grid VBO (vertex colours)
    glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(glm::vec4), grid_colors, GL_STATIC_DRAW);     // Put the grid colours in the VBO
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
    glm::vec4 board_points[1200];

    // Each cell is a square (2 triangles with 6 vertices)
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 10; j++) {
            glm::vec4 p1 = glm::vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
            glm::vec4 p2 = glm::vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
            glm::vec4 p3 = glm::vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
            glm::vec4 p4 = glm::vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);

            // Two points are reused
            board_points[6 * (10 * i + j)    ] = p1;
            board_points[6 * (10 * i + j) + 1] = p2;
            board_points[6 * (10 * i + j) + 2] = p3;
            board_points[6 * (10 * i + j) + 3] = p2;
            board_points[6 * (10 * i + j) + 4] = p3;
            board_points[6 * (10 * i + j) + 5] = p4;
        }
    }

    // Set up buffer objects
    glBindVertexArray(vao_IDs[1]);
    glGenBuffers(2, &vbo_IDs[2]);

    // Grid cell vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[2]);
    glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(glm::vec4), board_points, GL_STATIC_DRAW);
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);

    // Grid cell vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[3]);
    glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);

    // Current tile vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vbo_IDs[5]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_color);
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
    manager.Init(10, 20);

    // Load shaders and use the shader program
#ifdef __APPLE__
    GLuint program = angel::InitShader("vshader_mac.glsl", "fshader_mac.glsl");
#else
    GLuint program = angel::InitShader("vshader_unix.glsl", "fshader_unix.glsl");
#endif
    glUseProgram(program);

    // Get the location of the attributes (for glVertexAttribPointer() calls)
    v_position = glGetAttribLocation(program, "v_position");
    v_color = glGetAttribLocation(program, "v_color");

    // Create 3 Vertex Array Objects, each representing one 'object'. Store the names in array vao_IDs
    glGenVertexArrays(3, vao_IDs);

    // Initialize the grid, the board, and the current tile
    InitGrid();
    InitBoard();
    InitCurrentTile();

    // The location of the uniform variables in the shader program
    loc_size_x = glGetUniformLocation(program, "size_x");
    loc_size_y = glGetUniformLocation(program, "size_y");

    // Game initialization
    NewTile(); // create new next tile
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
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform1i(loc_size_x, size_x);    // x and y sizes are passed to the shader program to maintain shape of the vertices on screen
    glUniform1i(loc_size_y, size_y);

    glBindVertexArray(vao_IDs[1]);      // Bind the VAO representing the grid cells (to be drawn first)
    glDrawArrays(GL_TRIANGLES, 0, 1200);        // Draw the board (10 * 20 * 2 = 400 triangles)

    if (manager.get_game_state() != GameState::GameStateEnd) {
        glBindVertexArray(vao_IDs[2]);          // Bind the VAO representing the current tile (to be drawn on top of the board)
        glDrawArrays(GL_TRIANGLES, 0, 24);      // Draw the current tile (8 triangles)
    }

    glBindVertexArray(vao_IDs[0]);      // Bind the VAO representing the grid lines (to be drawn on top of everything else)
    glDrawArrays(GL_LINES, 0, 64);      // Draw the grid lines (21+11 = 32 lines)

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
    size_x = w;
    size_y = h;
    glViewport(0, 0, w, h);
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
    UpdateBoard();
    UpdateTileDisplay();
    glutTimerFunc(manager.get_tick_interval(), Tick, 0);
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
    switch (key) {
        case GLUT_KEY_LEFT:         // Move left
            manager.MoveTile(libconsts::kMoveLeft);
            UpdateTileDisplay();
            break;
        case GLUT_KEY_RIGHT:        // Move right
            manager.MoveTile(libconsts::kMoveRight);
            UpdateTileDisplay();
            break;
        case GLUT_KEY_UP:           // Rotate
            manager.RotateTile(libconsts::kClockWise);
            UpdateTileDisplay();
            break;
        case GLUT_KEY_DOWN:         // Drop down
            manager.MoveTile(libconsts::kMoveDown);
            UpdateTileDisplay();
            break;
        default:
            break;
    }
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
            break;
        case '2':   // '2' key enter normal mode
            manager.Normal();
            break;
        case '3':   // '3' key enter hard mode
            manager.Hard();
            break;
        case '4':   // '4' key enter insane mode
            manager.Insane();
            break;
        case 'r':   // 'r' key restarts the game
            manager.Restart();
            UpdateBoard();
            UpdateTileDisplay();
            break;
        case 'p':   // 'p' key pause or resume the game
            if (manager.get_game_state() == GameState::GameStatePause)
                manager.Resume();
            else
                manager.Pause();
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
    glutPostRedisplay();
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
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
#endif

    // Init glut window
    glutInitWindowSize(size_x, size_y);
    glutInitWindowPosition(libconsts::kWindowPositionX, libconsts::kWindowPositionY);
    glutCreateWindow("Fruit Tetris");

    // Init
#ifndef __APPLE__
    glewInit();
#endif
    Init();

    // Setup Callback functions
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutSpecialFunc(Special);
    glutKeyboardFunc(Keyboard);
    glutIdleFunc(Idle);
    glutTimerFunc(libconsts::kTickNormalMode, Tick, 0);

    // Start main loop
    glutMainLoop();
    return 0;
}
