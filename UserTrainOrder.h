//
// Created by JiamingShan on 2021/4/16.
//

#ifndef TRAINTICKET_BASICHEADER_H
#define TRAINTICKET_BASICHEADER_H


#include <cstring>
#include <string>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <sstream>
#include <limits>
#include "vector.hpp"
#include "DataStructures.h"
//FIXME 这只是个stub，记得删了



template<int N>
class cStringType {
private:
    char c[N] = {0};
public:
    cStringType() {}

    cStringType(const char *_c) {
        for (size_t i = 0; i < strlen(_c); ++i) {
            c[i] = _c[i];
        }
    }

    cStringType(const std::string &_c) {
        size_t sz = _c.size();
        for (size_t i = 0; i < sz; ++i) {
            c[i] = _c[i];
        }
    }

    cStringType &operator=(const char *_c) {
        memset(c, 0, sizeof(c));
        for (size_t i = 0; _c[i]; ++i) {
            c[i] = _c[i];
        }
        return *this;
    }

    operator std::string() const {
        return c;
    }

//    operator char*() const{
//        return c;
//    }

    operator bool() const {
        return c == "";
    }

    bool operator==(const cStringType &rhs) const {
        return std::string(c) == std::string(rhs.c);
    }

    bool operator!=(const cStringType &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const cStringType &rhs) const {
        return std::string(c) < std::string(rhs.c);
    }

    bool operator>(const cStringType &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const cStringType &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const cStringType &rhs) const {
        return !(*this < rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const cStringType &type) {
        os << type.c;
        return os;
    }

    template<class T>
    std::string operator+(T t) {
        return std::string(*this) + t;
    }

    friend std::string operator+(std::string s, cStringType<N> t) {
        return s + std::string(t);
    }
};

struct HourMinute {
    char hour = -1;
    char minute = -1;

    HourMinute() {}

    HourMinute(std::string str) : hour((str[0] - '0') * 10 + str[1] - '0'),
                                  minute((str[3] - '0') * 10 + str[4] - '0') {}

    operator std::string() const {
        if (hour == -1) return "xx:xx";
        std::string str;
        str += ('0' + hour / 10);
        str += ('0' + (hour - hour / 10 * 10));
        str += ':';
        str += ('0' + minute / 10);
        str += ('0' + (minute - minute / 10 * 10));
        return str;
    }

    explicit operator int() const {
        return hour * 60 + minute;
    }

    int operator+=(int x) {
        int iminute = minute, ihour = hour;
        iminute += x;
        ihour += iminute / 60;
        iminute %= 60;
        int overflow = ihour / 24;
        ihour %= 24;
        minute = iminute, hour = ihour;
        return overflow;//better 取模是否可优化？
    }

    bool operator<(const HourMinute &rhs) const {
        if (hour < rhs.hour)
            return true;
        if (rhs.hour < hour)
            return false;
        return minute < rhs.minute;
    }


    friend std::ostream &operator<<(std::ostream &os, const HourMinute &hourMinute) {
        os << ((hourMinute.hour < 10) ? "0" : "") << hourMinute.hour << ":" << ((hourMinute.minute < 10) ? "0" : "")
           << hourMinute.minute;
        return os;
    }
};

struct FullDate;

struct MonthDate {
    char month = 0;
    char date = 0;

    static constexpr int calendar[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    MonthDate() {}

    MonthDate(int month, int date) : month(month), date(date) {}

    MonthDate(FullDate);

    MonthDate(std::string str) : month(str[1] - '0'), date((str[3] - '0') * 10 + str[4] - '0') {}

    explicit MonthDate(int x) {
        month = 6;
        date = x;
        while (date > calendar[month]) {
            date -= calendar[month];
            ++month;
        }
    }

    MonthDate &operator++() {
        if (date == calendar[month]) {
            month += 1, date = 1;
            return *this;
        }
        date += 1;
        return *this;
    }

    MonthDate &operator+=(int x) {
        date += x;
        while (date > calendar[month]) {
            date -= calendar[month];
            ++month;
        }
        return *this;
    }

    MonthDate operator+(int x) const {
        MonthDate tmp(*this);
        tmp += x;
        return tmp;
    }

    explicit operator int() const {
        int ans = date;
        for (int i = month; i != 6; --i)
            ans += calendar[i - 1];
        return ans;
    }

    bool operator<(const MonthDate &rhs) const {
        if (month < rhs.month)
            return true;
        if (rhs.month < month)
            return false;
        return date < rhs.date;
    }

    bool operator<=(const MonthDate &rhs) const {
        return !(rhs < *this);
    }

    operator std::string() const {
        if (month == 0) return "xx-xx";
        std::string str;
        str += ('0' + month / 10);
        str += ('0' + (month - month / 10 * 10));
        str += '-';
        str += ('0' + date / 10);
        str += ('0' + (date - date / 10 * 10));
        return str;
    }

    friend std::ostream &operator<<(std::ostream &os, const MonthDate &date) {
        os << std::string(date);
        return os;
    }
};

constexpr int MonthDate::calendar[13];

struct FullDate {
    MonthDate monthDate;
    HourMinute hourMinute;

