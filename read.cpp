// read.cpp
#include <vector>
#include <string>
#include <print>
#include <istream>
#include <fstream>
#include <sstream>
#include <exception>

using std::vector;
using std::string;
using std::print;
using std::istream;
using std::ifstream;
using std::istringstream;
using std::runtime_error;
using std::invalid_argument;

// --- 共享结构体与声明 ---
struct Grade {
    string name;
    double midterm;
    double final;
    vector<double> homework;
    double total;
};
using CourseGrade = vector<Grade>;

extern istream& operator>>(istream& is, Grade& g);
// -------------------------

istream& operator>>(istream& is, Grade& g)
{
    print("  Reading a student grade... ");
    if (is) is >> g.name >> g.midterm >> g.final;
    if(!is.eof() && is.fail())
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

void read(CourseGrade& cg)
{
    print("Reading all student grades (name midterm final homework) from a file... \n");
    ifstream ifs("../../coursegrade.dat");
    if(!ifs) throw runtime_error("fail to open file");

    string line;
    istringstream iss;
    while (std::getline(ifs, line)) {
        iss.clear();
        iss.str(line);
        Grade g;
        if (iss >> g) {
            cg.push_back(g);
        }
    }
    if(ifs.eof()) print("Reading done!\n\n");
}