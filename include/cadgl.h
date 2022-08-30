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

} // namespace cadgl

#endif // CADGL_H
