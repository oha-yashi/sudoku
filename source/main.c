#include <ncurses.h>
#include "setting.h"
#include "calc.h"
#include "menu.h"

int main(void){
    startGame();
    if(isErrorWindowSize())goto END;

    menu();

    END: endGame();
    return 0;
}