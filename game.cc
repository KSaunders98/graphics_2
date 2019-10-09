//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        game.cc
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This class provides a data structure to store game
//                 state and provides helper functions for updating
//                 state based on delta time and handling mouse events.
//
//    Date:        10/8/2019
//
//*******************************************************************

// C/C++ Standard libraries
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

// Source libraries
#include "game.h"
#include "utilities.h"

//******************************************************************
//
//  Function:   Game::~Game
//
//  Purpose:    handles deallocating all dynamic memory for the game
//              class upon destruction
//
//  Parameters: none
//
//  Member/Global Variables: plane, food_drops, good_guys, bad_guys,
//                           trees
//
//  Pre Conditions:  all of the above variables must be valid vectors,
//                   and all of them must contain valid pointers to
//                   valid memory locations
//
//  Post Conditions: all of the game's objects will have been cleared
//                   and memory cleaned up
//
//  Calls:      none
//
//******************************************************************
Game::~Game() {
    // delete plane
    if (plane != nullptr) {
        delete plane;
    }

    // delete food drops
    for (GLuint i = 0; i < food_drops.size(); ++i) {
        delete food_drops[i];
    }

    // delete good guys
    for (GLuint i = 0; i < good_guys.size(); ++i) {
        delete good_guys[i];
    }

    // delete bad guys
    for (GLuint i = 0; i < bad_guys.size(); ++i) {
        delete bad_guys[i];
    }

    // delete trees
    for (GLuint i = 0; i < trees.size(); ++i) {
        delete trees[i];
    }
}

//******************************************************************
//
//  Function:   Game::set_window_size
//
//  Purpose:    sets the window size variable of the game class
//
//  Parameters: size
//
//  Member/Global Variables: window_size
//
//  Pre Conditions:  size must have a valid value
//
//  Post Conditions: window_size will equal size
//
//  Calls:      none
//
//******************************************************************
void Game::set_window_size(const vec2& size) {
    window_size = size;
}

//******************************************************************
//
//  Function:   Game::update
//
//  Purpose:    to update all of the game objets and state based on given
//              delta time
//
//  Parameters: dt
//
//  Member/Global Variables: good_guys, drops_left, food_drops, plane_visible,
//                           score, FOOD_PER_DROP
//
//  Pre Conditions:  all of the above variables must have valid values
//
//  Post Conditions: the game state will have been updated based on dt
//
//  Calls:      update_plane, update_food, update_bad_guys, update_good_guys
//
//******************************************************************
void Game::update(float dt) {
    // check if game ended
    if (good_guys.size() == 0 || (drops_left == 0 && food_drops.size() == 0 && !plane_visible)) {
        // check if player gets extra points
        if (drops_left > 0) {
            score += drops_left * FOOD_PER_DROP;  // give left over drops to player as points
            drops_left = 0;
        }

        return;
    }

    // call individual update functions
    update_plane(dt);
    update_food(dt);
    update_bad_guys(dt);
    update_good_guys(dt);
}

