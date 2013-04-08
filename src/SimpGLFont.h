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
#include <vector>
#include "OpenGLHeaders.h"
#include "SimpGL.h"

class SimpGLFont
{
public:
    SimpGLFont(std::string fileName);
    ~SimpGLFont();
    void render(float x, float y, std::string text);
    std::string getFilename();
private:
    int size;
    GLuint *textures;
    GLuint listbase;
    int references;
    std::string filename;
    friend class SimpGLFontCache;
};
void SimpGLInitFonts();

class SimpGLFontCache
{
public:
    static SimpGLFontCache* getInstance();
    SimpGLFont* getFontFromFileName(std::string fileName);
    void releaseFont(SimpGLFont* texToRelease);
    
private:
    
    std::vector<SimpGLFont*> fonts;
    static SimpGLFontCache* instance;
    
    SimpGLFontCache(){};  // Private so that it can  not be called
    SimpGLFontCache(SimpGLFontCache const&){};             // copy constructor is private
    SimpGLFontCache& operator=(SimpGLFontCache const&){};  // assignment operator is private
};

class SimpGLLabel : public SimpGLNode
{
public:
    SimpGLLabel(std::string fontName);
    ~SimpGLLabel();
    void setText(std::string newText);
    std::string getText();
    void setFont(std::string fontName);
    
    void render();
private:
    std::string text;
    SimpGLFont *font;
    
};

#endif /* defined(__SimpGL__SimpGLFont__) */
