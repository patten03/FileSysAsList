#include "fileSys.h"

FileSys::FileSys()
{
    this->Beg = nullptr;
    this->Cur = nullptr;
    this->CurDir = nullptr;

    this->fullPath = "C:";

    this->writeInFolder = false;
}

void FileSys::touch(File* NewFile)
{
    addElem(NewFile);
}

void FileSys::mkdir(Folder* NewFolder)
{
    addElem(NewFolder);
}

void FileSys::addElem(DirElem* NewElem)
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
    }

    // ������, ����� � �������� ������� ������ ���
    if(this->Beg == nullptr)
    {
        this->Beg = NewElem;
        this->Cur = this->Beg;
        return;
    }
    // ������, ����� � ��������� ����� ������ ���
    if(writeInFolder && Cur->type == 'd')
    {
        static_cast<Folder*>(this->Cur)->innerElemPtr = NewElem;
        NewElem->prevPtr = this->Cur;

        this->Cur = NewElem;
        writeInFolder = false;
        return;
    }
    // ������, ����� � �������� ������� ��� ���-�� ����
    else
    {
        this->Cur->nextPtr = NewElem;

        NewElem->prevPtr = this->Cur;
        this->Cur = NewElem;
        return;
    }
}

void FileSys::rm(File* ExFile)
{
    delElem(ExFile);
}

void FileSys::rmdir(Folder* ExFolder)
{
    delElem(ExFolder);
}

void FileSys::delElem(DirElem* ExElem)
{
    // ������, ����� ���� �������� ������������ � ������
    if (ExElem->prevPtr == nullptr && ExElem->nextPtr == nullptr)
    {
        Beg = nullptr;
        delete ExElem;
        ExElem = nullptr;

        return;
    }
    // ������, ����� ���� �������� ������� ������
    if (ExElem->prevPtr == nullptr)
    {
        Beg = ExElem->nextPtr;
        (ExElem->nextPtr)->prevPtr = nullptr;

        delete ExElem;
        ExElem = nullptr;
        
        return;
    }
    // ������, ����� ������� �������� ��������� ������ �����
    if (ExElem->prevPtr->getChild() == ExElem)
    {
        Folder* temp = static_cast<Folder*>(ExElem->prevPtr);
        temp->innerElemPtr = ExElem->nextPtr;
        ExElem->nextPtr->prevPtr = temp;

        delete ExElem;
        ExElem = nullptr;

        return;
    }
    // ������, ����� ���� �������� ������ ������
    if (ExElem->nextPtr == nullptr)
    {
        ExElem->prevPtr->nextPtr = nullptr;        

        Cur = ExElem->prevPtr;

        delete ExElem;
        ExElem = nullptr;

        return;
    }
    // ����� ������
    else
    {
        ExElem->prevPtr->nextPtr = ExElem->nextPtr;
        ExElem->nextPtr->prevPtr = ExElem->prevPtr;

        delete ExElem;
        ExElem = nullptr;

        return;
    }
}

void FileSys::ls()
{
    // ��������� �� �� �� ����� ��� � �����
    DirElem* curElem;
    if(this->CurDir != nullptr)
        curElem = CurDir->getChild();
    else
        curElem = this->Beg;
    
    // ����� ��������� � ������, ���� ���������� �����
    if(curElem == nullptr)
        std::cout << "������� ���������� �����" << std::endl;

    // ����������� �� ������� ���������� � ����� ����������
    while(curElem != nullptr)
    {
        std::cout << curElem->type << ' ' <<  curElem->name << std::endl;
        curElem = curElem->nextPtr;
    }
}

void FileSys::cd(std::string input)
{
    // ����� �� ������� �����
    if (input == ".." && CurDir != nullptr)
    {
        fullPath = fullPath.substr(0, fullPath.rfind('\\'));

        // ������, ����� �� ����� � �����
        if (fullPath == "C:")
        {
            CurDir = nullptr;
            Cur = Beg;

            while (Cur->nextPtr != nullptr)
                Cur = Cur->nextPtr;
        }
        else
        {
            std::string outerFolder = fullPath.substr(fullPath.rfind('\\') + 1, fullPath.size() - fullPath.rfind('\\') - 1);
            
            // ����������� ��������� ������� ����� �� ������� �����
            while (CurDir->prevPtr->name != outerFolder)
            {
                CurDir = CurDir->prevPtr;
            }
            CurDir = CurDir->prevPtr;

            Cur = CurDir;
            // ����������� ��������� Cur � ����� ������
            while (Cur->nextPtr != nullptr)
                Cur = Cur->nextPtr;
        }
    }

    // ��������� ������� ���������� � ��������� �����
    if (input != "..")
    {
        DirElem* DirPtr;
        // ������, ����� �� ��������� �� �����
        if (CurDir == nullptr)
            DirPtr = Beg;
        // ������, ����� �� ��������� �� �����
        else
            DirPtr = CurDir->getChild();

        // ����� ��������� ����� � ������� ����������
        while (DirPtr != nullptr && (DirPtr->name != input && DirPtr->type != 'd'))
        {
            DirPtr = DirPtr->nextPtr;
        }

        // ������, ����� ������ �� ���� �������
        if (DirPtr == nullptr)
            return;

        // ������, ����� ���� ������ �����
        if (DirPtr->name == input && DirPtr->type == 'd')
        {
            CurDir = DirPtr;
            fullPath = fullPath + "\\" + DirPtr->name;

            // ������, ����� ��������� ����� �����
            if (CurDir->getChild() == nullptr)
            {
                writeInFolder = true;
                Cur = CurDir;
            }

            // ���������� ����� ������ � ��������� �����
            else
            {
                Cur = CurDir->getChild();
                while (Cur->nextPtr != nullptr)
                {
                    Cur = Cur->nextPtr;
                }
            }
        }
    }
}

DirElem* FileSys::findElem(std::string name, char type)
{
    DirElem* res = nullptr;

    // ��������� �� �� �� ����� ��� � �����
    DirElem* curElem;
    if(this->CurDir != nullptr)
        curElem = CurDir->getChild();
    else
        curElem = this->Beg;
    
    // ����� ��������� � ������, ���� ���������� �����
    if(curElem == nullptr)
        std::cout << "������� ���������� �����" << std::endl;

    // ����������� �� ������� ���������� � ���������� ��������
    while(curElem != nullptr)
    {
        if (curElem->name == name && curElem->type == type)
        {
            res = curElem;
            return res;
        }
        curElem = curElem->nextPtr;
    }
    return res;
}

DirElem* DirElem::getChild()
{
    if (this != nullptr)
    {
        if (this->type == 'd')
        {
            Folder* Buff = static_cast<Folder*>(this);
            return Buff->innerElemPtr;
        }
        else
            return nullptr;  
    }
    else
        return nullptr;
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

Folder::~Folder()
{
    {
        DirElem* curPtr = this->innerElemPtr;
        
        // ���� �������� ���� ��������� ������ �����
        DirElem* buff;
        while (curPtr != nullptr)
        {
            buff = curPtr->nextPtr;
            delete curPtr;
            curPtr = buff;
        }
    }
}