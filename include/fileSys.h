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

        DirElem* getChild();

        DirElem();
};

class Folder: public DirElem
{
    public:
        DirElem *innerElemPtr;

        Folder();
        ~Folder();
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
        DirElem *Beg;         // Общее начало списка
        DirElem *Cur;         // Текущий элемент директории, с которого начинается запись
        DirElem *CurDir;      // Текущая папка, для вывода через ls()
        std::string fullPath; // Полный путь к выбранной папке

        // Отвечает за запись в последующие папки в директории
        bool writeInFolder;

        // Приведенные ниже методы схожи с команадами bash

        void touch(File* NewFile);         // создать файл
        void rm(File* ExFile);             // удалить файл
        void mkdir(Folder* NewFolder);     // создать папку
        void rmdir(Folder* ExFolder);      // удалить папку

        void addElem(DirElem* NewElem);    // подфункция для touch() и mkdir() 
        void delElem(DirElem* ExElem);     // подфункция для rm() и rmdir

        void ls();                   // вернуть содержимое текущей директории
        void cd(std::string folder); // изменить текущую директорию 

        DirElem* FileSys::findElem(std::string name, char type); // найти элемент с введенным название в текущей директории

        FileSys();
};

#endif // FILESYS_M