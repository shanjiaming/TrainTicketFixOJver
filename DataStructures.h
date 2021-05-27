//
// Created by JiamingShan on 2021/5/2.
//

#ifndef MAIN_CPP_MAPS_H
#define MAIN_CPP_MAPS_H

#include "vector.hpp"
#include "filemanip.h"
#include "BPlusTree.hpp"
#include <functional>
#include "map.hpp"


// Hash 将 Key 映射为一个 unsigned long long
template<class Key, class Value, class Hash>
class OuterUniqueUnorderMap {
private:
    char file[50];
    BPlusTree bpt;
    std::fstream fio;

public:
    OuterUniqueUnorderMap(const char *_datafile) : bpt((std::string("bpt_") + _datafile).c_str()) {
        strcpy(file, _datafile);
        std::fstream fin(_datafile, std::ios::in | std::ios::binary);
        if (!fin.is_open()) {
            std::fstream fout(_datafile, std::ios::out | std::ios::binary);
            fout.close();
        }
        fin.close();

        fio.open(_datafile, std::ios::in | std::ios::out | std::ios::binary);
    };

    ~OuterUniqueUnorderMap() { fio.close(); }

    bool insert(const sjtu::pair<Key, Value> &pair) {
        fio.seekg(0, std::ios::end);
        int pos = fio.tellp();

        bool flag = bpt.insert(Hash()(pair.first), pos, 1);
        if (flag)
            fio.write(reinterpret_cast<const char *>(&(pair.second)), sizeof(pair.second));

        return flag;
    }

    bool erase(const Key &key) {
        return bpt.erase(Hash()(key));
    }

    sjtu::pair<int, bool> find(const Key &key) {
        int f = bpt.query(Hash()(key));
        return {f, ~f ? true : false};
    }

    Value getItem(int pos) {
        Value ans;
        fio.seekg(pos, std::ios::beg);
        fio.read(reinterpret_cast<char *>(&ans), sizeof(ans));
        return ans;
    }

    void setItem(int pos, const Value &value) {
        fio.seekg(pos, std::ios::beg);
        fio.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }

    int get_size() const {
        return bpt.get_size();
    }

    bool empty() const {
        return bpt.get_size() == 0;
    }

    void clear() {
        fio.close();

        std::fstream fout(file, std::ios::out | std::ios::binary);
        fout.close();

        fio.open(file, std::ios::in | std::ios::out | std::ios::binary);

        bpt.clear();
    }
};


template<class Key, class Value, class Hash, int MAXN = 300000>
//Hash是一个模板类名，它实例化后的一个对象例为auto h = Hash<string>(), 这个对象重载了括号，比如可以h(1),然后返回一个size_t
class InnerUniqueUnorderMap {
private:
    using ull = unsigned long long;
    static constexpr int MOD = (MAXN <= 10000) ? 100003 : (MAXN <= 100000) ? 1000003 : (MAXN <= 300000) ? 233347
                                                                                                        : 10000019;
    int tot = 0, size = 0;
    int *last;
    struct edge {
        Value val;
        ull too;
        int pre;
    } *e;
public:

    // MAXN 表示 insert 次数上限，_mod 表示哈希模数，最好是质数
    InnerUniqueUnorderMap() {
        last = new int[MOD];
        e = new edge[MAXN + 1];
    }

    ~InnerUniqueUnorderMap() {
        delete last;
        delete e;
    }

    inline void add(int x, ull y, const Value &val) {
        e[++tot] = (edge) {val, y, last[x]};
        last[x] = tot;
        size++;
    }

    bool insert(const sjtu::pair<Key, Value> &pair) {
        ull hs = Hash()(pair.first);
        int hsmod = hs % MOD;
        for (int i = last[hsmod]; i; i = e[i].pre)
            if (e[i].too == hs)
                return 0;

        add(hsmod, hs, pair.second);
        return 1;
    }

    sjtu::pair<Value, bool> erase(const Key &key) {
        ull hs = Hash()(key);
        int hsmod = hs % MOD;
        for (int i = last[hsmod], *j = &last[hsmod]; i; j = &e[i].pre, i = e[i].pre)
            if (e[i].too == hs) {
                Value ans = e[i].val;
                *j = e[i].pre;
                size--;
                return {ans, 1};
            }

        return {Value(), 0};
    }

