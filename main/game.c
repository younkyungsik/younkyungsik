#include <stdlib.h>	//system() 사용, rand()사용
#include <time.h> 	//타임함수사용
#include "kbhit.h"	//사용자 정의함수 kbhit(), use_getch()...

//리눅스환경에서 gotoxy함수 사용정의(<windows.h>사용불가)
void gotoxy(int x, int y) {
     printf("\033[%d;%df",x,y);
     fflush(stdout);
}

void start();		//게임시작
void screen();		//게임 화면
void create_screen();	//게임화면 생성
void out_screen();	//화면 출력
void move_key(int mov); //키 움직임
void food();		//음식나옴
void body();		//스네이크 몸
void gameover();	//게임오버

//key code
#define LEFT 68
#define RIGHT 67
#define UP 65
#define DOWN 66

#define w 20	//y축 길이
#define h 30	//x축 길이 

int s_body=3;		//스네이크 몸길이
int arr_screen[w][h];	//게임화면
int x=w/2, y=h/2;	//스네이크의 머리 위치
int score = 0;		//점수

clock_t time_start, time_end, food_start ; //clock선언

int main(){
	
	start();
	system("setterm -cursor on");//커서 생성

	return 0;
}

//스네이크 게임 시작 메뉴
void start(){

	int key1;//입력키(전역변수)

	system("setterm -cursor off");//커서없앰
	system("clear");

	gotoxy(10,10); printf("< 스네이크 게임 >");

	gotoxy(30,15); printf("press key");
	gotoxy(40,30); printf("종료 : ESC");


	key1 = use_getch();//키 입력 까지 대기(화면 깜박임X)
		
	if(key1 == 27){
	system("clear");
	exit(0); //프로그램 내 강제종료
	}
	else if(key1 != 27){system("clear");screen();}

		
	return;

}//start()

//게임 틀 생성
void create_screen(){
	//게임 틀 생성
	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			if(i==0 || i==w-1 || j==0 || j==h-1) arr_screen[i][j]=1;
			else{arr_screen[i][j]=0;}
		}
		printf("\n");
	}


	arr_screen[x][y] = 10;	//스네이크의 머리위치를 생성 
	return;
}

//화면 뿌려줌
void out_screen(){

	system("clear");	
	//화면 뿌려줌
	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			if(arr_screen[i][j] == 1) printf("\033[0;35m■ \033[0m");
			else if(arr_screen[i][j] == 3) printf("\033[0;33m♬ \033[0m");
			else if(arr_screen[i][j] == 10) printf("\033[0;32m◆ \033[0m");
			else{printf("  ");}
		}
		printf("\n");
	}
		
	gotoxy(w+2,h/2+10); printf("< 스네이크 게임 >"); gotoxy(0,0);

	gotoxy(w+6,h/2-10); printf("ESC : 메뉴로 나감"); gotoxy(0,0);
	gotoxy(w+7,h/2-10); printf("W : 위"); gotoxy(0,0);
	gotoxy(w+8,h/2-10); printf("S : 아래"); gotoxy(0,0);	
	gotoxy(w+9,h/2-10); printf("A : 왼쪽"); gotoxy(0,0);
	gotoxy(w+10,h/2-10); printf("D : 오른쪽"); gotoxy(0,0);

	gotoxy(w+7,h/2+2);printf("\033[0;35m■ \033[0m : 벽"); gotoxy(0,0);	
	gotoxy(w+8,h/2+2);printf("\033[0;32m◆ \033[0m : 스네이크"); gotoxy(0,0);
	gotoxy(w+9,h/2+2); printf("\033[0;33m♬ \033[0m :먹이"); gotoxy(0,0);

	return ;
}

//음식
void food(){
	int fx = (rand() % (w-2))+1,fy = (rand() % (h-2))+1;//음식 좌표

	arr_screen[fx][fy] += 3;
	food_start = clock();
	return;
}


