

#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <regex>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "vector.hpp"
#include "UserTrain.h"
#include <functional>

void initialize();

void function_chooser();


namespace user {

    void login(Username, Password);

    void logout(Username);

    void add_user(Username, Username, Password, Name, MailAddr, Privilege);

    void query_profile(Username, Username);

    void modify_profile(Username, Username, Password, Name, MailAddr, Privilege);

}

namespace train {

    void
    add_train(TrainID, StationNum, SeatNum, StationNames, Prices, StartTime, TravelTimes, StopoverTimes, SaleDates,
              Type);

    void release_train(TrainID);

    void query_train(TrainID, MonthDate);

    void delete_train(TrainID);

    void query_ticket(StationName, StationName, MonthDate, TwoChoice);

    void query_transfer(StationName, StationName, MonthDate, TwoChoice);

    void buy_ticket(Username, TrainID, MonthDate, TicketNum, StationName, StationName, TwoChoice);

    void query_order(Username);

    void refund_ticket(Username, OrderNumth);
}

namespace sys {

    void noReturnClean();

    void clean();

    void exit();
}


int main() {
#ifdef FileI
    FileI
#endif
    initialize();
    while (true) {
        try {
            function_chooser();
        }
        catch (ErrorOccur) {
            Return(-1);
        }
    }
}

std::string input;

void initialize() {
    if (litnum == 1) {
        sys::noReturnClean();//FIXME used to debug
        cleanlog();
    }
}

void function_chooser() {//FIXME ʱ�������쳣������Ҫ������regex������������ĳ�staticʹ����20��������ʹ����Ҳ�ر������
    ResetClock;

    std::string s[10];

    static const mypair<std::string, std::string> chooser[17] = {
            {"add_user", "upnm cg "},
            {"login", "up "},
            {"logout", "u "},
            {"query_profile", "cu "},
            {"modify_profile", "cu pnmg"},
            {"add_train", "inmspxtody "},
            {"release_train", "i "},
            {"query_train", "id "},
            {"delete_train", "i "},
            {"query_ticket", "std p"},
            {"query_transfer", "std p"},
            {"buy_ticket", "uidnft q"},
            {"query_order", "u "},
            {"refund_ticket", "u n"},
            {"log", " "},
            {"clean", " "},
            {"exit", " "}};

//getline �Ǻ�ʱ�ģ�
//better �����������Ǻ�ʱ�ģ�ע��һ�¡�
    getline(std::cin, input);
    input.erase(0, input.find_first_not_of(' '));
    input.erase(input.find_last_not_of(' ') + 1);
//    if (cin.eof()) exit(0);
    //maybe ��֪����û��eof�Զ��رյĻ��ƣ�����Ϊû�У���Ϊ����û��buy����ֵ������Ҫ��̬�Żء�
    if (input.empty()) return;

//    Info(input);
    //memo ��־��Ϊ��ʱ���ص�����


    std::string funcNameStr, paraStr;
    std::stringstream(input) >> funcNameStr;
    for (auto i = begin(chooser); ; ++i) {
        if(i == end(chooser))Error("SYNTAX ERROR");
        if (funcNameStr == i->first) {
            paraStr = i->second;
            break;
        }
    }
    int ip = 0;
    const int parasz = paraStr.size();
    for (; paraStr[ip] != ' '; ++ip) {
        unsigned long loc = input.find(std::string(" -") + paraStr[ip] + " ");
        if(loc == std::string::npos) Error("NOT FIND PARAMETER");
        std::stringstream(input.substr(loc + 4)) >> s[ip];
    }
    for (++ip; ip < parasz; ++ip) {
        unsigned long loc = input.find(std::string(" -") + paraStr[ip] + " ");
        if(loc == std::string::npos) continue;
        std::stringstream(input.substr(loc + 4)) >> s[ip - 1];
    }
    static auto mystoi = [](std::string str){
        return str.empty() ? -1 : stoi(str);
    };

    static mypair<std::string, std::function<void()>> arr[17] = {
            {"add_user", [&s]() { user::add_user(s[4], s[0], s[1], s[2], s[3], mystoi(s[5]));}},
            {"login", [&s]() { user::login(s[0], s[1]); }},
            {"logout", [&s]() { user::logout(s[0]); }},
            {"query_profile", [&s]() { user::query_profile(s[0], s[1]); }},
            {"modify_profile", [&s]() { user::modify_profile(s[0], s[1], s[2], s[3], s[4], mystoi(s[5])); }},
            {"add_train", [&s]() {train::add_train(s[0], mystoi(s[1]), mystoi(s[2]), s[3], s[4], s[5], s[6], s[7], s[8],s[9]);}},
            {"release_train", [&s]() { train::release_train(s[0]); }},
            {"query_train", [&s]() { train::query_train(s[0], s[1]); }},
            {"delete_train", [&s]() { train::delete_train(s[0]); }},
            {"query_ticket", [&s]() { train::query_ticket(s[0], s[1], s[2], s[3]); }},
            {"query_transfer", [&s]() { train::query_transfer(s[0], s[1], s[2], s[3]); }},
            {"buy_ticket", [&s]() {train::buy_ticket(s[0], s[1], s[2], mystoi(s[3]), s[4], s[5], s[6]);}},
            {"query_order", [&s]() { train::query_order(s[0]); }},
            {"refund_ticket", [&s]() { train::refund_ticket(s[0], mystoi(s[1])); }},
            {"log", [&s]() { log(); }},
            {"clean", [&s]() { sys::clean(); }},
            {"exit", [&s]() { sys::exit(); }}};

    for (auto i = begin(arr); i != end(arr); ++i) {
        if (funcNameStr == i->first) {
            i->second();
            return;
        }
    }

    Error("SYNTAX ERROR");
}

