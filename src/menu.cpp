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
            "��������� ���������� ���������",
            "��������� ��������� ���������",
            "����� �� ���������"
        };
        ask(menuPanel);
        int choice = inputChoice(menuPanel.size());

        switch (choice)
        {
            case 1: // �������� �����
            {
                std::string file;
                int size;
                std::string date;

                file = inputName();
                // ����� �� ������� ������������
                if (file == "0")
                    {system("cls"); break;}
                // �������� �� ��� ������������� ����� �� ������ ���������
                if (MainSys.isExist(file, 'f'))
                {
                    std::cout << "���������� ������� ���� �� ������ ���������" << std::endl << std::endl;
                    break;
                }
                size = inputSize();
                // ����� �� ������� ������������
                if (size == -1)
                    {system("cls"); break;}
                date = inputDate();
                // ����� �� ������� ������������
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
            case 2: // �������� �����
            {
                std::string folder;
                std::string date;

                folder = inputName();
                // ����� �� ������� ������������
                if (folder == "0")
                    {system("cls"); break;}
                // �������� �� ��� ������������� ����� �� ������ ���������
                if (MainSys.isExist(folder, 'd'))
                {
                    std::cout << "���������� ������� ����� �� ������ ���������" << std::endl << std::endl;
                    break;
                }
                date = inputDate();
                // ����� �� ������� ������������
                if (date == "0")
                    {system("cls"); break;}
                
                Folder* BuffFolder = new Folder;
                BuffFolder->name = folder;
                BuffFolder->date = date;
                MainSys.mkdir(BuffFolder);   

                system("cls");
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
            case 7: // �������� ����������� ���������
            {
                // �������� �� ��, �������� �� ������� ��������� ������
                if (MainSys.Beg != nullptr)
                {
                    std::cout << "�� �������, ��� ������ ��������� ���������, �� �������� �������?" << std::endl;
                    std::vector<std::string> question = {
                        "��",
                        "���"
                    };
                    ask(question);
                    int ans = inputChoice(question.size());

                    if (ans == 2)
                        break;
                }
                
                // ����� �������� �������
                std::string filename = findFile("�������� ����, ���������� ��������� �������� �������:", receiveFSAL);
                if (filename != "0")
                {
                    MainSys.~FileSys();
                    MainSys = FileSys();
                    MainSys.loadFileSys(filename);                   
                }

                break;
            }
            case 8: // ���������� �������� �������
            {
                // �������� �� ������������� ��������
                if (MainSys.Beg != nullptr)
                {
                    std::string filename = askName("������� �������� �������� ��� ������������ ����� �� ����������:");
                    if (filename != "0")
                    {
                        filename = space2underscore(filename) + "_" + currentTime() + ".fsal";
                        MainSys.uploadFileSys(filename);
                        std::cout << "��������� ���� ��������� ��� ��������� " << filename << std::endl << std::endl;;
                    }
                }

                else
                    std::cout << "���������� ��������� ������ ���������" << std::endl << std::endl;

                break;
            }
            case 9: // ����� �� ���������
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
		return false; // ���� � �������� ����� � ����������� ����� ���
}