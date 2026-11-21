// A program that calculates the scores of all students in a course
// Created: Wei Gong  2026-11-16 16:43:44

// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 FreeTech <open-src@qq.com>

// CourseGrade calculation using a top-down, stepwise refinement approach

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <print>
using std::print;

#include <istream>
using std::istream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;

#include <algorithm>
using std::ranges::stable_sort;

#include <ranges>
#include <numeric>
#include <exception>
using std::exception;
using std::runtime_error;
using std::domain_error;
using std::invalid_argument;

// 顶层分解，添加错误处理
struct Grade //类型定义（需在 vector 实例化前定义完整类型）
{
    string name;
    double midterm;
    double final;
    vector<double> homework;
    double total; //总成绩，该数据成员可声明，也可不用声明
};

using CourseGrade = vector<Grade>; //类型别名

extern void read(CourseGrade& cg);
extern void computeSort(CourseGrade& cg);
extern void print(CourseGrade& cg);

int main()
{
    CourseGrade courseGrade;
    try {
        read(courseGrade);
        computeSort(courseGrade);
        print(courseGrade);
    } catch (const exception& e) {
        print("error: {}\n", e.what());
    } catch (...) {
        print("Other error!\n");
    }

    return 0;
}

// 第二层分解，添加错误检测
extern istream& operator>>(istream& is, Grade& g);
void read(CourseGrade& cg)
{
    print("Reading all student grades (name midterm final homework) from a file... \n");
    ifstream ifs("../../coursegrade.dat");
    if(!ifs) throw runtime_error("fail to open file");
    // read and store all the grades
    string line;
    istringstream iss;
    while (std::getline(ifs, line)) {
        iss.clear();                           // 清除上一轮残留的状态位
        iss.str(line);                         // 字符串输入流iss封装字符串line进行流式输入
        Grade g;                               // 假定能够对g进行iss >> g读取操作
        if (iss >> g) {
            cg.push_back(g);                   // 读取成绩成功后，添加g到cg中
        }
    }
    if(ifs.eof()) print("Reading done!\n\n");  // 读取完毕则打印
}

extern double score(Grade& g);
void computeSort(CourseGrade& cg)
{
    print("Processing all student grades... \n");
    for(auto& g: cg){
        g.total = score(g);  //假定能够进行score(g)操作
    }
    stable_sort(cg, [](auto& g1, auto& g2){
        return g1.total < g2.total;            // 使用 < 满足严格弱序
    });
    print("Processing done!\n\n");
}


void print(CourseGrade& cg)
{
    print("Course: {}\nCredit: {}\n", "software construction and implementation", 5);
    print("name      total\n");
    for(auto &g: cg){
        print("{:<10}{:<.1f}\n", g.name, g.total);
    }
}

// 第三层分解，添加错误检测
istream& operator>>(istream& is, Grade& g)
{
    print("  Reading a student grade... ");
    if (is) is >> g.name >> g.midterm >> g.final;
    if(!is.eof()&&is.fail())
        throw invalid_argument("Incorrect name, midterm and final grade.");

    if (is) {
        for (double h; is >> h;)
            g.homework.push_back(h);

        if(!is.eof() && is.fail())
            throw invalid_argument("Incorrect homework grade.");

        if(is.eof() && g.homework.size() != 0) {
            is.clear();
            print("Done!  {}\n", g.name);
        }
    }
    if (!is) print("Error!\n");
    return is;
}

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<std::ranges::forward_range R>
    requires Numeric<std::ranges::range_value_t<R>>
double median(R&& data);
double score(Grade& g)
{
    return 0.2 * g.midterm + 0.4 * g.final + 0.4 * median(g.homework);
}

// 第四层分解

template <std::ranges::forward_range R>
    requires Numeric<std::ranges::range_value_t<R>>
double median(R&& data)
{
    auto temp = std::ranges::to<std::vector<double>>(data);// 创建一个临时容器temp

    if (temp.empty())
        throw std::domain_error("median of an empty range");

    auto size = std::ssize(temp);
    // 计算无序范围中值
    auto mid = temp.begin() + size / 2;
    std::ranges::nth_element(temp, mid); // nth_element使用了Quickselect算法
    // 来计算无序范围中值，Avg. TC = O(N)

    if (size % 2 != 0) {
        return *mid;
    }
    // 提取左中值：前半部分的最大值
    auto left_max = std::ranges::max_element(temp.begin(), mid);
    // 使用 std::midpoint 防止极端值相加溢出
    return std::midpoint(*left_max, *mid);
}