void user::add_user(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                    Privilege privilege) {
    ResetClock;
    if (!existUsers.empty()) {//�����������������£���������Ӱ�죬��Ϊuser�ǲ��ᱻɾ����
        if(cur_username == "" ||privilege == -1) Error("EMPTY PARAMETER");
        auto curPrivilegePtr = loginUsers.find(cur_username);
        if (!curPrivilegePtr)Error("CURRENT USER DOES NOT LOGIN");
        Privilege curPrivilege = *curPrivilegePtr;
        if (privilege >= curPrivilege)Error("NO PRIVILEGE");
//        if (password.size() < 6)Error("PASSWORD IS TOO SHORT");
    } else {
        privilege = 10;
    }
    existUsers.insert({username, User(privilege, name, mailAddr, password)});
    Return(0);

}

void user::login(Username username, Password password) {
    ResetClock;
    InTrace("loginUser");
    auto CurUserPair = existUsers.find(username);
    if (!CurUserPair.second) Error("USER DOES NOT EXIST");
    const User &foundUser = existUsers.getItem(CurUserPair.first);
    if (foundUser.password != password) Error("WRONG PASSWORD");
    if (!loginUsers.insert({username, foundUser.privilege}))Error("USER HAS ALREADY LOGIN");
    Return(0);
}
//buyticketֻ�ı�loginUser��orderNumth����logout��exit��ʱ���д������orderNumth���������ÿ��buyticket����Լһ��дexistUser��ʱ�䡣

void user::logout(Username username) {
    ResetClock;
    OutTrace("loginUser");
    auto erasePair = loginUsers.erase(username);
    if (!erasePair.second)Error("CURRENT USER DOES NOT LOGIN");//erase in loginUsers
    Return(0);

}


void user::query_profile(Username cur_username, Username username) {
    ResetClock;
    auto curUserPtr = loginUsers.find(cur_username);
    if (!curUserPtr) Error("CURRENT USER DOES NOT LOGIN");
    auto UserPair = existUsers.find(username);
    if (!UserPair.second) Error("FINDING USER DOES NOT EXIST");
    const User &foundUser = existUsers.getItem(UserPair.first);
    Privilege &curPrivilege = *curUserPtr;
    if (!(curPrivilege > foundUser.privilege || cur_username == username)) Error("NO PRIVILEGE");
    Return(username + ' ' + foundUser.name + ' ' + foundUser.mailAddr + ' ' +
           std::to_string(foundUser.privilege));

}

