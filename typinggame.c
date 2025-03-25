#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define Q_NUM 100
#define A_NUM 10
#define WORD 128

int main(void)
{
	int cnt = 0, w_cnt = 0, correct = 0;
	double a_time;
	FILE	*fp;

	char DATA[Q_NUM][WORD];
	char Q[A_NUM][WORD];
	char A[A_NUM][WORD];

	if((fp = fopen("question.txt","r")) == NULL){
		printf("\a出題ファイルをオープンできませんでした。\n");
	}
	else{
		while(fscanf(fp,"%s",DATA[cnt]) > 0){
			cnt++;
		}
		fclose(fp);
	}
	//random初期化
	srand(time(NULL));

	int random;
	for(int i = 0; i < A_NUM; i++){
		random = rand() % cnt;

		strcpy(Q[i],DATA[random]);
		if(random != cnt-1)
			strcpy(DATA[random],DATA[--cnt]);
	}

	printf("◆◆◆ タイピング練習　10本ノック ◆◆◆　◎ 問題と同じ文字を入力してください\n\n");

	time_t start_time;
	start_time = time(NULL);

	for(int i = 0; i < A_NUM; i++){
		printf("問題 %2d：%s >> ",i+1,Q[i]);
		fflush(0);
		scanf("%s",A[i]);
	}

	a_time = difftime(time(NULL),start_time);

	printf("\n　■■■■■■■■■■■■■■■\n");
	printf("　■　～～～ 成績発表 ～～～　■\n");
	printf("　■■■■■■■■■■■■■■■\n");

	for(int i = 0; i <= 9; i++){
		printf("\n問題 %2d：%s\n→ 回答：%s",i+1,Q[i],A[i]);
			w_cnt += strlen(Q[i]);
		if(strcmp(Q[i],A[i]) == 0){
			correct++;
			printf("\n　+--------------------+\n");
			printf("　|　Congratulation !　|\n");
			printf("　+--------------------+\n");
		}
		else{
			printf("\n　+----------+\n");
			printf("　|　miss !　|\n");
			printf("　+----------+\n");
		}
	}
	putchar('\n');
	printf("　■■■■■■■■■■■■■■■■\n");
	printf("　■　正答数　　　： %2d / 10　　■\n",correct);
	printf("　■　文字数　　　： %6d　　 ■\n",w_cnt/3);
	printf("　■　回答タイム　： %2d分%4.1f秒 ■\n", (int)a_time/60, a_time-(int)a_time/60*60);
	printf("　■　タイム/文字 ： %4.1f秒　　 ■\n", a_time/(w_cnt/3));
	printf("　■■■■■■■■■■■■■■■■\n");
	return 0;
}