//******************************************************************
//
//  Function:   Game::init
//
//  Purpose:    initialized all of the game objects and data
//
//  Parameters: none
//
//  Member/Global Variables: plane, num_trees, trees, num_bad_guys, bad_guys,
//                           num_good_guys, good_guys, window_size_loc,
//                           shader_id, BACKGROUND_COLOR, PLANE_SPEED, PLANE_COLOR,
//                           PLANE_SIZE, TREE_MIN_SIZE, TREE_MAX_SIZE, TREE_COLOR,
//                           window_size, BAD_SPEED, BAD_BOOST_FACTOR,
//                           BAD_COLOR, BAD_SIZE, GOOD_MAX_FOOD, GOOD_SPPED,
//                           GOOD_BOOST_FACTOR, GOOD_COLOR, GOOD_SIZE
//
//  Pre Conditions:  all of the above variables must have valid values
//
//  Post Conditions: all of the game objects will have been created according
//                   to the game constants
//
//  Calls:      glClearColor, generate_random, glGetUniformLocation,
//              exit, Unit::set_color, Unit::set_size, Unit::set_position,
//              Unit::set_rotation, Unit::init, Circle::set_color, Circle::set_position,
//              Circle::init, is_traversable
//
//******************************************************************
void Game::init() {
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 1.0);  // set background color

    // create drop plane
    plane = new Unit(0, PLANE_SPEED, 1);
    plane->set_color(PLANE_COLOR);
    plane->set_size(vec2(PLANE_SIZE, PLANE_SIZE));
    plane->init(shader_id);

    for (GLuint i = 0; i < num_trees; ++i) {
        Circle* c = new Circle(TREE_MIN_SIZE + (TREE_MAX_SIZE - TREE_MIN_SIZE) * generate_random(), 0, 0);
        c->set_color(TREE_COLOR);
        c->set_position(window_size * vec2((generate_random() - 0.5), (generate_random() - 0.5)));
        c->init(shader_id);

        trees.push_back(c);
    }

    for (GLuint i = 0; i < num_bad_guys; ++i) {
        Unit* u = new Unit(0, BAD_SPEED, BAD_BOOST_FACTOR);
        u->set_color(BAD_COLOR);
        u->set_size(vec2(BAD_SIZE, BAD_SIZE));

        vec2 pos;
        do {
            pos = window_size * vec2((generate_random() - 0.5), (generate_random() - 0.5));
        } while(!is_traversable(pos));

        u->set_position(pos);
        u->set_rotation(generate_random() * 2 * E_PI);
        u->init(shader_id);

        bad_guys.push_back(u);
    }

    for (GLuint i = 0; i < num_good_guys; ++i) {
        Unit* u = new Unit(GOOD_MAX_FOOD, GOOD_SPEED, GOOD_BOOST_FACTOR);
        u->set_color(GOOD_COLOR);
        u->set_size(vec2(GOOD_SIZE, GOOD_SIZE));

        vec2 pos;
        do {
            pos = window_size * vec2((generate_random() - 0.5), (generate_random() - 0.5));
        } while(!is_traversable(pos));

        u->set_position(pos);
        u->set_rotation(generate_random() * 2 * E_PI);
        u->init(shader_id);

        good_guys.push_back(u);
    }

    window_size_loc = glGetUniformLocation(shader_id, "windowSize");  // set window_size_loc to the location of the "windowSize" variable in our shader
    if (window_size_loc == -1) {
        // error if window_size_loc wasn't found
        std::cerr << "Unable to find window_size_loc in shader.\n";
        exit(EXIT_FAILURE);
    }
}

//******************************************************************
//
//  Function:   Game::handle_click
//
//  Purpose:    to handle a user mouse click event
//
//  Parameters: pos
//
//  Member/Global Variables: BACKGROUND_COLOR, window_size, trees, food_drops,
//                           good_guys, bad_guys, drops_left, plane_visible,
//                           dropping_food, plane, SPEED_BOOST_DURATION
//
//  Pre Conditions:  all of the above variables must have valid values, and
//                   an opengl context must be valid and active
//
//  Post Conditions: the game object will have handled a user click at pos
//
//  Calls:      glClearColor, glClear, glReadBuffer, display, glFlush, glReadPixels,
//              Object::is_select_color, Unit::give_boost, is_traversable,
//              Unit::set_position, Unit::set_target_pos, generate_random
//
//******************************************************************
void Game::handle_click(const vec2& pos) {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // white clear color
    glClear(GL_COLOR_BUFFER_BIT);  // clear color buffer
    glReadBuffer(GL_BACK);  // read from back buffer

    display(true);  // draw our objects with selection rendering
    glFlush();  // ensure our objects are done drawing
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 1.0);  // restore background color to original

    unsigned char pixel_color[3];  // our color data
    glReadPixels(pos.x, window_size.y - pos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel_color);  // read color

    Object* clicked = nullptr;
    for (unsigned int i = 0; i < trees.size(); ++i) {
        if (trees[i]->is_select_color(pixel_color)) {
            // user clicked this tree, will block drop from happening
            clicked = trees[i];
            break;  // no need to check other trees, we found the one that was clicked
        }
    }

    if (clicked == nullptr) {
        for (unsigned int i = 0; i < food_drops.size(); ++i) {
            if (food_drops[i]->is_select_color(pixel_color)) {
                // user clicked this food_drop, will block drop from happening (can't drop on top of a drop)
                clicked = food_drops[i];
                break;  // no need to check other food_drops, we found the one that was clicked
            }
        }
    }

    if (clicked == nullptr) {
        for (unsigned int i = 0; i < good_guys.size(); ++i) {
            if (good_guys[i]->is_select_color(pixel_color)) {
                // user clicked this good guy, make it zoom!
                good_guys[i]->give_boost(SPEED_BOOST_DURATION);  // give boost
                clicked = good_guys[i];
                break;  // no need to check other guys, we found the one that was clicked
            }
        }
    }

    if (clicked == nullptr) {
        for (unsigned int i = 0; i < bad_guys.size(); ++i) {
            if (bad_guys[i]->is_select_color(pixel_color)) {
                // user clicked this bad guy, make it slow
                bad_guys[i]->give_boost(SPEED_BOOST_DURATION);  // give boost (it's boost factor is less than one, so it slows down)
                clicked = bad_guys[i];
                break;  // no need to check other guys, we found the one that was clicked
            }
        }
    }


    if (clicked == nullptr && drops_left > 0) {  // if our mouse click wasn't blocked and we have drops left, do one
        if (!plane_visible) {  // ensure plane isn't already doing a drop          
            vec2 scaled_pos = vec2(pos.x, window_size.y - pos.y) - window_size / 2;  // mouse position scaled into world coordinates
            if (!is_traversable(scaled_pos)) {  // one last check to make sure the clicked position is traversable
                return;
            }

            // schedule a food drop
            plane_visible = true;
            dropping_food = true;
            // set plane to random position off screen and make it target drop position
            plane->set_position(vec2(-window_size.x / 2 - PLANE_SIZE, window_size.y * (generate_random() - 0.5)));
            plane->set_target_pos(scaled_pos);

            drops_left--;  // we used one drop, so decrement
        }
    }
}

