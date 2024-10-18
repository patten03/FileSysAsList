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

        // �������� �� ������ � ����������� ����� � ����������
        bool writeInFolder;

        // ����������� ���� ������ ����� � ���������� bash

        void touch(File* NewFile);          // ������� ����
        void rm(File*& ExFile);             // ������� ����
        void mkdir(Folder* NewFolder);      // ������� �����
        void rmdir(Folder*& ExFolder);      // ������� �����

        void addElem(DirElem* NewElem);     // ���������� ��� touch() � mkdir() 

        void ls();           // ������� ���������� ������� ����������
        // DirElem& cd();    // �������� ������� ���������� 

        FileSys();
};

#endif // FILESYS_M