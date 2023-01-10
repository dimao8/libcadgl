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
glm::mat4 modelview;

GLFWwindow* window;

/* ********************************** main ********************************** */

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

  cadgl::init_canvas(800, 600);
  cadgl::redraw_canvas();
  cadgl::save_canvas("framebuffer.tga");

  glfwDestroyWindow(window);
  glfwTerminate();

  cadgl::destroy_canvas();

  return 0;
}
