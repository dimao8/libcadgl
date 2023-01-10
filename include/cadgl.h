/**
 * @file cadgl.h
 */

#ifndef CADGL_H
#define CADGL_H

#include <string>

namespace cadgl
{

  // Internal global variables
  
  // Interface functions

  const std::string& version();

  bool resolve(float* result, const std::string& expr);

  /**
   * Create global canvas 
   * 
   * \param [in] width  -- Width of the canvas area in pixels
   * \param [in] height -- Height of the canvas area in pixels
   * @return Return true if canvas is existing after creation. Otherwise
   *  return false.
   */
  bool init_canvas(int width, int height);

  /**
   * Destroy existing global canvas. If canvas is not exist, do nothing.
   * 
   * \note destroy_canvas() must be used on application termination to prevent
   * memory leaks.
   */
  void destroy_canvas();

  /**
   * Redraw context area.
   */
  void redraw_canvas();

  /**
   * Save canvas to the TGA image file.
   * 
   * \param [in] file_path -- Path and name of the TGA file.
   */
  void save_canvas(const std::string& file_path);

} // namespace cadgl

#endif // CADGL_H
