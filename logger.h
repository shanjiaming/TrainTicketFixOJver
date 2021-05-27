//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_LOGGER_H
#define CODE_LOGGER_H

#include <iostream>
#include "filemanip.h"
//#define OpenLog

#ifdef OpenLog

#define AnsCheck
static constexpr int litnum = 1;
static constexpr int bignum = 6;
#define FileI { std::freopen((std::string("../data/basic_") + std::to_string(bignum) + '/' + std::to_string(litnum) + ".in").c_str(), "r", stdin);};
        std::fstream fans((std::string("../data/basic_") + std::to_string(bignum) + '/' + std::to_string(litnum) + ".out").c_str());
#define TimeTracing
//#define MagnitudeTracing
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
inline auto& time_recorder(){ static sjtu::map<const char *, int> time_recorder; return time_recorder;}
inline auto& function_called_num(){ static sjtu::map<const char *, int> function_called_num; return function_called_num;}
inline auto& functionToBeTimed(){ static int *functionToBeTimed = nullptr; return functionToBeTimed;}
inline auto& tClockTimer(){ static clock_t tClockTimer = clock(); return tClockTimer;}
#define LocalClock(x)   do{if(functionToBeTimed())*functionToBeTimed() += clock() - tClockTimer();tClockTimer() = clock();functionToBeTimed() = &time_recorder()[x];++function_called_num()[x];}while(0)
#define ResetClock     LocalClock(__FUNCTION__)
#define StartStopWatch LocalClock("STOP_WATCH")
#define DisplayClock   for(auto f_t:time_recorder()) main_log << std::string("                      " ).replace(0,std::string(f_t.first).length(),f_t.first) << "\t\ttime: " << f_t.second << "   \t\thitnumber: " << function_called_num()[f_t.first] << "\t\taverage time: " << f_t.second/function_called_num()[f_t.first] << std::endl
//总是对不齐？随便调一调对不齐的地方前面空格的个数，说不定就对齐了。

#else
#define LocalClock
#define ResetClock
#define DisplayClock
#define StartStopWatch
#endif
//TODO 写一个东西，测文件读写次数/bpt访问次数

//-----------------------------------------------MagnitudeTracing
#ifdef MagnitudeTracing

sjtu::map<const char *, std::pair<int,int>> mag_recorder;
#define InTrace(x) ++mag_recorder[x].first;if(mag_recorder[x].second < mag_recorder[x].first)mag_recorder[x].second = mag_recorder[x].first
#define OutTrace(x) --mag_recorder[x].first
#define DisplayMagnitude for(auto c_p:mag_recorder)main_log << c_p.first << "\t" << c_p.second.second << std::endl
#else
#define InTrace
#define OutTrace
#define DisplayMagnitude
#endif
//todo 增加其它数据量级检测功能 如补票队长

//todo 增加tracetrain功能，对单个火车显示其一切的日志
//-----------------------------------------------Tracer
#ifdef TracerTracing
std::string traceAnswer;
#define Tracer do {if(BeTraced == TraceWanting){traceAnswer+=input; traceAnswer+="\n";}}while(0)
#define DisplayTrace main_log << CUT << traceAnswer << CUT;
#else
#define Tracer
#define DisplayTrace
#endif
//-----------------------------------------------LOG
inline void log() {
    DisplayClock;
    DisplayMagnitude;
    DisplayTrace;
    main_log << std::flush;
    std::ifstream fin("log.dat");
    std::string s;
    while (!fin.eof()) {
        getline(fin, s);
        std::cout << s << std::endl;
    }
    fin.close();
    fclear("log.dat");
}

inline void cleanlog() {
    fclear("log.dat");
}

//-----------------------------------------------OUTPUT
inline void Info(std::string x) {
    main_log << BLUE << x << END << std::endl;
}

class ErrorOccur {};

inline void Error(const char *x) {
    main_log << RED << "error: " << x << END << std::endl << CUT;
    throw ErrorOccur();
}

bool writeByMyself = false;
template<class T>
void Return(T thing){
    std::cout << thing << "\n";
//    log();
#ifdef AnsCheck
    if(writeByMyself) return;
    std::string ansstr, mystr;
    std::getline(fans, ansstr);
//    std::cout << ansstr << std::endl;
    std::stringstream ss;
    ss << thing;
    mystr = ss.str();
    if(mystr!=ansstr){
        main_log << RED  << "IWRONG IS: " << mystr << END << std::endl;
        main_log << RED  << "ANSWER IS: " << ansstr << END << std::endl << CUT;
        log();
        std::exit(0);
        freopen("../myin.txt", "r", stdin);
        writeByMyself = true;
    }
    main_log << GREEN  << "success " << thing << END << std::endl << CUT;
#endif

}

#else
static constexpr int litnum = 2;
class ErrorOccur {};
inline void log(){}
inline void cleanlog(){}
inline void Info(std::string x) {}
inline void Error(const char *x) { throw ErrorOccur(); }
template<class T> void Return(T thing){ std::cout << thing << "\n"; }

#define LocalClock
#define ResetClock
#define DisplayClock
#define StartStopWatch

#define Tracer
#define DisplayTrace

#define InTrace
#define OutTrace
#define DisplayMagnitude
#endif
//怎么查撞哈希啊
//测试驱动开发
//不能被多重包含，讨厌。


#endif //CODE_LOGGER_H
