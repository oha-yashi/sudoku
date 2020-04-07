#ifndef _FILEIO_H_
#define _FILEIO_H_

#define SAVE_FILE "np/save.dat"
#define DIR_NP "np/"

int read_np(const char *filename);
/* filename から読み込んだ盤面情報をdataに入れる
 * filename 1行目にデータの読み込みformatを入れる
*/
void read_file();
void read_save();
void save_data();
#endif