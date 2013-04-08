//
//  main.cpp
//  SimpGLTest
//
//  Created by Cole Faust on 12/29/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#include <iostream>
#include "SimpGL.h"
#include "OpenGLHeaders.h"
#include "SimpGLSprite.h"
#include "SimpGLFont.h"

class MainScene : public SimpGLNode
{
public:
    SimpGLFont *font;
    MainScene()
    {
        std::cout << "Hello, World!\n" << std::endl;
        
        SimpGLinitGL(640, 480, "", "Test");
        SimpGLManager::addRender(this);
        SimpGLManager::addUpdate(this);
        
        SimpGLSprite* test = new SimpGLSprite("pngtest.png");
        test->setX(200);
        test->setY(200);
        test->setColor(0.3, 0.3, 1.0);
        addChild(test);
        
        font = new SimpGLFont("VeraIt.ttf");
    }
    
    void render()
    {
        SimpGLNode::render();
        
        glColor3f(0, 1, 0);
        
        font->render(320, 200, "Hello, world!");
    }
    
    void update()
    {
    }
    
    
};

int main(int argc, char * argv[])
{
    new MainScene;
    SimpGLMainLoop();
    return 0;
}

