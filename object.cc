//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        object.cc
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

// C/C++ Standard libraries
#include <cstdlib>
#include <iostream>

// Source libraries
#include "object.h"
#include "utilities.h"

// initialize our static member variables
bool Object::initialized = false;
GLint Object::pos_loc = 0;
GLint Object::size_loc = 0;
GLint Object::rot_loc = 0;
GLint Object::col_loc = 0;
GLint Object::df_loc = 0;
GLuint Object::id_counter = 0;

//******************************************************************
//
//  Function:   Object::get_size
//
//  Purpose:    returns the size of the object
//
//  Parameters: none
//
//  Member/Global Variables: size
//
//  Pre Conditions:  variable size must have a value
//
//  Post Conditions: returns the value of size
//
//  Calls:      none
//
//******************************************************************
vec2 Object::get_size() const {
    return size;
}

//******************************************************************
//
//  Function:   Object::get_position
//
//  Purpose:    returns the position of the object
//
//  Parameters: none
//
//  Member/Global Variables: position
//
//  Pre Conditions:  variable position must have a value
//
//  Post Conditions: returns the value of position
//
//  Calls:      none
//
//******************************************************************
vec2 Object::get_position() const {
    return position;
}

//******************************************************************
//
//  Function:   Object::get_rotation
//
//  Purpose:    returns the rotation of the object
//
//  Parameters: none
//
//  Member/Global Variables: rotation
//
//  Pre Conditions:  variable rotation must have a value
//
//  Post Conditions: returns the value of rotation
//
//  Calls:      none
//
//******************************************************************
float Object::get_rotation() const {
    return rotation;
}

//******************************************************************
//
//  Function:   Object::get_color
//
//  Purpose:    returns the color of the object
//
//  Parameters: none
//
//  Member/Global Variables: color
//
//  Pre Conditions:  variable color must have a value
//
//  Post Conditions: returns the value of color
//
//  Calls:      none
//
//******************************************************************
vec3 Object::get_color() const {
    return color;
}

//******************************************************************
//
//  Function:   Object::set_size
//
//  Purpose:    sets the size of the object
//
//  Parameters: s
//
//  Member/Global Variables: size
//
//  Pre Conditions:  variable s must have a value
//
//  Post Conditions: sets size to s
//
//  Calls:      none
//
//******************************************************************
void Object::set_size(const vec2& s) {
    size = s;
}

//******************************************************************
//
//  Function:   Object::set_position
//
//  Purpose:    sets the position of the object
//
//  Parameters: p
//
//  Member/Global Variables: position
//
//  Pre Conditions:  variable p must have a value
//
//  Post Conditions: sets position to p
//
//  Calls:      none
//
//******************************************************************
void Object::set_position(const vec2& p) {
    position = p;
}

//******************************************************************
//
//  Function:   Object::set_rotation
//
//  Purpose:    sets the rotation of the object
//
//  Parameters: r
//
//  Member/Global Variables: rotation
//
//  Pre Conditions:  variable r must have a value
//
//  Post Conditions: sets rotation to r
//
//  Calls:      none
//
//******************************************************************
void Object::set_rotation(float r) {
    rotation = r;
}

//******************************************************************
//
//  Function:   Object::set_color
//
//  Purpose:    sets the color of the object
//
//  Parameters: c
//
//  Member/Global Variables: color
//
//  Pre Conditions:  variable c must have a value
//
//  Post Conditions: sets color to c
//
//  Calls:      none
//
//******************************************************************
void Object::set_color(const vec3& c) {
    color = c;
}

