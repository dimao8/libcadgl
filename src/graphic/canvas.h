/**
 * \file canvas.h
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <glm/glm.hpp>

#include <line_style.h>
#include <rect.h>

namespace cadgl
{
  /**
   * canvas provide universal drawing surface for drawing operations.
   * It can display any lines and curves, text and can use styles.
   * It also provide scaling and panning.
   * 
   * Canvas has unlimited surface.
   */
  class canvas
  {

  private:

    rect_t m_viewport;                                //!< Target screen viewport

  public:

    canvas(int x, int y, int width, int height);

  };
} // namespace cadgl


#endif // CANVAS_H