//******************************************************************
//
//  Function:   Game::display
//
//  Purpose:    to draw the game state
//
//  Parameters: selection_draw
//
//  Member/Global Variables: window_size_loc, window_size, trees, food_drops,
//                           good_guys, bad_guys, plane_visible, plane
//
//  Pre Conditions:  all of the above variables must have valid values, and
//                   an opengl context must be valid and active
//
//  Post Conditions: all of the game objects will have been drawn to the
//                   frame buffer
//
//  Calls:      glUniform2f, Circle::display, Unit::display, update_window_title
//
//******************************************************************
void Game::display(bool selection_draw) {
    glUniform2f(window_size_loc, window_size.x, window_size.y);  // send window size to shader

    // draw trees
    for (GLuint i = 0; i < trees.size(); ++i) {
        trees[i]->display(selection_draw);
    }

    // draw food drops
    for (GLuint i = 0; i < food_drops.size(); ++i) {
        food_drops[i]->display(selection_draw);
    }

    // draw good guys
    for (GLuint i = 0; i < good_guys.size(); ++i) {
        good_guys[i]->display(selection_draw);
    }

    // draw bad guys
    for (GLuint i = 0; i < bad_guys.size(); ++i) {
        bad_guys[i]->display(selection_draw);
    }

    // draw plane if it should be visible
    if (plane_visible) {
        plane->display(selection_draw);
    }

    update_window_title();
}

//******************************************************************
//
//  Function:   Game::display
//
//  Purpose:    to update all of the game objects and state based on given
//              delta time
//
//  Parameters: dt
//
//  Member/Global Variables: good_guys, drops_left, food_drops, plane_visible,
//                           score, FOOD_PER_DROP
//
//  Pre Conditions:  all of the above variables must have valid values
//
//  Post Conditions: the game state will have been updated based on dt
//
//  Calls:      update_plane, update_food, update_bad_guys, update_good_guys
//
//******************************************************************
void Game::target_food(Unit* unit, Circle* food, float range) {
    if (can_reach(unit->get_position(), food->get_position(), range)) {
        // see if unit has a target already or not
        if (unit->get_target_food() == nullptr) {
            unit->set_target_food(food);  // target food
        } else if (unit->get_target_food() != nullptr && unit->get_target_food() != food) {
            // unit already has a target, but see if this food is closer
            float new_length = length(food->get_position() - unit->get_position());
            float old_length = length(unit->get_target_food()->get_position() - unit->get_position());
            if (new_length < old_length) {
                unit->set_target_food(food);  // target food
            }
        }
    }
}