//******************************************************************
//
//  Function:   Object::init
//
//  Purpose:    initializes object information such as shader variable
//              locations
//
//  Parameters: shader_id
//
//  Member/Global Variables: pos_loc, size_loc, rot_loc, col_loc,
//                           df_loc, initialized, select_color,
//                           id_counter
//
//  Pre Conditions:  shader_id must correspond to a valid, active
//                   shader program
//
//  Post Conditions: sets all shader variable locations to their proper
//                   locations from the shader, errors and quits program
//                   if it can't find any, and initialized will be true
//
//  Calls:      glGetUniformLocation, exit
//
//******************************************************************
void Object::init(GLuint shader_id) {
    if (!Object::initialized) {
        Object::pos_loc = glGetUniformLocation(shader_id, "position");  // set pos_loc to the location of the "position" variable in our shader
        if (pos_loc == -1) {
            // error if pos_loc wasn't found
            std::cerr << "Unable to find pos_loc in shader.\n";
            exit(EXIT_FAILURE);
        }

        Object::size_loc = glGetUniformLocation(shader_id, "size");  // set size_loc to the location of the "size" variable in our shader
        if (size_loc == -1) {
            // error if size_loc wasn't found
            std::cerr << "Unable to find size_loc in shader.\n";
            exit(EXIT_FAILURE);
        }

        Object::rot_loc = glGetUniformLocation(shader_id, "rotation");  // set rot_loc to the location of the "rotation" variable in our shader
        if (rot_loc == -1) {
            // error if rot_loc wasn't found
            std::cerr << "Unable to find rot_loc in shader.\n";
            exit(EXIT_FAILURE);
        }

        Object::col_loc = glGetUniformLocation(shader_id, "color");  // set col_loc to the location of the "color" variable in our shader
        if (col_loc == -1) {
            // error if col_loc wasn't found
            std::cerr << "Unable to find col_loc in shader.\n";
            exit(EXIT_FAILURE);
        }

        Object::df_loc = glGetUniformLocation(shader_id, "darkening_factor");  // set df_loc to the location of the "darkening_factor" variable in our shader
        if (df_loc == -1) {
            // error if df_loc wasn't found
            std::cerr << "Unable to find df_loc in shader.\n";
            exit(EXIT_FAILURE);
        }

        Object::initialized = true;
    }

    // assign selection color
    GLuint id = id_counter++;  // using pre-increment, so our first id is 0 (clear color is white, so it corresponds to max id)
    float r = static_cast<float>(id & 0xFF) / 255;  // first byte of id is red component
    float g = static_cast<float>(id >> 8 & 0xFF) / 255;  // second byte of id is green component
    float b = static_cast<float>(id >> 16 & 0xFF) / 255;  // third byte of id is blue component
    select_color = vec3(r, g, b);
}

//******************************************************************
//
//  Function:   Object::is_select_color
//
//  Purpose:    determines whether the given color, col, matches
//              the object's select color
//
//  Parameters: col
//
//  Member/Global Variables: select_color
//
//  Pre Conditions:  col and select_color must have valid values
//
//  Post Conditions: returns true if col matches select_color
//
//  Calls:      cmpcolor
//
//******************************************************************
bool Object::is_select_color(unsigned char col[]) const {
    return cmpcolor(col, select_color);
}

//******************************************************************
//
//  Function:   Object::prepare_display
//
//  Purpose:    prepares the opengl parameters for displaying the
//              object
//
//  Parameters: selection_draw
//
//  Member/Global Variables: pos_loc, size_loc, rot_loc, col_loc,
//                           df_loc, select_color
//
//  Pre Conditions:  an OpenGL context and shader must be active and
//                   all of the shader variable locations must have
//                   proper, correct values
//
//  Post Conditions: the object will be ready to be drawn using opengl,
//                   all uniform variables will be loaded
//
//  Calls:      glUniform2f, glUniform1f, glUniform3f
//
//******************************************************************
void Object::prepare_display(bool selection_draw) {
    glUniform2f(Object::pos_loc, position.x, position.y);  // send position to shader
    glUniform1f(Object::rot_loc, rotation);  // send rotation to shader
    if (selection_draw) {
        glUniform2f(Object::size_loc, size.x * 1.25, size.y * 1.25);  // send slightly bigger size to shader
        glUniform3f(Object::col_loc, select_color.x, select_color.y, select_color.z);  // send select_color to shader
        glUniform1f(Object::df_loc, 0);  // send 0 darkening_factor to shader
    } else {
        glUniform2f(Object::size_loc, size.x, size.y);  // send size to shader
        glUniform3f(Object::col_loc, color.x, color.y, color.z);  // send color to shader
        glUniform1f(Object::df_loc, 1);  // send 1 darkening_factor to shader
    }
}
