#include <cstddef>
#include <iostream>
#include <optional>
#include <ranges>

// Початковий код:
//
// template <class R>
// double average(const R& r)
//
// Аналіз проблеми:
// Передавання представлення за const R& робить його
// константним усередині функції.
// Деякі представлення, зокрема filter_view,
// не підтримують виклик begin() для константного об'єкта.
//
// Тому початкова реалізація може не компілюватися
// під час роботи з такими діапазонами.
//
// Виправлення:
// Використовуємо forwarding reference R&& та концепцію
// std::ranges::input_range.
// Це дозволяє приймати різні типи діапазонів
// без обов'язкового копіювання.
//
// std::optional<double> використовується для представлення
// ситуації, коли діапазон порожній.


template <std::ranges::input_range R>
std::optional<double> average(R&& range) {
    double sum = 0.0;
    std::size_t count = 0;

    for (const auto value : range) {
        sum += static_cast<double>(value);
        ++count;
    }

    if (count == 0) {
        return std::nullopt;
    }

    return sum / static_cast<double>(count);
}

int main() {

    auto range = std::views::iota(1, 31)
        | std::views::filter([](int x) {
        return x % 3 == 0;
            })
        | std::views::transform([](int x) {
        return x * x;
            });

    const auto result = average(range);

    if (result) {
        std::cout<< *result << std::endl;
    }
    else {
        std::cout << "Vector is empty."
            "average does not exist.\n";
    }

    return 0;
}