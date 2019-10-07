//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        object.h
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This class provides a generic structure
//                 for 2D OpenGL objects that can be inherited from
//                 to create drawn objects.
//
//    Date:        10/3/2019
//
//*******************************************************************

#ifndef OBJECT_H
#define OBJECT_H

// Third-Party libraries
#include <Angel.h>

//******************************************************************
//
//  Class: Object (Abstract)
//
//  Purpose:  To provide basic functions to position, scale, and
//            orient a drawn object.
//
//  Functions:
//           Constructors
//             Object() initializes all values to 0
//           Destructor
//             ~Object() does nothing, but is virtual so inheriting
//                       classes can extend it
//           getters
//             get_size to return the object's size
//             get_position to return the object's position
//             get_rotation to return the object's rotation 
//             get_color to return the object's color
//           setters
//             set_size to set the object's size
//             set_position to set the object's position
//             set_rotation to set the object's rotation 
//             set_color to set the object's color
//           mutators
//             init() to initialize shader variables and select color
//           helpers
//             is_select_color(col) returns true if col matches the object's
//                                  select color
//             display(selection_draw) draws the object to the frame buffer
//             prepare_display(selection_draw) prepares the object to
//                                             be drawn
//
//******************************************************************

class Object {
 public:
    Object() : size(vec2()), position(vec2()), rotation(0), color(vec3()) {}
    virtual ~Object() {}

    // getters
    virtual vec2 get_size() const;
    virtual vec2 get_position() const;
    virtual float get_rotation() const;
    virtual vec3 get_color() const;

    // setters
    virtual void set_size(const vec2& s);
    virtual void set_position(const vec2& p);
    virtual void set_rotation(float r);
    virtual void set_color(const vec3& c);

    // mutators
    virtual void init(GLuint shader_id);

    // helpers
    bool is_select_color(unsigned char col[]) const;
    virtual void display(bool selection_draw) = 0;  // must be implemented by descendant class
 protected:
    vec2 size;
    vec2 position;
    float rotation;
    vec3 color;
    vec3 select_color;

    // static member variables
    static GLint pos_loc;  // shader position variable location
    static GLint size_loc;  // shader size variable location
    static GLint rot_loc;  // shader rotation variable location
    static GLint col_loc;  // shader color variable location
    static GLint df_loc;  // shader darkening factor variable location

    // helpers
    void prepare_display(bool selection_draw);
 private:
    static bool initialized;  // to keep track of whether the data has been created yet
    static GLuint id_counter;  // to keep track of object ID's
};

#endif