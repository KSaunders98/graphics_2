//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        unit.cc
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

// C/C++ Standard libraries
#include <cmath>

// Source libraries
#include "unit.h"
#include "utilities.h"

// initialize our static member variables
bool Unit::initialized = false;
GLuint Unit::vao = 0;

//******************************************************************
//
//  Function:   Unit::get_food
//
//  Purpose:    gets the amount of food the unit has gathered
//
//  Parameters: none
//
//  Member/Global Variables: food
//
//  Pre Conditions:  food must have a valid value
//
//  Post Conditions: returns the value of variable food
//
//  Calls:      none
//
//******************************************************************
float Unit::get_food() const {
    return food;
}

//******************************************************************
//
//  Function:   Unit::get_target_food
//
//  Purpose:    gets the pointer to the food that the unit is targeting
//
//  Parameters: none
//
//  Member/Global Variables: target_food
//
//  Pre Conditions:  target_food must have a valid value
//
//  Post Conditions: returns the value of variable target_food
//
//  Calls:      none
//
//******************************************************************
Object* Unit::get_target_food() const {
    return target_food;
}

//******************************************************************
//
//  Function:   Unit::set_position
//
//  Purpose:    sets the position of the unit
//
//  Parameters: pos
//
//  Member/Global Variables: target_pos
//
//  Pre Conditions:  pos must have a valid value and Object::set_position
//                   must be implemented
//
//  Post Conditions: sets position and target_pos of unit to pos
//
//  Calls:      Object::set_position
//
//******************************************************************
void Unit::set_position(const vec2& pos) {
    target_pos = pos;
    Object::set_position(pos);
}

//******************************************************************
//
//  Function:   Unit::set_target_food
//
//  Purpose:    sets the target of the unit to the given food object
//
//  Parameters: obj
//
//  Member/Global Variables: target_food, target_pos
//
//  Pre Conditions:  obj must either be nullptr or be a valid pointer
//                   to an object
//
//  Post Conditions: sets target_pos and target_food according to the
//                   given pointer (obj)
//
//  Calls:      none
//
//******************************************************************
void Unit::set_target_food(Object* obj) {
    if (obj != nullptr) {
        target_food = obj;
        target_pos = obj->get_position();
    } else {
        target_food = nullptr;
        target_pos = position;
    }
}

//******************************************************************
//
//  Function:   Unit::set_target_pos
//
//  Purpose:    sets the target of the unit to the given position
//
//  Parameters: pos
//
//  Member/Global Variables: target_food, target_pos
//
//  Pre Conditions:  pos must be a valid position value
//
//  Post Conditions: sets target_pos to pos and target_food to nullptr
//
//  Calls:      none
//
//******************************************************************
void Unit::set_target_pos(const vec2& pos) {
    target_food = nullptr;
    target_pos = pos;
}

//******************************************************************
//
//  Function:   Unit::give_food
//
//  Purpose:    gives the unit an amount of food
//
//  Parameters: amnt
//
//  Member/Global Variables: food, max_food
//
//  Pre Conditions:  amnt and max_food must be valid values
//
//  Post Conditions: tries to give the unit amnt food, and returns
//                   the total amount actually given, after maxing
//                   out at max_food
//
//  Calls:      none
//
//******************************************************************
float Unit::give_food(float amnt) {
    if ((food + amnt) > max_food) {
        float taken = max_food - food;
        food = max_food;
        return taken;  // return amount that we took
    } else {
        food += amnt;
        return amnt;  // return amount that we took
    }
}

//******************************************************************
//
//  Function:   Unit::give_boost
//
//  Purpose:    gives the unit a speed boost
//
//  Parameters: duration
//
//  Member/Global Variables: boost_duration
//
//  Pre Conditions:  duration must be a valid value
//
//  Post Conditions: boost_duration is set to duration
//
//  Calls:      none
//
//******************************************************************
void Unit::give_boost(float duration) {
    boost_duration = duration;
}

