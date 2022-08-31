/**
 * \file line_stipple.h
 */

#ifndef LINE_STIPPLE_H
#define LINE_STIPPLE_H

namespace cadgl
{
  namespace line
  {
    /**
     * Stipple style of the line. Can be one of solid, dash, dot, dash_dot, wavy and broken.
     */
    typedef enum
    {
      solid,              //!< Solid line
      dash,               //!< Line of wide dashes
      dot,                //!< Line of dots (short dashes)
      dash_dot,           //!< Line of dots and dashes
      wavy,               //!< Sine shape line
      broken              //!< Line with pulse-like breaks
    } stipple_t;
  }
}

#endif // LINE_STIPPLE_H
