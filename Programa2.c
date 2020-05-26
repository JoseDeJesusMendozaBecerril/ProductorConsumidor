#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <math.h>


#define SIZE_UNIVERSO 4
//Prototipos de funciones
int calcularScore(int* s , char** adn);
int* encontrarMotivos(char** adn,int tamMotivo); //l longitud patron oculto regresa el mejor s
void imprimirS(int* S, int numCadenasADN);

//Variables globales
int tamMotivo; // l

char* motivo;

int tamADN;

int numCadenasADN;

char** cadenasADN;

int** matrizPerfiles;

int* perfilObtenido;

int* maximos;
//Variables compartidas
int bestScore;



int main(){
  FILE *fichero;
  fichero = fopen("data.txt", "r");
  //TAM MOTIVO
  fscanf(fichero, "%d", &tamMotivo); printf("Tam Motivo(L): %d\n",tamMotivo);
  //MOTIVO
  motivo = (char*)malloc(tamMotivo*sizeof(char));
  fscanf(fichero, "%s",motivo); printf("Motivo: %s\n",motivo);
  //TAM CADENA DE ADN
  fscanf(fichero, "%d", &tamADN); printf("Tam ADN(n): %d\n",tamADN);
  //NUMERO DE CADENAS DE ADN (RENGLONES)
  numCadenasADN = 0;
  fscanf(fichero, "%d", &numCadenasADN); printf("Numero cadenas ADN(t): %d\n",numCadenasADN);
  //LECTURA DE RENGLONES DE ADN
  cadenasADN = (char**)malloc(numCadenasADN*sizeof(char*));
  for(int i=0; i < numCadenasADN; i++){ cadenasADN[i] =(char*) malloc(tamADN*sizeof(char));}
  for(int i = 0; i < numCadenasADN; i++ ){ fscanf(fichero, "%s",cadenasADN[i]);}
  for(int i = 0; i < numCadenasADN; i++ ){ for(int j=0; j < tamADN; j++){ printf("%c",cadenasADN[i][j]);}printf("\n" );}
  //Arreglo para maximos de perfilesAlineados
  maximos = calloc (tamMotivo ,sizeof(int));
  // Matriz temporal para perfilesAlineados
  matrizPerfiles = (int**) calloc (SIZE_UNIVERSO ,sizeof(int *));
  for(int i=0; i < SIZE_UNIVERSO; i++){ matrizPerfiles[i] =(int*) calloc(tamMotivo,sizeof(int)); }
  //Matriz para los mejores resultados de S
  perfilObtenido = (int*) calloc (tamMotivo, sizeof(int)); //se inicializa en 0
  printf("----------------------------------------------------------\n" );

  //COMIENZAN PRUEBAS
  int* S;
  S = (int*) malloc(numCadenasADN*sizeof(int));
  S[0] = 0;
  S[1] = 0;
  S[2] = 0;
  printf("\nImprimimos S\n" );
  imprimirS(S,numCadenasADN);
  printf("\n\n" );

  bestScore = calcularScore(S,cadenasADN);
  printf("\nPuntaje \n%d\n",bestScore );
  //S = encontrarMotivos(cadenasADN,tamMotivo);


      return 0;
}

int* encontrarMotivos(char** adn,int tamMotivo){
  int* s;
  int acarreo = 1;
  int posicion = numCadenasADN - 1;

  s = (int*) malloc(numCadenasADN*sizeof(int));
  int limite = pow ((tamADN - tamMotivo) +1 ,numCadenasADN); //(n-l+1)^t
  for(int i = 0; i < limite; i++){

  }
return s;
}

void imprimirS(int* S, int numCadenasADN){
  for(int i = 0; i < numCadenasADN; i++){
    printf("%d ",S[i]);
  }
}




























int calcularScore(int* s,char** cadenasADN){ //en s vienen los numeros
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
          if(matrizPerfiles[i][j] > perfilObtenido[j]){
              perfilObtenido[j] = matrizPerfiles[i][j];
          }
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