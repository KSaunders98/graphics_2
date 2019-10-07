//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        circle.cc
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

// C/C++ Standard libraries
#include <cmath>

// Source libraries
#include "circle.h"
#include "utilities.h"

// initialize our static member variables
bool Circle::initialized = false;
GLuint Circle::vao = 0;

//******************************************************************
//
//  Function:   Circle::Circle
//
//  Purpose:    creates a circle object with the given parameters
//
//  Parameters: radius, amnt, dim_speed
//
//  Member/Global Variables: init_radius, amount, init_amount, diminish_speed
//
//  Pre Conditions:  radius, amnt, and dim_speed must have valid values
//
//  Post Conditions: circle will be created with the given values
//
//  Calls:      set_radius()
//
//******************************************************************
Circle::Circle(float radius, float amnt, float dim_speed) {
    set_radius(radius);
    init_radius = radius;
    amount = amnt;
    init_amount = amnt;
    diminish_speed = dim_speed;
}

//******************************************************************
//
//  Function:   Circle::get_radius
//
//  Purpose:    gets the radius of the circle
//
//  Parameters: none
//
//  Member/Global Variables: size
//
//  Pre Conditions:  size must have a valid value
//
//  Post Conditions: the radius of the circle (size.x) will be returned
//
//  Calls:      none
//
//******************************************************************
float Circle::get_radius() const {
    return size.x;  // both x and y are equal to radius, so we can return one of them
}

//******************************************************************
//
//  Function:   Circle::get_amount
//
//  Purpose:    gets the amount of substance of the circle
//
//  Parameters: none
//
//  Member/Global Variables: amount
//
//  Pre Conditions:  amount must have a valid value
//
//  Post Conditions: amount will be returned
//
//  Calls:      none
//
//******************************************************************
float Circle::get_amount() const {
    return amount;
}

//******************************************************************
//
//  Function:   Circle::set_radius
//
//  Purpose:    sets the radius of the circle
//
//  Parameters: radius
//
//  Member/Global Variables: none
//
//  Pre Conditions:  radius must have a valid value
//
//  Post Conditions: the circle will have given radius
//
//  Calls:      Object::set_size
//
//******************************************************************
void Circle::set_radius(float radius) {
    set_size(vec2(radius, radius));
}

//******************************************************************
//
//  Function:   Circle::give_amount
//
//  Purpose:    gives the circle an amount of substance
//
//  Parameters: amnt
//
//  Member/Global Variables: amount
//
//  Pre Conditions:  amnt must have a valid value
//
//  Post Conditions: amount will be incremented by amnt
//
//  Calls:      none
//
//******************************************************************
void Circle::give_amount(float amnt) {
    amount += amnt;
}

//******************************************************************
//
//  Function:   Circle::take_amount
//
//  Purpose:    takes an amount of substance from the circle
//
//  Parameters: amnt
//
//  Member/Global Variables: amount
//
//  Pre Conditions:  amount and amnt must have valid values
//
//  Post Conditions: amount will either be 0 or decremented by amnt,
//                   and the amount taken from the circle will be returned
//
//  Calls:      none
//
//******************************************************************
float Circle::take_amount(float amnt) {
    if (amnt > amount) {  // check to see if we can provide that much
        float taken = amount;
        amount = 0;
        return taken;  // return amount we took
    } else {
        amount -= amnt;
        return amnt;  // return amount we took
    }
}

//******************************************************************
//
//  Function:   Circle::update
//
//  Purpose:    updates the circle's size based on amount and delta time
//
//  Parameters: dt
//
//  Member/Global Variables: amount, diminish_speed, init_radius
//
//  Pre Conditions:  dt, amount, diminish_speed, and init_radius must have
//                   valid values
//
//  Post Conditions: the circle's amount and size will be updated according
//                   to dt
//
//  Calls:      take_amount, set_radius
//
//******************************************************************
void Circle::update(float dt) {
    take_amount(dt * diminish_speed);
    set_radius((amount / init_amount) * init_radius);
}

