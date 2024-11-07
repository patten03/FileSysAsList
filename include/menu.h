#ifndef PROG_MENU
#define PROG_MENU

#include "fileSys.h"
#include <MenuWork.h>

void menu();
void editStructureMenu(FileSys& MainSys);
bool receiveFSAL(const std::string& filename);

#endif // PROG_MENU