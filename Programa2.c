#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Queue.h"

#define SIZE_UNIVERSO 4

//Prototipos de funciones
int calcularScore(int* s , int** matrizPerfiles);
int* encontrarMotivos(char** adn, int numCadenasADN); //l longitud patron oculto regresa el mejor s
void imprimirS(int* a, int t, int start, int n); //arreglo que se usara, numero de cadenas, start es 0, n es la longitud de la cadena
void generaS(int* a, int t, int start, int n);
void Consummer();

//Variables globales
int tamADN;
int tamMotivo;
int numCadenasADN;
char** cadenasADN;
int* perfilObtenido;//mejor conjunto de S
int hechosP=0,hechosC,totales;
int tamBuffer=10000000;

struct Queue* buffer;
sem_t empty,full,mutex;


//Variables compartidas
int bestScore;

int main(int argc, char const *argv[]){
  //abrir archivo
  FILE *fichero;
  int l;
  char* motivo; 
  int n;
  int t;
  buffer=createQueue(tamBuffer);
  sem_init(&empty,0,tamBuffer);
  sem_init(&full,0,1);
  sem_init(&mutex,0,1);
  
  if(argc>1) fichero = fopen(argv[1], "r");
  else fichero = fopen("data.txt", "r");
  //TAM MOTIVO
  fscanf(fichero, "%d", &tamMotivo);
  motivo = (char*)malloc(tamMotivo*sizeof(char));
  //MOTIVO este es el que se espera encontrar
  fscanf(fichero, "%s",motivo); 
  //TAM CADENA DE ADN 
  fscanf(fichero, "%d", &tamADN); 
  //NUMERO DE CADENAS DE ADN (RENGLONES)
  fscanf(fichero, "%d", &numCadenasADN); printf("Numero cadenas ADN(t): %d\n",numCadenasADN);
  //LECTURA DE RENGLONES DE ADN
  cadenasADN = (char**)malloc(numCadenasADN*sizeof(char*));
  for(int i=0; i < numCadenasADN; i++){ cadenasADN[i] =(char*) malloc(tamADN*sizeof(char));}
  for(int i = 0; i < numCadenasADN; i++ ){ fscanf(fichero, "%s",cadenasADN[i]);}

  printf("Tam Motivo(L): %d\n",tamMotivo);
  printf("Motivo: %s\n",motivo);
  printf("Tam ADN(n): %d\n",tamADN);
  printf("Cadenas de ADN\n");
  for(int i = 0; i < numCadenasADN; i++ ){ for(int j=0; j < tamADN; j++){ printf("%c",cadenasADN[i][j]);}printf("\n" );}
  //Matriz para los mejores resultados de S
  perfilObtenido = (int*) calloc (tamMotivo, sizeof(int)); //se inicializa en 0

  l=tamMotivo;
  n=tamADN;
  t=numCadenasADN;
  totales=pow ((tamADN - tamMotivo) +1 ,numCadenasADN); //(n-l+1)^t

  printf("----------------------------------------------------------\n" );


  
  int* S=(int*)calloc(t,sizeof(int));
  //Empieza productor
  //termina productor
  pthread_t cons[4];
  for(int i=0;i<4;i++) pthread_create(&cons[i], NULL,(void*)&Consummer,NULL);
  generaS(S,t,0,n-l+1);
  for(int i=0;i<4;i++) pthread_join(cons[i], NULL);

  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&mutex);

      return 0;
}

int* encontrarMotivos(char** adn, int numCadenasADN){
  int* s;
  int acarreo = 1;
  int posicion = numCadenasADN - 1;

  s = (int*) malloc(numCadenasADN*sizeof(int));
  int limite = pow ((tamADN - tamMotivo) +1 ,numCadenasADN); //(n-l+1)^t
  for(int i = 0; i < limite; i++){

  }
return s;
}

void generaS(int* a, int t, int start, int n){
  if(start==t){
    //entra para insertar a cola
    //Decide primero si termina
    if(hechosP!=totales){
      hechosP++;
      sem_wait(&empty);
      sem_wait(&mutex);
      int* new=(int*)malloc(t*sizeof(int));
      memcpy(new,a,t*sizeof(int));
      for(int i=0;i<t;i++) printf("%d", new[]);
      printf("\n");
      while (isFull(buffer));
      enqueue(buffer,new);
      //dequeue(buffer);
      sem_post(&mutex);
      sem_post(&full);
      //sale de cola
    }
     

    
  }
  else{
    for(int i=0; i<n; i++){
      a[start]=i;
      generaS(a, t, start+1,n);
    }
  }
}

