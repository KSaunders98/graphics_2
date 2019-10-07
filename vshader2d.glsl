//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        vshader2d.glsl
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This fragment shader takes as uniforms position, size,
//                 rotation, windowSize, color, and darkening_factor, and
//                 applies them to the base vertex color to achieve rendering
//                 2d objects with arbitrary positions and orientations.
//
//    Date:        10/6/2019
//
//*******************************************************************

attribute vec2 vPosition;

uniform vec2 position;
uniform vec2 size;
uniform float rotation;
uniform vec2 windowSize;
uniform vec3 color;
uniform float darkening_factor;

varying vec3 vColor;

void main() {
    // construct transform matrix to rotate coordinates by "rotation" degrees
    // counter-clockwise about the origin (0, 0)
    mat2 rot_transform = mat2(
        cos(rotation), sin(rotation),  // first column
        -sin(rotation), cos(rotation)  // second column
    );

    // construct view scaling matrix
    mat2 scale_transform = mat2(
        (2.0 / windowSize.x), 0,  // first column
        0, (2.0 / windowSize.y)  // second column
    );

    // perform rotation to the base coordinates of the primitive first,
    // then add the object position, then apply the view scaling
    gl_Position.xy = scale_transform * ((rot_transform * vPosition) * size + position);
    gl_Position.zw = vec2(0.0, 1.0);

    // make vertex colors darker the further from the object space origin the vertex is
    // note: a darkening_factor of 0 means don't do any darkening
    vColor = color * max(1.0 - darkening_factor * 0.5 * (vPosition.x*vPosition.x + vPosition.y*vPosition.y), 0.0);
}
