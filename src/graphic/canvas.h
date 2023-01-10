/**
 * \file canvas.h
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <GL/gl.h>

#include <map>
#include <string>

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

    rect m_viewport;                                  //!< Target screen viewport
    glm::vec2 m_scale;                                //!< Size of screen in millimeters
    glm::vec2 m_position;                             //!< Position of the left bottom corner in millimeters
    glm::mat4 m_projection;                           //!< Projection matrix
    std::map<int, glm::vec4> m_palette;               //!< Color palette

    // OpenGL values
    GLuint m_framebuffer;
    GLuint m_framebuffer_texture;

    GLuint m_vertex_shader;
    GLuint m_fragment_shader;
    GLuint m_shader_program;

    // Common colors
    int m_background_color_index;                     //!< Background color

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
     * 
     * \throw runtime_error
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

    /**
     * Save current framebuffer to the TGA file.
     * 
     * \param [in] file_path -- Full path and name of the file
     */
    void save_framebuffer(const std::string& file_path);

  };
} // namespace cadgl


#endif // CANVAS_H
