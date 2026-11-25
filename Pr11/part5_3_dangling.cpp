#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

std::vector<int> make() {
    return { 3, 1, 4 };
}

int main() {

   // Початковий код не компілюється:
   // auto it = std::ranges::max_element(make());
   // std::cout << *it << std::endl;
   //
   // Причина: make() повертає тимчасовий вектор.
   // Оскільки звичайний std::vector не є borrowed_range,
   // алгоритм std::ranges::max_element повертає
   // спеціальний тип std::ranges::dangling замість ітератора.
   //
   // Його неможливо розіменувати за допомогою оператора *.
   // Це захищає від використання ітератора після знищення
   // тимчасового контейнера.

   // Виправлення:
   // Зберігаємо результат make() в окремій змінній.
   // Тепер вектор існує протягом усього часу роботи з ним.


    const auto values = make();

    const auto it = std::ranges::max_element(values);

    if (it != values.end()) {
        std::cout << *it << std::endl;
    }
    else {
        std::cout << "Vector is empty.\n";
    }

    return 0;
}