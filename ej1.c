/*
**    Programa para ilustrar el uso de los hilos
**    y la memoria compartida.
**
**    Compilar con : gcc ej1.c -lpthread -o suma
**
*/

#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 10

#define TAM_ARR 1000

/* Prototipo de la funci�n para que un hilo sume */
void sumar(void *ptr);

/* Este arreglo es compartido por todos los hilos */
int arreglo[TAM_ARR];

/* Esta variable compartida por todos lo hilos indica el inicio del subarreglo para cada hilo */
int inicioSub = 0;

/* Esta es una variable compartida por todos los hilos */
int suma = 0;
/* La variable que usara el semaforo*/
pthread_mutex_t lock[2]; 
int main()
{

  /* Deben declarar un arreglo para manejar los hilos de tipo  pthread_t */
  pthread_t arrHilos[NUM_THREADS];
  for (int i=0;i<2;i++){
    if (pthread_mutex_init(&lock[i], NULL) != 0) { 
      printf("\n mutex init has failed\n"); 
      return 1; 
    }
  }
  
  /* Inicializar el arreglo */
  for(int i=0;i<TAM_ARR;i++){
    arreglo[i]=i+1;
  }
  /* Crear los NUM_THREADS hilos */
  for(int i=0;i<NUM_THREADS;i++){
    pthread_create( &arrHilos[i], NULL, (void *) &sumar, NULL);
  }
  for(int i=0;i<NUM_THREADS;i++){
    pthread_join(arrHilos[i],NULL);
  }
  /* El hilo principal debe mostrar la suma final */
  printf("el resultado es: %d\n",suma);
  printf("el resultado debe ser: %d\n",1000*(1001)/2);
   for (int i=0;i<2;i++){
    pthread_mutex_destroy(&lock[i]);
  }
  return 0;
}


void sumar( void *ptr )
{
  pthread_mutex_lock(&lock[0]); 
  int inicio = inicioSub;
  inicioSub = inicioSub + 100;
  pthread_mutex_unlock(&lock[0]); 
  for(int i=inicio;i<inicio+100;i++){
    pthread_mutex_lock(&lock[1]); 
    suma+=arreglo[i];
    pthread_mutex_unlock(&lock[1]); 
  }
  // printf("[%d] el resultado parcial es: %d\n",(inicio%1000)/100,suma);
  pthread_exit(NULL);
}