    FullDate() {}

    FullDate(const MonthDate &monthDate, const HourMinute &hourMinute) : monthDate(monthDate), hourMinute(hourMinute) {}

    FullDate &operator+=(int x) {
//        if(x == -1){hourMinute.hour = -1, monthDate.month = 0; return *this;}
        monthDate += (hourMinute += x);
        return *this;
    }

    bool operator<(const FullDate &rhs) const {
        if (monthDate < rhs.monthDate)
            return true;
        if (rhs.monthDate < monthDate)
            return false;
        return hourMinute < rhs.hourMinute;
    }

    explicit operator int() const {
        return int(monthDate) * 1440 + int(hourMinute);
    }

    operator std::string() const {
        return std::string(monthDate) + " " + std::string(hourMinute);
    }

    friend std::ostream &operator<<(std::ostream &os, const FullDate &date) {
        os << std::string(date);
        return os;
    }

//    operator MonthDate(){
//        return monthDate;
//    }

};

MonthDate::MonthDate(FullDate fullDate) {
    month = fullDate.monthDate.month;
    date = fullDate.monthDate.date;
}

typedef cStringType<21> Username;
typedef cStringType<31> Password;
typedef cStringType<16> Name;
typedef cStringType<31> MailAddr;
typedef int Privilege, OrderNumth;

struct User {
    Password password;
    MailAddr mailAddr;
    Name name;
    Privilege privilege;

    User() = default;

    User(Privilege privilege, const Name &name, const MailAddr &mailAddr,
         const Password &password, OrderNumth orderNumth = 0) : privilege(privilege), name(name), mailAddr(mailAddr),
                                                                password(password) {}

};//hack 坚持使用cStringType, rawuser 是否就不需要了？
//better existUsers可以全部丢进内存吗？

InnerUniqueUnorderMap<Username, Privilege> loginUsers;
//better 订单可以放在内存里，等logout或者exit时写回去吗？那样就要有通知补票的机制，怎么补呢？对于有保证login的操作，都可以如此做吗？
OuterUniqueUnorderMap<Username, User> existUsers("existUsers.dat");


typedef cStringType<21> TrainID;
typedef int StationNum, SeatNum, TicketNum;
typedef int Price;
typedef cStringType<31> StationName;
typedef cStringType<2> Type;
typedef cStringType<6> TwoChoice;
typedef std::string StationNames, Prices, TravelTimes, StopoverTimes, SaleDates;

//? zipArray{
//
//}
//struct ZipedTrain{
//
//};


struct Train {
    static constexpr int STATIONMAX = 100;
    static constexpr int DAYMAX = 93;
    TicketNum ticketNums[DAYMAX][STATIONMAX] = {0};
    StationName stations[STATIONMAX];//这里可以用表示替代，注意！
    Price prices[STATIONMAX];
    int arrivingTimes[STATIONMAX] = {0};
    int leavingTimes[STATIONMAX] = {0};//FIXME 应该是直接算出每站的绝对时间
    TrainID trainID;
    HourMinute startTime;
    MonthDate startSaleDate, endSaleDate;
    StationNum stationNum;
    SeatNum seatNum;
    Type type;
    bool is_released = false;

    Train() {}

