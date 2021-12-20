#include <stdio.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <termio.h> 


int use_getch(){ 
int ch; 
struct termios old; 
struct termios new; 
tcgetattr(0, &old); 
new = old; 
new.c_lflag &= ~(ICANON|ECHO); 
new.c_cc[VMIN] = 1; 
new.c_cc[VTIME] = 0; 
tcsetattr(0, TCSAFLUSH, &new); 
ch = getchar(); 
tcsetattr(0, TCSAFLUSH, &old); 
return ch; 
}


struct termios oldterm, new_term;
int ch_read;
void set_stdin(void);
void reset_stdin(void);
int kbhit();
void set_stdin(void)
{
    tcgetattr(0,&oldterm);
    new_term = oldterm;
    new_term.c_lflag &= ~(ICANON | ECHO | ISIG);  // 의미는 struct termios를 찾으면 됨.
    new_term.c_cc[VMIN] = 1;
    new_term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_term);
 
    ch_read = -1;
}
void reset_stdin(void)
{
     tcsetattr(0, TCSANOW, &oldterm);
}
 
// Non-blocking 형태로 키 데이터를 읽고 없으면 -1을 리턴 한다.
// kbhit() 형태로 바꿀 수도 있게죠.
int kbhit()
{
   char ch;
   int nread;
   int ich;
    new_term.c_cc[VMIN] = 0;
    new_term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_term);
    nread = read(0, &ch, 1);
    new_term.c_cc[VMIN] = 1;
    new_term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_term);
    if (nread >= 1) {
      ch_read = (int) ch;
      return ch_read;
    } else {
      return -1;
    }
}


