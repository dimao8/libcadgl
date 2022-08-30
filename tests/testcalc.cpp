#include <cadgl.h>
#include <iostream>

int main(int argc, char** argv)
{
  std::cout << cadgl::version() << std::endl;

  if (argc <= 1)
    return 1;

  std::cout << "Resolve " << argv[1] << std::endl;

  float f;
  if (cadgl::resolve(&f, std::string(argv[1])))
    {
      std::cout << f << std::endl;
      return 0;
    }
  else
    {
      std::cout << "fail" << std::endl;
      return 2;
    }
}