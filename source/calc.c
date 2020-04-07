#include "calc.h"

np data[81];

void reset(np d[]){
    for(int i=0; i<81; i++){
        d[i] = np_reset;
    }
}

int sumKouho(int n){
    int sum = 0;
    for(int i=1; i<=9; i++){
        sum += FLAG(data[n],i);
    }
    return sum;
}

void checkNumAll(){for(int n=0;n<81;n++)if(sumKouho(n)==1)for(int j=1;j<=9;j++)if(FLAG(data[n],j))inputNum(n,j);}

void delete_data(int n){
    int i;
    int buf[81] = {};
    for(i=0; i<81; i++){
        if(i==n){
            buf[i] = 0;
        }else{
            buf[i] = NUM(data[i]);
        }
    }
    reset(data);
    for(i=0; i<81; i++){
        inputNum(i, buf[i]);
    }
}

void delete_check(int n){
    delete_lrb(n, 3, 1, 'l');
    delete_lrb(n, 27, 9, 'r');
    delete_lrb(n, 9, 1, 'b');
}

void delete_lrb(int n, int byA, int byB, char c){
    int i, j, start, roop;
    switch(c){
        case 'l': start = LINE_ST(n); break;
        case 'r': start = ROW_ST(n); break;
        case 'b': start = BLOCK_ST(n); break;
    }
    for(i=0; i<3; i++)for(j=0; j<3; j++){
        roop = start + i*byA + j*byB;
        if(roop == n){
            continue;
        }else{
            flag_off(data[roop], NUM(data[n]));
        }
    }
}

/*  マスn に input を確定させる 0~9を入れる */
void inputNum(int n, int input){
    // D=****,****,****,*****, input=5
    if(input != 0){
        set_num(data[n], input);// D=0101,****,****,****
        flag_on(data[n], input);// D=0101,****,**1*,****
        for(int i=1; i<=9; i++){
            if(i != input)flag_off(data[n], i);
        }// D=0101,**00,0010,000*
        delete_check(n);
    }else if(input == 0){
        data[n] = np_reset;// D=0000,0011,1111,1110
    }
}

int isOnlyOneIn_lrb(int start, int find, int byA, int byB){
    int sum = 0;
    for(int i=0; i<3; i++)for(int j=0; j<3; j++){
        if(FLAG(data[start+i*byA+j*byB], find)) sum++;
    }
    return (sum==1) ? 1 : 0 ;
}

void findAllOnlyOne(){
    int isOnlyOne;
    for(int n=0; n<81; n++)for(int f=1; f<=9; f++){
        if(FLAG(data[n], f)){
            isOnlyOne = isOnlyOneIn_lrb(LINE_ST(n), f, 3, 1);
            isOnlyOne += isOnlyOneIn_lrb(ROW_ST(n), f, 27, 9);
            isOnlyOne += isOnlyOneIn_lrb(BLOCK_ST(n), f, 9, 1);
            if(isOnlyOne) inputNum(n, f);
        }
    }
}

void read_and_delete(){
    for(int lrb=0; lrb<9; lrb++){
        for(int kouho=1; kouho<=9; kouho++){
            read_delete_lrb(lrb*9, kouho, 3, 1, 9, 'b');
            read_delete_lrb(lrb, kouho, 27, 9, 1, 'b');
            read_delete_lrb( BLOCK_N_ST(lrb), kouho, 9, 1, 3, 'l');
            read_delete_lrb( BLOCK_N_ST(lrb), kouho, 1, 9, 27, 'r');
        }
    }
}

void read_delete_lrb(int start, int kouho, int byA, int byB, int byC, char c){
    int sum[3] = {}; int keep[3] = {};
    int i, j, k, roop, sum_all, cont;
    sum_all = cont = 0;

    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            roop = start + i*byA + j*byB;
            if(NUM(data[roop]) == kouho) return;
            sum[i] += FLAG(data[roop], kouho);
        }
        sum_all += sum[i];
    }
    for(i=0; i<3; i++){
        if(sum[i] == sum_all){
            for(j=0; j<3; j++){
                roop = start + i*byA + j*byB;
                keep[j] = roop;
            }
            cont = i+1;
            break;
        }
    }
    if(!cont--) return;
    switch(c){
        case 'l': start = LINE_ST(start + cont*byA); break;
        case 'r': start = ROW_ST(start + cont*byA); break;
        case 'b': start = BLOCK_ST(start + cont*byA); break;
    }
    for(i=0; i<3; i++)for(j=0; j<3; j++){
        roop = start + i*byB + j*byC;
        for(k=0; k<3; k++){
            if(roop == keep[k]) goto NEXT;
        }
        flag_off(data[roop], kouho);
        NEXT:;
    }
}

void findPairAll(){
    for(int i=0;i<9;i++){
        findPair(i*9, 3, 1);
        findPair(i, 27, 9);
        findPair(BLOCK_N_ST(i), 9, 1);
    }
}

void findPair(int start, int byA, int byB){
    int i, j, k, l, roop, roop2, cnt;
    for(i=0;i<3;i++)for(j=0;j<3;j++){
        roop = start + i*byA + j*byB;
        cnt = 0;
        if(sumKouho(roop)==2){
            //data[roop]が候補2つ
            for(k=0;k<3;k++)for(l=0;l<3;l++){
                roop2 = start + k*byA + l*byB;
                if(data[roop]==data[roop2])cnt++;
            }
            if(cnt==2){
                //data[roop]のペアがブロックを独占
                for(k=0;k<3;k++)for(l=0;l<3;l++){
                    roop2 = start + k*byA + l*byB;
                    if(data[roop2]!=data[roop]){
                        data[roop2] &= ~(data[roop]);
                        //data[roop]のフラグを消す
                    }
                }
            }
        }
    }
}

void only_two_pair_all(){
    for(int i=0; i<9; i++){
        only_two_pair_lrb(i*9, 3, 1);
        only_two_pair_lrb(i, 27, 9);
        only_two_pair_lrb(BLOCK_N_ST(i), 9, 1);
    }
}

void only_two_pair_lrb(int start, int byA, int byB){
    int i, j, k, l, roop, cnt;
    int q_k[10] = {};
    for(k=1;k<=9;k++){
        for(i=0;i<3;i++)for(j=0;j<3;j++){
            roop = start + i*byA + j*byB;
            if(FLAG(data[roop], k))q_k[k]++;
        }
    }
    int pair_k_in[10][2] = {};
    
    for(k=1; k<=9; k++){
        cnt = 0;
        if(q_k[k] == 2){
            for(i=0;i<3;i++)for(j=0;j<3;j++){
                roop = start + i*byA + j*byB;
                if(FLAG(data[roop], k)){
                    pair_k_in[k][cnt] = roop;
                    cnt++;
                }
            }
        }
    }
    //ブロック内で2箇所のみkが入るマスがpair_k_in[k][2]に入った
    for(k=1; k<=8; k++){//k<=8で正しい
        if(q_k[k] == 2){
            for(l=k+1; l<=9; l++){
                if(pair_k_in[k][0] == pair_k_in[l][0]
                && pair_k_in[k][1] == pair_k_in[l][1]){
                    //pairの組が一致したらそこでkとlが独占
                    data[ pair_k_in[k][0] ] = data[ pair_k_in[k][1] ] = 0x0;
                    data[ pair_k_in[k][0] ] = data[ pair_k_in[k][1] ] |= 1<<k;
                    data[ pair_k_in[k][0] ] = data[ pair_k_in[k][1] ] |= 1<<l;
                    //フラグ立てをkとlのみにする
                }
            }
        }
    }
}