void user::modify_profile(Username cur_username, Username username, Password password, Name name, MailAddr mailAddr,
                          Privilege privilege) {
    ResetClock;
    auto curUserPtr = loginUsers.find(cur_username);
    if (!curUserPtr) Error("CURRENT USER DOES NOT LOGIN");
    Privilege curPrivilege = *curUserPtr;
    auto ptr = existUsers.find(username);
    if (!ptr.second)
        Error("FINDING USER DOES NOT EXIST");
    User previous_user = existUsers.getItem(ptr.first);
    if (!(curPrivilege > previous_user.privilege || cur_username == username))
        Error("NO PRIVILEGE");
    if (!(privilege < curPrivilege))
        Error("NO PRIVILEGE");
    const User &changedUser = User(privilege == -1 ?
                                   previous_user.privilege : privilege,
                                   name == "" ? previous_user.name : name,
                                   mailAddr == "" ? previous_user.mailAddr : mailAddr,
                                   password == "" ? previous_user.password : password);
    existUsers.setItem(ptr.first, changedUser);
    if (privilege != -1) {
        auto ptr = loginUsers.find(username);
        if (ptr)
            *ptr = privilege;//better �ɸ�����������
    }
    Return(username + ' ' + changedUser.name + ' ' + changedUser.mailAddr + ' ' +
           std::to_string(changedUser.privilege));//��ȻӦ���������ڵ����ַ��ذ�

}

template<class T>
sjtu::vector<T> words_spliter(const std::string &_keyword) {
    sjtu::vector<T> ret;
    std::stringstream ss(_keyword);
    std::string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(oneword);
    }
    return ret;
}

sjtu::vector<int> ints_spliter(const std::string &_keyword) {
    sjtu::vector<int> ret;
    std::stringstream ss(_keyword);
    std::string oneword;
    while (getline(ss, oneword, '|')) {
        ret.push_back(stoi(oneword));
    }
    return ret;
}

void train::add_train(TrainID trainID, StationNum stationNum, SeatNum seatNum, StationNames stations, Prices prices,
                      StartTime startTime, TravelTimes travelTimes, StopoverTimes stopoverTimes, SaleDates saleDates,
                      Type type) {
    ResetClock;
//    Tracer;
    sjtu::vector<StationName> station_s = words_spliter<StationName>(stations);
    sjtu::vector<PassedMinutes> travelTime_s = ints_spliter(travelTimes);
    sjtu::vector<PassedMinutes> stopoverTime_s = ints_spliter(stopoverTimes);
    sjtu::vector<Price> price_s = ints_spliter(prices);
    sjtu::vector<MonthDate> saleDate_s = words_spliter<MonthDate>(saleDates);
    if (!(station_s.size() == stationNum && price_s.size() == stationNum - 1 && travelTime_s.size() == stationNum - 1
          && stopoverTime_s.size() == std::max(1, stationNum - 2))) {
        Error("NUM OF '|' DOES NOT MATCH");
    }

    if (!existTrains.insert(
            {trainID, Train(trainID, stationNum, station_s, seatNum, price_s, startTime, travelTime_s, stopoverTime_s,
                            saleDate_s, type)}))
        Error("TRAIN ALREADY EXIST!");
    Return(0);
}

auto getTrainPtr(TrainID trainID) {
    auto curTrainPair = existTrains.find(trainID);
    if (!curTrainPair.second) Error("FINDING TRAIN DOES NOT EXIST");
    return curTrainPair.first;
}

Train getTrain(TrainID trainID) {
    return existTrains.getItem(getTrainPtr(trainID));
}

void AssureLogin(Username username) {
    if (!loginUsers.find(username))Error("USER DOES NOT LOGIN");//fixme ע�⣬�������ĵط������Ա���һ��map�Ż�����֮����������滻��
}

