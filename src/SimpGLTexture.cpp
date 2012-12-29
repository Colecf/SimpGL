//
//  SimpGLTexture.cpp
//  OpenGL
//
//  Created by Cole Faust on 9/18/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#include "SimpGLTexture.h"
#include "libpng/png.h"
#include "libpng/pnginfo.h"
#include <iostream>

bool SimpGLTexture::loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
    
    if ((fp = fopen(name, "rb")) == NULL)
    {
        std::cout << "Cannot open texture, image is probably in the wrong spot." << std::endl;
        return false;
    }
    
    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
    
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }
    
    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        return false;
    }
    
    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }
    
    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);
    
    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);
    
    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, png_voidp_NULL);
    
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;
    
    switch (color_type) {
        case PNG_COLOR_TYPE_RGBA:
            outHasAlpha = true;
            break;
        case PNG_COLOR_TYPE_RGB:
            outHasAlpha = false;
            break;
        default:
            std::cout << "Color type " << color_type << " not supported" << std::endl;
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
    }
    //changed from onsigned int by cole
    unsigned long row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
    
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    
    for (int i = 0; i < outHeight; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
    
    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    
    /* Close the file */
    fclose(fp);
    
    /* That's it */
    return true;
}

SimpGLTexture::SimpGLTexture(std::string texName)
{
    fileName = texName;
    texID = 0;
    references = 1;
    if(!loadPngImage((char*)texName.c_str(), width, height, hasAlpha, &data))
    {
        std::cout << "Unable to load texture " << texName << std::endl;
    }
    
    glGenTextures( 1, &texID );
    glBindTexture( GL_TEXTURE_2D, texID );
    
    //TODO: Change the 4 and 3 to proper opengl constants
    //      They're probably GL_RGBA and GL_RGB
//    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width,
//                 height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
//                 data);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, width,
                 height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                 data);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, NULL );
    
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cout << "Error loading texture " << texName << "! Error: " << gluErrorString( error ) << std::endl;
        return;
    }

}

void SimpGLTexture::render(int x, int y)
{
    if (texID == 0) {
        return;
    }
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glBegin( GL_QUADS );
        glTexCoord2f( 0.f, 0.f ); glVertex2f( 0.f,   0.f );
        glTexCoord2f( 1.f, 0.f ); glVertex2f( width, 0.f );
        glTexCoord2f( 1.f, 1.f ); glVertex2f( width, height );
        glTexCoord2f( 0.f, 1.f ); glVertex2f( 0.f,   height );
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

int SimpGLTexture::getWidth() { return width; }
int SimpGLTexture::getHeight() { return height; }
std::string SimpGLTexture::getFileName() { return fileName; }
bool SimpGLTexture::getAlpha() { return hasAlpha; }
GLuint SimpGLTexture::getTexID() { return texID; }

//---------------------------------------------------------------
//---------------------------------------------------------------
//-------------------------SimpGLTextureCache--------------------
//---------------------------------------------------------------
//---------------------------------------------------------------

SimpGLTextureCache* SimpGLTextureCache::instance = NULL;


SimpGLTextureCache* SimpGLTextureCache::getInstance()
{
    if (!instance) {
        instance = new SimpGLTextureCache();
    }
    
    return instance;
}

SimpGLTexture* SimpGLTextureCache::getTextureFromFileName(std::string fileName)
{
    SimpGLTexture* texture;
    for (int i=0; i<textures.size(); i++) {
        texture = textures.at(i);
        if (texture->getFileName().compare(fileName) == 0) {
            texture->references++;
            return texture;
        }
    }
    
    texture = new SimpGLTexture(fileName);
    textures.push_back(texture);
    return texture;
}

void SimpGLTextureCache::releaseTexture(SimpGLTexture* texToRelease)
{
    SimpGLTexture* texture;
    int texToErase = -1;
    for (int i=0; i<textures.size(); i++) {
        texture = textures.at(i);
        if (texture == texToRelease) {
            
            texture->references--;
            if (texture->references == 0) {
                texToErase = i;
                break;
            }
        }
    }
    
    if (texToErase == -1) {
        return;
    }
    std::cout << "Deleting texture: " << texToRelease->getFileName() << std::endl;
    textures.erase(textures.begin()+texToErase);
    delete texture;
}