//******************************************************************
//
//  Function:   Unit::update
//
//  Purpose:    updates the unit's position, rotation, and other properties
//              based on the given delta time
//
//  Parameters: dt
//
//  Member/Global Variables: boost_duration, boost_factor, target_food,
//                           target_pos, target_rot, position, rotation
//
//  Pre Conditions:  dt must be a valid value
//
//  Post Conditions: the unit's position, rotation, and other
//                   properties are updated based dt
//
//  Calls:      std::max, length, is_at_target, std::atan2, normalize,
//              angle_difference, std::min
//
//******************************************************************
void Unit::update(float dt) {
    if (boost_duration > 1e-3) {
        boost_duration = std::max(boost_duration - dt, 0.0f);  // decrease boost_duration by dt and clamp to positive values
    }

    if (boost_duration > 1e-3) {
        dt *= boost_factor;  // multiply dt by boost_factor if boost_duration is greater than 0
    }

    if (target_food != nullptr) {
        target_pos = target_food->get_position();
    }

    if (!is_at_target()) {
        vec2 dir = target_pos - position;
        float movement = dt * speed;
        target_rot = std::atan2(dir.y, dir.x);  // angle unit towards target
        // avoid expensive square root
        if (dot(dir, dir) <= movement*movement) {
            position = target_pos;
        } else {
            position += normalize(dir) * movement;
        }
    }
    rotation += angle_difference(rotation, target_rot) * std::min(dt * 2, 1.0f);
}

//******************************************************************
//
//  Function:   Unit::init
//
//  Purpose:    initializes the unit's opengl data
//
//  Parameters: shader_id
//
//  Member/Global Variables: initialized
//
//  Pre Conditions:  shader_id must correspond to a valid, active
//                   shader program
//
//  Post Conditions: the unit's opengl data will be initialized,
//                   and initialized will be set to true
//
//  Calls:      Object::init, generate_data
//
//******************************************************************
void Unit::init(GLuint shader_id) {
    Object::init(shader_id);
    if (!Unit::initialized) {
        generate_data(shader_id);
        Unit::initialized = true;
    }
}

//******************************************************************
//
//  Function:   Unit::is_full
//
//  Purpose:    returns whether the unit is full of food or not
//
//  Parameters: none
//
//  Member/Global Variables: food, max_food
//
//  Pre Conditions:  food and max_food must have valid values
//
//  Post Conditions: returns true if food == max_food
//
//  Calls:      float_equal
//
//******************************************************************
bool Unit::is_full() const {
    return float_equal(food, max_food);
}

//******************************************************************
//
//  Function:   Unit::is_at_target
//
//  Purpose:    returns whether the unit is at its target or not
//
//  Parameters: none
//
//  Member/Global Variables: position, target_pos
//
//  Pre Conditions:  position and target_pos must have valid values
//
//  Post Conditions: returns true if position == target_pos
//
//  Calls:      float_equal
//
//******************************************************************
bool Unit::is_at_target() const {
    return float_equal(position.x, target_pos.x)
        && float_equal(position.y, target_pos.y);
}

//******************************************************************
//
//  Function:   Unit::display
//
//  Purpose:    displays the unit to the frame buffer
//
//  Parameters: selection_draw
//
//  Member/Global Variables: vao, NUM_TRIANGLES
//
//  Pre Conditions:  selection_draw, vao, and NUM_TRIANGLES must have
//                   valid values, and an opengl context must be active
//
//  Post Conditions: draws the unit to the frame buffer
//
//  Calls:      glBindVertexArray, Object::prepare_display, glDrawArrays,
//              glBindVertexArray
//
//******************************************************************
void Unit::display(bool selection_draw) {
    glBindVertexArray(Unit::vao);  // bind vertex array
    prepare_display(selection_draw);
    glDrawArrays(GL_TRIANGLES, 0, Unit::NUM_TRIANGLES * 3);  // draw unit
    glBindVertexArray(0);  // unbind vertex array
}

//******************************************************************
//
//  Function:   Unit::generate_data
//
//  Purpose:    generates the opengl data for the unit and sends it to
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
//  Post Conditions: the unit's opengl data will be initialized,
//                   and vao will contain the vertex array object id
//
//  Calls:      glGenVertexArrays, glBindVertexArray, glGenBuffers,
//              glBindBuffer, glBufferData, glEnableVertexAttribArray,
//              glVertexAttribPointer, BUFFER_OFFSET
//
//******************************************************************
void Unit::generate_data(GLuint shader_id) {
    vec2 points[Unit::NUM_TRIANGLES * 3];  // 3 points per triangle
    // left triangle
    points[0] = vec2(0, 0);  // make tip of unit at zero
    points[1] = vec2(-1, 0.5);
    points[2] = vec2(-0.75, 0);
    // right triangle
    points[3] = vec2(0, 0);
    points[4] = vec2(-0.75, 0);
    points[5] = vec2(-1, -0.5);

    // Create a vertex array object
    glGenVertexArrays(1, &Unit::vao);
    glBindVertexArray(Unit::vao);
        
    // Create and initialize a buffer object
    GLuint buffer;  // pointer to opengl buffer to hold our vertex data
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, Unit::NUM_TRIANGLES * 3 * 2 * sizeof(float), points, GL_STATIC_DRAW);

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
