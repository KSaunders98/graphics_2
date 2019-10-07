//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        game-main.cc
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This main file handles setting up the opengl context for
//                 the food drop game, along with the game object and window.
//                 It also sets up window, keyboard, and mouse callbacks.
//
//    Date:        10/6/2019
//
//*******************************************************************

// C/C++ Standard libraries
#include <cstdlib>
#include <iostream>

// Source libraries
#include "game.h"

// Shader constants
constexpr char const* VERTEX_SHADER_FILE = "vshader2d.glsl";
constexpr char const* FRAGMENT_SHADER_FILE = "fshader2d.glsl";

// Window constants
const unsigned int INIT_WINDOW_WIDTH = 1200;  // window width in pixels
const unsigned int INIT_WINDOW_HEIGHT = 600;  // window height in pixels
constexpr char const* WINDOW_TITLE = "Food Drop Game | Loading...";  // window title

// Game Options
const unsigned int NUM_BAD_GUYS = 6;  // number of bad guys to create
const unsigned int NUM_GOOD_GUYS = 10;  // number of good guys to create
const unsigned int NUM_TREES = 40;  // number of trees to create
const unsigned int MAX_DROPS = 8;  // number of food drops to allow the user to have

// Global variables
// Note: I normally wouldn't use global variables, but it seems like you can't pass any arguments to the display callback
Game* game;  // Pointer to game object, will be created dynamically
vec2 window_size;  // Variable that holds window size
GLuint shader_id;  // Variable that holds opengl shader id

//******************************************************************
//
//  Function:   init_shader
//
//  Purpose:    initializes the opengl shader
//
//  Parameters: none
//
//  Member/Global Variables: VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
//
//  Pre Conditions:  VERTEX_SHADER_FILE and FRAGMENT_SHADER_FILE must
//                   have valid values and refer to glsl source files
//
//  Post Conditions: the shader program id will be returned
//
//  Calls:      InitShader, glUseProgram
//
//******************************************************************
GLuint init_shader() {
    // Load shaders and use the resulting shader program
    GLuint program = InitShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);  // pointer to opengl shader program
    glUseProgram(program);

    return program;
}

//******************************************************************
//
//  Function:   display
//
//  Purpose:    display callback that handles drawing the game
//
//  Parameters: none
//
//  Member/Global Variables: game
//
//  Pre Conditions:  game must point to a valid, initialized game object
//
//  Post Conditions: the game will be drawn and displayed to the screen
//
//  Calls:      glClear, game::display, glutSwapBuffers
//
//******************************************************************
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    game->display();
    glutSwapBuffers();
}

//******************************************************************
//
//  Function:   idle
//
//  Purpose:    idle callback that handles updating the game between
//              render frames
//
//  Parameters: none
//
//  Member/Global Variables: game
//
//  Pre Conditions:  game must point to a valid, initialized game object
//
//  Post Conditions: the game state will be updated according to the
//                   calculated delta time and a frame redraw will
//                   scheduled
//
//  Calls:      glutGet, game::update, glutPostRedisplay
//
//******************************************************************
void idle() {
    static float last_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    float current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    float dt = current_time - last_time;
    last_time = current_time;

    game->update(dt);

    glutPostRedisplay();
}

//******************************************************************
//
//  Function:   reshape
//
//  Purpose:    window resize callback
//
//  Parameters: width, height
//
//  Member/Global Variables: window_size, game
//
//  Pre Conditions:  game must point to a valid, initialized game object
//
//  Post Conditions: the game object will be informed of the window resize,
//                   window_size will be updated, the gl viewport will be updated,
//                   and a frame redraw will be scheduled
//
//  Calls:      game::update_window_size, glViewport, glutPostRedisplay
//
//******************************************************************
void reshape(int width, int height) {
    window_size = vec2(width, height);
    game->set_window_size(window_size);  // inform game of updated window size
    glViewport(0, 0, width, height);  // update opengl viewport
    glutPostRedisplay();  // schedule a screen redraw
}

//******************************************************************
//
//  Function:   keyboard_func
//
//  Purpose:    keyboard callback that handles resetting the game
//              when the 'r' key is pressed
//
//  Parameters: key, x, y
//
//  Member/Global Variables: game, window_size, NUM_BAD_GUYS, NUM_GOOD_GUYS,
//                           NUM_TREES, MAX_DROPS, shader_id
//
//  Pre Conditions:  all of the above game constants must have valid values,
//                   shader_id must refer to a valid, active opengl shader
//                   program, and window_size must have a valid value
//
//  Post Conditions: if the r key is pressed, the game will be reinitialized
//
//  Calls:      game::update_window_size, game::init, glutPostRedisplay
//
//******************************************************************
void keyboard_func(unsigned char key, int x, int y) {
    if (key == 'r') {
        delete game;
        game = new Game(NUM_BAD_GUYS, NUM_GOOD_GUYS, NUM_TREES, MAX_DROPS, shader_id);
        game->set_window_size(window_size);
        game->init();

        glutPostRedisplay();
    }
}

//******************************************************************
//
//  Function:   mouse_func
//
//  Purpose:    mouse callback that handles passing left mouse button
//              clicks to the game
//
//  Parameters: button, state, x, y
//
//  Member/Global Variables: game
//
//  Pre Conditions:  game must point to a valid game object
//
//  Post Conditions: if the left mouse button was clicked, the game will
//                   have handled it
//
//  Calls:      game::handle_click
//
//******************************************************************
void mouse_func(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        game->handle_click(vec2(x, y));  // let game handle left mouse click
    }
}

//******************************************************************
//
//  Function:   main
//
//  Purpose:    main function that handles initializing glut and glew, creates
//              a window, sets up callbacks, and creates the game obejct
//
//  Parameters: argc, argv
//
//  Member/Global Variables: game, window_size, NUM_BAD_GUYS, NUM_GOOD_GUYS,
//                           NUM_TREES, MAX_DROPS, INIT_WINDOW_WIDTH,
//                           INIT_WINDOW_HEIGHT
//                           
//
//  Pre Conditions:  all of the above constants must have valid values
//
//  Post Conditions: the opengl context and window will be created and
//                   active, along with the shader and game object
//
//  Calls:      glutInit, glutInitDisplayMode, glutInitWindowPosition,
//              glutCreateWindow, glutDisplayFunc, glutReshapeFunc,
//              glutKeyboardFunc, glutMouseFunc, glutIdleFunc, glewInit,
//              init_shader, game::update_window_size, game::init,
//              glutMainLoop
//
//******************************************************************

int main(int argc, char** argv) {
    // Standard GLUT initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);  // use double buffering, RGBA, and multisampling
    glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
    window_size = vec2(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);  // place window top left on display
    glutCreateWindow(WINDOW_TITLE);

    glutDisplayFunc(display);  // display callback, invoked when window opened or needs redrawn
    glutReshapeFunc(reshape);  // window resize callback
    glutKeyboardFunc(keyboard_func);  // keyboard event callback
    glutMouseFunc(mouse_func);  // mouse event callback
    glutIdleFunc(idle);  // idle callback, called between frames

    glewInit();  // initialize glew

    shader_id = init_shader();

    // initialize our game object
    game = new Game(NUM_BAD_GUYS, NUM_GOOD_GUYS, NUM_TREES, MAX_DROPS, shader_id);  // create game with parameters
    game->set_window_size(window_size);
    game->init();  // initialize our game object (including opengl elements it uses)

    glutMainLoop();  // enter event loop

    delete game;  // clean up resources

    return EXIT_SUCCESS;
}