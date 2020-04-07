#include <ncurses.h>
#include <string.h>
#include "calc.h"
#include "fileio.h"
#include "menu.h"
#include "setting.h"

#define DIR_LOG "logdata/"
#define WIDTH 61
#define LIMIT 30

#define PUT_LOG(n, i)   FLAG(data[n], i) ? i+'0' : ' '
#define AT_LINE(n)      data[n]==buf[n] ? fprintf(fp, "@@@@@@") : fprintf(fp, "^^^^^@")
#define DASH_LINE(n)    data[n]==buf[n] ? fprintf(fp, "------") : fprintf(fp, "^^^^^-")

extern np data[81];
extern np buf[81];

char filename_log[FILENAME_MAX] = {};
int isauto = 0;

void read_log(char *buf){
    char fname[FILENAME_MAX];
    sprintf(fname, "%s%s", DIR_LOG, buf);
    int r = read_np(fname);
    if(r==0) printw("read %s", fname);
    getch();
}

void reset_logfile(){
    FILE *fp;
    if( (fp = fopen(filename_log, "w"))==NULL ){
        fclose(fp);
        return;
    }
    fclose(fp);
}

void write_log(int step, int num, char *text){
    FILE *fp;
    int i, j, k, roop;
    if( (fp = fopen(filename_log, "a"))==NULL ){
        fclose(fp);
        return;
    }

    fprintf(fp, "STEP : %d - %d : ", step, num);printw("%2d - %d : ", step, num);
    fprintf(fp, "%s\n", text);printw("%s\n", text);

    fprintf(fp, " ");
    for(i=0;i<WIDTH+2; i++){
        fprintf(fp, "@");
    }
    fprintf(fp, "\n ");
    for(i=0;i<9;i++){
        
        fprintf(fp, "%d:@", i);
        for(j=0;j<3;j++){
            roop = i*9 + j*3;
            fprintf(fp, "|%c %c %c|%c %c %c|%c %c %c|@",
                PUT_LOG(roop  , 1), PUT_LOG(roop  , 2), PUT_LOG(roop  , 3),
                PUT_LOG(roop+1, 1), PUT_LOG(roop+1, 2), PUT_LOG(roop+1, 3),
                PUT_LOG(roop+2, 1), PUT_LOG(roop+2, 2), PUT_LOG(roop+2, 3)
            );
        }
        fprintf(fp, "\n ");

        fprintf(fp, "%d:@", i);
        for(j=0;j<3;j++){
            roop = i*9 + j*3;
            fprintf(fp, "|%c %c %c|%c %c %c|%c %c %c|@",
                PUT_LOG(roop  , 4), PUT_LOG(roop  , 5), PUT_LOG(roop  , 6),
                PUT_LOG(roop+1, 4), PUT_LOG(roop+1, 5), PUT_LOG(roop+1, 6),
                PUT_LOG(roop+2, 4), PUT_LOG(roop+2, 5), PUT_LOG(roop+2, 6)
            );
        }
        fprintf(fp, "\n ");

        fprintf(fp, "%d:@", i);
        for(j=0;j<3;j++){
            roop = i*9 + j*3;
            fprintf(fp, "|%c %c %c|%c %c %c|%c %c %c|@",
                PUT_LOG(roop  , 7), PUT_LOG(roop  , 8), PUT_LOG(roop  , 9),
                PUT_LOG(roop+1, 7), PUT_LOG(roop+1, 8), PUT_LOG(roop+1, 9),
                PUT_LOG(roop+2, 7), PUT_LOG(roop+2, 8), PUT_LOG(roop+2, 9)
            );
        }
        fprintf(fp, "\n ");
        
        
        if(i==2||i==5||i==8){
            fprintf(fp, "@@@@");
            AT_LINE(i*9+0);AT_LINE(i*9+1);AT_LINE(i*9+2);
            fprintf(fp, "@@");
            AT_LINE(i*9+3);AT_LINE(i*9+4);AT_LINE(i*9+5);
            fprintf(fp, "@@");
            AT_LINE(i*9+6);AT_LINE(i*9+7);AT_LINE(i*9+8);
            fprintf(fp, "@\n ");
        }else{
            fprintf(fp, "----");
            DASH_LINE(i*9+0);DASH_LINE(i*9+1);DASH_LINE(i*9+2);
            fprintf(fp, "--");
            DASH_LINE(i*9+3);DASH_LINE(i*9+4);DASH_LINE(i*9+5);
            fprintf(fp, "--");
            DASH_LINE(i*9+6);DASH_LINE(i*9+7);DASH_LINE(i*9+8);
            fprintf(fp, "@\n ");
        }
    }

    /* 情報書き込み 進捗表示 */
    if(sumsumKouho()==81){
        fprintf(fp, "ALL complete!\n\n");printw("ALL complete!\n");
    }else{
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int sumsumKouho(){
    int comp = 0;
    for(int i=0;i<81;i++){
        comp += sumKouho(i);
    }
    return comp;
}

void menu_log(){
    int cnt = 1, ssK = sumsumKouho();
    while(cnt < LIMIT){
        if(ssK==81) break;
        /* 判定処理 */
        only_two_pair_all();
        if(ssK > sumsumKouho()){
            /* 処理で候補を減らせたら */
            write_log(cnt, 1, "two pairs -> delete num except pair");
            move_data(data, buf);
            ssK = sumsumKouho();
        }if(ssK==81) break;

        findPairAll();
        if(ssK > sumsumKouho()){
            write_log(cnt, 2, "two pairs -> delete pair in LineRowBlock");
            move_data(data, buf);
            ssK = sumsumKouho();
        }if(ssK==81) break;

        read_and_delete();
        if(ssK > sumsumKouho()){
            write_log(cnt, 3, "read LineRowBlock and delete");
            move_data(data, buf);
            ssK = sumsumKouho();
        }if(ssK==81) break;

        findAllOnlyOne();
        if(ssK > sumsumKouho()){
            write_log(cnt, 4, "onlyone in LineRowBlock -> input");
            move_data(data, buf);
            ssK = sumsumKouho();
        }if(ssK==81) break;

        checkNumAll();
        if(ssK > sumsumKouho()){
            write_log(cnt, 5, "onlyone in data -> input");
            move_data(data, buf);
            ssK = sumsumKouho();
        }if(ssK==81) break;

        if(isauto){}else getch();
        cnt++;
    }
}

int main(int argc, char *argv[]){
    /* start */
    initscr();cbreak();echo();idlok(stdscr, TRUE);scrollok(stdscr, TRUE);

    if(argc == 1 || argc > 3){
        printw("prepare %%1d-data logdata/filename\n$ ./log filename (auto)");getch();endwin();return 0;
    }
    
    if(argc == 3 && !strcmp(argv[2], "auto"))isauto = 1;

    printw("log solving steps of %s\n", argv[1]);
    read_log(argv[1]);
    sprintf(filename_log, "%s%s_log", DIR_LOG, argv[1]);
    reset_logfile();

    printw("[Enter] to start!");
    move_data(data, buf);
    getch();
    write_log(0, 0, "start");

    menu_log();

    getch();

    /* end */
    endwin();
    return 0;
}