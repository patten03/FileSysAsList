#include "fileSys.h"

// Конструктор файловой системы
FileSys::FileSys()
{
    // Определение всех указателей для начальной работы
    this->Beg = nullptr;
    this->Cur = nullptr;
    this->CurDir = nullptr;

    // Определение начала файловой системы
    this->fullPath = "C:";

    this->writeInFolder = false;
}

// Деструктор файловой системы
FileSys::~FileSys()
{
    DirElem* curElem = Beg;
    if (Beg != nullptr)
    {
        // Цикл по удаление всех элементов файловой системы
        while (curElem->nextPtr != nullptr)
        {
            DirElem* buff = curElem;
            curElem = curElem->nextPtr;
            delete buff;
        }
        delete curElem;            
    }
}

// Функция создания файла
void FileSys::touch(File* NewFile)
{
    addElem(NewFile);
}

// Функция создания папки
void FileSys::mkdir(Folder* NewFolder)
{
    addElem(NewFolder);
}

// Функция добавления элемента к файловой системе
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

// Функция удаления файла
void FileSys::rm(File* ExFile)
{
    delElem(ExFile);
}

// Функция удаления папки
void FileSys::rmdir(Folder* ExFolder)
{
    delElem(ExFolder);
}

// Функция удаления элемента из файловой системы
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

// Функция вывода всех элементов файлов системы в текущей папке
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
    // Вывод верхней строки таблицы
    else
        std::cout
            << std::setw(12) << std::left << "Тип"
            << std::setw(28) << std::left << "Дата создания"
            << std::setw(22) << std::right << "Размер в kb" << "  "
            << std::left << "Название" << std::endl;
    // Прохождение по текущей директории и вывод информации
    while(curElem != nullptr)
    {
        // Вывод папки в консоль
        if (curElem->type == 'd')
            std::cout
                << std::setw(12) << std::left << "папка"
                << std::setw(28) << std::left <<  curElem->date
                << std::setw(22) << std::right << "" << "  "
                << std::left << curElem->name << std::endl;
        // Вывод файла в консоль
        if (curElem->type == 'f')
            std::cout
                << std::setw(12) << std::left << "файл"
                << std::setw(28) << std::left <<  curElem->date
                << std::setw(22) << std::right << static_cast<File*>(curElem)->size << "  "
                << std::left << curElem->name << std::endl;

        curElem = curElem->nextPtr;
    }
}

