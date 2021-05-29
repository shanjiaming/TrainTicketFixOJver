//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_LOGGER_H
#define CODE_LOGGER_H

#include <iostream>

//#define OpenLog

#ifdef OpenLog

#define AnsCheck
static constexpr int litnum = 1;
static constexpr int bignum = 5;
#define FileI { std::freopen((std::string("../data/basic_") + std::to_string(bignum) + '/' + std::to_string(litnum) + ".in").c_str(), "r", stdin);};
std::fstream fans(
        (std::string("../data/basic_") + std::to_string(bignum) + '/' + std::to_string(litnum) + ".out").c_str());
#define TimeTracing
/*
 * AnsCheck为文件对标机开关，需要把data文件夹放入目录使用
 * FileI为读入文件路径，如果想控制台输入只需要把这行注掉
 * fans为答案文件路径，与AnsCheck联合使用（当AnsCheck被关闭时自动失效）
 */

//#define TracerTracing
#define BeTraced trainID
#define TraceWanting "LeavesofGrass"


auto RED = "\033[0;31;1m";
auto GREEN = "\033[0;32;1m";
auto YELLOW = "\033[0;33;1m";
auto BLUE = "\033[0;34;1m";
auto END = "\033[0m";
auto CUT = "---------------------------------------------\n";


std::ofstream main_log("log.dat", std::ios::app);

//-----------------------------------------------TIME TRACING

#ifdef TimeTracing

#include <ctime>
#include "map.hpp"

inline auto &time_recorder() {
    static sjtu::map<const char *, int> time_recorder;
    return time_recorder;
}

inline auto &function_called_num() {
    static sjtu::map<const char *, int> function_called_num;
    return function_called_num;
}

inline auto &functionToBeTimed() {
    static int *functionToBeTimed = nullptr;
    return functionToBeTimed;
}

inline auto &tClockTimer() {
    static clock_t tClockTimer = clock();
    return tClockTimer;
}

inline void LocalClock(const char *x) {
    do {
        if (functionToBeTimed())*functionToBeTimed() += clock() - tClockTimer();
        tClockTimer() = clock();
        functionToBeTimed() = &time_recorder()[x];
        ++function_called_num()[x];
    }
    while (0);
}

#define ResetClock     LocalClock(__FUNCTION__)

inline void DisplayClock() {
    for (auto f_t:time_recorder())
        main_log
        << std::string("                         ").replace(0, std::string(f_t.first).length(), f_t.first)
                << "total time: " << std::string("                ").replace(0, std::to_string(f_t.second).length(), std::to_string(f_t.second))
                << "hit number: " << std::string("                ").replace(0, std::to_string(function_called_num()[f_t.first]).length(), std::to_string(function_called_num()[f_t.first]))
        << "average time: " << f_t.second / function_called_num()[f_t.first] << std::endl;
}

#else
inline void LocalClock(){}
#define ResetClock
inline void DisplayClock(){}
#endif

//-----------------------------------------------LOG
inline void cleanlog() {
    std::ofstream fileout("log.dat");
    fileout.close();
}

inline void log() {
    DisplayClock();
    main_log << std::flush;
    std::ifstream fin("log.dat");
    std::string s;
    while (!fin.eof()) {
        getline(fin, s);
        std::cout << s << std::endl;
    }
    fin.close();
    cleanlog();
}

//-----------------------------------------------OUTPUT
inline void Info(std::string x) {
    main_log << BLUE << x << END << std::endl;
}

class ErrorOccur {
};

inline void Error(const char *x) {
    main_log << RED << "error: " << x << END << std::endl << CUT;
    throw ErrorOccur();
}

bool writeByMyself = false;

template<class T>
void Return(T thing) {
    std::cout << thing << "\n";
//    log();
#ifdef AnsCheck
    if (writeByMyself) return;
    std::string ansstr, mystr;
    std::getline(fans, ansstr);
//    std::cout << ansstr << std::endl;
    std::stringstream ss;
    ss << thing;
    mystr = ss.str();
    if (mystr != ansstr) {
        main_log << RED << "IWRONG IS: " << mystr << END << std::endl;
        main_log << RED << "ANSWER IS: " << ansstr << END << std::endl << CUT;
        log();
        std::exit(0);
        freopen("../myin.txt", "r", stdin);
        writeByMyself = true;
    }
    main_log << GREEN << "success " << thing << END << std::endl << CUT;
#endif

}

#else
static constexpr int litnum = 2;
class ErrorOccur {};
inline void log(){}
inline void cleanlog(){}
inline void Info(std::string x) {}
inline void Error(const char *x) { throw ErrorOccur(); }
template<class T> void Return(T thing){
    std::stringstream ss;
    ss << thing;
    printf("%s\n", ss.str().c_str());
}

inline void LocalClock(const char* x){}
#define ResetClock
inline void DisplayClock(){}

#endif

#endif //CODE_LOGGER_H
