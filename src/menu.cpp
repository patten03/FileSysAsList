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
            "Загрузить сохраненую структуру",
            "Сохранить созданную структуру",
            "Выйти из программы"
        };
        ask(menuPanel);
        int choice = inputChoice(menuPanel.size());

        switch (choice)
        {
            case 1: // Создание файла
            {
                std::string file;
                int size;
                std::string date;

                file = inputName();
                // Выход по желанию пользователя
                if (file == "0")
                    {system("cls"); break;}
                // Проверка на уже существование файла со схожим названием
                if (MainSys.isExist(file, 'f'))
                {
                    std::cout << "Невозможно создать файл со схожим названием" << std::endl << std::endl;
                    break;
                }
                size = inputSize();
                // Выход по желанию пользователя
                if (size == -1)
                    {system("cls"); break;}
                date = inputDate();
                // Выход по желанию пользователя
                if (date == "0")
                    {system("cls"); break;}
                
                File* BuffFile = new File;
                BuffFile->name = file;
                BuffFile->date = date;
                BuffFile->size = size;
                MainSys.touch(BuffFile);   

                system("cls");
                break;
            }
            case 2: // Создание папки
            {
                std::string folder;
                std::string date;

                folder = inputName();
                // Выход по желанию пользователя
                if (folder == "0")
                    {system("cls"); break;}
                // Проверка на уже существование файла со схожим названием
                if (MainSys.isExist(folder, 'd'))
                {
                    std::cout << "Невозможно создать папку со схожим названием" << std::endl << std::endl;
                    break;
                }
                date = inputDate();
                // Выход по желанию пользователя
                if (date == "0")
                    {system("cls"); break;}
                
                Folder* BuffFolder = new Folder;
                BuffFolder->name = folder;
                BuffFolder->date = date;
                MainSys.mkdir(BuffFolder);   

                system("cls");
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
            case 7: // Загрузка сохраненной структуры
            {
                // Проверка на то, является ли текущая структура пустой
                if (MainSys.Beg != nullptr)
                {
                    std::cout << "Вы уверены, что хотите загрузить структуру, не сохранив текущую?" << std::endl;
                    std::vector<std::string> question = {
                        "Да",
                        "Нет"
                    };
                    ask(question);
                    int ans = inputChoice(question.size());

                    if (ans == 2)
                        break;
                }
                
                // Выбор файловой системы
                std::string filename = findFile("Выберите файл, содержащий структуру файловой системы:", receiveFSAL);
                if (filename != "0")
                {
                    MainSys.~FileSys();
                    MainSys = FileSys();
                    MainSys.loadFileSys(filename);                   
                }

                break;
            }
            case 8: // Сохранение файловой системы
            {
                // Проверка на существование структры
                if (MainSys.Beg != nullptr)
                {
                    std::string filename = askName("Введите желаемое название для сохраняемого файла со структурой:");
                    if (filename != "0")
                    {
                        filename = space2underscore(filename) + "_" + currentTime() + ".fsal";
                        MainSys.uploadFileSys(filename);
                        std::cout << "Структура была сохранена под названием " << filename << std::endl << std::endl;;
                    }
                }

                else
                    std::cout << "Невозможно сохранить пустую структуру" << std::endl << std::endl;

                break;
            }
            case 9: // Выход из программы
            {
                quit = true;
                break;
            }
            default: break;
        }
    }
}

bool receiveFSAL(const std::string& filename)
{
	int dotPos = filename.rfind(".fsal");
	if (dotPos != std::string::npos)
	{
		std::string extension = filename.substr(dotPos, filename.size() - 1);
		return (extension == ".fsal");
	}
	else
		return false; // если в названии файла с расширением точки нет
}