    bool empty() const {
        return size == 0;
    }

    int get_size() const {
        return size;
    }

    void clear() {
        tot = size = 0;
        memset(last, 0, sizeof(int) * MOD);
    }

    Value *find(Key key) {
        ull hs = Hash()(key);
        int hsmod = hs % MOD;
        for (int i = last[hsmod]; i; i = e[i].pre)
            if (e[i].too == hs)
                return &e[i].val;

        return nullptr;
    }
};


template<class T>
struct InnerList {
    struct Node {
        T t;
        Node *n = nullptr;
        Node *p = nullptr;

        Node() {}

        Node(T t, Node *n, Node *p) : t(t), n(n), p(p) {}
    };

    Node *root = nullptr;
    Node *rear = nullptr;
    int num = 0;

    int size() const{ return num; }

    InnerList() {
        rear = root = new Node();
    }

    InnerList(const InnerList &) = delete;

    InnerList &operator=(const InnerList &) = delete;

    virtual ~InnerList() {
        while (root) {
            Node *nextroot = root->n;
            delete root;
            root = nextroot;
        }
    }

    void writeToFile(std::fstream &file, Address address, int timesOfSpace = 1) {
        const int bitnum = sizeof(T) * size() * timesOfSpace;
        if (!bitnum) return;
        file.seekg(address);
        char *fixzero = new char[bitnum]{0};
        int i = -int(sizeof(T));
        auto fakeroot = root;
        while (fakeroot) {
            Node *nextroot = fakeroot->n;
            if (nextroot) memcpy(fixzero + (i += sizeof(T)), reinterpret_cast<char *>(&nextroot->t), sizeof(T));
            fakeroot = nextroot;
        }
        file.write(reinterpret_cast<const char *>(fixzero), bitnum);
        delete[] fixzero;
    }


    friend InnerList *mergeList(InnerList *dist, InnerList *src) {
        auto distlast = dist->root;
        while (distlast->n) distlast = distlast->n;//better 随便写个O(100),估计没事
        auto srcroot = src->root;
        distlast->n = srcroot->n;
        if (srcroot->n)srcroot->n->p = distlast;
        dist->num += src->num;
        delete srcroot;
        return dist;
    }


    struct Iterator {
        Node *ptr;

        Iterator() = default;

        Iterator(Node *ptr) : ptr(ptr) {}

        Iterator &operator++() {
            ptr = ptr->n;
            return *this;
        }

        Iterator operator++(int) {//I promise that while using ++ and * and ->, won't use erase and pushfront.
            Iterator ret = *this;
            ++*this;
            return ret;
        }

        T &operator*() const {
            return ptr->t;
        }

        T *operator->() const noexcept {
            return &**this;
        }

        bool operator==(const Iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const Iterator &rhs) const {
            return !(rhs == *this);
        }
    };

    void push_front(T t) {//push to the place where begin() is pointing to.
        root->n = new Node(t, root->n, root);
        if (root->n->n)root->n->n->p = root->n;
        ++num;
        if(num==1) rear = root->n;
    }
    void push_back(T t) {//push to the place where begin() is pointing to.
        rear = rear->n = new Node(t, rear->n, rear);
        ++num;
    }

    void erase(Iterator &iter) {//return Iterator to next element, where ++Iterator go to the same element.
        Node *tmp = iter.ptr;
        if(tmp==rear) rear = rear->p;
        tmp->p->n = tmp->n;
        if (tmp->n)
            tmp->n->p = tmp->p;
        Node *ret = tmp->p;
        delete tmp;
        iter.ptr = ret;
        --num;
    }

    Iterator begin() const {
        return root->n;
    }

    Iterator end() const {
        return nullptr;
    }

    void clear() {
        Node *tmp = root->n;
        while (tmp) {
            Node *nexttmp = tmp->n;
            delete tmp;
            tmp = nexttmp;
        }
        root->n = nullptr;
        rear = root;
        num = 0;
    }

