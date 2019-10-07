//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        utilities.h
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

#ifndef UTILITIES_H
#define UTILITIES_H

// Third Party libraries
#include <Angel.h>

// Definition of pi to many decimal places
#define E_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

// NOTE: cmpcolor code is written by Professor Chelberg
// Function to compare a 24 bit RGB color to a vec3 color
bool cmpcolor(unsigned char colora[], vec3 colorb);

// Function to generate a random number between 0 and 1
float generate_random();

// NOTE: Method to find angle difference is from https://stackoverflow.com/a/11498248
// Function to calculate the smallest angle difference between two angle
float angle_difference(float a, float b);

// Function to determine if two floats are equal
bool float_equal(float a, float b);

#endif