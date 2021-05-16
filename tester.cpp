//
// Created by JiamingShan on 2021/4/20.
//
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <string>
#include "DataStructures.h"
#include <functional>

using namespace std;


struct HashInt {
    unsigned long long operator()(const int &str) {
        return str;
    }
};


void testmap() {
    InnerOuterMultiUnorderMap<int, int, HashInt> q("testInOut.dat");
    q.clear();
    for (int j = 1; j <= 4; ++j)
    q.insert({1, j*2});
    q.insert({100, 2});
    q.find(1)->print();
    q.find(100)->print();


//    cout << *(q.find(1,4));

}

void testOutUniquemap() {
    OuterUniqueUnorderMap<int, int, HashInt> q("testInOut.dat");
    int oper;
//    q.clear();
//    for (int j = 1; j <= 3; ++j)
//        q.insert({1, j});
//    cout << q.getItem(q.find(1).first);

}


Queue<int> q("testQ.dat");
void testQ(){
    decltype(q.begin()) it = q.begin();
    int oper;
    while(1)
    {cin >> oper;
        switch (oper) {
            case 0:{
                int x;
                cin >> x;
                q.push_front(x);
                break;
            }
            case 7:{
                int x;
                cin >> x;
                q.push_back(x);
                break;
            }
            case 1:{
                it = q.begin();
                break;
            }
            case 2:{
                ++it;
                break;
            }
            case 3:{
                q.erase(it);
                break;
            }
            case 4:{
                ++it;
                break;
            }
            case 5:{
                for (;it != q.end(); ++ it){
                    cout << *it << " ";
                }
                cout << endl;
                break;
            }
            default: {
                cout << "error";
//                q.~Queue();
                exit(0);
            }
        }}
};

int main() {
    testQ();
}
