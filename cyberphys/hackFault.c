#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define ECOMPKILLED -2
#include "FreeRTOS.h"

int counter = 0;

char * tab = "test";

int innerFunct(int x){
    return x+4;
}

int fault2(){
     printf("tab");
     return 1;
}

int fault3(int x){
     x+=1;
     return x;
}

int fault4(char * buf){
   for(int i=0; i < 40; i++){
     buf[i] = 't';
   }
   return 1;
}

int fault5(int x){
    innerFunct(x);
}

int fault6(char * global_buffer){
   for(int i=0; i < 40; i++){
     global_buffer[i] = '6';
   }
   return 1;
}

int fault7(char * global_buffer){
  char buffer[5];
  global_buffer = &buffer;
  return 1;
}

int fault8(){
     counter++;
     return counter;
}

int fault9(int (*funct)()){
     funct();
}


int test(){
     return 1;
}

int fault(char * buffer){
     
     
     //memset(buffer, 0x00, sizeof(buffer));
     printf("first test\n");
     //char buffer2[30];
     strcpy(buffer,"check access to variable");
     printf("second test\n");
     if(counter == 0){
          counter++;
          char * p=NULL;
          strcpy(p, "always fault");
     }
     strcpy(buffer, "testing after killed behavior");
     //printf("buffer2:%s\n",buffer2);
     
     return 1;
}

static void CheriFreeRTOS_FaultHandler(void* pvParameter1, uint32_t comp_id) __attribute__((used)) {
     printf("HANDLER HACKFAULT CALLED\n");
     return;
}
