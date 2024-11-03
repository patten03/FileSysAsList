#include "menu.h"

void menu()
{
    FileSys MainSys;

    // Вывод вступления
    std::cout << "Добро пожаловать в программу FileSysAsList" << std::endl;

    bool quit(false); // переменная выхода из программы
	while (!quit)
    {
        std::cout << "Текущая директория: " << MainSys.fullPath << std::endl;
        std::cout << "Выберите действие:" << std::endl;
        std::vector<std::string> menuPanel{
            "Создать файл",
            "Создать папку",
            "Удалить файл",
            "Удалить папку",
            "Изменить директорию",
            "Вывести все элементы текущей папки",
            "Выйти из программы"
        };
        ask(menuPanel);
        int choice = inputChoice(menuPanel.size());

        switch (choice)
        {
            case 1: // Создание файла
            {
                std::string question = "Введите название создаваемого файла, для выхода введите <0>";
                std::string file = askName(question);
                if (file != "0")
                {
                    // Проверка на уже существование файла со схожим названием
                    if (!MainSys.isExist(file, 'f'))
                    {
                        File* BuffFile = new File;
                        BuffFile->name = file;
                        MainSys.touch(BuffFile);                          
                    }
                    else
                        std::cout << "Невозможно создать файл со схожим названием" << std::endl << std::endl;
                }

                break;
            }
            case 2: // Создание папки
            {
                std::string question = "Введите название создаваемой папки, для выхода введите <0>";
                std::string folder = askName(question);
                if (folder != "0")
                {
                    // Проверка на уже существование папки со схожим названием
                    if (!MainSys.isExist(folder, 'd'))
                    {
                        Folder* BuffFolder = new Folder;
                        BuffFolder->name = folder;
                        MainSys.mkdir(BuffFolder);                           
                    }
                    else
                        std::cout << "Невозможно создать папку со схожим названием" << std::endl << std::endl;                    
                }

                break;
            }
            case 3: // Удаление файла
            {
                std::string question = "Введите название удаляемого файла, для выхода введите <0>";
                std::string file = askName(question);
                if (file != "0")
                {
                    File* BuffFile = static_cast<File*>(MainSys.findElem(file, 'f'));

                    if (BuffFile != nullptr)
                        MainSys.rm(BuffFile);
                    else
                        std::cout << "В текущей директории нет файла с данным названием" << std::endl << std::endl;                   
                }

                break;        
            }
            case 4: // Удаление папки
            {
                std::string question = "Введите название удаляемой папки файла, для выхода введите <0>";
                std::string folder = askName(question);
                if (folder != "0")
                {
                    Folder* BuffFolder = static_cast<Folder*>(MainSys.findElem(folder, 'd'));

                    if (BuffFolder != nullptr)
                        MainSys.rmdir(BuffFolder);
                    else
                        std::cout << "В текущей директории нет папки с данным названием" << std::endl << std::endl;                    
                }

                break;
            }
            case 5: // Изменение директории
            {
                std::cout << MainSys.fullPath << std::endl;
                std::string question = "Введите название папки, в которую хотите войти,\nдля выхода из текущей папки введите <..>,\nдля выхода введите <0>";
                std::string folder = askName(question);
                if (folder != "0")
                    MainSys.cd(folder);

                break;
            }
            case 6: // Вывод элементов текущей директории
            {
                std::cout << "Список элементов в текущей директории:" << std::endl;
                MainSys.ls();
                std::cout << "Для выхода в меню нажмите любую клавишу" << std::endl;
                _getch();
                system("cls");

                break;
            }
            case 7: // Выход из программы
            {
                quit = true;
            }
        }
    }
}

