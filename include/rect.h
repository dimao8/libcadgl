#ifndef RECT_H
#define RECT_H

#include <glm/vec2.hpp>

namespace cadgl
{
  struct rect
  {
    glm::uvec2 m_position;
    glm::uvec2 m_size;
    rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    : m_position(x, y),
      m_size(width, height)
    {

    }
    rect(const glm::uvec2& position, glm::uvec2& size)
    : m_position(position),
      m_size(size)
    {
      
    }
  };
} // namespace cadgl


#endif // RECT_H