//스네이크 게임 화면
void screen(){
	x=w/2; y=h/2; //x,y위치 초기화
	int bb=1;//스네이크가 보는 방향
	char key2;//키입력
	double sn_time;//스네이크 움직임 시간 변수

	create_screen(); //게임 틀 생성
	
	food();//먹이생성
	out_screen();//화면 뿌려줌	
	
	while(1){
	time_start = clock();	// 오로지 게임시간 측정	
	time_end = clock();	// 스네이크 움직일때 사용하는 시간
	double sn_time = (double)time_end/CLOCKS_PER_SEC;	// 여기서 사용

		gotoxy(w+4,h/2+10); printf("게임시간 : %ld초",time_start/1000000);
		gotoxy(0,0);
		gotoxy(w+7,h+5); printf("개발용 : %f",sn_time);
		gotoxy(0,0);
		
		gotoxy(w+5,h/2+10); printf("점수 : %d",score); gotoxy(0,0);
				
		gameover();//게임오버인지 확인
		
		// 매 초 스네이크 움직임 구현
//		if(0.5 == (double)time_end/CLOCKS_PER_SEC){//0.5초임
//					
//			move_key(bb); //현재 머리방향 앞으로 진행
//			out_screen();//화면 뿌려줌
//		}

		set_stdin();	//kbhit() 관련함수
		if ((key2 = kbhit()) != -1) {	
			/*
			*  key_mov == 1 : up
			*  key_mov == 2 : down
			*  key_mov == 3 : left
			*  key_mov == 4 : right
			*/
			switch(key2){
				case 'w':
					bb=1;
					move_key(bb);
					out_screen();//화면 뿌려줌	
					break;
				case 's':
					bb=2;
					move_key(bb);
					out_screen();//화면 뿌려줌	
					break;
				case 'a':
					bb=3;
					move_key(bb);
					out_screen();//화면 뿌려줌	
					break;
				case 'd':
					bb=4;
					move_key(bb);
					out_screen();//화면 뿌려줌	
					break;
				default :
					out_screen();
					break;
				} //switch	
      	
			if(key2==27) start();
	
		//key2 = getch();//키 입력 까지 대기(화면 깜박임X)
		
		} //if
		
	} //while	
reset_stdin(); //kbhit()관련함수
	return;
}//screen() : 끝

void body(){
	for(int i=0;i<s_body;i++){
		arr_screen[x][y] += 10;
	}
	return;
}


void move_key(int mov){
	
	/*
	*  mov == 1 : up
	*  mov == 2 : down
	*  mov == 3 : left
	*  mov == 4 : right
	*/
	if(mov == 1){ //up
		arr_screen[x][y] -= 10;
		x--;
		arr_screen[x][y] += 10;
		
		//먹이를 먹었을 때
		if(arr_screen[x][y] == 13){
			arr_screen[x][y] -= 3;//먹이 없앰
			s_body++;	//스네이크몸길이 추가
			out_screen();
		
			score += 100;
			food();//먹이 생성			
		}

	}
	else if (mov == 2){ //down
	
		arr_screen[x][y] -= 10;	
		x++;
		arr_screen[x][y] += 10;
		
		//먹이를 먹었을 때
		if(arr_screen[x][y] == 13){
			arr_screen[x][y] -= 3;//먹이 없앰
			s_body++;	//스네이크몸길이 추가
			out_screen();
			
			score += 100;
			food();//먹이 생성			
		}

	}
	else if(mov == 3){ //left
		arr_screen[x][y] -= 10;
		y--;
		arr_screen[x][y] += 10;
		
		//먹이를 먹었을 때
		if(arr_screen[x][y] == 13){
			arr_screen[x][y] -= 3;//먹이 없앰
			s_body++;	//스네이크 몸길이 추가

			out_screen();
			
			score += 100;
			food();//먹이 생성			
		}

	}
	else if(mov == 4){ //right

		arr_screen[x][y] -= 10;
		y++;
		arr_screen[x][y] += 10;
		
		//먹이를 먹었을 때
		if(arr_screen[x][y] == 13){
			arr_screen[x][y] -= 3;//먹이 없앰
			s_body++;	//스네이크 몸길이 추가
			
			out_screen();
			
			score += 100;
			food();//먹이 생성			
		}

	}
	else{return;}
	return;
}

void gameover(){

	for(int i=0;i<w;i++){
		for(int j=0;j<h;j++){
			if(arr_screen[i][j] == 11) {
				system("clear");
				gotoxy(10,10); printf("< 게임오버 >\n");
				gotoxy(30,15); printf("ESC : 메뉴로 나가기");
				return;
			}
		}
	}

	return ;
}
