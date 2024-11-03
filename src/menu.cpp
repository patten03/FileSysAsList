#include "menu.h"

void menu()
{
    FileSys MainSys;

    // ����� ����������
    std::cout << "����� ���������� � ��������� FileSysAsList" << std::endl;

    bool quit(false); // ���������� ������ �� ���������
	while (!quit)
    {
        std::cout << "������� ����������: " << MainSys.fullPath << std::endl;
        std::cout << "�������� ��������:" << std::endl;
        std::vector<std::string> menuPanel{
            "������� ����",
            "������� �����",
            "������� ����",
            "������� �����",
            "�������� ����������",
            "������� ��� �������� ������� �����",
            "����� �� ���������"
        };
        ask(menuPanel);
        int choice = inputChoice(menuPanel.size());

        switch (choice)
        {
            case 1: // �������� �����
            {
                std::string question = "������� �������� ������������ �����, ��� ������ ������� <0>";
                std::string file = askName(question);
                if (file != "0")
                {
                    // �������� �� ��� ������������� ����� �� ������ ���������
                    if (!MainSys.isExist(file, 'f'))
                    {
                        File* BuffFile = new File;
                        BuffFile->name = file;
                        MainSys.touch(BuffFile);                          
                    }
                    else
                        std::cout << "���������� ������� ���� �� ������ ���������" << std::endl << std::endl;
                }

                break;
            }
            case 2: // �������� �����
            {
                std::string question = "������� �������� ����������� �����, ��� ������ ������� <0>";
                std::string folder = askName(question);
                if (folder != "0")
                {
                    // �������� �� ��� ������������� ����� �� ������ ���������
                    if (!MainSys.isExist(folder, 'd'))
                    {
                        Folder* BuffFolder = new Folder;
                        BuffFolder->name = folder;
                        MainSys.mkdir(BuffFolder);                           
                    }
                    else
                        std::cout << "���������� ������� ����� �� ������ ���������" << std::endl << std::endl;                    
                }

                break;
            }
            case 3: // �������� �����
            {
                std::string question = "������� �������� ���������� �����, ��� ������ ������� <0>";
                std::string file = askName(question);
                if (file != "0")
                {
                    File* BuffFile = static_cast<File*>(MainSys.findElem(file, 'f'));

                    if (BuffFile != nullptr)
                        MainSys.rm(BuffFile);
                    else
                        std::cout << "� ������� ���������� ��� ����� � ������ ���������" << std::endl << std::endl;                   
                }

                break;        
            }
            case 4: // �������� �����
            {
                std::string question = "������� �������� ��������� ����� �����, ��� ������ ������� <0>";
                std::string folder = askName(question);
                if (folder != "0")
                {
                    Folder* BuffFolder = static_cast<Folder*>(MainSys.findElem(folder, 'd'));

                    if (BuffFolder != nullptr)
                        MainSys.rmdir(BuffFolder);
                    else
                        std::cout << "� ������� ���������� ��� ����� � ������ ���������" << std::endl << std::endl;                    
                }

                break;
            }
            case 5: // ��������� ����������
            {
                std::cout << MainSys.fullPath << std::endl;
                std::string question = "������� �������� �����, � ������� ������ �����,\n��� ������ �� ������� ����� ������� <..>,\n��� ������ ������� <0>";
                std::string folder = askName(question);
                if (folder != "0")
                    MainSys.cd(folder);

                break;
            }
            case 6: // ����� ��������� ������� ����������
            {
                std::cout << "������ ��������� � ������� ����������:" << std::endl;
                MainSys.ls();
                std::cout << "��� ������ � ���� ������� ����� �������" << std::endl;
                _getch();
                system("cls");

                break;
            }
            case 7: // ����� �� ���������
            {
                quit = true;
            }
        }
    }
}

