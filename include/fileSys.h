#ifndef FILESYS_M
#define FILESYS_M

#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include <iomanip>
#include <sstream>

// ����� ����� ��� ��������� �������� �������
// ��� ����� ��� ����� � �����
class DirElem
{
    public:
        char type;        // ��� ��������
        std::string name; // �������� ��������
        std::string date; // ����� ��������
        DirElem *prevPtr; // ��������� �� ���������� �������
        DirElem *nextPtr; // ��������� �� ���������� �������

        // ��������� ��������� �� �������� � �����
        DirElem* getChild();

        DirElem();
};

// ����� ��� �����
class Folder: public DirElem
{
    public:
        DirElem *innerElemPtr; // ��������� �� ���������� �����

        Folder();
        ~Folder(); // ����������� ���������� � ��������� ����� ���������� � �����
};

// ����� ��� ������
class File: public DirElem
{
    public:
        unsigned int size; // ������ ����� � ����������

        File();
};

// �����, ���������� �� ������ �������� �������
class FileSys
{
    public:
        DirElem *Beg;         // ��������� �� ����� ������ ������
        DirElem *Cur;         // ��������� �� ������� ������� ����������, � �������� ���������� ������
        DirElem *CurDir;      // ��������� �� ������� �����, ��� ������ ����� ls()
        std::string fullPath; // ������ ���� � ��������� �����

        // �������� �� ������ � ����������� ����� � ����������
        bool writeInFolder;

        // ����������� ���� ������ ����� � ���������� bash
        void touch(File* NewFile);         // ������� ����
        void rm(File* ExFile);             // ������� ����
        void mkdir(Folder* NewFolder);     // ������� �����
        void rmdir(Folder* ExFolder);      // ������� �����

        void addElem(DirElem* NewElem);    // ���������� ��� touch() � mkdir() 
        void delElem(DirElem* ExElem);     // ���������� ��� rm() � rmdir

        void ls();                   // ������� ���������� ������� ����������
        void cd(std::string folder); // �������� ������� ���������� 

        DirElem* FileSys::findElem(std::string name, char type); // ����� ������� � ��������� �������� � ������� ����������
        bool isExist(std::string name, char type);               // �������� �� ������������� �������� �� ������ ���������

        void loadFileSys(std::string filename);    // ���������� ������ �������
        void uploadFileSys(std::string filename);  // �������� �������� �������
        // ����������� ������ ��������� ��� ���������� � "��������" ���� �� ����������
        void writeElem(std::fstream& stream, DirElem* Elem, unsigned int level);

        FileSys();
        ~FileSys();
};

std::string inputName(); // ������� ����� �������� ����� ��� ����� � ����������
int inputSize();         // ������� ����� ������� �����
std::string inputDate(); // ������� ����� ������� � ���� �������� ����� ��� �����

#endif // FILESYS_M