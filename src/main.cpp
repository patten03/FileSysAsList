#include <MenuWork.h>
#include "fileSys.h"

int main()
{
    standartSettings();

    FileSys MainSys;
    Folder *Folder1 = new Folder;
    Folder *Folder2 = new Folder;

    File *File1, *File2, *File3, *File4;
    File1 = new File;
    File2 = new File;
    File3 = new File;
    File4 = new File;

    File1->name = "1.txt";
    File2->name = "2.txt";
    Folder1->name = "folder1";
    Folder2->name = "folder2";
    File3->name = "3.txt";
    File4->name = "4.txt";

    MainSys.touch(File1);
    MainSys.touch(File2);
    MainSys.mkdir(Folder1);

    MainSys.ls();

    MainSys.cd("folder1");

    MainSys.touch(File3);
    MainSys.mkdir(Folder2);

    MainSys.cd("folder2");

    MainSys.touch(File4);

    MainSys.ls();
    MainSys.cd("..");
    MainSys.cd("..");
    
    MainSys.rm(File1);
    MainSys.rmdir(Folder1);

    return 0;
}