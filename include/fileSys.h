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

// Общий класс для элементов файловой системы
// для таких как файлы и папки
class DirElem
{
    public:
        char type;        // тип элемента
        std::string name; // название элемента
        std::string date; // время создания
        DirElem *prevPtr; // указатель на предыдущий элемент
        DirElem *nextPtr; // указатель на следующщий элемент

        // Получение указателя на элементы в папке
        DirElem* getChild();

        DirElem();
};

// Класс для папок
class Folder: public DirElem
{
    public:
        DirElem *innerElemPtr; // указатель на содержимое папки

        Folder();
        ~Folder(); // рекурсивный деструктор с удалением всего содежимого в папке
};

// Класс для файлов
class File: public DirElem
{
    public:
        unsigned int size; // размер файла в килобайтах

        File();
};

// Класс, отвечающий за работу файловой системы
class FileSys
{
    public:
        DirElem *Beg;         // указатель на общее начало списка
        DirElem *Cur;         // указатель на текущий элемент директории, с которого начинается запись
        DirElem *CurDir;      // указатель на текущую папка, для вывода через ls()
        std::string fullPath; // полный путь к выбранной папке

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
        bool isExist(std::string name, char type);               // проверка на существование элемента со схожим названием

        void loadFileSys(std::string filename);    // сохранение файлов системы
        void uploadFileSys(std::string filename);  // загрузка файловой системы
        // рекурсивная запись элементов для сохранения в "реальный" файл на компьютере
        void writeElem(std::fstream& stream, DirElem* Elem, unsigned int level);

        FileSys();
        ~FileSys();
};

std::string inputName(); // функция ввода названия файла или папки с проверками
int inputSize();         // функция ввода размера файла
std::string inputDate(); // функция ввода времени и даты создания файла или папки

#endif // FILESYS_M