    void print() const {
        for (auto it = begin(); it != end(); ++it) std::cout << *it << " ";
        std::cout << std::endl;
    }
};

template<class Key, class Value, class Hash>
class InnerOuterMultiUnorderMap {//复杂度分析：每次到threshold刷的时候就要访问一次bpt
public:
    FileName fileName;
    std::fstream file;
    static constexpr int THRESHOLD = 40;//memo 1 是debug用的数据，到时候再改回来

    InnerOuterMultiUnorderMap(FileName fileName) : fileName(fileName),
                                                   outmapper((std::string("inout_") + fileName).c_str()) {
        fcreate(fileName);
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
    }

    ~InnerOuterMultiUnorderMap() {
        //better 已偷懒：在一次文件打开后，login的人logout以后也不会再刷回去，只有结束的时候才刷。这会导致小幅的多余内存浪费。
        //...对内部所有人进行刷数据
        for (auto it = keySetter.begin(); it != keySetter.end(); ++it) {
            Data *dataptr = mapper.find(*it);
            putToFileWithoutFear(dataptr);
            outmapper.setItem(outmapper.find(*it).first, *dataptr);
        }
        for (auto it = listSetter.begin(); it != listSetter.end(); ++it) delete (*it);
        file.close();
    }


    void insert(const sjtu::pair<Key, Value> &pair) {
        Data *dataptr = safeGetDataFromInnerMapper(pair.first);
        dataptr->listptr->push_front(pair.second);
        if (dataptr->listptr->size() > THRESHOLD)
            putToFileWithoutFear(dataptr);
    }


    typedef Value *Iterator;

    Iterator find(Key key, int n) {//找到第n新插入的东西。
        InnerList<Value> *valueList = find(key);
        if (n < 1 || n > valueList->size()) return nullptr;
        auto it = valueList->begin();
        while (--n) ++it;
        return &(it.ptr->t);
    }


    void setAddressAndMaxFromOuterMapNumWhenFirstMeetKey(Key key) {
        auto fd = outmapper.find(key);
        assert(fd.second);
        CoreData cd = outmapper.getItem(fd.first);
        //这两个assert auto 都可以被只压成一行
        InnerList<Value> *listptr = new InnerList<Value>();
        listSetter.push_front(listptr);
        keySetter.push_front(key);
        if (cd.address == -2) {
            file.seekg(0, std::ios::end);
            cd.address = file.tellp();
            constexpr char allzero[THRESHOLD * sizeof(Value)] = {0};
            file.write(allzero, THRESHOLD * sizeof(Value));
        }
        mapper.insert({key, Data(listptr, cd.address, cd.maxnum, cd.nownum)});
    };//caution login 的时候注意get一下

    InnerList<Value> *readListFromFile(std::fstream &file, Address address, int &nownum) {
        if (nownum == 0) {
            return new InnerList<Value>();
        }
        file.seekg(address);
        const int bitnum = sizeof(Value) * nownum;
        char *fixzero = new char[bitnum]{0};
        file.read(fixzero, bitnum);
        InnerList<Value> *listptr = new InnerList<Value>();
        for (int i = nownum - 1; ~i; --i) {
            Value t;
            memcpy(reinterpret_cast<char *>(&t), fixzero + i * sizeof(Value), sizeof(Value));
            listptr->push_front(t);
        }
        memset(fixzero, 0, bitnum);
        file.seekg(address);
        file.write(fixzero, bitnum);
        delete[] fixzero;
        nownum = 0;
        return listptr;
    }


    InnerList<Value> *find(Key key) {//caution find 出来的链表会即使在内存中栈空间清除吗？ 不然写了这个也白白地没有用。
        //better 可以取消门槛机制，每次find直接刷掉 但是发现return出来了不知道什么时候才用完，故不行。
        Data * dataptr = safeGetDataFromInnerMapper(key);
        InnerList<Value> *listptr = readListFromFile(file, dataptr->address, dataptr->nownum);
        mergeList(dataptr->listptr, listptr);
        return dataptr->listptr;
    };