void train::release_train(TrainID trainID) {
    ResetClock;
    Tracer;
    auto trainPtr = getTrainPtr(trainID);
    Train train = existTrains.getItem(trainPtr);
    if (train.is_released == 1)Error("TRAIN HAS ALREADY BE RELEASED");
    train.is_released = 1;
    existTrains.setItem(trainPtr, train);

//stub
    for (int i = 0; i < train.stationNum; ++i)
        addPassedTrainPtr(train.stations[i], trainPtr);

    Return(0);
}

void train::query_train(TrainID trainID, MonthDate startingMonthDate) {
    ResetClock;
    Tracer;
    Train train = getTrain(trainID);
    if (startingMonthDate < train.startSaleDate || train.endSaleDate < startingMonthDate)
        Error("QUERY DATE NOT IN SALE DATE");
//��Ȼ�ܴ�˵�ˣ�����˵�ĸ� maybe
    std::string ans;
    Return(trainID + " " + train.type);
    //TODO ���������Ƿ�����
    int test = int(startingMonthDate);
    for (int i = 0; i < train.stationNum; ++i) {
        Return(train.stations[i] + " " + ((i != 0) ? std::string(FullDate(startingMonthDate, train.startTime) += train
                .arrivingTimes[i]) : "xx-xx xx:xx") + " -> " +
               ((i + 1 != train.stationNum) ? std::string(FullDate(startingMonthDate, train.startTime) += train
                       .leavingTimes[i]) : "xx-xx xx:xx") + " " + std::to_string(train.prices[i]) + " " +
               ((i + 1 != train.stationNum) ? std::to_string(train.ticketNums[int(startingMonthDate)][i]) : "x"));
    }
}

void train::delete_train(TrainID trainID) {
    ResetClock;
    Tracer;
    //hack ��Ϊ��n������ѡ���������
    //getTrain�����׳��쳣�����鱣֤�ˡ�
    if (getTrain(trainID).is_released)Error("DELETE TRAIN IS RELEASED");
    existTrains.erase(trainID);
    Return(0);
}


void train::query_order(Username username) {
    ResetClock;
    //    Tracer;
    AssureLogin(username);
    auto orderList = userOrders.find(username);
    Return(orderList->size());
    for (auto it = orderList->begin(); it != orderList->end(); ++it) {
        Return(std::string("[") + stringlizeOrderStatus(it->status) + "] " + std::string(*it));
    }
}

enum FunctionName {
    BUY_TICKET, QUERY_TICKET, QUERY_TRANSFER_FROM, QUERY_TRANSFER_TO, REFUND_TICKET, INFORM_QUEUE
};

struct OrderCalculator {
    Order order;
    int timeret;
    Username username;
    TicketNum ticketNum;
    TwoChoice wannaWaitToBuyIfNoEnoughTicket;
    decltype(waitQueue.begin()) *orderIterIter;
    TrainPtr trainPtr;
    FullDate arriveTomid;
//    MidStation


