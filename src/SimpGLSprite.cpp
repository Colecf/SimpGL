//
//  SimpGLAnimatedSprite.cpp
//  OpenGL
//
//  Created by Cole Faust on 12/21/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#include "SimpGLSprite.h"
#include <iostream>


SimpGLSprite::SimpGLSprite(std::string fileName)
{
    x = y = 0;
    red = green = blue = 0.0;
    rotation = 0;
    opacity = 1;
    texture = SimpGLTextureCache::getInstance()->getTextureFromFileName(fileName);
    width = texture->getWidth();
    height = texture->getHeight();
}

SimpGLSprite::~SimpGLSprite()
{
    SimpGLTextureCache::getInstance()->releaseTexture(texture);
}

void SimpGLSprite::render()
{
    if (!texture && texture->getTexID() == 0) {
        return;
    }
    
    glPushMatrix();
        glColor4f(red, green, blue, opacity);
        glTranslatef(x, y, 0);
        //The 0, 0, 1, is the vector we're rotating around
        glRotatef(rotation, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, texture->getTexID());
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f( -width/2, -height/2 );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( width/2, -height/2 );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( width/2, height/2 );
            glTexCoord2f( 0.f, 1.f ); glVertex2f( -width/2, height/2 );
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        SimpGLNode::render();
    glPopMatrix();
}

int SimpGLSprite::getWidth() { return width; }
int SimpGLSprite::getHeight() { return height; }
int SimpGLSprite::getRotation() { return rotation; }
float SimpGLSprite::getOpacity() { return opacity; }
void SimpGLSprite::setWidth(int newWidth) { width = newWidth; }
void SimpGLSprite::setHeight(int newHeight) { height = newHeight; }
void SimpGLSprite::setOpacity(float newOpacity) { opacity = newOpacity; }
void SimpGLSprite::setRotation(int newRotation)
{
    while (newRotation >= 360 || newRotation < 0)
    {
        if (newRotation >= 360) {
            newRotation -= 360;
        } else if (newRotation < 0) {
            newRotation += 360;
        }
    }
    rotation = newRotation;
}
void SimpGLSprite::setColor(float r, float g, float b)
{
    red = r;
    green = g;
    blue = b;
}

SimpGLAnimatedSprite::SimpGLAnimatedSprite(std::string fileName, int newFrameWidth, int newFrameHeight,
                                           int newTotalFrames, int newDeltaFrame)
                                            : SimpGLSprite(fileName)
{
    frame = 0;
    frameCounter = 0;
    frameWidth = newFrameWidth;
    frameHeight = newFrameHeight;
    deltaFrame = newDeltaFrame;
    totalFrames = newTotalFrames;
    red = 1;
    green = 1;
    blue = 1;
}
SimpGLAnimatedSprite::~SimpGLAnimatedSprite()
{
    //Windows computers don't like this for some reason
    //SimpGLSprite::~SimpGLSprite();
}

void SimpGLAnimatedSprite::render()
{
    if (!texture && texture->getTexID() == 0) {
        return;
    }
    
    float lowerLeftX = (float)(frame*frameWidth)/(float)width;
    float lowerLeftY = 1.0-((float)frameHeight/(float)height);
    while (lowerLeftX >= 1) {
        lowerLeftX -= 1;
        lowerLeftY -= (float)frameHeight/(float)height;
    }
    
    glPushMatrix();
        glColor4f(red, green, blue, opacity);
        glTranslatef(x, y, 0);
        //The 0, 0, 1, is the vector we're rotating around
        glRotatef(rotation, 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, texture->getTexID());
        glBegin( GL_QUADS );
            glTexCoord2f( lowerLeftX, lowerLeftY ); glVertex2f( -frameWidth/2, -frameHeight/2 );
            glTexCoord2f( lowerLeftX+((float)frameWidth/(float)width), lowerLeftY ); glVertex2f( frameWidth/2, -frameHeight/2 );
            glTexCoord2f( lowerLeftX+((float)frameWidth/(float)width), lowerLeftY+((float)frameHeight/(float)height) ); glVertex2f( frameWidth/2, frameHeight/2 );
            glTexCoord2f( lowerLeftX, lowerLeftY+((float)frameHeight/(float)height) ); glVertex2f( -frameWidth/2, frameHeight/2 );
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        SimpGLNode::render();
    glPopMatrix();
}

void SimpGLAnimatedSprite::update()
{
    frameCounter++;
    if (frameCounter == deltaFrame) {
        frameCounter = 0;
        frame++;
        if (frame > totalFrames) {
            frame = 0;
        }
    }
    
    SimpGLNode::update();
}