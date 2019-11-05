#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>

#define BLOCKED 0
#define RUNNING 1
#define READY	2

int ggt(int m, int n);

int kgv(int m, int n);



typedef struct task { 
   char* name;
   int ptime;
   int ctime;
   int priority;
   int deadline;
   int atime;
   int state;
   int ceu;
   double u;

} task;

void sort (task *ta, int comp, int size);
void simplified();;

int size = 1;
int i = 0;


