#include "canvas.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

const glm::vec4 default_color(0.0, 0.0, 0.0, 1.0);

namespace cadgl
{

  /*******************  canvas::canvas  *******************/

  canvas::canvas(int x, int y, int width, int height)
  : m_background_color_index(0)
  {
    m_palette.emplace(0, default_color);

    set_scale(glm::vec2());
    resize(x, y, width, height);
  }

  /*******************  canvas::resize  *******************/

  void canvas::resize(int x, int y, int width, int height)
  {
    m_viewport.m_position = glm::uvec2(x, y);
    m_viewport.m_size = glm::uvec2(width, height);
    glViewport(m_viewport.m_position.x, m_viewport.m_position.y,
      m_viewport.m_size.x, m_viewport.m_size.y);
  }

  /********************  canvas::draw  ********************/

  void canvas::draw()
  {
    glClearColor(m_palette.at(m_background_color_index).r,
      m_palette.at(m_background_color_index).g,
      m_palette.at(m_background_color_index).b,
      m_palette.at(m_background_color_index).a);

    // TODO : Draw further
  }

  /******************  canvas::set_scale  *****************/

  void canvas::set_scale(const glm::vec2& scale)
  {
    m_scale = scale;
    update_matrix();
  }

  /****************  canvas::update_matrix  ***************/

  void canvas::update_matrix()
  {
    m_projection = glm::ortho(-m_position.x, m_scale.x - m_position.x,
      -m_position.y, m_scale.y - m_position.y);
  }
} // namespace cadgl