    void run(FunctionName functionName, TrainID trainID, const MonthDate& monthDate, StationName fromStation,
             StationName toStation) {
//        Tracer;
        const bool giveTrainPtrInsteadOfTrainID = functionName == QUERY_TICKET || functionName == QUERY_TRANSFER_FROM ||
                                                  functionName == QUERY_TRANSFER_TO;
        if (!giveTrainPtrInsteadOfTrainID)trainPtr = getTrainPtr(trainID);
        Train train = existTrains.getItem(trainPtr);
        if (giveTrainPtrInsteadOfTrainID) trainID = train.trainID;
        if (!train.is_released) Error("TRAIN HAS NOT BEEN RELEASED YET");
        const int fromint = train.findStation(fromStation), toint = train.findStation(toStation);
        const PassedMinutes leavingTime = train.leavingTimes[fromint], arrivingTime = train.arrivingTimes[toint];
        HourMinute startTime = train.startTime;
        if (monthDate.month < 6 ||  monthDate.date < 0 || monthDate.date > 31) Error("INVALID MONTHDATE");
        int index = int(monthDate) - ( startTime += leavingTime);
        if (functionName == QUERY_TRANSFER_TO)
            if (int(arriveTomid.hourMinute) > int(startTime)) ++index;
        if (index > int(train.endSaleDate))
            Error("OUT OF SALEDATE--INDEX ");
        if (index < int(train.startSaleDate))
            if(functionName != QUERY_TRANSFER_TO)
                Error("OUT OF SALEDATE--INDEX ");
            else index = int(train.startSaleDate);
        const MonthDate trainStartDay(index);
        if (fromint == -1 || toint == -1)Error("CANNOT FIND STATION");
        TicketNum minTicket = 0x3f3f3f3f;
        if (fromint >= toint) Error("REVERSED PAIR");//�Ⱥţ���ֹԭ��tp
        for (int i = fromint; i < toint; ++i)
            minTicket = std::min(minTicket, train.ticketNums[index][i]);
        if (functionName != REFUND_TICKET && functionName != INFORM_QUEUE)
            order = Order(username, PENDING,
                          trainID, fromStation, toStation,
                          FullDate(trainStartDay, train.startTime) += leavingTime,
                          FullDate(trainStartDay, train.startTime) += arrivingTime,
                          train.prices[toint] - train.prices[fromint],
                          ticketNum
            );

        //����order������ͬģʽ

        if (functionName == QUERY_TICKET) {
            timeret = train.arrivingTimes[toint] - train.leavingTimes[fromint];
            order.num = minTicket;
            return;
        }
        if (functionName == QUERY_TRANSFER_FROM || functionName == QUERY_TRANSFER_TO) {
            order.num = minTicket;
            return;
        }
        if (functionName == BUY_TICKET) {
            AssureLogin(username);
            if(train.seatNum < ticketNum) Error("REALLY NO ENOUGH TICKET: MORE THAN SEATNUM");
            if (minTicket < ticketNum) {//no enough ticket
                if (wannaWaitToBuyIfNoEnoughTicket == "false") Error("NO ENOUGH TICKET");
                //better Queue ���Բ�����Queue�����ӣ�����trainIDΪkey��һ��map��������refund_ticket��ʱ����Դ�����ٲ��������ĸ��Ӷȣ��������ڴ���Ϊ��
                waitQueue.push_back(order);//pending6
                InTrace("queueLength");
                userOrders.insert({username, order});
                Return("queue");
                return;
            }
            order.status = SUCCESS;//success
            userOrders.insert({username, order});
            for (int i = fromint; i < toint; ++i) {
                train.ticketNums[index][i] -= ticketNum;
            }
            existTrains.setItem(trainPtr, train);
            Return((long long)(order.price) * ticketNum);
            return;
        }

        if (functionName == REFUND_TICKET) {
            Info(std::string("info: refund_ticket ") + trainID + " " + std::string(monthDate) + " " +
                 std::to_string(order.num));
            for (int i = fromint; i < toint; ++i)//strange Ϊʲô����lambda����
                train.ticketNums[index][i] += order.num;
            existTrains.setItem(trainPtr, train);
            for (auto orderIter = waitQueue.begin(); orderIter != waitQueue.end(); ++orderIter) {
                if (orderIter->trainID != trainID || orderIter->arrivingTime < order.leavingTime ||
                    order.arrivingTime < orderIter->leavingTime)
                    continue;
                orderIterIter = &orderIter;
                try {
                    run(INFORM_QUEUE, trainID, orderIter->leavingTime, orderIter->fromStation, orderIter->toStation);
                }
                catch (ErrorOccur) { continue; }
            }
            // queue ���Գ��п��ٷ���order�ķ�������֮��Ȼ������֮����Ҫ�Բ���ô��Ҫ����Ϊ�ڴ�queue�ȶ��ֿ졣
            Return(0);
            return;
        }
        if (functionName == INFORM_QUEUE) {//֪ͨ��Ʊ����
            //orderIter Ϊ�����е�ĳһ���ˣ�����Ҫ����Ǹ��˵�order
//            Error("Debuginggggg!");
            auto &orderIter = *orderIterIter;
            if (minTicket < orderIter->num)return;
            //todo check valid maybe checked? I don't know
//            Error("I DIDNT CHECK");
            orderIter->status = SUCCESS;//success
            for (int i = fromint; i < toint; ++i) {
                train.ticketNums[index][i] -= orderIter->num;
            }
            existTrains.setItem(trainPtr, train);
            auto orderListPtr = userOrders.find(orderIter->username);
            for (auto iter = orderListPtr->begin(); iter != orderListPtr->end(); ++iter) {
                if (*iter == *orderIter) {
                    iter->status = SUCCESS;
                    waitQueue.erase(orderIter);
                    Info(std::string("successed BuPiao   username:") + iter->username + "  trainID  " + iter->trainID
                         + " fromStation  " + iter->fromStation + " toStation  " + iter->toStation +
                         "   leavingTime  " + std::string(iter->leavingTime) + "   arrivingTime  " +
                         std::string(iter->arrivingTime)
                         + "   num  " + std::to_string(iter->num));
                    return;
                }
            }
            assert(false);
        }
    }
} orderCalculator;

