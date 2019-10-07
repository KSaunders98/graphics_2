//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        unit.h
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This class expands upon the generic object class
//                 to provide a chevron-type shape that can be drawn.
//                 It also provides member functions for collecting food,
//                 and updating target locations.
//
//    Date:        10/3/2019
//
//*******************************************************************

#ifndef UNIT_H
#define UNIT_H

// Source libraries
#include "object.h"

//******************************************************************
//
//  Class: Unit : public Object
//
//  Purpose:  To provide a chevron-type shape that can drawn and moved
//            about the view space.
//
//  Functions:
//           Constructors
//             Unit() = delete
//             Unit(max_f, spd, bst_factor) create unit with given max
//                                          food, speed, and boost factor
//           getters
//             get_food to return the amount of food the unit has gathered
//             get_target_food to return the food that the unit is targeting
//           setters
//             set_position to set the unit's position
//             set_target_food to set the unit's target food
//             set_target_pos to set the unit's target position
//           mutators
//             give_food(amnt) to give food to the unit
//             give_boost(duration) to give a boost in speed to the unit
//             update(dt) to update the unit's movement and animations
//             init() to initialize opengl data and such
//           helpers
//             is_full() returns true if the unit is full of food
//             is_at_target() returns true if the unit is at the target
//             display(selection_draw) draws the unit to the frame buffer
//           private helpers
//             generate_data(shader_id) generates the opengl data for the unit
//  
//******************************************************************

class Unit : public Object {
 public:
    Unit() = delete;  // no default constructor
    Unit(float max_f, float spd, float bst_factor) : food(0), max_food(max_f), speed(spd),
        boost_factor(bst_factor), target_food(nullptr), target_pos(vec2()),
        target_rot(rotation), boost_duration(0) {}

    // getters
    float get_food() const;
    Object* get_target_food() const;

    // setters
    virtual void set_position(const vec2& pos);
    void set_target_food(Object* obj);
    void set_target_pos(const vec2& pos);

    // mutators
    float give_food(float amnt);
    void give_boost(float duration);
    void update(float dt);
    virtual void init(GLuint shader_id);

    // helpers
    bool is_full() const;
    bool is_at_target() const;
    virtual void display(bool selection_draw);
 private:
    float food;
    float max_food;
    float speed;
    float boost_factor;
    Object* target_food;
    vec2 target_pos;
    float target_rot;
    float boost_duration;

    // static member variables
    static const GLuint NUM_TRIANGLES = 2;  // number of triangles to construct a unit out of
    static bool initialized;  // to keep track of whether the data has been created yet
    static GLuint vao;  // the vao for the unit data

    // private helpers
    void generate_data(GLuint shader_id);
};

#endif