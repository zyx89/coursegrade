// process.cpp
#include <vector>
#include <string>
#include <print>
#include <algorithm>
#include <ranges>

using std::vector;
using std::string;
using std::print;
using std::ranges::stable_sort;

// --- 共享结构体与声明 ---
struct Grade {
    string name;
    double midterm;
    double final;
    vector<double> homework;
    double total;
};
using CourseGrade = vector<Grade>;

extern double score(Grade& g);
// -------------------------

void computeSort(CourseGrade& cg)
{
    print("Processing all student grades... \n");
    for(auto& g: cg){
        g.total = score(g);
    }
    stable_sort(cg, [](auto& g1, auto& g2){
        return g1.total < g2.total;
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