    //caution 拷贝赋值都是浅拷贝！
//    Train(const Train&) = delete;
//    Train &operator=(const Train&) = delete;
    Train(TrainID _trainID, StationNum _stationNum, const sjtu::vector<StationName> &_stations, SeatNum _seatNum,
          const sjtu::vector<Price> &_prices, const HourMinute &_startTime,
          const sjtu::vector<int> &_arrivingTimes, const sjtu::vector<int> &_leavingTimes,
          const sjtu::vector<MonthDate> &_saleDate, const Type &_type) : trainID(_trainID), stationNum(_stationNum),
                                                                         seatNum(_seatNum),
                                                                         startTime(_startTime), type(_type) {
        for (int i = 0; i < stationNum; ++i) stations[i] = _stations[i];
        arrivingTimes[0] = leavingTimes[stationNum - 1] = 0;
        arrivingTimes[1] = _arrivingTimes[0], leavingTimes[0] = 0;
        prices[0] = 0;
        for (int i = 1; i < stationNum; ++i) {
            prices[i] = prices[i - 1] + _prices[i - 1];
        }
        for (int i = 1; i <= stationNum - 2; ++i) {
            leavingTimes[i] = arrivingTimes[i] + _leavingTimes[i - 1];
            arrivingTimes[i + 1] = leavingTimes[i] + _arrivingTimes[i];
        }
        startSaleDate = _saleDate[0], endSaleDate = _saleDate[1];

        for (int i = int(_saleDate[0]); i <= int(_saleDate[1]); ++i)
            for (int j = 0; j < _stationNum - 1; ++j)
                ticketNums[i][j] = _seatNum;
    }

    int findStation(const StationName &stationName) const {
        for (int i = 0; i < stationNum; ++i) {
            if (stations[i] == stationName) return i;
        }
        return -1;
    }
};

struct ZipedTrain {
    static constexpr int STATIONMAX = 100;
    static constexpr int DAYMAX = 93;
    unsigned short ticketNums[DAYMAX][STATIONMAX] = {0};
    unsigned char ismore[DAYMAX][STATIONMAX] = {0};
    StationName stations[STATIONMAX];//这里可以用表示替代，注意！
    Price prices[STATIONMAX];
    int arrivingTimes[STATIONMAX] = {0};
    int leavingTimes[STATIONMAX] = {0};//FIXME 应该是直接算出每站的绝对时间
    TrainID trainID;
    HourMinute startTime;
    MonthDate startSaleDate, endSaleDate;
    StationNum stationNum;
    SeatNum seatNum;
    Type type;
    bool is_released = false;

    ZipedTrain() {}

    ZipedTrain(const Train &train) : trainID(train.trainID), stationNum(train.stationNum),
                                     seatNum(train.seatNum),
                                     startTime(train.startTime), type(train.type), is_released(train.is_released),
                                     startSaleDate(train.startSaleDate), endSaleDate(train.endSaleDate) {
        for (int i = 0; i < stationNum; ++i) {
            stations[i] = train.stations[i];
            prices[i] = train.prices[i];
            leavingTimes[i] = train.leavingTimes[i];
            arrivingTimes[i] = train.arrivingTimes[i];
        }
        for (int i = int(train.startSaleDate); i <= int(train.endSaleDate); ++i)
            for (int j = 0; j < train.stationNum - 1; ++j) {
                ticketNums[i][j] = train.ticketNums[i][j] >> 8;
                ismore[i][j] = train.ticketNums[i][j] & 255U;
            }
    }

    operator Train() const {
        Train train;
        train.trainID = trainID, train.stationNum = stationNum,
        train.seatNum = seatNum, train.startTime = startTime,
        train.type = type, train.is_released = is_released;
        for (int i = 0; i < stationNum; ++i) {
            train.stations[i] = stations[i];
            train.prices[i] = prices[i];
            train.leavingTimes[i] = leavingTimes[i];
            train.arrivingTimes[i] = arrivingTimes[i];
        }
        train.startSaleDate = startSaleDate, train.endSaleDate = endSaleDate;

        for (int i = int(startSaleDate); i <= int(endSaleDate); ++i)
            for (int j = 0; j < stationNum - 1; ++j)
                train.ticketNums[i][j] = int(ticketNums[i][j]) << 8 | ismore[i][j];
        return train;
    }
};


OuterUniqueUnorderMap<TrainID, ZipedTrain> existTrains("existTrains.dat");

typedef int TrainPtr;

enum Status {
    SUCCESS, PENDING, REFUNDED
};

std::string stringlizeOrderStatus(Status Status) {
    if (Status == SUCCESS) return "success";
    if (Status == PENDING) return "pending";
    if (Status == REFUNDED) return "refunded";
    return "ERROR";
}

struct Order {
    StationName fromStation;
    StationName toStation;
    Username username;//better username其实并不需要，可以把这个空间压掉。 但是要想压掉，得在队列里加入order对应的name。这样值得吗？
    TrainID trainID;

