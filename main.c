#include "scheduling.h"

/*
 *	exercise 1.3
 *	Real-time Systems
 *	Rate monotonic scheduling*
 *	Author: Ibrahim Cinar
 */

int ggt (int m, int n)
{
	if (n==0)
	  return m;
	else
	  return ggt(n, m%n);
}

int kgv(int m,int n)
{
	int o = ggt(m,n);
	int p = (m * n) /o;
	return p;
}

void sort (task *ta, int comp, int size)
{
	int i, j;
	task tmp;
	for (i = 0; i < size; i++) {	
		for (j = 0; j < size; j++) {	
			if (comp == 1) { 
				if (ta[i].ptime < ta[j].ptime) {		
					tmp = ta[i];
					ta[i] = ta[j];
					ta[j] = tmp;
				}
			} else if (comp == 2) {
				if (ta[i].deadline < ta[j].deadline) {
					tmp = ta[i];
					ta[i] = ta[j];
					ta[j] = tmp;
				}
			}	
		}
	}			

}


int exact_rta(task *ta, int n) 
{
  int i;
  int resp = ta[n].ctime;
  int tmp = 0;
  int ct = ta[n].ctime;
  double s;
  while (resp != tmp) {
  	tmp = resp;
	s = 0;
	for (i = 0; i <= n; i++) {
		s = s+ (ceil((double)resp / (double)ta[i].ptime)*(double) ta[i].ctime);
		resp = ct + (int)s;
	 	
	}
  }
  printf("EXACT RESPONSE TIME %s: %d\n",ta[n].name, resp);
  return resp;
 }


int simplified_rta(task *ta, int n)
{
  int i;
  int resp = ta[n].ctime;
  int ct = ta[n].ctime;
  double s = 0;
  int tmp = 0;
  while (tmp != resp) {	
  	tmp = resp;
	s = 0;
	for (i = 0; i <= n-1; i++) {
		s = s + (ceil((double) ta[n].deadline/ (double)ta[i].ptime)*(double)ta[i].ctime);
  		resp = ct + (int)s;
 	}
  }
   
  printf("SIMPLIFIED RESPONSE TIME %s:  %d\n",ta[i].name, resp);

  return resp;
}


void swap (task *ta ,int a, int b)
{
	task t; 
	
	t  = ta[b];
	ta[b] = ta[a];
	ta[a] = t;
}

