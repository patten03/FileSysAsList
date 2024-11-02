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
                std::string question = "������� �������� ������������ �����";
                std::string file = askName(question);
                File* BuffFile = new File;
                BuffFile->name = file;
                MainSys.touch(BuffFile); 

                break;
            }
            case 2: // �������� �����
            {
                std::string question = "������� �������� ����������� �����";
                std::string folder = askName(question);
                Folder* BuffFolder = new Folder;
                BuffFolder->name = folder;
                MainSys.mkdir(BuffFolder);

                break;
            }
            case 3: // �������� �����
            {
                std::string question = "������� �������� ���������� �����";
                std::string file = askName(question);
                File* BuffFile = static_cast<File*>(MainSys.findElem(file));

                if (BuffFile != nullptr)
                    MainSys.rm(BuffFile);
                else
                    std::cout << "� ������� ���������� ��� ����� � ������ ���������" << std::endl << std::endl;

                break;        
            }
            case 4: // �������� �����
            {
                std::string question = "������� �������� ��������� ����� �����";
                std::string folder = askName(question);
                Folder* BuffFolder = static_cast<Folder*>(MainSys.findElem(folder));

                if (BuffFolder != nullptr)
                    MainSys.rmdir(BuffFolder);
                else
                    std::cout << "� ������� ���������� ��� ����� � ������ ���������" << std::endl << std::endl;

                break;
            }
            case 5: // ��������� ����������
            {
                std::cout << MainSys.fullPath << std::endl;
                std::string question = "������� �������� �����, � ������� ������ �����,\n��� ������ �� ������� ����� ������� <..>";
                std::string folder = askName(question);
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
                // TODO ������� �����-������ �������� ���������
            }
        }
    }
}

