#include "fileSys.h"

FileSys::FileSys()
{
    this->Beg = nullptr;
    this->Cur = nullptr;

    writeInFolder = false;
}

void FileSys::touch(File* NewFile)
{
    // Случай, когда через cd() была сменена папка и туда требуется добавить элемент
    if(writeInFolder && Cur->type == 'd')
    {
        // Перевод родительского класса в класс потомка для получения доступа к полю innerElemPtr
        DirElem* temp = static_cast<Folder*>(this->Cur)->innerElemPtr;

        // Случай, если папка пуста
        if (temp == nullptr); // Оставляем указатель Cur на своем месте
        // Если в папке уже находятся элементы
        else
        {
            // Нахождение конца списка
            while(temp->nextPtr != nullptr)
                temp = temp->nextPtr;
            Cur = temp;
        }
        
        addElem(NewFile);
    }
    // Случай, когда требуется продолжить запись в текущей папке
    else
        addElem(NewFile);
}

void FileSys::mkdir(Folder* NewFolder)
{
    // Случай, когда через cd() была сменена папка и туда требуется добавить элемент
    if(writeInFolder && Cur->type == 'd')
    {
        // Перевод родительского класса в класс потомка для получения доступа к полю innerElemPtr
        DirElem* temp = static_cast<Folder*>(this->Cur)->innerElemPtr;

        // Случай, если папка пуста
        if (temp == nullptr); // Оставляем указатель Cur на своем месте
        // Если в папке уже находятся элементы
        else
        {
            // Нахождение конца списка
            while(temp->nextPtr != nullptr)
                temp = temp->nextPtr;
            Cur = temp;
        }
        
        addElem(NewFolder);
    }
    // Случай, когда требуется продолжить запись в текущей папке
    else
        addElem(NewFolder);
}

void FileSys::addElem(DirElem* NewElem)
{
    // Случай, когда в файловой системе ничего нет
    if(this->Beg == nullptr)
    {
        this->Beg = NewElem;
        this->Cur = this->Beg;
    }
    // Случай, когда в выбранной папке ничего нет
    if(writeInFolder && Cur->type == 'd')
    {
        static_cast<Folder*>(this->Cur)->innerElemPtr = NewElem;
        NewElem->prevPtr = this->Cur;

        this->Cur = NewElem;
    }
    // Случай, когда в файловой системе уже что-то есть
    else
    {
        this->Cur->nextPtr = NewElem;

        NewElem->prevPtr = this->Cur;
        this->Cur = NewElem;
    }
}

void FileSys::rm(File*& ExFile)
{
    // Случай, когда файл является единственным в списке
    if (ExFile->prevPtr == nullptr && ExFile->nextPtr == nullptr)
    {
        Beg = nullptr;
        delete ExFile;
        ExFile = nullptr;
        
        return;
    }
    // Случай, когда файл является началом списка
    if (ExFile->prevPtr == nullptr)
    {
        Beg = ExFile->nextPtr;
        (ExFile->nextPtr)->prevPtr = nullptr;

        delete ExFile;
        ExFile = nullptr;
        
        return;
    }
    // Случай, когда файл является концом списка
    if (ExFile->nextPtr == nullptr)
    {
        ExFile->prevPtr->nextPtr = nullptr;        

        delete ExFile;
        ExFile = nullptr;
    }
    // Общий случай
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
    // Случай, когда файл является единственным в списке
    if (ExFolder->prevPtr == nullptr && ExFolder->nextPtr == nullptr)
    {
        Beg = nullptr;
        delete ExFolder;
        ExFolder = nullptr;
        
        return;
    }
    // Случай, когда файл является началом списка
    if (ExFolder->prevPtr == nullptr)
    {
        Beg = ExFolder->nextPtr;
        (ExFolder->nextPtr)->prevPtr = nullptr;

        delete ExFolder;
        ExFolder = nullptr;
        
        return;
    }
    // Случай, когда файл является концом списка
    if (ExFolder->nextPtr == nullptr)
    {
        ExFolder->prevPtr->nextPtr = nullptr;        

        delete ExFolder;
        ExFolder = nullptr;
    }
    // Общий случай
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
    this->type = 'n'; // n - None, элемент не является ни папкой, ни файлом
    this->prevPtr = nullptr;
    this->nextPtr = nullptr;
    this->name = "";
    this->date = 0;
}

Folder::Folder()
{
    this->type = 'd'; // d - directory, элемент является папкой
    this->innerElemPtr = nullptr;
}

File::File()
{
    this->type = 'f'; // f - file, элемент является файлом
    this->size = 0;
}