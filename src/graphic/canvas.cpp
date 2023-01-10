#include "canvas.h"
#include "opengl.h"
#include "tga.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <iostream>
#include <stdexcept>
#include <fstream>

const glm::vec4 default_color(0.0, 0.0, 0.0, 1.0);

const char* vert = "#version 330 core\n \
\n \
layout(location = 0) in vec2 vposition;\n \
layout(location = 1) in vec4 vcolor;\n \
\n \
uniform mat4 projection_matrix;\n \
uniform mat4 modelview_matrix;\n \
\n \
out vec4 fcolor;\n \
\n \
void main()\n \
{\n \
  gl_Position = modelview_matrix*projection_matrix*vec4(vposition, 0.0, 1.0);\n \
\n \
  fcolor = vcolor;\n \
}\n \
\0";

const char* frag = "#version 330 core\n \
\n \
in vec4 fcolor;\n \
\n \
out vec4 color;\n \
\n \
void main()\n \
{\n \
  color = fcolor;\n \
}\n \
\0";

// TODO : Should canvas be a singleton?

namespace cadgl
{

  /* ***************************** canvas::canvas ***************************** */

  canvas::canvas(int x, int y, int width, int height)
  : m_background_color_index(0),
    m_viewport(x, y, width, height)
  {
    m_palette.emplace(0, default_color);

    // Init OpenGL
    if (!load_glext_3_3_core())
      {
        throw std::runtime_error("canvas::canvas");
        return;
      }

    // Framebuffer
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glGenTextures(1, &m_framebuffer_texture);
    glBindTexture(GL_TEXTURE_2D, m_framebuffer_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebuffer_texture, 0);

    // Shaders
    GLint status;
    GLint sz = 0;
    char* log;
    
    m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (m_vertex_shader == 0)
      {
#ifndef NDEBUG
        std::cout << "[E] Can not create shader" << std::endl;
#endif // NDEBUG
        throw std::runtime_error("canvas::canvas");
        return;
      }
    glShaderSource(m_vertex_shader, 1, &vert, nullptr);
    glCompileShader(m_vertex_shader);
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
      {
#ifndef NDEBUG
        std::cout << "[E] Something goes wrong with vertex shader" << std::endl;
        glGetShaderiv(m_vertex_shader, GL_INFO_LOG_LENGTH, &sz);
        log = new char[sz + 1];
        glGetShaderInfoLog(m_vertex_shader, sz + 1, &status, log);
        std::cout << "[I] " << log << std::endl;
        delete[] log;
#endif // NDEBUG
        throw std::runtime_error("canvas::canvas");
        return;
      }

    m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (m_fragment_shader == 0)
      {
#ifndef NDEBUG
        std::cout << "[E] Can not create shader" << std::endl;
#endif // NDEBUG
        throw std::runtime_error("canvas::canvas");
        return;
      }
    glShaderSource(m_fragment_shader, 1, &frag, nullptr);
    glCompileShader(m_fragment_shader);
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
      {
#ifndef NDEBUG
        std::cout << "[E] Something goes wrong with fragment shader" << std::endl;
        glGetShaderiv(m_fragment_shader, GL_INFO_LOG_LENGTH, &sz);
        log = new char[sz + 1];
        glGetShaderInfoLog(m_fragment_shader, sz + 1, &status, log);
        std::cout << "[I] " << log << std::endl;
        delete[] log;
#endif // NDEBUG
        throw std::runtime_error("canvas::canvas");
        return;
      }

    m_shader_program = glCreateProgram();
    if (m_shader_program == 0)
      {
#ifndef NDEBUG
        std::cout << "[E] Can not create shader" << std::endl;
#endif // NDEBUG
        throw std::runtime_error("canvas::canvas");
        return;
      }
    glAttachShader(m_shader_program, m_vertex_shader);
    glAttachShader(m_shader_program, m_fragment_shader);
    glLinkProgram(m_shader_program);
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
      {
#ifndef NDEBUG
        std::cout << "[E] Something goes wrong with shader program" << std::endl;
        glGetProgramiv(m_shader_program, GL_INFO_LOG_LENGTH, &sz);
        log = new char[sz + 1];
        glGetProgramInfoLog(m_shader_program, sz + 1, &status, log);
        std::cout << "[I] " << log << std::endl;
        delete[] log;
#endif // NDEBUG
        throw std::runtime_error("canvas::canvas");
        return;
      }

    glUseProgram(m_shader_program);

    // Set projection matrix
    // TODO : Delete this -->
    float sheet_aspect = 210.0f/297.0f;
    float canvas_aspect = static_cast<float>(m_viewport.m_size.x)/static_cast<float>(m_viewport.m_size.y);
    float k;

    glViewport(m_viewport.m_position.x, m_viewport.m_position.y, 
      m_viewport.m_size.x, m_viewport.m_size.y);
    if (canvas_aspect > sheet_aspect)
      {
        k = 297.0f*canvas_aspect - 210.0f;
        m_projection = glm::ortho(-k*0.5f, 210.0f + k*0.5f, 0.0f, 297.0f);
      }
    else
      {
        k = 210.0f/canvas_aspect - 297.0f;
        m_projection = glm::ortho(0.0f, 210.0f, -k*0.5f, 297.0f + k);
      }
    // <-- 
  }

