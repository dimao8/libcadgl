/**
 * \file canvas.h
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <map>

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
    glm::vec2 m_scale;                                //!< Size of screen in millimeters
    glm::vec2 m_position;                             //!< Position of the left bottom corner in millimeters
    glm::mat4 m_projection;                           //!< Projection matrix
    std::map<int, glm::vec4> m_palette;               //!< Color palette

    // Common colors
    int m_background_color_index;                     //!< Background color

    /**
     * Set scaling factor for x and y.
     * 
     * \param [in] scale  -- Scale factor
     */
    void set_scale(const glm::vec2& scale);

    /**
     * Update projection matrix when scale or position is changed.
     * 
     */
    void update_matrix();

  public:

    /**
     * Create new canvas and occupy window area with (x, y) (width, height).
     * 
     * \param [in] x      -- X position of the canvas
     * \param [in] y      -- Y position of the canvas
     * \param [in] width  -- Width of the canvas
     * \param [in] height -- Height of the canvas
     */
    canvas(int x, int y, int width, int height);

    /**
     * Callback for the window resize.
     * 
     * \param [in] x      -- X position of the canvas
     * \param [in] y      -- Y position of the canvas
     * \param [in] width  -- Width of the canvas
     * \param [in] height -- Height of the canvas
     */
    void resize(int x, int y, int width, int height);

    /**
     * Draw current canvas content on the window.
     */
    void draw();

  };
} // namespace cadgl


#endif // CANVAS_H
