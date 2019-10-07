//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        utilities.cc
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This extra code provides utility functions that
//                 are used by the various classes within this project.
//
//    Date:        10/3/2019
//
//*******************************************************************

// C/C++ Standard libraries
#include <cmath>
#include <random>

#define EPSILON 1e-3

// Source libraries
#include "utilities.h"

// NOTE: cmpcolor code is written by Professor Chelberg
//******************************************************************
//
//  Function:   cmpcolor
//
//  Purpose:    compares a 24-bit RGB color with a vec3 color
//
//  Parameters: colora, colorb
//
//  Member/Global Variables: none
//
//  Pre Conditions:  colora and colorb must have valid values
//
//  Post Conditions: returns true if colora matches colorb
//
//  Calls:      none
//
//******************************************************************
bool cmpcolor(unsigned char colora[], vec3 colorb) {
  return((colora[0]==int(colorb.x*255+0.5)) &&
	 (colora[1]==int(colorb.y*255+0.5)) &&
	 (colora[2]==int(colorb.z*255+0.5)));
}

//******************************************************************
//
//  Function:   generate_random
//
//  Purpose:    generates a random number in the range 0 to 1
//
//  Parameters: none
//
//  Member/Global Variables: none
//
//  Pre Conditions:  none
//
//  Post Conditions: returns a random number in the range 0 to 1
//
//  Calls:      none
//
//******************************************************************
float generate_random() {
    static std::default_random_engine generator;
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);

    return distribution(generator);
}

// NOTE: Method to find angle difference is from https://stackoverflow.com/a/11498248
//******************************************************************
//
//  Function:   angle_difference
//
//  Purpose:    returns angle b minus angle a, as the smallest angle
//              possible
//
//  Parameters: a, b
//
//  Member/Global Variables: E_PI
//
//  Pre Conditions:  angles a and b must have valid values, and E_PI
//                   must equal pi
//
//  Post Conditions: returns b - a, as the smallest angle possible
//                   (can be positive or negative)
//
//  Calls:      std::fmod
//
//******************************************************************
float angle_difference(float a, float b) {
    float dif = std::fmod(b - a + E_PI, E_PI * 2);
    if (dif < 0) {
        dif += E_PI * 2;
    }
    return dif - E_PI;
}

//******************************************************************
//
//  Function:   float_equal
//
//  Purpose:    determines whether two floats are equal
//
//  Parameters: a, b
//
//  Member/Global Variables: EPSILON
//
//  Pre Conditions:  a and b must be valid values, and EPSILON must
//                   be a small threshold value
//
//  Post Conditions: returns true if the absolute difference between
//                   b and a is smaller than epsilon
//
//  Calls:      std::fabs
//
//******************************************************************
bool float_equal(float a, float b) {
    return (std::fabs(a - b) < EPSILON);
}
