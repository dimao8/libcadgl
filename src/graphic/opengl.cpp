#include "opengl.h"

#include <GLFW/glfw3.h>

#include <iostream>

#ifndef NDEBUG
#define GET_PROC_ADDRESS(x,y) y = reinterpret_cast<x>(glfwGetProcAddress(#y)); \
if (y == nullptr) \
  { \
    std::cout << "[E] Can not load ``" << #y << "\'\'" << std::endl; \
    return false; \
  }
#else
#define GET_PROC_ADDRESS(x,y) y = reinterpret_cast<x>(glfwGetProcAddress(#y)); \
if (y == nullptr) \
  return false;
#endif // NDEBUG

PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;

PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;

PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;

PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;

bool load_glext_3_3_core()
{
  GET_PROC_ADDRESS(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
  GET_PROC_ADDRESS(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
  GET_PROC_ADDRESS(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);

  GET_PROC_ADDRESS(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
  GET_PROC_ADDRESS(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
  GET_PROC_ADDRESS(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);

  GET_PROC_ADDRESS(PFNGLGENBUFFERSPROC, glGenBuffers);
  GET_PROC_ADDRESS(PFNGLBINDBUFFERPROC, glBindBuffer);
  GET_PROC_ADDRESS(PFNGLBUFFERDATAPROC, glBufferData);
  GET_PROC_ADDRESS(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);

  GET_PROC_ADDRESS(PFNGLCREATESHADERPROC, glCreateShader);
  GET_PROC_ADDRESS(PFNGLSHADERSOURCEPROC, glShaderSource);
  GET_PROC_ADDRESS(PFNGLGETSHADERIVPROC, glGetShaderiv);
  GET_PROC_ADDRESS(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
  GET_PROC_ADDRESS(PFNGLDELETESHADERPROC, glDeleteShader);
  GET_PROC_ADDRESS(PFNGLCOMPILESHADERPROC, glCompileShader);

  GET_PROC_ADDRESS(PFNGLCREATEPROGRAMPROC, glCreateProgram);
  GET_PROC_ADDRESS(PFNGLATTACHSHADERPROC, glAttachShader);
  GET_PROC_ADDRESS(PFNGLLINKPROGRAMPROC, glLinkProgram);
  GET_PROC_ADDRESS(PFNGLUSEPROGRAMPROC, glUseProgram);
  GET_PROC_ADDRESS(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
  GET_PROC_ADDRESS(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
  GET_PROC_ADDRESS(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);

  GET_PROC_ADDRESS(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
  GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
  GET_PROC_ADDRESS(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
  GET_PROC_ADDRESS(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);

  return true;
}