//
//  SimpGLKeys.cpp
//  OpenGL
//
//  Created by Cole Faust on 11/5/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#include "SimpGLKeys.h"

bool* keySpecialStates = new bool[246];
bool* keyStates = new bool[256];

void keyPressed (unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyUp (unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void keySpecial (int key, int x, int y) {
    keySpecialStates[key] = true;
}
void keySpecialUp (int key, int x, int y) {
    keySpecialStates[key] = false;
}