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
    }

    // Случай, когда в файловой системе ничего нет
    if(this->Beg == nullptr)
    {
        this->Beg = NewElem;
        this->Cur = this->Beg;
        return;
    }
    // Случай, когда в выбранной папке ничего нет
    if(writeInFolder && Cur->type == 'd')
    {
        static_cast<Folder*>(this->Cur)->innerElemPtr = NewElem;
        NewElem->prevPtr = this->Cur;

        this->Cur = NewElem;
        writeInFolder = false;
        return;
    }
    // Случай, когда в файловой системе уже что-то есть
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
    // Случай, когда файл является единственным в списке
    if (ExElem->prevPtr == nullptr && ExElem->nextPtr == nullptr)
    {
        Beg = nullptr;
        delete ExElem;
        ExElem = nullptr;

        return;
    }
    // Случай, когда файл является началом списка
    if (ExElem->prevPtr == nullptr)
    {
        Beg = ExElem->nextPtr;
        (ExElem->nextPtr)->prevPtr = nullptr;

        delete ExElem;
        ExElem = nullptr;
        
        return;
    }
    // Случай, когда элемент является начальным внутри папки
    if (ExElem->prevPtr->getChild() == ExElem)
    {
        Folder* temp = static_cast<Folder*>(ExElem->prevPtr);
        temp->innerElemPtr = ExElem->nextPtr;
        ExElem->nextPtr->prevPtr = temp;

        delete ExElem;
        ExElem = nullptr;

        return;
    }
    // Случай, когда файл является концом списка
    if (ExElem->nextPtr == nullptr)
    {
        ExElem->prevPtr->nextPtr = nullptr;        

        Cur = ExElem->prevPtr;

        delete ExElem;
        ExElem = nullptr;

        return;
    }
    // Общий случай
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
    // Находимся ли мы на диске или в папке
    DirElem* curElem;
    if(this->CurDir != nullptr)
        curElem = CurDir->getChild();
    else
        curElem = this->Beg;
    
    // Вывод сообщения в случае, если директория пуста
    if(curElem == nullptr)
        std::cout << "Текущая директория пуста" << std::endl;

    // Прохождение по текущей директории и вывод информации
    while(curElem != nullptr)
    {
        std::cout << curElem->type << ' ' <<  curElem->name << std::endl;
        curElem = curElem->nextPtr;
    }
}

void FileSys::cd(std::string input)
{
    // Выход из текущей папки
    if (input == ".." && CurDir != nullptr)
    {
        fullPath = fullPath.substr(0, fullPath.rfind('\\'));

        // Случай, когда мы вышли к диску
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
            
            // Перемещение указателя текущей папки во внешнюю папку
            while (CurDir->prevPtr->name != outerFolder)
            {
                CurDir = CurDir->prevPtr;
            }
            CurDir = CurDir->prevPtr;

            Cur = CurDir;
            // Перемещение указателя Cur в конец списка
            while (Cur->nextPtr != nullptr)
                Cur = Cur->nextPtr;
        }
    }

    // Изменение текущей директории в введенную папку
    if (input != "..")
    {
        DirElem* DirPtr;
        // Случай, когда мы находимся на диске
        if (CurDir == nullptr)
            DirPtr = Beg;
        // Случай, когда мы находимся на диске
        else
            DirPtr = CurDir->getChild();

        // Поиск введенной папки в текущей директории
        while (DirPtr != nullptr && (DirPtr->name != input && DirPtr->type != 'd'))
        {
            DirPtr = DirPtr->nextPtr;
        }

        // Случай, когда ничего не было найдено
        if (DirPtr == nullptr)
            return;

        // Случай, когда была найден папка
        if (DirPtr->name == input && DirPtr->type == 'd')
        {
            CurDir = DirPtr;
            fullPath = fullPath + "\\" + DirPtr->name;

            // Случай, когда выбранная папка пуста
            if (CurDir->getChild() == nullptr)
            {
                writeInFolder = true;
                Cur = CurDir;
            }

            // Нахождение конца списка в выбранной папке
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

    // Находимся ли мы на диске или в папке
    DirElem* curElem;
    if(this->CurDir != nullptr)
        curElem = CurDir->getChild();
    else
        curElem = this->Beg;
    
    // Вывод сообщения в случае, если директория пуста
    if(curElem == nullptr)
        std::cout << "Текущая директория пуста" << std::endl;

    // Прохождение по текущей директории и нахождение элемента
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

Folder::~Folder()
{
    {
        DirElem* curPtr = this->innerElemPtr;
        
        // цикл удаление всех элементов внутри папки
        DirElem* buff;
        while (curPtr != nullptr)
        {
            buff = curPtr->nextPtr;
            delete curPtr;
            curPtr = buff;
        }
    }
}