//******************************************************************
//
//  Function:   Game::update_food
//
//  Purpose:    to update all of the food drops based on given delta time
//
//  Parameters: dt
//
//  Member/Global Variables: food_drop, bad_guys, good_guys, BAD_RANGE
//                           BAD_FOOD_RATE, GOOD_RANGE, GOOD_FOOD_RATE
//
//  Pre Conditions:  all of the above variables must have valid values
//
//  Post Conditions: the food drops in the game will have been updated based
//                   on dt
//
//  Calls:      Circle::is_gone, Unit::get_target_food, Unit::set_target_food,
//              target_food, Circle::take_amount, Circle::give_amount,
//              Unit::give_food, Circle::update, std::swap
//
//******************************************************************
void Game::update_food(float dt) {
    for (GLuint i = 0; i < food_drops.size(); ++i) {
        if (food_drops[i]->is_gone()) {  // food ran out, need to handle deleting it
            Circle* food = food_drops[i];  // get pointer to food
            // order doesn't need to preserved, so we can pop out in constant time
            std::swap(food_drops[i], food_drops[food_drops.size() - 1]);
            food_drops.pop_back();

            // tell bad guys that were targeting it that it ran out
            for (GLuint j = 0; j < bad_guys.size(); ++j) {
                if (bad_guys[j]->get_target_food() == food) {
                    bad_guys[j]->set_target_food(nullptr);
                }
            }
            // tell good guys that were targeting it that it ran out
            for (GLuint j = 0; j < good_guys.size(); ++j) {
                if (good_guys[j]->get_target_food() == food) {
                    good_guys[j]->set_target_food(nullptr);
                }
            }

            delete food;  // clear up resources

            i--;  // we put a different element at index i, so next loop iteration needs to be at i again
        } else {  // food it not gone
            // update food for bad guys
            for (GLuint j = 0; j < bad_guys.size(); ++j) {
                // make bad guy target food if possible
                target_food(bad_guys[j], food_drops[i], BAD_RANGE);

                // if bad guy is targeting food and is already there, make it take the food
                if (bad_guys[j]->get_target_food() == food_drops[i] && bad_guys[j]->is_at_target()) {
                    float avail = food_drops[i]->take_amount(BAD_FOOD_RATE * dt);  // take food from drop
                    score -= avail;  // decrement score by avail
                }
            }

            // update food for good guys
            for (GLuint j = 0; j < good_guys.size(); ++j) {
                // make good guy target food if possible
                target_food(good_guys[j], food_drops[i], GOOD_RANGE);

                // if good guy is targeting food and is already there, make it take the food
                if (good_guys[j]->get_target_food() == food_drops[i] && good_guys[j]->is_at_target()) {
                    float amnt = GOOD_FOOD_RATE * dt;  // amount of food the guy will take
                    float avail = food_drops[i]->take_amount(amnt);  // take food from drop
                    avail -= good_guys[j]->give_food(avail);  // give food to unit;
                    score += amnt - avail;  // increment score by avail
                    if (avail >= 1e-3) {  // if avail is greater than 0
                        food_drops[i]->give_amount(avail);  // unit couldn't take all of the food, give back to drop
                    }
                }
            }

            food_drops[i]->update(dt);  // update food_drop size, etc.
        }
    }
}

//******************************************************************
//
//  Function:   Game::update_bad_guys
//
//  Purpose:    to update all of the bad guys based on given delta time
//
//  Parameters: dt
//
//  Member/Global Variables: bad_guys, BAD_RANGE
//
//  Pre Conditions:  all of the above variables must have valid values
//
//  Post Conditions: the bad guys in the game will have been updated based
//                   on given delta time (dt)
//
//  Calls:      Unit::get_target_food, Unit::is_at_target, Unit::get_position,
//              generate_random, can_reach, Unit::set_target_pos, Unit::update
//
//******************************************************************
void Game::update_bad_guys(float dt) {
    for (GLuint i = 0; i < bad_guys.size(); ++i) {
        // if bad guy doesn't have a target, give it a random position target
        if (bad_guys[i]->get_target_food() == nullptr && bad_guys[i]->is_at_target()) {
            // keep generating random positions within the bad guy's range until one is traversable
            // this is a naive approach, but it works
            vec2 pos;
            do {
                pos = bad_guys[i]->get_position() + BAD_RANGE * vec2((generate_random() - 0.5) * 2, (generate_random() - 0.5) * 2);
            } while(!can_reach(bad_guys[i]->get_position(), pos, BAD_RANGE));

            bad_guys[i]->set_target_pos(pos);  // set target position
        }

        bad_guys[i]->update(dt);  // update position and rotation, etc.
    }
}

