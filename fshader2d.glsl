//******************************************************************* 
//
//    Program:     Project 2 - Food Drop Game
//    File:        fshader2d.glsl
//
//    Author:      Kirk Saunders
//    Email:       ks825016@ohio.edu
//
//    Description: This fragment shader simply takes a varying vertex color
//                 and passes it through.
//
//    Date:        10/6/2019
//
//*******************************************************************
varying vec3 vColor;

void main() {
    gl_FragColor = vec4(vColor.rgb, 1.0);
}
