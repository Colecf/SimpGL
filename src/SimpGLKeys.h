//
//  SimpGLKeys.h
//  OpenGL
//
//  Created by Cole Faust on 11/5/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#ifndef __OpenGL__SimpGLKeys__
#define __OpenGL__SimpGLKeys__

extern bool* keySpecialStates;
extern bool* keyStates;

void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void keySpecial (int key, int x, int y);
void keySpecialUp (int key, int x, int y);
#endif /* defined(__OpenGL__SimpGLKeys__) */
