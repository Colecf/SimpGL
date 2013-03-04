//
//  SimpGLTexture.h
//  OpenGL
//
//  Created by Cole Faust on 9/18/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#ifndef __OpenGL__SimpGLTexture__
#define __OpenGL__SimpGLTexture__

#include "OpenGLHeaders.h"
#include "SimpGL.h"
#include <string>
#include <vector>
#define png_infopp_NULL (png_infopp)NULL
#define png_voidp_NULL (void*)NULL

class SimpGLTexture
{
public:
    
    friend class SimpGLTextureCache;
    
    static bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);
    
    SimpGLTexture(std::string);
    ~SimpGLTexture();
    void render(int x, int y);
    
    
    int getWidth();
    int getHeight();
    std::string getFileName();
    bool getAlpha();
    GLuint getTexID();
    
    
    
private:
    
    std::string fileName;
    int width;
    int height;
    bool hasAlpha;
    GLubyte *data;
    GLuint texID;
    
    //Used by friend class SimpGLTextureCache
    //To figure out when to release the tex
    int references;
};


class SimpGLTextureCache
{
public:
    static SimpGLTextureCache* getInstance();
    SimpGLTexture* getTextureFromFileName(std::string fileName);
    void releaseTexture(SimpGLTexture* texToRelease);
    void setResourcePath(std::string newResourcePath);
    std::string getResourcePath();
    
private:
    
    std::vector<SimpGLTexture*> textures;
    static SimpGLTextureCache* instance;
    static std::string resourcePath;
    
    SimpGLTextureCache(){};  // Private so that it can  not be called
    SimpGLTextureCache(SimpGLTextureCache const&){};             // copy constructor is private
    SimpGLTextureCache& operator=(SimpGLTextureCache const&){};  // assignment operator is private
};





#endif /* defined(__OpenGL__SimpGLTexture__) */
