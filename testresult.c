/*
 *  extre13-21r.c
 *
 *  Created on: 2024/11/07
 *      Author: Webデザイン科
 */

// 練習問題13-21r
//模擬試験の成績順位を表示する
#include <stdio.h>
#include <string.h>

#define N_MAX 128		//名前の上限
#define S_MAX 64		//生徒情報、試験結果を保存する上限
#define I_MAX 10		//生徒IDの桁上限
#define K_MAX 5		//科目数の上限

typedef struct{
	char	id[I_MAX];
	char	name[N_MAX];
}Student;
typedef struct{
	char	id[I_MAX];
	char	name[N_MAX];
	int		point[K_MAX];
	int		sum;
}Result;
void swap_Student(Result *x,Result *y)
{
	Result temp = *x;
	*x = *y;
	*y = temp;
}
void point_sort(Result temp[],int num)
{
	for(int i = 0; i < num-1; i++){
		for(int j = num-1; j > i; j--){
			if(temp[j-1].sum < temp[j].sum)
				swap_Student(&temp[j-1],&temp[j]);
		}
	}
}
void id_sort(Result temp[],int num)
{
	for(int i = 0; i < num-1; i++){
		for(int j = num-1; j > i; j--){
			if(strcmp(temp[j-1].id,temp[j].id) > 0)
				swap_Student(&temp[j-1],&temp[j]);
		}
	}
}
void result_print(FILE *dfp,Result st[],int cnt_st,Result ob[],int cnt_ob,int cnt_obj)
{
	//IDで名前が見つかった配列の表示・書き込み
	for(int i = 0; i < cnt_st; i++){
		printf("　%3d %8s    %s  %4d",i+1,st[i].id,st[i].name,st[i].sum);
		fprintf(dfp,"　%3d %8s    %s  %4d",i+1,st[i].id,st[i].name,st[i].sum);
		for(int j = 0; j < cnt_obj; j++){
			printf("  %4d", st[i].point[j]);
			fprintf(dfp,"  %4d", st[i].point[j]);
		}
		printf("\n");
		fprintf(dfp,"\n");
	}
	//IDで名前が見つからなかった配列が存在すれば表示・書き込み
	if(cnt_ob > 0){
		printf("　－－－－－－－－－－－（該当者なし）－－－－－－－－－－－－\n");
		fprintf(dfp,"　－－－－－－－－－－－（該当者なし）－－－－－－－－－－－－\n");
		for(int i = 0; i < cnt_ob; i++){
			printf("　    %8s  ？？？？？？？？？？    %4d",ob[i].id,ob[i].sum);
			fprintf(dfp,"　    %8s  ？？？？？？？？？？    %4d",ob[i].id,ob[i].sum);
			for(int j = 0; j < cnt_obj; j++){
				printf("  %4d",ob[i].point[j]);
				fprintf(dfp,"  %4d",ob[i].point[j]);
			}
			printf("\n");
			fprintf(dfp,"\n");
		}
	}
}
//--- main関数 ---//
int main(void)
{
	Student	info[S_MAX];					//生徒ID情報受取用
	Result		temp, st[S_MAX], ob[S_MAX];	//一時受取用、IDあるデータ格納、IDないデータ格納
	int		cnt_id = 0;						//生徒情報で読み込めたIDの数はいくつか記憶
	char	obj[K_MAX][N_MAX];

	FILE	*mfp, *sfp, *dfp;				//ex1321.txt用、生徒情報ファイル・成績一覧ファイル用、書込みファイル用
	char	fname[N_MAX] = "ex1321.txt";	//一番最初に読むファイル名で初期化し、そのあと単独文字列系で使い回す
	char	dname[N_MAX] = "ex1321_out.txt";//出力用ファイル名

	//各々のファイル名が格納されているex1321.txtをmfpで開く
	if((mfp = fopen(fname,"r")) == NULL){
		printf("\aファイル\"%s\"をオープンできませんでした。\n",fname);
	}
	else{
		//1行目はファイル名なのでどうでもいいから読込だけで無視
		fscanf(mfp,"%s",fname);
		//2行目の生徒情報ファイルを読み込みsfpでオープン
		fscanf(mfp,"%s",fname);
		if((sfp = fopen(fname,"r")) == NULL){
			printf("\aファイル\"%s\"をオープンできませんでした。\n",fname);
		}
		else{
			//1行目のファイル名はどうでもいいから無視
			fscanf(sfp,"%s",fname);
			//2秒目以降の生徒情報をStudent配列に保存してsfpを閉じる
			while(fscanf(sfp,"%s%s",info[cnt_id].id,info[cnt_id].name) == 2){
				cnt_id++;
			}
			fclose(sfp);

			//書込み用のファイルをオープン
			if((dfp = fopen(dname,"w")) == NULL){
				printf("\aファイル\"%s\"をオープンできませんでした。\n",dname);
			}
			else{
				printf("※※　模擬試験の成績一覧　※※\n");
				fprintf(dfp,"※※　模擬試験の成績一覧　※※\n");
				//mfpで3行目から1行読むごとにsfpで模試ファイルをオープン
				while(fscanf(mfp,"%s",fname) > 0){
					if((sfp = fopen(fname,"r")) == NULL){
						printf("\aファイル\"%s\"をオープンできませんでした。\n",fname);
					}
					else{
						//1行目の見出しと科目を保存・見出しを表示
						fscanf(sfp,"%s%s%s%s%s%s",fname,obj[0],obj[1],obj[2],obj[3],obj[4]);
						printf("○%s\n　順位　生徒番号   氏    名            総合点",fname);
						fprintf(dfp,"○%s\n　順位　生徒番号   氏    名            総合点",fname);
						//科目数を確認・科目を表示
						int cnt_obj = 0;
						for(int i = 0; i <= K_MAX; i++){
							cnt_obj = i;
							if(obj[i][0] == '-' || i == K_MAX)
								break;
							printf("  %s",obj[i]);
							fprintf(dfp,"  %s",obj[i]);
						}
						printf("\n");
						fprintf(dfp,"\n");
						//3行目以降、成績データがなくなるまで保存
						int cnt_st = 0;
						int cnt_ob = 0;
						while(fscanf(sfp,"%s%d%d%d%d%d",temp.id,&temp.point[0],&temp.point[1],&temp.point[2],&temp.point[3],&temp.point[4]) > 0){
							//合計点を算出
							temp.sum = 0;
							for(int i = 0; i < cnt_obj; i++)
								temp.sum += temp.point[i];
							//IDを元に名前を特定、特定不可で保存する配列を分ける
							int k;
							for(k = 0; k < cnt_id; k++){
								//全てのIDを走査
								if(strcmp(temp.id, info[k].id) == 0){
									//IDが見つかったとき、名前を代入し完成した構造体をstにコピーする
									strcpy(temp.name, info[k].name);
									st[cnt_st++] = temp;
									break;
								}
							}
							//IDが見つからなかったとき（if文が全て通り過ぎたとき）構造体をobにコピーする
							if(k == cnt_id)
								ob[cnt_ob++] = temp;
						}
						//stを合計点が高い順にソート
						point_sort(st,cnt_st);
						//obをID順にソート
						id_sort(ob,cnt_ob);
						//結果を表示＆書き込み
						result_print(dfp,st,cnt_st,ob,cnt_ob,cnt_obj);
					}
					//1ファイル分おわったらsfpを閉じる
					fclose(sfp);
				}//模試情報が読み込めなくなるまで繰り返す
			}
			fclose(dfp);
		}
		fclose(mfp);
	}
	return 0;
}
