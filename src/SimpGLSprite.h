//
//  SimpGLAnimatedSprite.h
//  OpenGL
//
//  Created by Cole Faust on 12/21/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#ifndef __OpenGL__SimpGLAnimatedSprite__
#define __OpenGL__SimpGLAnimatedSprite__

#include "SimpGLTexture.h"

class SimpGLSprite : public SimpGLNode
{
public:
    
    SimpGLSprite(std::string fileName);
    ~SimpGLSprite();
    
    virtual void render();
    
    int getWidth();
    int getHeight();
    int getRotation();
    float getOpacity();
    void setWidth(int newWidth);
    void setHeight(int newHeight);
    void setRotation(int newRotation);
    void setOpacity(float newOpacity);
    
protected:
    //typedef SimpGLNode super;
    
    SimpGLTexture *texture;
    
    int width, height;
    float opacity;
};

class SimpGLAnimatedSprite : public SimpGLSprite
{
public:
    SimpGLAnimatedSprite(std::string fileName, int newFrameWidth, int newFrameHeight, int newTotalFrames, int newDeltaFrame);
    ~SimpGLAnimatedSprite();
    
    virtual void render();
    virtual void update();
    
protected:
    int frame, frameWidth, frameHeight, totalFrames, deltaFrame, frameCounter;
};

#endif /* defined(__OpenGL__SimpGLAnimatedSprite__) */
