#include <cadgl.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <cstddef>

#pragma pack(push, 1)
typedef struct color_map_specification_tag
{
  uint16_t first_entry_index;
  uint16_t color_map_length;
  uint8_t color_map_entry_size;
} color_map_specification_t;

typedef struct tga_header_tag
{
  uint8_t id_length;
  uint8_t color_map_type;
  uint8_t image_type;
  color_map_specification_t color_map_specification;
  uint16_t x_origin;
  uint16_t y_origin;
  uint16_t image_width;
  uint16_t image_height;
  uint8_t pixel_depth;
  uint8_t image_descriptor;
} tga_header_t;
#pragma pack(pop)



#define IMAGE_TYPE_UNUSED                   (0x00)
#define IMAGE_TYPE_UNCOMPRESSED_COLORMAP    (0x01)
#define IMAGE_TYPE_UNCOMPRESSED_TRUE_COLOR  (0x02)
#define IMAGE_TYPE_UNCOMPRESSED_GRAYSCALE   (0x03)
#define IMAGE_TYPE_RLE_COLORMAP             (0x0A)
#define IMAGE_TYPE_RLE_TRUE_COLOR           (0x0B)
#define IMAGE_TYPE_RLE_GRAYSCALE            (0x0C)

#define IMAGE_DESCRIPTOR_VERTICAL_BIT       (0x20)
#define IMAGE_DESCRIPTOR_HORIZONTAL_BIT     (0x10)
#define IMAGE_DESCRIPTOR_BOTTOM_LEFT        (0x00)
#define IMAGE_DESCRIPTOR_BOTTOM_RIGHT       (IMAGE_DESCRIPTOR_HORIZONTAL_BIT)
#define IMAGE_DESCRIPTOR_TOP_LEFT           (IMAGE_DESCRIPTOR_VERTICAL_BIT)
#define IMAGE_DESCRIPTOR_TOP_RIGHT          (IMAGE_DESCRIPTOR_VERTICAL_BIT | IMAGE_DESCRIPTOR_HORIZONTAL_BIT)

const GLfloat mesh[36] =
{
//  pos           color
  -105.0, -148.5, 1.0, 1.0, 1.0, 1.0,
   105.0, -148.5, 1.0, 1.0, 1.0, 1.0,
   105.0,  148.5, 1.0, 1.0, 1.0, 1.0,

  -105.0, -148.5, 1.0, 1.0, 1.0, 1.0,
   105.0,  148.5, 1.0, 1.0, 1.0, 1.0,
  -105.0,  148.5, 1.0, 1.0, 1.0, 1.0
};

glm::mat4 projection;

const char* vert = "#version 330 core \n \
\n \
layout(location = 0) in vec2 vposition; \n \
layout(location = 1) in vec4 vcolor; \n \
\n \
uniform mat4 projection_matrix; \n \
\n \
\n out vec4 fcolor; \
\n \
\n void main() \
\n { \
\n  gl_Position = projection_matrix*vec4(vposition, 1.0, 1.0); \
\n \
\n  fcolor = vcolor; \
\n } \
\n\0";

const char* frag = "#version 330 core \n \
\n \
\n in vec4 fcolor; \
\n \
\n out vec4 color; \
\n \
\n void main() \
\n { \
\n  color = fcolor; \
\n } \
\n\0";

GLFWwindow* window;

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