//better ��order_calculator ������΢С���࣬�����д���ȵ�

void train::query_ticket(StationName fromStation, StationName toStation, MonthDate monthDate,
                         TwoChoice sortFromLowerToHigherBy) {
    ResetClock;
    if (sortFromLowerToHigherBy == "")sortFromLowerToHigherBy = "time";
    auto trainPtrs = findCommonTrain(fromStation, toStation);//better station ֱ���ֳ�Outer��Iterator������ַ��
    //stub in order to use sort in <algorithm>, I use std::vector instead of sjtu::vector
    std::vector<mypair<mypair<int, TrainID>, std::string>> vans;
    for (TrainPtr trainPtr : trainPtrs) {
        try {
            orderCalculator.trainPtr = trainPtr;
            orderCalculator.run(QUERY_TICKET, "", monthDate, fromStation, toStation);
            vans.push_back(
                    {{(sortFromLowerToHigherBy == "time") ? orderCalculator.timeret : orderCalculator.order.price,
                      orderCalculator.order.trainID},
                     std::string(orderCalculator.order)});
        } catch (ErrorOccur) {
            continue;
        }
    }
    //stub ��vector�ɵ͵�������
#include <algorithm>

    sort(vans.begin(), vans.end());
    Return(vans.size());
    for (auto i : vans) Return(i.second);
//TODO
}

void train::query_transfer(StationName fromStation, StationName toStation, MonthDate monthDate,
                           TwoChoice sortFromLowerToHigherBy) {
    ResetClock;
    if (sortFromLowerToHigherBy == "")sortFromLowerToHigherBy = "time";

    auto midStations = findMidStation(fromStation, toStation);

    mypair<mypair<int, PassedMinutes>, mypair<Order, Order>> bestAns;
    mypair<mypair<int, PassedMinutes>, mypair<Order, Order>> tmpAns;
    bestAns.first.first = 0x3f3f3f3f;
    //caution ע��s��t���г���˫���㣡������mid��Ҫͣ�����ܻ���ͬ��s��t�г��ĸ��ŵ�mid������
    for (auto midStation : midStations) {
        for (auto startTrainPtr : midStation.second.first) {
            orderCalculator.trainPtr = startTrainPtr;
            try {
                orderCalculator.run(QUERY_TRANSFER_FROM, "", monthDate, fromStation, midStation.first); } catch (
                    ErrorOccur) { continue; }
            tmpAns.second.first = orderCalculator.order;
            orderCalculator.arriveTomid = orderCalculator.order.arrivingTime;
            for (auto endTrainPtr : midStation.second.second) {
                if (startTrainPtr == endTrainPtr) continue;
                orderCalculator.trainPtr = endTrainPtr;
                try {
                    orderCalculator.run(QUERY_TRANSFER_TO, "", orderCalculator.arriveTomid.monthDate, midStation.first,
                                        toStation);
                } catch (ErrorOccur) { continue; }
                //                     (sortFromLowerToHigherBy == "time") ? int(orderCalculator.order.leavingTime) : orderCalculator.order.price
                tmpAns.second.second = orderCalculator.order;
                tmpAns.first.first = (sortFromLowerToHigherBy == "time") ? (int(tmpAns.second.second.arrivingTime) -
                                                                            int(tmpAns.second.first.leavingTime)) : (
                                             tmpAns.second.first.price + tmpAns.second.second.price);
                tmpAns.first.second = int(tmpAns.second.first.arrivingTime) - int(tmpAns.second.first.leavingTime);
                if (tmpAns.first < bestAns.first)
                {
                    bestAns = tmpAns;
                }
            }
        }
    }
    if (bestAns.first.first == 0x3f3f3f3f) {
        Return(0);
        return;
    }

    Return(std::string(bestAns.second.first));
    Return(std::string(bestAns.second.second));
}


