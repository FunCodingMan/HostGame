#include <iostream>

int main()
{
    std::cout << "Type 3 numbers: ";

    int num1, num2, num3;
    if (std::cin >> num1 && std::cin >> num2 && std::cin >> num3)
    {
        std::cout << (num1 + num2 + num3) << std::endl;
        return 0;
    }
    return 1;
}