//******************************************************************
//
//  Function:   Game::update_good_guys
//
//  Purpose:    to update all of the good guys based on given delta time
//
//  Parameters: dt
//
//  Member/Global Variables: good_guys, score, GOOD_RANGE, GOOD_MAX_FOOD
//
//  Pre Conditions:  all of the above variables must have valid values
//
//  Post Conditions: the good guys in the game will have been updated based
//                   on given delta time (dt)
//
//  Calls:      Unit::is_full, Unit::get_target_food, Unit::is_at_target,
//              Unit::get_position, generate_random, can_reach, Unit::set_target_pos,
//              Unit::update, std::swap
//
//******************************************************************
void Game::update_good_guys(float dt) {
    for (GLuint i = 0; i < good_guys.size(); ++i) {
        if (good_guys[i]->is_full()) {  // if good guy is full
            // need to remove from game
            Unit* u = good_guys[i];  // get pointer of good guy
            // order doesn't need to preserved, so we can pop out in constant time
            std::swap(good_guys[i], good_guys[good_guys.size() - 1]);
            good_guys.pop_back();

            delete u;  // clear up resources

            score += GOOD_MAX_FOOD * 2;  // increment score by double the amount of food the good guy got

            i--;  // we put a different element at index i, so next loop iteration needs to be at i again
        } else {
            // if good guy doesn't have a target, give it a random position target
            if (good_guys[i]->get_target_food() == nullptr && good_guys[i]->is_at_target()) {
                // keep generating random positions within the good guy's range until one is traversable
                // this is a naive approach, but it works
                vec2 pos;
                do {
                    pos = good_guys[i]->get_position() + GOOD_RANGE * vec2((generate_random() - 0.5) * 2, (generate_random() - 0.5) * 2);
                } while(!can_reach(good_guys[i]->get_position(), pos, GOOD_RANGE));
                
                good_guys[i]->set_target_pos(pos);  // set target position
            }

            good_guys[i]->update(dt);  // update position, rotation, etc.
        }
    }
}

//******************************************************************
//
//  Function:   Game::update_plane
//
//  Purpose:    to update the plane based on given delta time
//
//  Parameters: dt
//
//  Member/Global Variables: plane, plane_visible, dropping_food, FOOD_SIZE,
//                           FOOD_PER_DROP, FOOD_ROT_SPEED, FOOD_COLOR,
//                           shader_id, window_size, PLANE_SIZE
//
//  Pre Conditions:  all of the above variables must have valid values
//
//  Post Conditions: the plane in the game will have been updated based
//                   on given delta time (dt)
//
//  Calls:      Unit::is_at_target, Unit::get_position, Circle::set_color,
//              Circle::set_position, Circle::init, Unit::set_target_pos,
//              generate_random, Unit::update
//
//******************************************************************
void Game::update_plane(float dt) {
    if (plane_visible && plane->is_at_target()) {  // if plane has finished one of two stages
        if (dropping_food) {  // plane reached drop position
            // create food drop at location
            Circle* c = new Circle(FOOD_SIZE, FOOD_PER_DROP, FOOD_ROT_SPEED);
            c->set_color(FOOD_COLOR);
            c->set_position(plane->get_position());
            c->init(shader_id);
            food_drops.push_back(c);

            // make plane target somewhere random off-screen to the right
            plane->set_target_pos(vec2(window_size.x / 2 + PLANE_SIZE, window_size.y * (generate_random() - 0.5)));

            dropping_food = false;
        } else {  // plane finished drop and left screen
            plane_visible = false;
        }
    }

    if (plane_visible) {
        plane->update(dt);  // update position, rotation, etc.
    }
}

//******************************************************************
//
//  Function:   Game::is_within_bounds
//
//  Purpose:    to determine whether the given position is within game
//              view bounds or not
//
//  Parameters: pos
//
//  Member/Global Variables: window_size
//
//  Pre Conditions:  window_size and pos must have valid values
//
//  Post Conditions: will return true if pos is within the window view bounds
//
//  Calls:      none
//
//******************************************************************
bool Game::is_within_bounds(const vec2& pos) const {
    vec2 half_window_size = window_size / 2;
    if (pos.x < -half_window_size.x || pos.x > half_window_size.x) {  // x component out of window range
        return false;
    }
    if (pos.y < -half_window_size.y || pos.y > half_window_size.y) {  // y component out of window range
        return false;
    }

    return true;
}