void imprimirS(int* a, int t, int start, int n){
  if(start==t){

    for(int i=0;i<t;i++) printf("%d", a[i]);
      printf("\n");

  }
  else{
    for(int i=0; i<n; i++){
      a[start]=i;
      imprimirS(a, t, start+1,n);
    }
  }
}


int calcularScore(int* s, int** matrizPerfiles){ //en s vienen los numeros
  int inicioExtraccion = 0;
  for(int i = 0; i < numCadenasADN; i++){
    inicioExtraccion = s[i];
    for(int j=0 ; j < tamMotivo; j++){ //Creacion de matriz de perfiles alineados
      if(cadenasADN[i][inicioExtraccion] == 'A')     {  matrizPerfiles[0][j]++;}
      else if(cadenasADN[i][inicioExtraccion] == 'T'){  matrizPerfiles[1][j]++;}
      else if(cadenasADN[i][inicioExtraccion] == 'G'){  matrizPerfiles[2][j]++;}
      else if(cadenasADN[i][inicioExtraccion] == 'C'){  matrizPerfiles[3][j]++;}
      inicioExtraccion++;
    }
  }

  //Imprimo perfiles alineados
  printf("Perfiles alineados\n");
  for(int i=0; i < SIZE_UNIVERSO; i++ ){
    for(int j = 0; j < tamMotivo; j++){
      printf("%d",matrizPerfiles[i][j]);
    }
    printf("\n" );
  }
  printf("\n" );

  //Sumar valores para obtener el maximo de cada columna y sacar el score
  //printf("Valores maximos de cada columna\n");
  for(int i = 0; i < SIZE_UNIVERSO; i++){
    for(int j = 0; j < tamMotivo; j++){
      //mutex lock
      if(matrizPerfiles[i][j] > perfilObtenido[j]){
      perfilObtenido[j] = matrizPerfiles[i][j];
      }
      //Mutex unlock
    }
  }

  printf("Extraccion de valores que mas repiten \n");
  int puntaje=0;
  for(int i = 0; i < tamMotivo; i++){
    printf("%d ",perfilObtenido[i] );
    puntaje+=perfilObtenido[i];
  }
  printf("\n" );
  return puntaje;
}


void Consummer(){
    //Arreglo para maximos de perfilesAlineados
  int* maximos = calloc (tamMotivo ,sizeof(int));
  int* S;
  // Matriz temporal para perfilesAlineados
  int **matrizPerfiles = (int**) calloc (SIZE_UNIVERSO ,sizeof(int *));
  for(int i=0; i < SIZE_UNIVERSO; i++){ matrizPerfiles[i] =(int*) calloc(tamMotivo,sizeof(int)); }
 /*
  * Entra a cola para sacar S
  */

  sem_wait(&mutex);
  while (hechosC!=totales+1){
    int lc=hechosC;
    hechosC++;
    sem_post(&mutex);

    sem_wait(&full);
    sem_wait(&mutex);
    int* a;
    a=dequeue(buffer);
    int* new=(int*)malloc(numCadenasADN*sizeof(int));
    //memcpy(new,a,numCadenasADN*sizeof(int));
    //for(int i=0;i<numCadenasADN;i++) printf("%d", new[i]);
    //printf("\n");
    sem_post(&mutex);
    sem_post(&empty);
  sem_wait(&mutex);
  }
  sem_post(&mutex);
  /*
 
  

  bestScore = calcularScore(S ,matrizPerfiles);
  printf("\nPuntaje \n%d\n",bestScore );
  S = encontrarMotivos(cadenasADN,tamMotivo);
  
*/
}
/*
int* encontrarMotivos(char** adn,int tamMotivo, int numCadenasADN){
  int* s;
  s = (int*) malloc(numCadenasADN*sizeof(int));
  int limite = pow ((tamADN - tamMotivo) +1 ,numCadenasADN); //(n-l+1)^t

return s;
}
*/