    FullDate arrivingTime;
    FullDate leavingTime;
    Price price;
    int num;
    //hack 时间戳
    static int timestamp;
    int timestamplocal;
    Status status;

    //或许需要ordernumth来做唯一认证，在refundticket的时候可以知道退了炸队列的到底是队列里的哪笔订单？
    Order() {}

    Order(const Username &username, const Status &status, const TrainID &trainId,
          const StationName &fromStation, const StationName &toStation,
          const FullDate &leavingTime, const FullDate &arrivingTime, Price price, int num) : username(username),
                                                                                             status(status),
                                                                                             trainID(trainId),
                                                                                             fromStation(
                                                                                                     fromStation),
                                                                                             toStation(toStation),
                                                                                             leavingTime(
                                                                                                     leavingTime),
                                                                                             arrivingTime(
                                                                                                     arrivingTime),
                                                                                             price(price),
                                                                                             num(num) { timestamplocal = ++timestamp; }

    operator std::string() {
        std::string ans;
        ans += (trainID + " " + fromStation + " " + std::string(leavingTime) + " -> " + toStation + " " +
                std::string(arrivingTime) + " " + std::to_string(price) + " " + std::to_string(num));
        return ans;
    }

    bool operator==(const Order &rhs) const {
        return
                timestamplocal == rhs.timestamplocal &&//hack
                username == rhs.username &&
                trainID == rhs.trainID &&
                fromStation == rhs.fromStation &&
                toStation == rhs.toStation;
    }

    bool operator!=(const Order &rhs) const {
        return !(rhs == *this);
    }
};

int Order::timestamp = 0;

InnerOuterMultiUnorderMap<StationName, TrainPtr> stmap("stationName_trainPtr.dat");

void addPassedTrainPtr(StationName stationName, TrainPtr trainPtr) {
    stmap.insert({stationName, trainPtr});
}


sjtu::map<TrainPtr, bool> findCommonTrain(StationName fromStation, StationName toStation) {
    auto iter_s = stmap.find(fromStation);//很奇怪，find是主开销，明明应该是内存中办事的。
    if (!iter_s) return sjtu::map<TrainPtr, bool>();
    auto iter_t = stmap.find(toStation);
    if (!iter_t) return sjtu::map<TrainPtr, bool>();
    sjtu::map<TrainPtr, bool> it_i, it_j, ret;
    for (auto it = iter_s->begin(); it != iter_s->end(); ++it) it_i.insert({*it, false});
    for (auto it = iter_t->begin(); it != iter_t->end(); ++it)
        if (it_i.find(*it) != it_i.end()) ret.insert({*it, false});
    return ret;
}


typedef sjtu::map<StationName, sjtu::pair<sjtu::vector<TrainPtr>, sjtu::vector<TrainPtr>>> MidRetType;

MidRetType findMidStation(StationName fromStation, StationName toStation) {
    MidRetType midStations;
    auto iter_s = stmap.find(fromStation);
    if (!iter_s) return MidRetType();
    auto iter_t = stmap.find(toStation);
    if (!iter_t) return MidRetType();
    for (auto it = iter_s->begin(); it != iter_s->end(); ++it) {
        const Train &train = existTrains.getItem(*it);
        int i = 0;
        for (; i != train.stationNum && train.stations[i] != fromStation; ++i);
        for (++i; i != train.stationNum; ++i) {
            midStations[train.stations[i]].first.push_back(*it);
        }
    }
    for (auto it = iter_t->begin(); it != iter_t->end(); ++it) {
        const Train &train = existTrains.getItem(*it);
        int i = 0;
        for (; i != train.stationNum && train.stations[i] != toStation; ++i) {
            midStations[train.stations[i]].second.push_back(*it);
        }
    }
    MidRetType midRet;
    for (auto it : midStations) {
        if (it.second.first.size() && it.second.second.size()) {
            midRet.insert(it);
        }
    }
    //better outofsalesday-index 应该拿到这边来判——实际上什么都可以在这边做，因为写成暴力了嘛
    return midRet;
}


InnerOuterMultiUnorderMap<Username, Order> userOrders("user_orders.dat");

//InnerOuterMultiUnorderMap<TrainID, Order> waitQueue("wait_queue.dat");
Queue<Order> waitQueue("wait_queue.dat");

#endif //TRAINTICKET_BASICHEADER_H
