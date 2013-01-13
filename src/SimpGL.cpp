//
//  SimpGL.cpp
//  OpenGL
//
//  Created by Cole Faust on 9/13/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#include "SimpGL.h"
#include "OpenGLHeaders.h"
#include "SimpGLKeys.h"
#include "SimpGLTexture.h"

int screenWidth;
int screenHeight;

SimpGLNode::SimpGLNode()
{
    children = new std::vector<SimpGLNode*>;
    x = 0;
    y = 0;
}
SimpGLNode::~SimpGLNode()
{
    children->clear();
    delete children;
}
void SimpGLNode::render()
{
    for (int i=0; i<children->size(); i++) {
        children->at(i)->render();
    }
}

void SimpGLNode::update()
{
    for (int i=0; i<children->size(); i++) {
        children->at(i)->update();
    }
}

std::vector<SimpGLNode*> SimpGLNode::getChildrenWithTag(int searchTag)
{
    std::vector<SimpGLNode*> toReturn;
    for (int i=0; i<children->size(); i++) {
        if (children->at(i)->getTag() == searchTag) {
            toReturn.push_back(children->at(i));
        }
    }
    return toReturn;
}

std::vector<SimpGLNode*>* SimpGLNode::getChildren(){ return children; }
void SimpGLNode::addChild(SimpGLNode* newChild){ children->push_back(newChild); }

void SimpGLNode::removeChild(SimpGLNode* child)
{
    int spotToRemove;
    for (int i=0; i<children->size(); i++) {
        if (children->at(i) == child) {
            spotToRemove = i;
            break;
        }
    }
    children->erase(children->begin()+spotToRemove);
}

int SimpGLNode::getX() { return x; }
int SimpGLNode::getY() { return y; }
int SimpGLNode::getRotation() { return rotation; }
int SimpGLNode::getTag() { return tag; }
void SimpGLNode::setX(int newX) { x = newX; }
void SimpGLNode::setY(int newY) { y = newY; }
void SimpGLNode::setRotation(int newRotation) { rotation = newRotation; }
void SimpGLNode::setTag(int newTag) { tag = newTag; }

void SimpGLUpdateManager::addUpdate(SimpGLNode* newToUpdate)
{
    toUpdate.push_back(newToUpdate);
}
void SimpGLUpdateManager::addRender(SimpGLNode* newToRender)
{
    toRender.push_back(newToRender);
}
void SimpGLUpdateManager::removeUpdate(SimpGLNode* toRemove)
{
    for (int i=0; i<toUpdate.size(); i++) {
        if (toUpdate.at(i) == toRemove) {
            toUpdate.erase(toUpdate.begin()+i);
            return;
        }
    }
}
void SimpGLUpdateManager::removeRender(SimpGLNode* toRemove)
{
    for (int i=0; i<toRender.size(); i++) {
        if (toRender.at(i) == toRemove) {
            toRender.erase(toRender.begin()+i);
            return;
        }
    }
}
void SimpGLUpdateManager::update(int value)
{
    glutTimerFunc(1000/60, &SimpGLUpdateManager::update, 0);
    
    for (int i=0; i<toUpdate.size(); i++) {
        toUpdate.at(i)->update();
    }
    glutPostRedisplay();
}
void SimpGLUpdateManager::render()
{
    glLoadIdentity();
    glClear( GL_COLOR_BUFFER_BIT );
    
    for (int i=0; i<toRender.size(); i++) {
        toRender.at(i)->render();
    }
    
    glutSwapBuffers();
}
std::vector<SimpGLNode*> SimpGLUpdateManager::toUpdate;
std::vector<SimpGLNode*> SimpGLUpdateManager::toRender;




//Inits OpenGL and GLUT
//Parameters:
//width: screen width
//height: screen height
//renderFunc: function that will be openGL's render function
//fps: the frames per second you want the program to run at
//updateFunc: function that will be called every frame specified by your frames per second
//argc/argv: the argc/argv from your main, required by glut.
bool SimpGLinitGL(int width, int height, int argc, char* argv[], std::string newResourcePath)
{
    screenHeight = height;
    screenWidth = width;
    SimpGLTextureCache::getInstance()->setResourcePath(newResourcePath);
    glutInit( &argc, argv );
    
	//Create Double Buffered Window
	glutInitDisplayMode( GLUT_DOUBLE );
	glutInitWindowSize( width, height );
	glutCreateWindow( "OpenGL" );
    
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, width, 0.0, height, 1.0, -1.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    glEnable( GL_TEXTURE_2D );
    
    //Enables transparency
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(keySpecial);
    glutKeyboardUpFunc(keyUp);
    glutSpecialUpFunc(keySpecialUp);
    
    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }
    
    for (int i=0; i<256; i++) {
        keyStates[i] = false;
        if (i<246) {
            keySpecialStates[i] = false;
        }
    }
    
    glutDisplayFunc(&SimpGLUpdateManager::render);
    glutTimerFunc(1000/60, &SimpGLUpdateManager::update, 0);
    
    return true;
}

//Makes the coords relative to the lower left
//as opposed to the upper left
void processKeypress(unsigned char c, int &x, int &y)
{
    y = screenHeight - y;
}