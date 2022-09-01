/**
 * \file line_style.h
 */

#ifndef LINE_STYLE_H
#define LINE_STYLE_H

#include <glm/vec4.hpp>
#include <line_stipple.h>

#define DEFAULT_LINE_WIDTH 0.4

namespace cadgl
{
  class line_style
  {

  protected:

    float m_width;
    glm::vec4 m_color;
    line::stipple_t m_stipple;

  public:

    /**
     * \brief Create new line style
     * 
     * \param [in] width    -- Width of the line in millimeters
     * \param [in] color    -- Color of the line
     * \param [in] stipple  -- Stipple pattern. See cadgl::line::stipple
     */
    line_style(
      float width = DEFAULT_LINE_WIDTH,
      const glm::vec4& color = glm::vec4(1.0, 1.0, 1.0, 1.0),
      line::stipple_t stipple = line::solid)
    : m_width(width),
      m_color(color),
      m_stipple(stipple)
    {}

  };
}

#endif // LINE_STYLE_H
