#include <cadgl.h>
#include "calc/calc.h"
#include "graphic/canvas.h"

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

#include <iostream>

namespace cadgl
{

  const std::string version_string = VERSION;

  canvas* global_canvas = nullptr;

  /* ********************************* version ******************************** */

  const std::string& version()
  {
    return version_string;
  }

  /* ********************************* resolve ******************************** */

  bool resolve(float* result, const std::string& expr)
  {
    // TODO : Test resolver. Delete this -->

    calc resolver;

    return resolver.resolve(result, expr);

    // <--
  }

  /* ******************************* init_canvas ****************************** */

  bool init_canvas(int width, int height)
  {
    if (global_canvas != nullptr)
      {
  #ifndef NDEBUG        
        std::cout << "[W] Canvas is already exist" << std::endl;
  #endif // NDEBUG
        return true;
      }

    try
      {
        global_canvas = new canvas(0, 0, width, height);
      }
    catch (const std::runtime_error& err)
      {
  #ifndef NDEBUG        
        std::cout << "[E] Can not create canvas" << std::endl;
  #endif // NDEBUG
        return false;
      }
  #ifndef NDEBUG        
        std::cout << "[I] Create canvas" << std::endl;
  #endif // NDEBUG

    return true;
  }

/* ***************************** destroy_canvas ***************************** */

  void destroy_canvas()
  {
    if (global_canvas == nullptr)
      return;

    delete global_canvas;
    global_canvas = nullptr;

  #ifndef NDEBUG        
    std::cout << "[I] Destroy canvas" << std::endl;
  #endif // NDEBUG
  }

  /* ****************************** redraw_canvas ***************************** */

  void redraw_canvas()
  {
    if (global_canvas == nullptr)
      return;

    global_canvas->draw();
  }

  /* ******************************* save_canvas ****************************** */

  void save_canvas(const std::string& file_path)
  {
    if (global_canvas == nullptr)
      return;

    global_canvas->save_framebuffer(file_path);
  }

} // namespace cadgl
