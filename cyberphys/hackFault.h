#include <string.h>
#include <stdio.h>
int innerFunct(int x);
int fault(char * buffer);
int fault2();
int fault3(int x);
int fault4(char buf[50]);
int fault5(int x);
int fault6(char * global_buffer);
int fault7(char * global_buffer);
int fault8();
int fault9(int (*func)(void));
//void fault();