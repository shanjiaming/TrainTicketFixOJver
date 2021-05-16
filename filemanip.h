//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_FILEMANIP_H
#define CODE_FILEMANIP_H

#include <string>
#include <fstream>
#include <cassert>

typedef std::string FileName;
typedef int Address;

inline void fcreate(FileName fn){//OK to not to use ios::binary
    std::ifstream fin(fn);
    if (fin) {
        fin.close();
        return;
    }
    std::ofstream fooo(fn);
    assert(fooo);
    fooo.close();
}

inline void fclear(FileName fn){
    std::ofstream fileout(fn,std::ios::trunc);
    if(!fileout){
        std::cout << "Create file failure...\n";
        exit(0);
    }
    fileout.close();
}

#define openfile file.open(fileName, ios::in | ios::out | ios::binary);assert(file);

#define closefile file.close();

template <class T>
void fwrite(std::ostream &_file, const T &t) {
    _file.write(reinterpret_cast<const char *>(&t), sizeof(t));
}

template <class T>
void fread(std::istream &_file, T &t) {
    _file.read(reinterpret_cast<char *>(&t), sizeof(t));
}

#endif //CODE_FILEMANIP_H
