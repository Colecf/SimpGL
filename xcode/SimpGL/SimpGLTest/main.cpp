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

class MainScene : public SimpGLNode
{
public:
    void run(int argc, char * argv[])
    {
        std::cout << "Hello, World!\n" << std::endl;
        
        SimpGLinitGL(640, 480, argc, argv, "");
        SimpGLUpdateManager::addRender(this);
        
        SimpGLSprite* test = new SimpGLSprite("pngtest.png");
        test->setX(200);
        test->setY(200);
        addChild(test);
        
        glutMainLoop();
    }
    
    void render()
    {
        SimpGLNode::render();
        
        glColor3f(1, 0, 0);
        glTranslatef(10, 10, 0);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(0, 50);
        glVertex2f(50, 50);
        glVertex2f(50, 0);
        glEnd();
    }
    
    
};

int main(int argc, char * argv[])
{
    MainScene *scene = new MainScene;
    scene->run(argc, argv);
    return 0;
}

