// scoreschool.cpp
#include <vector>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <concepts>
#include <exception>

using std::vector;

// --- 共享结构体 ---
struct Grade {
    std::string name;
    double midterm;
    double final;
    vector<double> homework;
    double total;
};
// -------------------------

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <std::ranges::forward_range R>
    requires Numeric<std::ranges::range_value_t<R>>
double median(R&& data)
{
    auto temp = std::ranges::to<std::vector<double>>(data);
    if (temp.empty())
        throw std::domain_error("median of an empty range");

    auto size = std::ssize(temp);
    auto mid = temp.begin() + size / 2;
    std::ranges::nth_element(temp, mid);

    if (size % 2 != 0) return *mid;
    auto left_max = std::ranges::max_element(temp.begin(), mid);
    return std::midpoint(*left_max, *mid);
}

double score(Grade& g)
{
    return 0.2 * g.midterm + 0.4 * g.final + 0.4 * median(g.homework);
}