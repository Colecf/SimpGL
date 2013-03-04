//
//  SimpGLFont.cpp
//  SimpGL
//
//  Created by Cole Faust on 2/9/13.
//  Copyright (c) 2013 Cole Faust. All rights reserved.
//

#include "SimpGLFont.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <iostream>

namespace  {
    FT_Library ftLibrary;
    bool enabled;
}

SimpGLFont::SimpGLFont(std::string filename)
{
    textures = new GLuint[128];
    FT_Face face;
    if(FT_New_Face(ftLibrary, filename.c_str(), 0, &face))
    {
        std::cout << "Cannot load font " << filename << std::endl;
        return;
    }
    
    size=24;
    
    // *64 because freetype uses 1/64th pixels
    FT_Set_Char_Size(face, size*64, size*64, 96, 96);
    
    listbase = glGenLists(128);
    glGenTextures(128, textures);
    
    for(int i=0; i<128; i++)
    {
        if(FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_DEFAULT))
            std::cout << "Unable to load glyph " << i << " of font " << filename << std::endl;
        
        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph)) {
            std::cout << "Unable to get glyph " << i << " of font " << filename << std::endl;
        }
    }
    
    //free the face
    FT_Done_Face(face);
}

void SimpGLInitFonts()
{
    enabled = true;
    if(FT_Init_FreeType(&ftLibrary))
    {
        std::cout << "Unable to init freetype!" << std::endl;
        enabled = false;
    }
}