//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        circle.h
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This class expands upon the generic object class
//                 to provide a circle that can be drawn.
//                 It also provides member functions for keeping track of
//                 amount of substance (food), and resizing accordingly.
//
//    Date:        10/3/2019
//
//*******************************************************************

#ifndef CIRCLE_H
#define CIRCLE_H

// Source libraries
#include "object.h"

//******************************************************************
//
//  Class: Circle : public Object
//
//  Purpose:  To provide a circle shape that can be drawn and moved
//            about the view space.
//
//  Functions:
//           Constructors
//             Circle() = delete
//             Circle(radius, amnt, dim_speed) create circle with given
//                                             radius, amount of substance,
//                                             and diminish speed
//           getters
//             get_radius to return the radius of the circle
//             get_amount to return the amount of substance the circle has
//           setters
//             set_radius to set the circle's radius
//           mutators
//             give_amount(amnt) to give an amount of substance to the circle
//             take_amount(duration) to take an amount of substance from the circle
//             update(dt) to update the circle's size and animations
//             init(shader_id) to initialize opengl data and such
//           helpers
//             is_gone() returns true if the circle is out of substance
//             display(selection_draw) draws the circle to the frame buffer
//           private helpers
//             generate_data(shader_id) generates the opengl data for the circle
//
//******************************************************************

class Circle : public Object {
 public:
    Circle() = delete;  // no default constructor
    Circle(float radius, float amnt, float dim_speed);

    // getters
    float get_radius() const;
    float get_amount() const;

    // setters
    void set_radius(float r);

    // mutators
    void give_amount(float amnt);
    float take_amount(float amnt);
    void update(float dt);
    void init(GLuint shader_id);

    // helpers
    bool is_gone() const;
    void display(bool selection_draw);
 private:
    float amount;  // could be amount of food, or amount of tree (amount of something the circle has)
    float init_amount;  // original amount directly after initialization
    float diminish_speed;  // speed at which the amount diminishes over time
    float init_radius;  // initial radius

    // static member variables
    static const GLuint NUM_TRIANGLES = 50;  // number of triangles to construct a circle out of
    static bool initialized;  // to keep track of whether the data has been created yet
    static GLuint vao;  // the vao for the circle data

    // private helpers
    void generate_data(GLuint shader_id);
};

#endif