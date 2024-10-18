#ifndef FILESYS_M
#define FILESYS_M

#include <string>
#include <ctime>
#include <vector>
#include <iostream>

class DirElem
{
    public:
        char type;
        std::string name;
        std::time_t date;
        DirElem *prevPtr;
        DirElem *nextPtr;

        DirElem();
};

class Folder: public DirElem
{
    public:
        DirElem *innerElemPtr;

        Folder();
};

class File: public DirElem
{
    public:
        unsigned int size;

        File();
};

class FileSys
{
    public:
        DirElem *Beg;
        DirElem *Cur;

        // Отвечает за запись в последующие папки в директории
        bool writeInFolder;

        // Приведенные ниже методы схожи с команадами bash

        void touch(File* NewFile);          // создать файл
        void rm(File*& ExFile);             // удалить файл
        void mkdir(Folder* NewFolder);      // создать папку
        void rmdir(Folder*& ExFolder);      // удалить папку

        void addElem(DirElem* NewElem);     // подфункция для touch() и mkdir() 

        void ls();           // вернуть содержимое текущей директории
        // DirElem& cd();    // изменить текущую директорию 

        FileSys();
};

#endif // FILESYS_M