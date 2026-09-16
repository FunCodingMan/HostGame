#include <iostream>
#include <vector>

int main()
{
    std::cout << "Enter count of numbers: ";
    int n = 0;

    if (!(std::cin >> n))
    {
        std::cerr << "Error: you must type count of numbers!" << std::endl;
        return 1;
    }
    if (n <= 0)
    {
        std::cerr << "Error: N must be greater than 0!" << std::endl;
        return 1;
    }

    std::vector<int> numbers;

    for (int i = 0; i < n; ++i)
    {
        int num = 0;
        std::cin >> num;
        numbers.push_back(num);
    }

    int sum = 0;
    int max = numbers[0];
    int min = numbers[0];

    for (int num : numbers)
    {
        sum += num;
        max = num > max ? num : max;
        min = num < min ? num : min;
    }

    float avrg = (float)sum / n;

    std::cout << "Sum of numbers: " << sum << std::endl;
    std::cout << "Arithmetic mean: " << avrg << std::endl;
    std::cout << "Min number: " << min << std::endl;
    std::cout << "Max number: " << max << std::endl;

    return 0;
}