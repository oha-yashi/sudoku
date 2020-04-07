#ifndef _CALC_H_
#define _CALC_H_

typedef unsigned short np;
/*  各マスのデータ。ビット管理する
    aaaa00bbbbbbbbb0(2)
    a: 確定した数字
    b: 候補のフラグ
*/

/* d にはdataをいれる, nにはマスの数字を入れる iにはマスに入れる数字1~9を入れる*/
/* d から定数を返す */
#define np_reset 0x03fe //0000,0011,1111,1110
#define NUM(d) ((d)>>12)
#define FLAG(d, i) ((d)>>i & 1)
/* d に操作をする */
#define set_num(d, i) d = d & 0x0fff | (i<<12) /* 0x**** -> 0x0*** -> 0xi*** */
#define flag_on(d, i) d |= 1<<i // i桁目1にする
#define flag_off(d, i) d &= ~(1<<i) // i桁目0にする
/* n から定数を返す */
#define LINE(n) ((n)/9)
#define ROW(n) ((n)%9)
#define BLOCK(n) (LINE(n)/3*3 + ROW(n)/3)
#define LINE_ST(n) ((n)/9*9) //0~81->{0,1,2,3,4,5,6,7,8,9}
#define ROW_ST(n) ((n)%9) //0~81->{0,9,18,27,36,45,54,63,72}
#define BLOCK_ST(n) ((n)/27*27 + (n)%9/3*3) //0~81->{0,3,6,27,30,33,54,57,60}

#define BLOCK_N_ST(n) ((n)/3*27 + (n)%3*3) //0~8->{0,3,6,27,30,33,54,57,60}

void reset(np[]);
int sumKouho(int);
void checkNumAll();

void delete_data(int);

void delete_check(int);
void delete_lrb(int, int, int, char);

void inputNum(int, int);

int isOnlyOneIn_lrb(int start, int find, int byA, int byB);
void findAllOnlyOne();

void read_and_delete();
void read_delete_lrb(int start, int kouho, int byA, int byB, int byC, char c);
/* start: 走査開始マス
 * kouho: 探索する候補
 * byA, byB, byC: ループを回す時何マス飛ばしにするか
 1: 右に1つ, 3: 右に1ブロック, 9: 下に1つ, 27: 下に1ブロック
 * c: 2回目の探索の開始マスを決めるためのswitch変数
 *
 * ループ1: kouhoのフラグの数を小ブロックごとに得る
 * ループ2: フラグが小ブロック1つに集まっていればkeepに入れる
 * if: contにkeepする小ブロックの番号1~3が入っている
 cont=0のみreturn終了, contは次に向けてデクリメント
 * switch: 2回目の探索のstartを再設定
 * ループ3: keep以外のマスでkouhoのフラグを消す
*/
void findPairAll();
void findPair(int start, int byA, int byB);

void only_two_pair_all();
void only_two_pair_lrb(int start, int byA, int byB);
#endif