//
//  SimpGLMenu.cpp
//  OpenGL
//
//  Created by Cole Faust on 12/5/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#include "SimpGLMenu.h"
#include "OpenGLHeaders.h"

void SimpGLButton::render()
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(rotation, 0, 0, 1);
    
    //render stuff here
    
    SimpGLNode::render();
    glPopMatrix();
}