//
//  SimpGLFont.h
//  SimpGL
//
//  Created by Cole Faust on 2/9/13.
//  Copyright (c) 2013 Cole Faust. All rights reserved.
//

#ifndef __SimpGL__SimpGLFont__
#define __SimpGL__SimpGLFont__

#include <string>
#include "OpenGLHeaders.h"

class SimpGLFont
{
public:
    SimpGLFont(std::string fileName);
private:
    int size;
    GLuint *textures;
    GLuint listbase;
};
void SimpGLInitFonts();

#endif /* defined(__SimpGL__SimpGLFont__) */
