#include <ncurses.h> //中でstdio.hをincludeしている
#include <string.h>
#include <time.h>
#include "fileio.h"
#include "calc.h"

extern np data[81];

int read_np(const char *filename){
    FILE *fp;
    if( (fp = fopen(filename, "r")) ==NULL){
        printw("%s file open error!!", filename);
        fclose(fp);
        return -1;
    }
    int i, buf;
    char format[50], fbuf[50];
    if(fscanf(fp, "%%%s", fbuf)){
        /* なにかしら変換指定子の存在を確認 */
        /* "%hoge"の形式のみ通過 fbufには"hoge"のみ入る */
        sprintf(format, "%%%s", fbuf);
        /* "hoge" で格納されているのを "%hoge" に戻してformatに入れる */
        for(i=0; i<81; i++){
            fscanf(fp, format, &buf);
            if(!strcmp(format, "%1d")){
                inputNum(i, buf);
            }else if(!strcmp(format, "%x") || !strcmp(format, "%4x")){
                data[i] = buf;
            }
        }
    }else{
        printw("in %s, no format!", filename);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return 0;
}

void read_file(){
    char fname[FILENAME_MAX];
    char buf[FILENAME_MAX];
    printw("\n%s[filename] -> %s", DIR_NP, DIR_NP);
    echo();
    scanw("%s", buf);
    noecho();
    sprintf(fname, "%s%s", DIR_NP, buf);
    int r = read_np(fname);
    if(r==0) printw("read %s", fname);
    getch();
}

void read_save(){
    addch('\n');//read_fileと改行数を合わせる
    int r = read_np(SAVE_FILE);
    if(r==0) printw("read %s", SAVE_FILE);
    getch();
}

void save_data(){
    addch('\n');//read_fileと改行数を合わせる
    FILE *fp;
    if( (fp = fopen(SAVE_FILE, "w")) ==NULL){
        addstr("file open error!!");
        return;
    }
    fprintf(fp, "%%x\n");
    /*再読み込みで必要な変換指定子 %x を書き込む*/
    for(int i=0; i<81; i++){
        fprintf(fp, "%04x ", data[i]); //ここは%04x固定で
        if(i%9==8)fputc('\n', fp);
    }
    /*10行目時間情報書き込み*/
    time_t now = time(NULL);
    struct tm *timer = localtime(&now);
    fprintf(fp, "saved at %4d/%02d/%02d %02d:%02d:%02d",
    timer->tm_year + 1900,
    timer->tm_mon + 1,
    timer->tm_mday,
    timer->tm_hour,
    timer->tm_min,
    timer->tm_sec
    );
    fclose(fp);
    printw("write %s", SAVE_FILE);
}