#include <string.h>
#include <stdio.h>

#define ECOMPKILLED -2

int fault(char * buffer){

//memset(buffer, 0x00, sizeof(buffer));

     printf("first test\n");
     strcpy(buffer,"killed compartment should stop here");
     printf("second test\n");
     char * p;
     strcpy(p, "always fault");
     return 1;
}