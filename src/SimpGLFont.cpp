//
//  SimpGLFont.cpp
//  SimpGL
//
//  Most of this code is a slightly-edited version
//  of nehe's tutorial code
//  http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
//

#include "SimpGLFont.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <iostream>
#include <vector>

namespace  {
    FT_Library ftLibrary;
    bool enabled;
    
    //returns the lowest power of 2 >= a
    int next_p2 (int a )
    {
        int rval=1;
        while(rval<a) rval*=2;
        return rval;
    }
    
    //makes the opengl coords equal to
    //pixels of the screen
    void pushScreenCoordinateMatrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        GLint   viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
        glPopAttrib();
    }
    
    // Pops The Projection Matrix Without Changing The Current
    // MatrixMode.
    void pop_projection_matrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();
    }
}

SimpGLFont::SimpGLFont(std::string filename)
{
    
    if(!enabled) return;
    textures = new GLuint[128];
    FT_Face face;
    int error = FT_New_Face(ftLibrary, filename.c_str(), 0, &face);
    if(error)
    {
        std::cout << "Cannot load font " << filename << ", error " << error << std::endl;
        return;
    }
    
    
    
    int size=24;
    
    // *64 because freetype uses 1/64th points
    // 1 point = 1/72 inches
    FT_Set_Char_Size(face, size*64, size*64, 96, 96);
    
    listbase = glGenLists(128);
    glGenTextures(128, textures);
    
    
    //without the unsigned it'll get up to 127 then wrap to -128
    for(unsigned char ch=0; ch<128; ch++)
    {
        if(FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
        {
            std::cout << "Unable to load glyph " << ch << " of font " << filename << std::endl;
            return;
        }
        
        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph))
        {
            std::cout << "Unable to get glyph " << ch << " of font " << filename << std::endl;
            return;
        }
        FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
        FT_Bitmap& bitmap = bitmap_glyph->bitmap;
        
        //making a power of 2 texture. Darn those old graphics cards...
        int width = next_p2( bitmap.width );
        int height = next_p2( bitmap.rows );
        
        GLubyte* expanded_data = new GLubyte[ 2 * width * height];
        
        //Copies bitmap.buffer to expanded_data, but also adds
        // 0s to the edges (because we make it a power of 2 in size)
        for(int j=0; j <height;j++) {
            for(int i=0; i < width; i++){
                expanded_data[2 * (i + j * width)] = 255;
                expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
            }
        }
        
        glBindTexture( GL_TEXTURE_2D, textures[ch]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
        
        delete[] expanded_data;
        
        glNewList(listbase+ch,GL_COMPILE);
        
        glBindTexture(GL_TEXTURE_2D,textures[ch]);
        
        glPushMatrix();
        glTranslatef(bitmap_glyph->left, bitmap_glyph->top-bitmap.rows,0);
        
        float x=(float)bitmap.width / (float)width;
        float y=(float)bitmap.rows / (float)height;
        
        //use bitmap.rows to remove padding
        glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
        glTexCoord2d(0,y); glVertex2f(0,0);
        glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
        glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
        glEnd();
        glPopMatrix();
        glTranslatef(face->glyph->advance.x >> 6 ,0,0);
        
        // Increment The Raster Position As If We Were A Bitmap Font.
        // (Only Needed If You Want To Calculate Text Length)
        // glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);
        glEndList();
        FT_Done_Glyph(glyph);
    }
    
    //free the face
    FT_Done_Face(face);
}

SimpGLFont::~SimpGLFont()
{
    glDeleteLists(listbase, 128);
    glDeleteTextures(128, textures);
    delete[] textures;
}

void SimpGLFont::render(float x, float y, std::string text)
{
    pushScreenCoordinateMatrix();
    
    // creates extra space for spacing between lines
    float height = size/0.63f;
    
    //create a vector of all the lines
    std::vector<std::string> lines;
    int lastNewLine = 0;
    for (int i=0; i<text.length(); i++) {
        if (text.at(i)=='\n') {
            std::string line = text.substr(lastNewLine, i+1);
            lastNewLine = i+1;
        }
    }
    lines.push_back(text.substr(lastNewLine, text.length()));
    
    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glListBase(listbase);
    float modelview_matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
    
    for(int i=0;i<lines.size();i++) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(x,y-height*i,0);
        glMultMatrixf(modelview_matrix);
        
        // The Commented Out Raster Position Stuff Can Be Useful If You Need To
        // Know The Length Of The Text That You Are Creating.
        // If You Decide To Use It Make Sure To Also Uncomment The glBitmap Command
        // In make_dlist().
        // glRasterPos2f(0,0);
        glCallLists((int)lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
        // float rpos[4];
        // glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
        // float len=x-rpos[0]; (Assuming No Rotations Have Happend)
        
        glPopMatrix();
    }
    
    glPopAttrib();
    
    pop_projection_matrix();
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