//******************************************************************
//
//  Function:   Game::is_traversable
//
//  Purpose:    to determine whether the given position is traversable
//              for the units or not
//
//  Parameters: pos
//
//  Member/Global Variables: trees, BAD_SIZE, GOOD_SIZE
//
//  Pre Conditions:  the above variables must have valid values
//
//  Post Conditions: will return true if pos is traversable by units
//
//  Calls:      is_within_bounds, length, Circle::get_position, Circle::get_radius,
//              std::max
//
//******************************************************************
bool Game::is_traversable(const vec2& pos) const {
    if (!is_within_bounds(pos)) {
        return false;
    }

    for (GLuint i = 0; i < trees.size(); ++i) {
        // if position is within the tree's radius, it is not traversable
        if (length(pos - trees[i]->get_position()) < (trees[i]->get_radius() + std::max(BAD_SIZE, GOOD_SIZE) / 2)) {
            return false;
        }
    }

    return true;
}

//******************************************************************
//
//  Function:   Game::can_reach
//
//  Purpose:    to determine whether given position a is reachable by
//              given position b within range
//
//  Parameters: a, b, range
//
//  Member/Global Variables: trees, BAD_SIZE, GOOD_SIZE
//
//  Pre Conditions:  the above variables must have valid values
//
//  Post Conditions: will return true if a is reachable by b within range
//
//  Calls:      is_within_bounds, length, Circle::get_position, Circle::get_radius,
//              std::max, dot
//
//******************************************************************
bool Game::can_reach(const vec2& a, const vec2& b, float range) const {
    if (!is_within_bounds(b)) {
        // if a isn't in bounds either, let it slide
        // this exception prevents crashing, but as a result
        // some units may be lost outside the bounds of the window
        // forever if you resize the window and they are cut off
        if (is_within_bounds(a)) {
            return false;
        }
    }

    vec2 dir = b - a;  // get displacement vector from b to a
    float dist = length(dir);  // we need the length to normalize anyways, so we can't avoid this square root

    if (dist > range) {
        return false;
    }

    for (GLuint i = 0; i < trees.size(); ++i) {
        vec2 dir2 = trees[i]->get_position() - a;
        float scalar_proj = dot(dir, dir2) / dist;  // scalar projection of dir2 onto dir
        float min_radius = (trees[i]->get_radius() + std::max(BAD_SIZE, GOOD_SIZE) / 2);  // minimum radius for outside of tree
        min_radius *= min_radius;  // square minimum radius

        if (scalar_proj > 0 && scalar_proj < dist) {  // check whether projection is actually on line segement defined by b and a
            vec2 projection = a + (dir / dist) * scalar_proj;  // vector projection of dir2 onto dir

            vec2 radius_vec = trees[i]->get_position() - projection;  // displacement vector from tree center to projection point
            // check whether length of radius_vec squared falls within minimum radius
            // (doing this to avoid expensive square root calls)
            if (dot(radius_vec, radius_vec) < min_radius) {
                return false;
            }
        }

        vec2 radius_vec = trees[i]->get_position() - b;
        // finally, check if the final point falls within the tree's radius
        // (again, doing this to avoid expensive square root calls)
        if (dot(radius_vec, radius_vec) < min_radius) {
            return false;
        }
    }

    return true;
}

//******************************************************************
//
//  Function:   Game::update_window_title
//
//  Purpose:    to update the window title to contain game information
//
//  Parameters: none
//
//  Member/Global Variables: good_guys, drops_left, food_drops, plane_visible,
//                           score, GAME_TITLE
//
//  Pre Conditions:  the above variables must have valid values
//
//  Post Conditions: the window title will be updated with game information
//
//  Calls:      glutSetWindowTitle
//
//******************************************************************
void Game::update_window_title() const {
    // create string stream to write message to
    std::stringstream sstream;
    sstream.precision(1);
    sstream << GAME_TITLE;

    if (good_guys.size() == 0 || (drops_left == 0 && food_drops.size() == 0 && !plane_visible)) {
        // game ended
        sstream << " | GAME OVER! ";
        if (good_guys.size() == 0) {
            sstream << "YOU WIN! ";
        } else {
            sstream << "YOU LOSE! ";
        }
        sstream << "Final Score: " << std::fixed << score;
    } else {
        sstream << " | Score: " << std::fixed << score << ", Drops left: " << drops_left;
    }

    // convert string stream to c-type string and set it as window title
    glutSetWindowTitle(sstream.str().c_str());
}
