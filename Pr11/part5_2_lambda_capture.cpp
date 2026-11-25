#include <iostream>
#include <windows.h>

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int k = 2;

    auto byValue = [k](int x) {
        return x * k;
        };

    auto byRef = [&k](int x) {
        return x * k;
        };

    k = 10;

    std::cout << byValue(3) << ' '
        << byRef(3) << '\n';
    // Аналіз результату:
    // byValue використовує збережену копію k = 2:
    // 3 * 2 = 6.
    //
    // byRef використовує актуальне значення k = 10:
    // 3 * 10 = 30.
    return 0;
}