  /* ***************************** canvas::resize ***************************** */

  void canvas::resize(int x, int y, int width, int height)
  {
    m_viewport.m_position = glm::uvec2(x, y);
    m_viewport.m_size = glm::uvec2(width, height);
    glViewport(m_viewport.m_position.x, m_viewport.m_position.y,
							 m_viewport.m_size.x, m_viewport.m_size.y);
  }

  /* ****************************** canvas::draw ****************************** */

	void canvas::draw()
	{
		glClearColor(m_palette.at(m_background_color_index).r,
			m_palette.at(m_background_color_index).g,
			m_palette.at(m_background_color_index).b,
			m_palette.at(m_background_color_index).a);
		glClear(GL_COLOR_BUFFER_BIT);

// TODO : Draw further
    // TODO : Delete this -->
    const GLfloat mesh[] =
      {
        0.0, 0.0,       1.0, 1.0, 1.0, 1.0,
        210.0, 0.0,     1.0, 1.0, 1.0, 1.0,
        210.0, 297.0,   1.0, 1.0, 1.0, 1.0,

        0.0, 0.0,       1.0, 1.0, 1.0, 1.0,
        210.0, 297.0,   1.0, 1.0, 1.0, 1.0,
        0.0, 297.0,     1.0, 1.0, 1.0, 1.0
      };

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    if (vao == 0)
      std::cout << "[W] Can not create vertex array" << std::endl;
    else
      std::cout << "[I] Vertex array is created" << std::endl;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (vbo == 0)
      std::cout << "[W] Can not create vertex buffer" << std::endl;
    else
      std::cout << "[I] Vertex buffer is created" << std::endl;

    glBufferData(GL_ARRAY_BUFFER, 36*sizeof(GLfloat), mesh, GL_STATIC_DRAW);
    
    GLint index;
    glm::mat4 model_view = glm::identity<glm::mat4>();

    index = glGetUniformLocation(m_shader_program, "projection_matrix");
    if (index < 0)
      std::cout << "[W] ``projection_matrix'' is not a valid uniform name" << std::endl;
    else
      std::cout << "[I] ``projection_matrix'' was found" << std::endl;
    glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr<GLfloat>(m_projection));
    index = glGetUniformLocation(m_shader_program, "modelview_matrix");
    if (index < 0)
      std::cout << "[W] ``modelview_matrix'' is not a valid uniform name" << std::endl;
    else
      std::cout << "[I] ``modelview_matrix'' was found" << std::endl;
    glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr<GLfloat>(model_view));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),
      reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),
      reinterpret_cast<void*>(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glUseProgram(m_shader_program);

    glm::vec4 v = model_view*m_projection*glm::vec4(mesh[6], mesh[7], 0.0, 1.0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    // <--
  }

  /* ************************** canvas::update_matrix ************************* */

  void canvas::update_matrix()
  {
	  m_projection = glm::ortho(-m_position.x, m_scale.x - m_position.x,
														  -m_position.y, m_scale.y - m_position.y);
  }

  /* ************************ canvas::save_framebuffer ************************ */

  void canvas::save_framebuffer(const std::string& file_path)
  {
    uint8_t* buffer;

    std::ofstream ofs(file_path);
    if (ofs.bad())
      {
#ifndef NDEBUG
        std::cout << "[W] Can not create file ``" << file_path << "\'\'" << std::endl;
#endif // NDEBUG
        return;
      }

    buffer = new uint8_t[m_viewport.m_size.x*m_viewport.m_size.y*3];

    glReadPixels(0, 0, m_viewport.m_size.x, m_viewport.m_size.y, GL_RGB, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(buffer));

    tga_header_t hdr =
    {
      0,
      0,
      IMAGE_TYPE_UNCOMPRESSED_TRUE_COLOR,
      {0, 0, 0},
      0,
      0,
      static_cast<uint16_t>(m_viewport.m_size.x),
      static_cast<uint16_t>(m_viewport.m_size.y),
      24,
      IMAGE_DESCRIPTOR_BOTTOM_LEFT
    };

    ofs.write(reinterpret_cast<char*>(&hdr), sizeof(tga_header_t));
    for (int j = 0; j < 600; j++)
      {
        for (int i = 0; i < 800; i++)
          {
            ofs.write(reinterpret_cast<char*>(buffer + (j*800 + i)*3), 3);
          }
      }

    ofs.close();

#ifndef NDEBUG
    std::cout << "[I] Framebuffer saved to the file ``" << file_path << "\'\'" << std::endl;
#endif // NDEBUG

    delete[] buffer;
  }
} // namespace cadgl