//******************************************************************
//
//  Function:   Circle::init
//
//  Purpose:    initializes the circle's opengl data
//
//  Parameters: shader_id
//
//  Member/Global Variables: initialized
//
//  Pre Conditions:  shader_id must correspond to a valid, active opengl
//                   shader program
//
//  Post Conditions: initialize will be set to true and the circle's
//                   opengl data will be initialized
//
//  Calls:      Object::init, generate_data
//
//******************************************************************
void Circle::init(GLuint shader_id) {
    Object::init(shader_id);
    if (!Circle::initialized) {
        generate_data(shader_id);
        Circle::initialized = true;
    }
}

//******************************************************************
//
//  Function:   Circle::is_gone
//
//  Purpose:    determines whether the circle's amount of substance
//              is gone
//
//  Parameters: none
//
//  Member/Global Variables: amount
//
//  Pre Conditions:  amount must have a valid value
//
//  Post Conditions: true will be returned if amount equals 0
//
//  Calls:      float_equal
//
//******************************************************************
bool Circle::is_gone() const {
    return float_equal(amount, 0.0);
}

//******************************************************************
//
//  Function:   Circle::display
//
//  Purpose:    displays the circle to the frame buffer
//
//  Parameters: selection_draw
//
//  Member/Global Variables: vao, NUM_TRIANGLES
//
//  Pre Conditions:  selection_draw, vao, and NUM_TRIANGLES must have
//                   valid values, and an opengl context must be active
//
//  Post Conditions: draws the circle to the frame buffer
//
//  Calls:      glBindVertexArray, Object::prepare_display, glDrawArrays,
//              glBindVertexArray
//
//******************************************************************
void Circle::display(bool selection_draw) {
    glBindVertexArray(Circle::vao);  // bind vertex array
    Object::prepare_display(selection_draw);
    glDrawArrays(GL_TRIANGLE_FAN, 0, Circle::NUM_TRIANGLES + 2);  // draw circle
    glBindVertexArray(0);  // unbind vertex array
}

//******************************************************************
//
//  Function:   Unit::generate_data
//
//  Purpose:    generates the opengl data for the circle and sends it to
//              the graphics card
//
//  Parameters: shader_id
//
//  Member/Global Variables: NUM_TRIANGLES, vao
//
//  Pre Conditions:  NUM_TRIANGLES must have a valid value, and
//                   a valid opengl context must be active, and
//                   shader_id must correspond to a valid, active
//                   shader program
//
//  Post Conditions: the circle's opengl data will be initialized,
//                   and vao will contain the vertex array object id
//
//  Calls:      glGenVertexArrays, glBindVertexArray, glGenBuffers,
//              glBindBuffer, glBufferData, glEnableVertexAttribArray,
//              glVertexAttribPointer, BUFFER_OFFSET
//
//******************************************************************
void Circle::generate_data(GLuint shader_id) {
    vec2 points[Circle::NUM_TRIANGLES + 2];
    points[0] = vec2(0, 0);  // first point is center of circle

    for (GLuint i = 0; i < Circle::NUM_TRIANGLES + 1; ++i) {
        float angle = static_cast<float>(i) / Circle::NUM_TRIANGLES * 2 * E_PI;
        points[i + 1] = vec2(std::cos(angle), std::sin(angle));
    }

    // Create a vertex array object
    glGenVertexArrays(1, &Circle::vao);
    glBindVertexArray(Circle::vao);
        
    // Create and initialize a buffer object
    GLuint buffer;  // pointer to opengl buffer to hold our vertex data
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (Circle::NUM_TRIANGLES + 2) * 2 * sizeof(float), points, GL_STATIC_DRAW);

    // Initialize the vertex position attribute from the vertex shader
    GLint vert_loc = glGetAttribLocation(shader_id, "vPosition");  // get location of vPosition attrib in shader
    if (vert_loc == -1) {
        std::cerr << "Unable to find vPosition attribute in shader.\n";
        exit(EXIT_FAILURE);
    }
    glEnableVertexAttribArray(vert_loc);  // enable attribute array
    glVertexAttribPointer(vert_loc, 2, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(0));  // create vertex attribute pointer for our data

    // clean up after ourselves, unbind our buffer and vao
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}