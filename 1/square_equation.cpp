#include <cmath>
#include <iostream>

int main()
{
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    std::cout << "Enter coefficients a, b and c for 'ax^2 + bx + c = 0': ";
    std::cin >> a >> b >> c;

    if (a == 0.0f)
    {
        std::cout << "Error: coefficient 'a' cannot be zero." << std::endl;
        return 1;
    }

    float D = b * b - 4 * a * c;

    if (D > 0.0f)
    {
        float x1 = (-b + std::sqrt(D)) / (2 * a);
        float x2 = (-b - std::sqrt(D)) / (2 * a);
        std::cout << "x1 = " << x1 << std::endl;
        std::cout << "x2 = " << x2 << std::endl;
    }
    else if (D == 0)
    {
        float x = -b / 2;
        std::cout << "x = " << x << std::endl;
    }
    else
    {
        std::cout << "There are no real roots." << std::endl;
    }
    return 0;
}