int main(void)
{

  task *t;
  printf("\n\n(1)Rate Monotonic Scheduling \n(2)Deadline Monotonic Scheduling\n");

  //number maximum of one char
  char *scheduling = malloc(sizeof(char));
  scanf(" %s", scheduling);
  int sched = atoi(scheduling);

  //filename with maximum 255 letters
  char *filename = malloc (255 * sizeof(char)); 
  printf("Textdatei für das Parsen: ");
  scanf("%s",filename);

  //open text file
  char line[128]; 		//maximum line size of textfile
  FILE *file;			// to open file
  char trennzeichen[] = "\t";   //trennzeichen zwischen die Werten
  	
  file = fopen(filename,"r");	//textdatei wird geöffnet
  
  if (file == NULL)
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }
  

  //Textdatei parsen
  char *wort;	//einzelne String in textdatei 
  
  t = malloc(size * sizeof(task)); //Werte und Taskname wird dort gespeichert

  //Zeilenweise lesen einer Textdatei
  while (fgets(line, sizeof(line), file)) {
	//falls es mehr als eine Zeile gibt wird die groeße der t verändert (mehr als ein Task)
        if (i > 0){
		size++;
		t = realloc(t, size * sizeof(task));
	}		
	//Name der Task:
	wort = strtok(line, trennzeichen);
	t[i].name = malloc(strlen(wort) * sizeof(char));
	strcpy(t[i].name,wort);
	
	//Periodendauer der Task	
	wort = strtok(NULL, trennzeichen);
	t[i].ptime =(int) atoi(wort);	
	
	//Ausführungszeit der Task
	wort = strtok(NULL, trennzeichen);	
	t[i].ctime =(int) atoi(wort);
	
	//Deadline für DMS eines Tasks
	if (sched == 2) {
		wort = strtok(NULL, trennzeichen);
		t[i].deadline = (int) atoi(wort);
	}
	t[i].state = READY;	
	i++;	
  } 
  fclose(file); 
  
  int j;   
  
  //Tasks werden nach Periodenzeit sortiert
  sort(t,sched,size);

 
  //sortierte Tasks mit Prioritäten versehen und die Utilization berechnet
  double comp, ll = 0;
  i = 1;
  for (j = size-1; j>=0; j--)
  {
 	t[j].u = (double)t[j].ctime / (double)t[j].ptime;	//Berechnung der Utilization eines Tasks
	ll = ll + t[j].u;					//Summe der Utilization
  	t[j].priority = i;					//Prioritätenvergabe
	if (j < size-1)						//für den Fall, dass mehrere Tasks den gleichen Periodenzeit haben (gleiche Prio)
		if (t[j].ptime == t[j+1].ptime)
			t[j].priority = t[j+1].priority;
 	i++;
  }

  
  if (sched == 1) {	
  	comp = size * (pow(2, 1/(double)size)-1);	//Liu und Layland Formel Annäherung von 0.6999 (ln 2)

  	//Anwendung Liu und Layland
  	printf("\nLiu and Layland: ");
  	if (ll <= comp)
		  printf("Schedulable!\n");
  	else
		  printf("Not schedulable!\n");
   
  //Darstellung der Tabelle laut Aufgabe 1.3:

  	printf("\nTASK:\t|PTIME:\t|CTIME:\t|PRIO:\t|UTILIZATON:\n");
  	printf("-------------------------------------------\n");
  	for (i=0; i<size; i++) {
		printf(" %s\t| %d\t| %d\t| %d\t| %f\n",t[i].name,t[i].ptime,t[i].ctime, t[i].priority, t[i].u);
		printf("\t|\t|\t|\t|\n");	
  	}
  }
  else if (sched == 2) {
  	printf("\nTASK:\t|PTIME:\t|CTIME:\t|DEADLINE:\t|PRIO:\t|UTILIZATON:\n");
  	printf("------------------------------------------------------------\n");
  	for (i=0; i<size; i++) {
		printf(" %s\t| %d\t| %d\t| %d\t\t| %d\t| %f\n",t[i].name,t[i].ptime,t[i].ctime,t[i].deadline, t[i].priority, t[i].u);
		printf("\t|\t|\t|\t\t|\t|\n");	
  	}


  }
  //kgv von alle Tasks:
  int maxt = t[0].ptime;
  
  if (size > 1)
   for (i = 1; i<size; i++) {
	maxt = kgv(maxt,t[i].ptime);	
   }

  printf("KGV: %d\n\n", maxt);

  //TODO: Exercise 2.1 
   
  printf("Timeline: ");
  i = 0;		
  j = 0;		//current time unit
  int cp = -1;		//current process
  while (j < maxt) {
	//printf("%d\n", j);	
	for (i = 0; i < size; i++){ 
	   if ((j+1) % t[i].ptime == 0)
		   t[i].state = READY;
	   if (t[i].state == READY && t[i].atime <= j) {
		printf("%s", t[i].name);
		t[i].state = RUNNING;
		cp = i;  
		break;
	  }
	}
	if (cp > -1) {
		t[cp].ceu++;
		if (t[cp].ceu == t[cp].ctime) {
			if ((j+1) % t[cp].ptime != 0)
				t[cp].state = BLOCKED;
			else
				t[cp].state = READY;
			t[cp].atime += t[cp].ptime;
			t[cp].deadline = t[cp].atime;
			t[cp].ceu = 0;
	
		}
	}
        for (i = 0; i < size; i++) {
		if (t[i].deadline < j) {
	//		printf("Ende\n");
	//		free(t);
	//		return 0;
		}
	}
    j++;
  }
  printf("\n");
  // Exercise 2.2
 
  for (i = 0; i < size; i++) { 
 	exact_rta(t,i);
  	simplified_rta(t,i);
  } 
  
  // Exercise 2.3
  int ok; 
  for (i = 0; i < size; i++) {
	for (j = i; j < size; j++) {
		swap(t,i,j);
		ok = exact_rta(t,i);
		if (ok <= t[i].deadline)
				break;
	}
	if (ok >= t[i].deadline) {
		printf("\nkeine Lösung gefunden\n");
		break;
	}
  } 

  printf("\n\n"); 
  free(t);
  return 0;
}
