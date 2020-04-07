#include <ncurses.h>
#include "menu.h"
#include "setting.h"
#include "calc.h"
#include "fileio.h"

extern np data[81];
np buf[81];
np save[81];

/*  Line|Row Show
    showAll系に対応した表示座標
    markで使う*/
int LS[9] = {0,1,2,4,5,6,8,9,10};
int RS[9] = {1,5,9,15,19,23,29,33,37};

void menu(){
    int i, ch, now;
    ch = now = 0;
    reset(data);reset(buf);reset(save);
    while(1){
        /* 盤面, メニュー表示 */
        showAll();
        markKouho2or3();
        mark_data_void();
        mark(now, 2, '$');
        printmenu(now);

        /* 入力分岐 */
        ch = getch();
        if(ch == 'q') break;
        if(ch == 'c'){changeKouho(&now); continue;}
        if(ch == 'b'){move_data(buf, data); continue;}
        if(ch == 'd'){move_data(data, buf); delete_data(now); continue;}
        if(ch == 'x'){move_data(data, buf); reset(data); now=0; continue;}
        if(ch == 'i'){read_file(); continue;}
        if(ch == 'w'){save_data(); continue;}
        if(ch == 'r'){read_save(); continue;}
        if(ch == 'k'){
            for(i=1; i<=9; i++){
                showAllKouho(i);
                getch();
            }
        }
        if(ch == ' '){
            now++;
            if(now==81)now=0;
        }
        if(KEY_DOWN <= ch && ch <= KEY_RIGHT){
            udrl(ch, &now);
        }
        if('0' <= ch && ch <= '9'){
            move_data(data, buf);
            inputNum(now, ch - '0');
            now++;
            if(now==81)now=0;
        }

        /* 判定処理 */
        only_two_pair_all();
        findPairAll();
        read_and_delete();
        findAllOnlyOne();
        checkNumAll();
    }
}

void printmenu(int now){
    int i;
    move(12,0);
    printw("now(%2d: %04x)", now, data[now]);
    if(NUM(data[now]) > 0){
        printw("[%d] < ", NUM(data[now]));
    }else{
        printw("[_] < ");
    }
    for(i=1; i<=9; i++){
        if(FLAG(data[now], i)){
            printw("%d ", i);
        }
    }
    addstr(">\n");
    addstr("[^] [v] [<] [>] [Space] [");
    addstr_rev_nml("K", "ouho] [");
    addstr_rev_nml("C", "hange]\n[");
    addstr_rev_nml("B", "ack] [");
    addstr_rev_nml("D", "elete] [");
    addstr_rev_nml("X", ":reset] [");
    addstr_rev_nml("I", "mport]\n[");
    addstr_rev_nml("W", "rite_save] [");
    addstr_rev_nml("R", "ead_save] [");
    addstr_rev_nml("Q", "uit]");
    refresh();
}

void showAll(){
    int i, j;
    move(0,0);
    for(i=0;i<81;i++){
        if(NUM(data[i])){
            printw("[%d] ", NUM(data[i]));
        }else{
            printw("[ ] ");
        }
        if(ROW(i)==2 || ROW(i)==5)addstr("@ ");
        if(ROW(i) == 8)addstr("@\n");
        if(i%27==26){
            for(j=0;j<YOKO;j++)addch('=');
            addch('\n');
        }
    }
    refresh();
}

/* kをkouhoとするマスを表示 */
void showAllKouho(int k){
    int i, j;
    move(0,0);
    for(i=0;i<81;i++){
        if(FLAG(data[i],k) && !NUM(data[i])){
            attron(COLOR_PAIR(3));
            printw("<%d> ", k);
            attron(COLOR_PAIR(1));
        }else if(NUM(data[i]) != 0){
            printw("[%d] ", NUM(data[i]));
        }else{
            printw("[ ] ");
        }
        if(i%3==2)addstr("@ ");
        if(ROW(i) == 8)addch('\n');
        if(i%27==26){
            for(j=0;j<41;j++)addch('=');
            addch('\n');
        }
    }
    refresh();
}

void addstr_rev_nml(char *r, char *n){
    reverse(ON);addstr(r);reverse(OFF);addstr(n);
}

void udrl(int ch, int *n){
    int now = *n;
    switch(ch){
        case KEY_UP:
            *n = LINE(now)!=0 ? now-9 : now+72;
            break;
        case KEY_DOWN:
            *n = LINE(now)!=8 ? now+9 : now-72;
            break;
        case KEY_LEFT:
            *n = ROW(now)!=0 ? now-1 : now+8;
            break;
        case KEY_RIGHT:
            *n = ROW(now)!=8 ? now+1 : now-8;
            break;
    }
}

void mark(int n, int cp, char c){
    /* マス n の枠を色番号 cp の文字 c で囲む */
    attron(COLOR_PAIR(cp));
    mvprintw(LS[LINE(n)], RS[ROW(n)]-1, "%c", c);
    mvprintw(LS[LINE(n)], RS[ROW(n)]+1, "%c", c);
    attron(COLOR_PAIR(1));
}

void markKouho2or3(){
    for(int i=0; i<81; i++){
        if(sumKouho(i) == 2) mark(i, 4, '!');
        if(sumKouho(i) == 3) mark(i, 3, '?');
    }
}

void mark_data_void(){
    // 確定なしで候補全滅だと0x0000になる。間違い。
    for(int i=0; i<81; i++){
        if(data[i] == 0x0000) mark(i, 2, 'X');
    }
}

void changeKouho(int *n){
    int now = *n;
    int i, change;
    move(12,0);
    for(i=1; i<=9; i++){
        if(FLAG(data[now], i)){
            printw("%d ", i);
        }
    }
    addstr("whitch to change -> ");
    change = getch() - '0';
    if(1 <= change && change <= 9){
        if(FLAG(data[now], change)){
            flag_off(data[now], change);
        }else{
            flag_on(data[now], change);
        }
    }
}

void move_data(np from[], np to[]){
    for(int i=0; i<81; i++){
        to[i] = from[i];
    }
}