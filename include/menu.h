#ifndef PROG_MENU
#define PROG_MENU

#include "fileSys.h"
#include <MenuWork.h>

// Основоное меню программы
void menu(); 

// Подменю программы, отвечающее за сохранение и загрузку структуры
void editStructureMenu(FileSys& MainSys);

// Функция определения файлов с расширением .fsal
bool receiveFSAL(const std::string& filename);

#endif // PROG_MENU