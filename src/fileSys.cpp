#include "fileSys.h"

FileSys::FileSys()
{
    this->Beg = nullptr;
    this->Cur = nullptr;

    writeInFolder = false;
}

void FileSys::touch(File* NewFile)
{
    // ������, ����� ����� cd() ���� ������� ����� � ���� ��������� �������� �������
    if(writeInFolder && Cur->type == 'd')
    {
        // ������� ������������� ������ � ����� ������� ��� ��������� ������� � ���� innerElemPtr
        DirElem* temp = static_cast<Folder*>(this->Cur)->innerElemPtr;

        // ������, ���� ����� �����
        if (temp == nullptr); // ��������� ��������� Cur �� ����� �����
        // ���� � ����� ��� ��������� ��������
        else
        {
            // ���������� ����� ������
            while(temp->nextPtr != nullptr)
                temp = temp->nextPtr;
            Cur = temp;
        }
        
        addElem(NewFile);
    }
    // ������, ����� ��������� ���������� ������ � ������� �����
    else
        addElem(NewFile);
}

void FileSys::mkdir(Folder* NewFolder)
{
    // ������, ����� ����� cd() ���� ������� ����� � ���� ��������� �������� �������
    if(writeInFolder && Cur->type == 'd')
    {
        // ������� ������������� ������ � ����� ������� ��� ��������� ������� � ���� innerElemPtr
        DirElem* temp = static_cast<Folder*>(this->Cur)->innerElemPtr;

        // ������, ���� ����� �����
        if (temp == nullptr); // ��������� ��������� Cur �� ����� �����
        // ���� � ����� ��� ��������� ��������
        else
        {
            // ���������� ����� ������
            while(temp->nextPtr != nullptr)
                temp = temp->nextPtr;
            Cur = temp;
        }
        
        addElem(NewFolder);
    }
    // ������, ����� ��������� ���������� ������ � ������� �����
    else
        addElem(NewFolder);
}

void FileSys::addElem(DirElem* NewElem)
{
    // ������, ����� � �������� ������� ������ ���
    if(this->Beg == nullptr)
    {
        this->Beg = NewElem;
        this->Cur = this->Beg;
    }
    // ������, ����� � ��������� ����� ������ ���
    if(writeInFolder && Cur->type == 'd')
    {
        static_cast<Folder*>(this->Cur)->innerElemPtr = NewElem;
        NewElem->prevPtr = this->Cur;

        this->Cur = NewElem;
    }
    // ������, ����� � �������� ������� ��� ���-�� ����
    else
    {
        this->Cur->nextPtr = NewElem;

        NewElem->prevPtr = this->Cur;
        this->Cur = NewElem;
    }
}

void FileSys::rm(File*& ExFile)
{
    // ������, ����� ���� �������� ������������ � ������
    if (ExFile->prevPtr == nullptr && ExFile->nextPtr == nullptr)
    {
        Beg = nullptr;
        delete ExFile;
        ExFile = nullptr;
        
        return;
    }
    // ������, ����� ���� �������� ������� ������
    if (ExFile->prevPtr == nullptr)
    {
        Beg = ExFile->nextPtr;
        (ExFile->nextPtr)->prevPtr = nullptr;

        delete ExFile;
        ExFile = nullptr;
        
        return;
    }
    // ������, ����� ���� �������� ������ ������
    if (ExFile->nextPtr == nullptr)
    {
        ExFile->prevPtr->nextPtr = nullptr;        

        delete ExFile;
        ExFile = nullptr;
    }
    // ����� ������
    else
    {
        ExFile->prevPtr->nextPtr = ExFile->nextPtr;
        ExFile->nextPtr->prevPtr = ExFile->prevPtr;

        delete ExFile;
        ExFile = nullptr;
    }
}

void FileSys::rmdir(Folder*& ExFolder)
{
    // ������, ����� ���� �������� ������������ � ������
    if (ExFolder->prevPtr == nullptr && ExFolder->nextPtr == nullptr)
    {
        Beg = nullptr;
        delete ExFolder;
        ExFolder = nullptr;
        
        return;
    }
    // ������, ����� ���� �������� ������� ������
    if (ExFolder->prevPtr == nullptr)
    {
        Beg = ExFolder->nextPtr;
        (ExFolder->nextPtr)->prevPtr = nullptr;

        delete ExFolder;
        ExFolder = nullptr;
        
        return;
    }
    // ������, ����� ���� �������� ������ ������
    if (ExFolder->nextPtr == nullptr)
    {
        ExFolder->prevPtr->nextPtr = nullptr;        

        delete ExFolder;
        ExFolder = nullptr;
    }
    // ����� ������
    else
    {
        ExFolder->prevPtr->nextPtr = ExFolder->nextPtr;
        ExFolder->nextPtr->prevPtr = ExFolder->prevPtr;

        delete ExFolder;
        ExFolder = nullptr;
    }
}

void FileSys::ls()
{
    DirElem* curElem = this->Beg;
    while(curElem != nullptr)
    {
        std::cout << curElem->type << ' ' <<  curElem->name << std::endl;
        curElem = curElem->nextPtr;
    }
}

DirElem::DirElem()
{
    this->type = 'n'; // n - None, ������� �� �������� �� ������, �� ������
    this->prevPtr = nullptr;
    this->nextPtr = nullptr;
    this->name = "";
    this->date = 0;
}

Folder::Folder()
{
    this->type = 'd'; // d - directory, ������� �������� ������
    this->innerElemPtr = nullptr;
}

File::File()
{
    this->type = 'f'; // f - file, ������� �������� ������
    this->size = 0;
}