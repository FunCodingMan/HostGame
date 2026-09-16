#include <iostream>
#include <vector>

int main()
{
    int count = 0;
    int sum = 0;
    int num = 0;

    while (std::cin >> num && num != 0)
    {
        sum += num;
        count++;
    }

    if (count == 0)
    {
        std::cerr << "Error: first number is zero!" << std::endl;
        return 1;
    }

    std::cout << "Count of numbers: " << count << std::endl;
    std::cout << "Sum of numbers: " << sum << std::endl;

    std::cout << "Histogram: ";
    for (int i = 0; i < count; ++i)
    {
        std::cout << '*';
    }

    std::cout << std::endl;

    return 0;
}