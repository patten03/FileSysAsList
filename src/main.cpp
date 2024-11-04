#include <MenuWork.h>
#include "fileSys.h"
#include "menu.h"

int main()
{
    standartSettings();

    // FileSys FS;
    // FS.loadFileSys("файловая_система.fsal");
    // FS.ls();

    // FS.cd(FS.findElem("3",'d')->name);
    // FS.ls();

    // FS.cd(FS.findElem("6",'d')->name);
    // FS.ls();

    menu();

    return 0;
}