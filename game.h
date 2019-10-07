//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        game.h
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This class provides a data structure to store game
//                 state and provides helper functions for updating
//                 state based on delta time and handling mouse events.
//
//    Date:        10/6/2019
//
//*******************************************************************

#ifndef GAME_H
#define GAME_H

// C/C++ Standard libraries
#include <vector>

// Third-Party libraries
#include <Angel.h>

// Source libraries
#include "circle.h"
#include "unit.h"

// Game visual constants
constexpr char const* GAME_TITLE = "Food Drop Game";  // name of game to display in window title
const vec3 BAD_COLOR = vec3(0.75, 0, 0);
const vec3 GOOD_COLOR = vec3(0, 0, 0.75);
const vec3 FOOD_COLOR = vec3(1, 0.75, 0);
const vec3 TREE_COLOR = vec3(0, 0.75, 0);
const vec3 PLANE_COLOR = vec3(0.1, 0.1, 0.1);
const float BAD_SIZE = 30;
const float GOOD_SIZE = 30;
const float FOOD_SIZE = 20;
const float TREE_MIN_SIZE = 25;
const float TREE_MAX_SIZE = 55;
const float PLANE_SIZE = 65;
const vec3 BACKGROUND_COLOR = vec3(225/255.0, 191/255.0, 146/255.0);  // background color of window

// Gameplay constants
const float FOOD_PER_DROP = 1000;  // amount of food in a drop
const float FOOD_ROT_SPEED = 50;  // amount of food that rots every second
const float GOOD_MAX_FOOD = 350;  // amount of food that a good guy needs to be full
const float BAD_FOOD_RATE = 250;  // how much food bad guys take per second
const float GOOD_FOOD_RATE = 150;  // how much food good guys take per second
const float BAD_SPEED = 40;  // how far bad guys can move per second
const float GOOD_SPEED = 30;  // how far good guys can move per second
const float BAD_RANGE = 200;  // how far bad guys can see food from
const float GOOD_RANGE = 150;  // how far good guys can see food from
const float GOOD_BOOST_FACTOR = 4;  // speed boost factor for good guys
const float BAD_BOOST_FACTOR = 0.25;  // speed boost factor for bad guys
const float SPEED_BOOST_DURATION = 5;  // how long do speed boosts last for
const float PLANE_SPEED = 600;  // speed of drop plane

//******************************************************************
//
//  Class: Game
//
//  Purpose:  To provide a data structure that handles game state, drawing,
//            and updating based on delta time.
//
//  Functions:
//           Constructors
//             Game() = delete
//             Game(num_b_guys, num_g_guys,
//                  num_ts, drops, shader)  create game with given number good and
//                                          bad guys, number trees, drops, and
//                                          shader id
//           setters
//             set_window_size to set the game's window size variable
//           mutators
//             update(dt) to update the game objects' positions and such
//                        based on given delta time
//             init() to initialize game objects and opengl data and such
//           helpers
//             handle_click(pos) to handle a mouse click at pos
//             display(selection_draw) to draw the game elements to
//                                     the frame buffer
//           private helpers
//             target_food(unit, food, range) makes unit target food
//                                            if in range and can reach
//             update_food(dt) updates all of the food drops based on
//                             given delta time
//             update_bad_guys(dt) updates all of the bad guys based on
//                             given delta time
//             update_good_guys(dt) updates all of the good guys based on
//                             given delta time
//             update_plane(dt) updates the plane based on given delta
//                              time
//             is_within_bounds(pos) determines whether the given position
//                                   is within the window bounds
//             is_traversable(pos) determines whether the given position
//                                 is traversable by a unit or not
//             can_reach(a, b, range) determines whether position b is
//                                    reachable by position a within range
//             update_window_title() handles updating the window title with
//                                   game information
//  
//******************************************************************

class Game {
 public:
    Game() = delete;  // no default constructor
    Game(GLuint num_b_guys, GLuint num_g_guys, GLuint num_ts, GLuint drops, GLuint shader)
        : score(0), drops_left(drops), num_bad_guys(num_b_guys), num_good_guys(num_g_guys),
          num_trees(num_ts), shader_id(shader), plane(nullptr), plane_visible(false),
          dropping_food(false), window_size(vec2()) {}
    Game(const Game&) = delete;  // no copy constructor
    Game operator=(const Game&) = delete;  // no copy assignment operator
    ~Game();

    // setters
    void set_window_size(const vec2& size);

    // mutators
    void update(float dt);
    void init();

    // helpers
    void handle_click(const vec2& pos);
    void display(bool selection_draw = false);
 private:
    float score;  // user score
    GLuint drops_left;  // drops left for the player

    GLuint num_bad_guys;  // max number of bad guys
    GLuint num_good_guys;  // max number of good guys
    GLuint num_trees;  // max number of trees
    GLuint shader_id;  // opengl shader proram id
    GLint window_size_loc;  // opengl window size shader uniform variable

    std::vector<Unit*> bad_guys;  // vector containing bad guys
    std::vector<Unit*> good_guys;  // vector containing good guys
    std::vector<Circle*> trees;  // vector containing trees
    std::vector<Circle*> food_drops;  // vector containing food drops

    Unit* plane;  // plane that makes the food drops
    bool plane_visible;  // whether or not the plane is visible
    bool dropping_food;  // whether or not the plane is dropping food

    vec2 window_size;  // window size variable

    // private helpers
    void target_food(Unit* unit, Circle* food, float range);
    void update_food(float dt);
    void update_bad_guys(float dt);
    void update_good_guys(float dt);
    void update_plane(float dt);
    bool is_within_bounds(const vec2& pos) const;
    bool is_traversable(const vec2& pos) const;
    bool can_reach(const vec2& a, const vec2& b, float range) const;
    void update_window_title() const;
};

#endif