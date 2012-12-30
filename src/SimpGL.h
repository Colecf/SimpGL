//
//  SimpGL.h
//  OpenGL
//
//  Created by Cole Faust on 9/13/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#ifndef __SimpGL__
#define __SimpGL__

#include <stdio.h>
#include <vector>

class SimpGLNode
{
public:
    SimpGLNode();
    ~SimpGLNode();
    
    virtual void render();
    virtual void update();
    std::vector<SimpGLNode*>* getChildren();
    void addChild(SimpGLNode* newChild);
    void removeChild(SimpGLNode* child);
    std::vector<SimpGLNode*> getChildrenWithTag(int searchTag);
    
    int getX();
    int getY();
    int getRotation();
    int getTag();
    void setX(int newX);
    void setY(int newY);
    void setRotation(int newRotation);
    void setTag(int newTag);
    
    
protected:
    std::vector<SimpGLNode*> *children;
    int x, y, rotation, tag;
};

class SimpGLUpdateManager
{
public:
    static void addUpdate(SimpGLNode* newToUpdate);
    static void addRender(SimpGLNode* newToRender);
    static void removeUpdate(SimpGLNode* toRemove);
    static void removeRender(SimpGLNode* toRemove);
    
    //Don't ever call these, they're for GLUT
    static void update(int value);
    static void render();
    
private:
    static std::vector<SimpGLNode*> toUpdate;
    static std::vector<SimpGLNode*> toRender;
};

bool SimpGLinitGL(int width, int height, int argc, char* argv[]);
void processKeypress(unsigned char c, int &x, int &y);

#endif
