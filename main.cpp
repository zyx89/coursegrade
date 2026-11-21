// main.cpp
#include <vector>
#include <string>
#include <print>
#include <exception>

using std::vector;
using std::string;
using std::print;
using std::exception;

// --- 共享结构体与声明 ---
struct Grade {
    string name;
    double midterm;
    double final;
    vector<double> homework;
    double total;
};
using CourseGrade = vector<Grade>;

extern void read(CourseGrade& cg);
extern void computeSort(CourseGrade& cg);
extern void print(CourseGrade& cg);
// -------------------------

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