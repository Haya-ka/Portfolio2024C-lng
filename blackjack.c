/*
 * extre12-22.c
 *
 *  Created on: 2024/10/30
 *      Author: Webデザイン科
 */

// extre12-22 ブラックジャック（チップ持ち越しver）
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NAME_LEN 64		//氏名の文字数
#define CARD_MAX 52
#define BJ 21
//--- トランプのカード ---//
typedef struct{
	int pow;
	char role[NAME_LEN];
	int num;
}Card;
//--- プレイヤー ---//
typedef struct{
	char name[NAME_LEN];
	Card pick[8];
	int sum;
}Player;
//--- デッキの初期化 ---//
void card_set(Card deck[])
{
	int		k = 0;					//　山札の添え字（0～51まで回る）

	for(int i = 1; i <= 13; i++){		//10,J,Q,Kは全て10点とする
		for(int j = 1; j <= 4; j++){
			if(i < 10)
				deck[k].pow = i;
			else
				deck[k].pow = 10;
			if(j == 1)
				strcpy(deck[k].role,"ク ラ ブ　");
			else if(j == 2)
				strcpy(deck[k].role,"ダ イ ヤ　");
			else if(j == 3)
				strcpy(deck[k].role,"ハ ー ト　");
			else
				strcpy(deck[k].role,"スペード　");
			deck[k].num = i;
			k++;
		}
	}
	//最後にジョーカーを混ぜる
	//deck[CARD_MAX-1].pow = BJ;
	//strcpy(deck[CARD_MAX-1].role,"ジョーカー");
	//deck[CARD_MAX-1].num = BJ;
}
//山札をシャッフルする
void shuffle(Card d[])
{
	int a,b;
	Card temp;

	//適当に入れ替えてシャッフルした気になる
	for(int i = 0; i < CARD_MAX; i++){
		a = rand() % CARD_MAX;
		b = rand() % CARD_MAX;

		temp = d[a];
		d[a] = d[b];
		d[b] = temp;
	}
}
//手札を表示する
void card_print(const Player m[],int com,int you)
{
	putchar('\n');
	//comの出力
	if(com == 1)
		printf("□ %-12s <--P>\t",m[0].name);
	else
		printf("□ %-12s <%2dP>\t",m[0].name,m[0].sum);

	for(int i = 0; i < com; i++)
		printf("　　%s <%2d>",m[0].pick[i].role,m[0].pick[i].num);
	if(com == 1)
		printf("　　********** <-->");
	putchar('\n');

	//youの出力
	printf("■ %-12s <%2dP>\t",m[1].name,m[1].sum);
	for(int i = 0; i < you; i++)
		printf("　　%s <%2d>",m[1].pick[i].role,m[1].pick[i].num);
	putchar('\n');
}
//--- カードを引く ---//
void card_pick(Player m[],Card d[],int p,int t,int cnt)
{
	//デッキの一番上のカードを手札に上書きする
	m[p].pick[t].pow = d[cnt].pow;
	strcpy(m[p].pick[t].role,d[cnt].role);
	m[p].pick[t].num = d[cnt].num;
	//リザルト用の合計点を格納する
	m[p].sum += d[cnt].pow;
}
//--- 勝敗を判定（0：親の勝ち　1：子の勝ち -1：引き分け） ---//
int judge(int com,int you)
{
	if(com > 21)	com = 0;	//親がBJ↑　powを0にする
	if(you > 21)	you = 0;	//子がBJ↑　powを0にする

	if(com > you)		//親が子より強い　親の勝ち
		return 0;
	else if(you > com)	//子が親より強い　子の勝ち
		return 1;
	else				//引き分け
		return -1;
}
void animation()
{
	putchar('\n');
	for(int i = 0; i < 10; i++){
		printf("☆☆");
		fflush(0);
		Sleep(100);
	}
	putchar('\n');
}
//--- main関数 ---//
int main(void)
{
	int cnt = CARD_MAX;		//0枚目から順番にカードを引くけど、最初はリセットさせたいのでMAXで初期化
	int tip, bet, more, end, winner;	//ゲーム制御
	//戦績を記憶
	int win = 0, lose = 0, draw = 0;

	char fname[64];
	FILE *fp;

	//山札を定義・初期化
	Card	deck[CARD_MAX];
	card_set(deck);
	//プレイヤー情報を初期化
	Player member[] = {
			{"Computer",{{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0}},0},
			{"Player",{{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0},{0,"0",0}},0},
	};
	//random初期化
	srand(time(NULL));

	printf("■□■□■□■□ BLACKJACK ☆ START ■□■□■□■□\n");

	printf("\n残チップを確認します。\nユーザーネームを入力してください：");		fflush(0);		scanf("%s",fname);

	if((fp = fopen(fname,"w+")) == NULL){
		printf("\n\aデータが見つかりませんでした。\n");
		printf("　プレイヤーファイルを新設します。\n");
		fp = fopen(fname,"w+");
		printf("\n手持ちのチップは？_");		fflush(0);		scanf("%d",&tip);
	}
	else{
		fscanf(fp,"%d",&tip);	//ファイルに残チップを読み込む
		if(tip == 0){
			printf("\n残チップは0です。\n追加のチップは？_");		fflush(0);		scanf("%d",&tip);
		}
		else
			printf("\n繰り越したチップは%d枚です。\n",tip);
	}


	do{
		//引いたカード枚数を初期化
		int you = 0, com = 0;
		//得点を初期化する
		member[0].sum = 0;
		member[1].sum = 0;

		//カードの残数が16より少なければ山札と引いた数をリセットする
		if(CARD_MAX-cnt < 16){
			shuffle(deck);
			cnt = 0;
		}
		//親:0（COM）が2枚引いたあと子:1（YOU）が2枚引く
		for(int i = 0; i < 2; i++){
			card_pick(member,deck,0,com++,cnt++);
		}
		for(int i = 0; i < 2; i++){
			card_pick(member,deck,1,you++,cnt++);
		}
		//現在の状態を表示
		card_print(member,1,you);	//COMの引数が1のとき手札を伏せるようになっている

		do{
		printf("\n賭けるチップは？_");		fflush(0);		scanf("%d",&bet);
		}while(bet > tip);

		//youのターン：手札8枚までカードを追加で引くことができる
		for(int i = 0; you < 8; i++){
			//バーストしていたら追加は引かない
			if(member[1].sum >= BJ)	break;
			//さらにドローするかどうか
			printf("\nさらにカードを引きますか【 Yes…1 / No…0 】：");		fflush(0);		scanf("%d",&more);
			//もし終了する場合は繰り返しから抜ける
			if(more == 0)		break;
			//続ける場合はカードを引く
			card_pick(member,deck,1,you++,cnt++);
			//現在の状態を表示
			card_print(member,1,you);
		}
		//comのターン：手札8回までカードを追加で引くことができる
		for(int i = 0; com < 8; i++){
			//sumが16を越えたら引かないことにする
			if(member[0].sum > 16)	break;
			card_pick(member,deck,0,com++,cnt++);
		}

		animation();

		//リザルトを表示
		card_print(member,com,you);
		putchar('\n');
		winner = judge(member[0].sum,member[1].sum);

		if(winner == 0){
			printf("　　　☆ W I N ☆　　　　　　　　　　L O S E\n");
			printf("　＋---------------＋　　　　　＋---------------＋\n");
			printf("　｜ 親　%-10s｜　　VS　　｜ 子　%-10s｜\n",member[0].name,member[1].name);
			printf("　＋---------------＋　　　　　＋---------------＋\n");
			if(member[winner].sum == BJ){
				printf("　 ☆ BLACK JACK ☆\n");
			}
			else{
			}
			lose++;
			tip-=bet;
			printf("\n　[[[ 増減：-%d / 手持ちのチップ：%d ]]]\n\n", bet, tip);
		}
		if(winner == 1){
			printf("　　　　L O S E 　　　　　　　　　 ☆ W I N ☆\n");
			printf("　＋---------------＋　　　　　＋---------------＋\n");
			printf("　｜ 親　%-10s｜　　VS　　｜ 子　%-10s｜\n",member[0].name,member[1].name);
			printf("　＋---------------＋　　　　　＋---------------＋\n");
			if(member[winner].sum == BJ){
				printf("　　　　　　　　　　　　　　BLACK JACK BONUS !! BET x2\n");
				bet+=bet;
			}
			else{
			}
			win++;
			tip+=bet;
			printf("\n　[[[ 増減：+%d / 手持ちのチップ：%d ]]]\n\n", bet, tip);
		}
		if(winner == -1){
			printf("　　　　　　　　　　　 D R A W\n");
			printf("　＋---------------＋　　　　　＋---------------＋\n");
			printf("　｜ 親　%-10s｜　　VS　　｜ 子　%-10s｜\n",member[0].name,member[1].name);
			printf("　＋---------------＋　　　　　＋---------------＋\n");
			if(member[0].sum == BJ)
				printf("　  ☆ BLACK JACK ☆　　　　　　 ☆ BLACK JACK ☆\n");
			draw++;
			printf("\n　[[[ 増減：±%d / 手持ちのチップ：%d ]]]\n\n", 0, tip);
	}
		if(tip == 0){
			printf("　============ GMAE OVER !! ============\n");
			break;
		}

		printf("\nゲームを続けますか【 Yes…1 / No…0 】：");		fflush(0);		scanf("%d",&end);

	}while(end != 0);


	printf("\n■□■□■□■□ BLACKJACK ☆ E N D ■□■□■□■□\n\n");

	printf("☆ %-8s：戦績 %2d 勝 %2d 敗 %2d 分　残チップ：%d\n\n",member[1].name,win,lose,draw,tip);


	fprintf(fp,"%d\n",tip);	//ファイルに残チップを書き込む
	fclose(fp);

	return 0;
}
