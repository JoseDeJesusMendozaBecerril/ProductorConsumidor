#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Queue.h"

#define SIZE_UNIVERSO 4
#define NUM_THREADS 4

//Prototipos de funciones
int calcularScore(int* s );
int* encontrarMotivos(char** adn, int numCadenasADN); //l longitud patron oculto regresa el mejor s
void imprimirS(int* a, int t, int start, int n); //arreglo que se usara, numero de cadenas, start es 0, n es la longitud de la cadena
void generaS(int* a, int t, int start, int n);
void Consummer();
void imprimirMotivo(int* S);

//Variables globales
int debeSalir=0;
int tamADN;
int tamMotivo;
int numCadenasADN;
char** cadenasADN;
char* consensus;
int* perfilObtenido;//mejor conjunto de S
int hechosP=0,hechosC=0,totales;
int tamBuffer=10;

struct Queue* buffer;
sem_t empty,full;
sem_t mutex[10];

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
  sem_init(&full,0,0);
  for (int i = 0; i < 10; i++) sem_init(&mutex[i],0,1);

  if(argc>1) fichero = fopen(argv[1], "r");
  else fichero = fopen("datosPrueba.dat", "r");
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
  perfilObtenido = (int*) calloc (numCadenasADN, sizeof(int)); //se inicializa en 0

  l=tamMotivo;
  n=tamADN;
  t=numCadenasADN;
  consensus=(char*)malloc((l+1)*sizeof(char));
  consensus[l]='\0';
  totales=pow ((tamADN - tamMotivo) +1 ,numCadenasADN); //(n-l+1)^t

  printf("----------------------------------------------------------\n" );

  int* S=(int*)calloc(t,sizeof(int));
  //S[0] = 1;
  //S[1] = 2;
  //S[2] = 3;
  //S[3] = 2;
  //S[4] = 0;
  //int score = calcularScore(S);
  //printf("%d\n",score );


  //Empieza productor
  //termina productor

  pthread_t cons[NUM_THREADS];
  for(int i=0;i<NUM_THREADS;i++) pthread_create(&cons[i], NULL,(void*)&Consummer,NULL);
  generaS(S,t,0,n-l+1);
  //for(int i=0;i<NUM_THREADS;i++) pthread_join(cons[i], NULL);
  while (hechosC!=totales);
  sem_destroy(&empty);
  sem_destroy(&full);
  for (int i = 0; i < 10; i++) sem_destroy(&mutex[i]);
  printf("\n\nConjunto S obtenido: ");
  imprimirMotivo(perfilObtenido);
  printf("\n");
      return 0;
}



void generaS(int* a, int t, int start, int n){
  if(start==t){
    //entra para insertar a cola
    //Decide primero si termina
    if(hechosP!=totales){
      hechosP++;
      sem_wait(&empty); // - SE DUERME SI SE HACE NEGATIVO , SI SE LLENA NO HAY VACIOS ENTONCES SE BLOQUEA
      sem_wait(&mutex[0]); // - BLOQUEO
      int* new=(int*)malloc(t*sizeof(int));
      memcpy(new,a,t*sizeof(int));
      //for(int i=0;i<t;i++) printf("%d", new[i]);
      //printf("\n");
      enqueue(buffer,new);
      //dequeue(buffer);
      sem_post(&mutex[0]); // + DEBLOQUEO
      sem_post(&full);  // +
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

void imprimirMotivo(int* s){
  for(int i=0; i < numCadenasADN; i++){
    printf(" %d ",s[i]);
  }
}

int calcularScore(int* s){ //en s vienen los numeros
  int **matrizPerfiles = (int**) calloc (SIZE_UNIVERSO ,sizeof(int *));
  for(int i=0; i < SIZE_UNIVERSO; i++){ matrizPerfiles[i] =(int*) calloc(tamMotivo,sizeof(int)); }
  int *perfil= (int*) calloc (tamMotivo, sizeof(int));

  for (int i = 0; i < SIZE_UNIVERSO; i++) {
      for(int j = 0; j < tamMotivo; j++){
          matrizPerfiles[i][j] = 0;
      }
  }



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
/*
  for (int i = 0; i < numCadenasADN; ++i){
    for (int j = 0; j < tamMotivo; ++j)
    {
     printf("%d ",matrizPerfiles[i][j]);
    }
    printf("\n");
  }
*/
  //Sumar valores para obtener el maximo de cada columna y sacar el score
//  printf("Valores maximos de cada columna\n");
  for (int i = 0; i < tamMotivo; i++){
    perfil[i] = 0;
  }

  for(int i = 0; i < SIZE_UNIVERSO; i++){
    for(int j = 0; j < tamMotivo; j++){
      sem_wait(&mutex[1]); //mutex lock
      if(matrizPerfiles[i][j] > perfil[j]){
        perfil[j] = matrizPerfiles[i][j];
      }
      sem_post(&mutex[1]); //Mutex unlock
    }
  }


  //printf("Extraccion de valores que mas repiten \n");
  int puntaje = 0;
  for(int i = 0; i < tamMotivo; i++){
    //printf("%d ",perfilObtenido[i] );
    puntaje+=perfil[i];
  }
  
  //printf("\n" );
  free(perfil);
  for(int i=0; i < SIZE_UNIVERSO; i++){ free(matrizPerfiles[i]);}
  free(matrizPerfiles);


  return puntaje;
}


void Consummer(){
    //Arreglo para maximos de perfilesAlineados
  //int* maximos = calloc (tamMotivo ,sizeof(int));
  int* S; //Este es el que se va recorriendo
  // Matriz temporal para perfilesAlineados

 /*
  * Entra a cola para sacar S
  */
  sem_wait(&mutex[2]);
  while (hechosC!=totales){
    sem_post(&mutex[2]);

    sem_wait(&full);
    sem_wait(&mutex[0]);
    int* a=dequeue(buffer);
    if(a!=NULL){
      S=a;
      hechosC++;
    }
    sem_post(&mutex[0]);
    sem_post(&empty);

    //imprimirMotivo(S);
    int locScore;
    if(S!=NULL){

      locScore= calcularScore(S);
      //printf("LS_%d",locScore );

      //printf("\n");

      sem_wait(&mutex[4]);
      if (locScore>bestScore){
        bestScore=locScore;
        memcpy(perfilObtenido,S,numCadenasADN*sizeof(int));
        printf("\nBS:_%d",locScore);
        imprimirMotivo(S);
        printf("\n");
      }
      

      sem_post(&mutex[4]);
    }

    free(S);


  sem_wait(&mutex[2]);
  }
  sem_post(&mutex[2]);
  //printf("_%d_c\n",hechosC);

}
