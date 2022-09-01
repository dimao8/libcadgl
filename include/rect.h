#ifndef RECT_H
#define RECT_H

#include <glm/vec2.hpp>

namespace cadgl
{
  typedef struct rect_tag
  {
    glm::uvec2 m_position;
    glm::uvec2 m_size;
  } rect_t;
} // namespace cadgl


#endif // RECT_H