// Функция смены директории в файловой системе
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
        // Общий случай
        else
        {
            // Получение названия текущей папки
            std::string outerFolder = fullPath.substr(fullPath.rfind('\\') + 1, fullPath.size() - fullPath.rfind('\\') - 1);
            
            // Перемещение указателя текущей папки во внешнюю папку
            while (CurDir->prevPtr->name != outerFolder)
            {
                CurDir = CurDir->prevPtr;
            }
            CurDir = CurDir->prevPtr;


            // Перемещение указателя Cur в конец списка
            // Проверка на то, является ли текущая папка пустой
            if (CurDir->getChild() != nullptr)
            {
                Cur = CurDir->getChild();
                while (Cur->nextPtr != nullptr)
                    Cur = Cur->nextPtr;                
            }
            // Случай, когда папка является пустой
            else
            {
                Cur = CurDir;
                writeInFolder = true;
            }
            
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
        while (DirPtr != nullptr && (DirPtr->name != input || DirPtr->type != 'd'))
        {
            DirPtr = DirPtr->nextPtr;
        }

        // Случай, когда ничего не было найдено
        if (DirPtr == nullptr)
            return;

        // Случай, когда была найдена папка
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

// Функция нахождения элемента в текущей директории
DirElem* FileSys::findElem(std::string name, char type)
{
    DirElem* res = nullptr;

    // Находимся ли мы на диске или в папке
    DirElem* curElem;
    if(this->CurDir != nullptr)
        curElem = CurDir->getChild();
    else
        curElem = this->Beg;

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

// Функция проверки на существование элемента в текущей директории
bool FileSys::isExist(std::string name, char type)
{
    DirElem* buff = findElem(name, type);
    if (buff == nullptr)
        return false;
    else
        return true;
}

// Функция загрузки структуры из "реального" файла
void FileSys::loadFileSys(std::string filename)
{  
    std::fstream in;
    in.open(filename, std::ios_base::in);

    unsigned int prevLevel = 0, curLevel = 0; // уровни вложенности папок в файловой системе
    while (!in.eof())
    {
        std::string line;
        std::getline(in, line);

        // Проверка на то, есть ли в конце файла пустая строка
        if (line.empty())
            break;

        // Нахождение уровня текущего элемента файловой системы
        curLevel = 0;
        for(char c: line)
            if (c == '\t')
                curLevel++;

        // Очистка строки от отступов
        line.erase(0, curLevel);

        // Получения данных об элементе
        std::stringstream ss(line);
        std::string buff;

        char type;
        std::string name;
        std::string date;
        int size;

        std::getline(ss, buff, ';');
        type = buff[0];
        std::getline(ss, buff, ';');
        name = buff;
        std::getline(ss, buff, ';');
        date = buff;
        if (type == 'f')
        {
            std::getline(ss, buff, ';');
            size = std::stoi(buff);
        }

        // Случай, когда в папке находятся элементы
        if (curLevel > prevLevel)
            this->cd(this->Cur->name); // переход в последнюю добавленную папку
        // Случай, когда элементов в текущей папке нет и элемент находится с папкой в одной общей папке
        if (curLevel < prevLevel)
            this->cd("..");
        
        // Добавление элемента в структуру
        if (type == 'd')
        {
            Folder* Buff = new Folder;
            Buff->name = name;
            Buff->type = type;
            Buff->date = date;
            this->addElem(Buff);
        }
        else
        {
            File* Buff = new File;
            Buff->name = name;
            Buff->type = type;
            Buff->date = date;
            Buff->size = size;
            this->addElem(Buff);
        }

        prevLevel = curLevel;
    }

    in.close();
}

// Функция сохранение структуры в "реальный" файл
void FileSys::uploadFileSys(std::string filename)
{
    std::fstream out;
    out.open(filename, std::ios_base::out);

    // Рекурсивная запись элементов в файл
    writeElem(out, Beg, 0);

    out.close();
}

// Рекурсивная запись элементов в файл
void FileSys::writeElem(std::fstream& stream, DirElem* Elem, unsigned int level)
{
    // Ввод элемента в файл
    if (Elem->type == 'f')
        stream
            << std::string(level, '\t')
            << Elem->type << ';'
            << Elem->name << ';'
            << Elem->date << ';'
            << static_cast<File*>(Elem)->size
            << std::endl;

    if (Elem->type == 'd')
        stream
            << std::string(level, '\t')
            << Elem->type << ';'
            << Elem->name << ';'
            << Elem->date
            << std::endl;

    // Ввод элементов из папки с отступом
    if (Elem->getChild() != nullptr)
        writeElem(stream, Elem->getChild(), level + 1);

    // Продолжение ввода элементов, если таковые есть
    if (Elem->nextPtr != nullptr)
        writeElem(stream, Elem->nextPtr, level);
}

// Функция получения указателя на содержимое папки
DirElem* DirElem::getChild()
{
    // Проверка указателя на пустоту
    if (this != nullptr)
    {
        // Проверка на тип элемента
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

// Конструктор элемента
DirElem::DirElem()
{
    // Обнуление всех полей
    this->type = 'n'; // n - none, элемент не является ни папкой, ни файлом
    this->prevPtr = nullptr;
    this->nextPtr = nullptr;
    this->name = "";
    this->date = "";
}

Folder::Folder()
{
    this->type = 'd'; // d - directory, элемент является папкой
    this->innerElemPtr = nullptr;
}

// Конструктор файла
File::File()
{
    this->type = 'f'; // f - file, элемент является файлом
    this->size = 0;   // обнуление поля размера файла
}

// Деструктор папки с рекурсивным удалением содержимого папки
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

// Функция ввода названия элемента
std::string inputName()
{
    std::string res = "";

    // Regex для вводимого названия, который содержит хотя бы один запрещенный символ 
    std::regex check{R"([^\\\/:*?<>"|]+)", std::regex::collate};
    std::cout << "Введите название файла/папки, для выхода введите <0>" << std::endl;

    bool approved(false);
    while (!approved)
    {
        std::cout << ">>";
        std::getline(std::cin, res);

        std::smatch sm;
        // Проверка на наличие запрещенных символов
        if (std::regex_match(res, sm, check))
            approved = true;
        else
            std::cout << "Название файла/папки не может содержать символы \\/:*?<>\"| и не может быть пустым" << std::endl;
    }

    return res;
}

// Функция ввода размера файла в килобайтах
int inputSize()
{
    std::string res = "";

    // Regex для вводимой строки, которая должна содержать только цифры или символ вывода
    std::regex check{R"(([0-9]+)|(-))", std::regex::collate};
    std::cout << "Введите размер файла в килобайтах, для выхода введите <->" << std::endl;

    bool approved(false);
    while (!approved)
    {
        std::cout << ">>";
        std::getline(std::cin, res);

        std::smatch sm;
        // Проверка на корректность ввода
        if (std::regex_match(res, sm, check))
            approved = true;
        else
            std::cout << "Число не может содержать символы кроме 0-9" << std::endl;
    }

    // Проверка на ввод символа выхода
    if (res == "-")
        return -1;
    else
        return std::stoi(res);
}

// Функция ввода даты и времени создания файла
std::string inputDate()
{
    std::string time = "";
    std::string date = "";

    // Regex для ограничения временного формата от 00:00 до 23:59
    std::regex check{R"(([01][0-9]|2[0-3]):([0-5][0-9])|(0))", std::regex::collate};

    // Ввод времени создания
    std::cout << "Введите время создания в формате hh:mm, для выхода введите <0>" << std::endl;
    bool approved(false);
    while (!approved)
    {
        std::cout << ">>";
        std::getline(std::cin, time);

        std::smatch sm;
        // Проверка на корректность ввода времени
        if (std::regex_match(time, sm, check))
            approved = true;
        else
            std::cout << "Неверный временной формат" << std::endl;
    }

    // Выход из ввода по желанию пользователя
    if (time == "0")
        return "0";

    // Regex для ограничения временного формата от 01.01.0001 до 31.12.9999
    check = std::regex{R"(((0[1-9]|[12][0-9]|3[01])\.(0[1-9]|1[0-2])\.([0-9]{3}[1-9]|[0-9]{2}[1-9][0-9]))|0)", std::regex::collate};

    // Ввод даты создания
    std::cout << "Введите дату создания в формате DD.MM.YYYY,\nдата создания не должна быть ранее 1 года н.э. и не позже 9999 года,\nдля выхода введите <0>" << std::endl;
    approved = false;
    while (!approved)
    {
        std::cout << ">>";
        std::getline(std::cin, date);

        // Выход из ввода по желанию пользователя
        if (date == "0")
            return "0";

        std::smatch sm;
        // Первичная проверка на корректность ввода
        if (std::regex_match(date, sm, check))
        {
            // Проверка на существование даты, без 29.02 в не високосный год и подобные ошибки
            std::tm tm{};
            tm.tm_mday = std::stoi(date.substr(0,2));
            tm.tm_mon = std::stoi(date.substr(3,2)) - 1;
            tm.tm_year = std::stoi(date.substr(6,4)) - 1900;
            // Функция mktime() сдвигает дату, если она была некорректно введена на подобии 29.02.2003->01.03.2003
            std::mktime(&tm);

            // Проверка на существование даты
            if (tm.tm_mday != std::stoi(date.substr(0, 2)))
                std::cout << "Введенной даты не существует" << std::endl;
            else
                approved = true;
        }
        else
            std::cout << "Неверный временной формат" << std::endl;
    }
    
    return time + " " + date;
}