    void clear() {
        for (auto it = listSetter.begin(); it != listSetter.end(); ++it)
            delete (*it);
        mapper.clear();
        outmapper.clear();
        file.close();
        file.open(fileName, std::ios::out);
        file.close();
        file.open(fileName, std::ios::out | std::ios::in | std::ios::binary);
    }

//todo 还没可持久化
//better 空间太大刷入外存 接口不变 这跟Queue的原理应当类似
private:
    struct CoreData {
        Address address = -2;
        int maxnum = THRESHOLD;
        int nownum = 0;//指文件中的现有数据量

        CoreData() {};

        CoreData(Address address, int maxnum, int nownum) : address(address), maxnum(maxnum), nownum(nownum) {}
    };

    struct Data : CoreData {
        InnerList<Value> *listptr = nullptr;

        Data() {}

        Data(InnerList<Value> *listptr, Address address, int maxnum, int nownum) : listptr(listptr),
                                                                                   CoreData(address, maxnum, nownum) {}
    };

    InnerUniqueUnorderMap<Key, Data, Hash> mapper;
    OuterUniqueUnorderMap<Key, CoreData, Hash> outmapper;
    InnerList<InnerList<Value> *> listSetter;
    InnerList<Key> keySetter;


    Data *safeGetDataFromInnerMapper(Key key) {
        Data *dataptr = mapper.find(key);
        if (dataptr)
            return dataptr;
        auto iterpair = outmapper.find(key);
        if (!iterpair.second) {
            outmapper.insert({key, CoreData()});
        }
        setAddressAndMaxFromOuterMapNumWhenFirstMeetKey(key);
        dataptr = mapper.find(key);
        return dataptr;
    }

    void putToFileWithoutFear(Data *&dataptr) {
        InnerList<Value> *&valueList = dataptr->listptr;
        mergeList(valueList, readListFromFile(file, dataptr->address, dataptr->nownum));
        dataptr->nownum = valueList->size();
        if (dataptr->maxnum > valueList->size()) {//刷数据也在find里刷了。之后注意一下。
            valueList->writeToFile(file, dataptr->address);
        } else {
            file.seekg(0, std::ios::end);
            dataptr->maxnum *= 2;
            dataptr->address = file.tellg();
            valueList->writeToFile(file, dataptr->address, 2);//fixme 没把文件的东西拿出来
        }
        valueList->clear();
    }

};



//recommend to implement by a list
//singleton pattern


template<class T>
struct Queue : InnerList<T> {
    FileName fileName;
    std::fstream file;

    using Node = typename InnerList<T>::Node;
    using Iterator = typename InnerList<T>::Iterator;

    Queue(FileName fileName) : fileName(fileName) {//better 构建队列需不需要100w次文件读写？不过这显然不是瓶颈，但是可玩。
        fcreate(fileName);

        file.open(fileName, std::ios::in | std::ios::binary);
        assert(file);
        file.seekg(0, std::ios::end);
        file.tellg();
        const int bitnum = file.tellg();
        if (bitnum != 0) {
            file.seekg(0, std::ios::beg);
            char *fixzero = new char[bitnum]{0};
            file.read(fixzero, bitnum);
            int lastNotZero = bitnum - 1;

            for (int i = lastNotZero / sizeof(T); ~i; --i) {
                T t;
                memcpy(reinterpret_cast<char *>(&t), fixzero + i * sizeof(T), sizeof(T));
                this->push_front(t);
            }
            delete[] fixzero;

        }
        file.close();
    }

    virtual ~Queue() override {
        file.open(fileName, std::ios::out);
        file.close();
        file.open(fileName, std::ios::in |std::ios::out | std::ios::binary);
        InnerList<T>::writeToFile(file, 0);
        file.close();
    }
};


struct HashString {
    unsigned long long operator()(const std::string &str) {
        int ans = 0;
        for (int i = 0; str[i] != '\0'; ++i) ans = ans * 19260817 + str[i];
        return ans;
    }
};

//memo 关于写法：queue整万划分，queue基本在内存中进行，直到内存达万划入一个整万块。在refund时，一个个把整万块拿出来在栈空间检查，然后如果某个整万块退成了，把那个块在内存里改掉后在外存里全部覆写一遍。

//如果队列够短，是不用做整万划分的。

#endif //MAIN_CPP_MAPS_H