void train::buy_ticket(Username username, TrainID trainID, MonthDate monthDate, TicketNum ticketNum,
                       StationName fromStation,
                       StationName toStation, TwoChoice wannaWaitToBuyIfNoEnoughTicket) {
    ResetClock;
    Tracer;
    if (wannaWaitToBuyIfNoEnoughTicket == "") wannaWaitToBuyIfNoEnoughTicket = "false";

    orderCalculator.ticketNum = ticketNum;
    orderCalculator.username = username;
    orderCalculator.wannaWaitToBuyIfNoEnoughTicket = wannaWaitToBuyIfNoEnoughTicket;
    orderCalculator.run(BUY_TICKET, trainID, monthDate, fromStation, toStation);
}


//queue���ּ��أ��رշŻ�
//existUsers���ּ��أ��رշŻ�

//todo User �� orderTotalNum ����exist��login�������



void train::refund_ticket(Username username, OrderNumth orderNumth) {
    ResetClock;
//    Tracer;
    if (orderNumth == -1) orderNumth = 1;
    AssureLogin(username);

    const auto orderIter = userOrders.find(username, orderNumth);
    if (!orderIter)Error("WRONG ORDERNUMTH");
    Order &order = *orderIter;
    if (order.status == REFUNDED)Error("ALREADY REFUNDED");
    if (order.status == PENDING) {
        for (auto it = waitQueue.begin(); it != waitQueue.end(); ++it) {
            if (*it == order) {
                waitQueue.erase(it);
                order.status = REFUNDED;
                Return(0);
                OutTrace("queueLength");
                return;
            }
        }
        assert(false);
    }
    order.status = REFUNDED;
    orderCalculator.order = order;
    orderCalculator.run(REFUND_TICKET, order.trainID, order.leavingTime, order.fromStation, order.toStation);
}


void sys::noReturnClean() {
    existUsers.clear();
    existTrains.clear();
    loginUsers.clear();
    waitQueue.clear();
    userOrders.clear();
    stmap.clear();
}

void fake_exit() {
    existUsers.~OuterUniqueUnorderMap();
    existTrains.~OuterUniqueUnorderMap();
    loginUsers.~InnerUniqueUnorderMap();
    waitQueue.~Queue();
    userOrders.~InnerOuterMultiUnorderMap();
    stmap.~InnerOuterMultiUnorderMap();
    InnerOuterMultiUnorderMap<Username, Order, HashString> userOrders("user_orders.dat");
    Queue<Order> waitQueue("wait_queue.dat");
    InnerOuterMultiUnorderMap<StationName, TrainID, HashString> stmap("stationName_trainID.dat");//���Ǽٵġ�
    InnerUniqueUnorderMap<Username, Privilege, HashString> loginUsers;
    OuterUniqueUnorderMap<Username, User, HashString> existUsers("existUsers.dat");
    OuterUniqueUnorderMap<TrainID, Train, HashString> existTrains("existTrains.dat");

}

void sys::clean() {
    sys::noReturnClean();
    Return(0);
}

void cache_putback() {
    //maybe use loginUser.ordernumth to write existUsers.ordernumth
}

void sys::exit() {
    Return("bye");
    log();//FIXME to debug
//    noReturnClean();
//    cache_putback();
//    fake_exit();//FIXME to debug
    std::exit(0);
}

//better ȫ�ڴ��д������bpt�� stationnum���ں��٣���ȫ����ѧϰuserorder��������bpt��ֻ��Ҫ����OuterUniqueMapΪInnerOuterUniqueMap���ɣ