//
//  OpenGLHeaders.h
//  OpenGL
//
//  Created by Cole Faust on 9/12/12.
//  Copyright (c) 2012 Cole Faust. All rights reserved.
//

#ifndef OpenGL_OpenGLHeaders_h
#define OpenGL_OpenGLHeaders_h
#ifdef __APPLE__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#elif _WIN32

#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#else

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#endif
#endif
