#include <cadgl.h>
#include "calc/calc.h"

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

namespace cadgl
{

  const std::string version_string = VERSION;

/*****************  version  *****************/

  const std::string& version()
  {
    return version_string;
  }

  /*****************  resolve  *****************/

  bool resolve(float* result, const std::string& expr)
  {
    // TODO : Test resolver. Delete this -->

    calc resolver;

    return resolver.resolve(result, expr);

    // <--
  }

} // namespace cadgl
