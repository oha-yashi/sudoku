#include <ncurses.h>
#include <string.h>
#include "setting.h"

int winx, winy;

void startGame(){
    /* setting */
    initscr();
    cbreak();
    noecho();
    idlok(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    keypad(stdscr, TRUE);

    /* color setting */
    start_color();
    colorSetting();
    bkgd(COLOR_PAIR(1));

    getmaxyx(stdscr, winy, winx);
}

void colorSetting(){
    init_pair(1, colorBLACK, colorWHITE);
    init_pair(2, colorRED, colorWHITE);
    init_pair(3, colorGREEN, colorWHITE);
    init_pair(4, colorBLUE, colorWHITE);
}

/* 画面サイズ エラー:1 OK:0 を返す */
int isErrorWindowSize(){
    int rt = 0;
    /* 高さ不足 */
    if(winy < TATE_ALL){
        printw("SHORT HEIGHT ERROT!! NEED %d, now:%d\n", TATE_ALL, winy);
        rt++;
    }
    /* 幅不足 */
    if(winx < YOKO_ALL){
        printw("SHORT WIDTH ERROR! NEED %d, now:%d", YOKO_ALL, winx);
        rt++;
    }
    /* エラーあり */
    if(rt){
        getch();
        return rt;
    }
    /* WINDOW SIZE CLEAR */
    /* GAME START表示 */
    char *str1 = "GAME START!!";
    char *str2 = "PUSH ANY KEY!!";
    mvaddstr(winy/2, (winx-strlen(str1))/2, str1);
    mvaddstr(winy/2+1, (winx-strlen(str2))/2, str2);
    getch();
    clear();
    return 0;
}

void endGame(){
    clear();
    char *str1 = "GAME END!!";
    char *str2 = "PUSH ANY KEY!!";
    mvaddstr(winy/2, (winx-strlen(str1))/2, str1);
    mvaddstr(winy/2+1, (winx-strlen(str2))/2, str2);
    getch();
    endwin();
}

void reverse(int OnOff){
    if(OnOff)attron(A_REVERSE);
    if(!OnOff)attrset(0);
}