int main(int argc, char** argv)
{
  projection = glm::ortho(-198.0, 198.0, -148.5, 148.5);

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);

  window = glfwCreateWindow(800, 600, "Test canvas", nullptr, nullptr);
  if (window == nullptr)
    return -1;

  glfwMakeContextCurrent(window);

  // Load glext
  glGenFramebuffers = reinterpret_cast<PFNGLGENFRAMEBUFFERSPROC>(glfwGetProcAddress("glGenFramebuffers"));
  glBindFramebuffer = reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>(glfwGetProcAddress("glBindFramebuffer"));
  glFramebufferTexture2D = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DPROC>(glfwGetProcAddress("glFramebufferTexture2D"));

  glGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(glfwGetProcAddress("glGenVertexArrays"));
  glBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(glfwGetProcAddress("glBindVertexArray"));
  glDeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(glfwGetProcAddress("glDeleteVertexArrays"));

  glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(glfwGetProcAddress("glGenBuffers"));
  glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(glfwGetProcAddress("glBindBuffer"));
  glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(glfwGetProcAddress("glBufferData"));
  glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(glfwGetProcAddress("glDeleteBuffers"));

  glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(glfwGetProcAddress("glCreateShader"));
  glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(glfwGetProcAddress("glShaderSource"));
  glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(glfwGetProcAddress("glGetShaderiv"));
  glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(glfwGetProcAddress("glGetShaderInfoLog"));
  glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(glfwGetProcAddress("glDeleteShader"));
  glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(glfwGetProcAddress("glCompileShader"));

  glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(glfwGetProcAddress("glCreateProgram"));
  glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(glfwGetProcAddress("glAttachShader"));
  glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(glfwGetProcAddress("glLinkProgram"));
  glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(glfwGetProcAddress("glUseProgram"));
  glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(glfwGetProcAddress("glDeleteProgram"));
  glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(glfwGetProcAddress("glGetProgramiv"));
  glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(glfwGetProcAddress("glGetProgramInfoLog"));

  glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(glfwGetProcAddress("glGetUniformLocation"));
  glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(glfwGetProcAddress("glUniformMatrix4fv"));
  glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(glfwGetProcAddress("glVertexAttribPointer"));

  glEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(glfwGetProcAddress("glEnableVertexAttribArray"));

  // Create framebuffer
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  GLuint framebuffer_texture;
  glGenTextures(1, &framebuffer_texture);
  glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mesh), mesh, GL_STATIC_DRAW);

  GLuint vshader, fshader, prog;
  GLint sz = 0;
  GLint status;
  char* log;

  vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshader, 1, &vert, nullptr);
  glCompileShader(vshader);
  glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE)
    {
      std::cout << "Something goes wrong with vertex shader" << std::endl;
      std::cout << "Shader source was:\n" << vert << std::endl;
      glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &sz);
      log = new char[sz + 1];
      glGetShaderInfoLog(vshader, sz + 1, &status, log);
      std::cout << log << std::endl;
    }
  
  sz = 0;

  fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader, 1, &frag, nullptr);
  glCompileShader(fshader);
  glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE)
    {
      std::cout << "Something goes wrong with fragment shader" << std::endl;
      std::cout << "Shader source was:\n" << frag << std::endl;
      glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &sz);
      log = new char[sz + 1];
      glGetShaderInfoLog(fshader, sz + 1, &status, log);
      std::cout << log << std::endl;
    }

  prog = glCreateProgram();
  glAttachShader(prog, vshader);
  glAttachShader(prog, fshader);
  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (status != GL_TRUE)
    {
      std::cout << "Something goes wrong with shader program" << std::endl;
      glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &sz);
      log = new char[sz + 1];
      glGetProgramInfoLog(prog, sz + 1, &status, log);
      std::cout << log << std::endl;
    }

  glUseProgram(prog);

  GLint loc;
  loc = glGetUniformLocation(prog, "projection_matrix");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), reinterpret_cast<void*>(2*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glFlush();

  GLubyte buffer[800*600*3] = {0};

  // Get pixels
  glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(buffer));
  std::ofstream ofs("framebuffer.tga");
  tga_header_t hdr =
  {
    .id_length = 0,
    .color_map_type = 0,
    .image_type = IMAGE_TYPE_UNCOMPRESSED_TRUE_COLOR,
    .color_map_specification =
    {
      .first_entry_index = 0,
      .color_map_length = 0,
      .color_map_entry_size = 0
    },
    .x_origin = 0,
    .y_origin = 0,
    .image_width = 800,
    .image_height = 600,
    .pixel_depth = 24,
    .image_descriptor = IMAGE_DESCRIPTOR_BOTTOM_LEFT
  };

  ofs.write(reinterpret_cast<char*>(&hdr), sizeof(tga_header_t));
  for (int j = 0; j < 600; j++)
    {
      for (int i = 0; i < 800; i++)
        {
          ofs.write(reinterpret_cast<char*>(buffer + (j*800 + i)*3), 3);
        }
    }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
