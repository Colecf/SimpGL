#include "SimpGLFont.h"
#include "OpenGLHeaders.h"

void renderText(std::string text, void* font, int x, int y)
{
    glPushMatrix();
    
    glRasterPos2i(x, y);
    for (int i = 0; i < text.length(); i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
    
    glPopMatrix();
}

void renderText(std::string text, int x, int y)
{
    renderText(text, GLUT_BITMAP_HELVETICA_18, x, y);
}