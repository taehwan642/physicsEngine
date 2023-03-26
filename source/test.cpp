#include <iostream>
#include <Vector.h>
int main()
{
    Math::Vector v = Math::Vector();
    v.x = 10;
    std::cout << "Hello World " << v.GetX();
    return 0;
}
