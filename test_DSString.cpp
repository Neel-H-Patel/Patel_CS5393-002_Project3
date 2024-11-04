#include <iostream>
#include <vector>
#include <algorithm>
#include "DSString.h"

int main()
{
    DSString myString = "Hello, World!";
    std::cout << myString << "\n";

    myString = "Good bye!";
    std::cout << myString << "\n";

    std::cout << "substr: " << myString.substring(5, 3) << "\n";

    DSString a = "test";
    DSString b;
    b = a;

    std::cout << std::boolalpha;
    std::cout << (a == b) << "\n";

    b[0] = 'T';
    std::cout << "a is now: " << a << "\n";
    std::cout << (a == b) << "\n";

    std::vector<DSString> strings = {
        DSString("bb"),
        DSString("aaa"),
        DSString("ddd"),
        DSString("ee"),
        DSString("ccc")};

    for (const auto &s : strings)
        std::cout << s << "\n";

    std::cout << "found ddd: " << (std::find(strings.begin(), strings.end(), DSString("ddd")) != strings.end()) << "\n";
    std::cout << "found z: " << (std::find(strings.begin(), strings.end(), DSString("z")) != strings.end()) << "\n";

    std::sort(strings.begin(), strings.end());

    for (const auto &s : strings)
        std::cout << s << "\n";

    std::cout << "found ddd: " << std::binary_search(strings.begin(), strings.end(), DSString("ddd")) << "\n";
    std::cout << "found z: " << std::binary_search(strings.begin(), strings.end(), DSString("z")) << "\n";

    return 0;
}
