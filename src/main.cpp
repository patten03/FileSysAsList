#include <MenuWork.h>
#include "fileSys.h"

int main()
{
    standartSettings();

    FileSys MainSys;
    Folder *Folder1 = new Folder;

    File *File1, *File2, *File3, *File4;
    File1 = new File;
    File2 = new File;
    File3 = new File;
    File4 = new File;

    File1->name = "1.txt";
    File2->name = "2.txt";
    Folder1->name = "folder1";
    File3->name = "3.txt";

    MainSys.touch(File1);
    MainSys.touch(File2);
    MainSys.mkdir(Folder1);
    MainSys.writeInFolder = true;
    MainSys.touch(File3);
    MainSys.touch(File4);

    MainSys.ls();

    MainSys.rmdir(Folder1);
    MainSys.rm(File2);
    MainSys.rm(File3);
    MainSys.rm(File1);
    
    return 0;
}