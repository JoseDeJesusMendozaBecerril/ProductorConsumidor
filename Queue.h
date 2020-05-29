typedef struct {
    int front, rear,capacity,elements;
    int** q;
}Queue;

int* qPeek(Queue q) {
   return q.q[q.front];
}
int qIsEmpty(Queue q){
    return q.elements==0;
}
int qIsFull(Queue q){
    return q.elements==q.capacity;
}
int qSize(Queue q){
    return q.elements;
}

Queue newQueue(int capacity){
    Queue new;
    new.front=new.rear=0;
    new.capacity=capacity;
    new.q=(int**)calloc(capacity,sizeof(int*));
}

Queue deleteQueue(Queue toDelete){
    free(toDelete.q);
}
/*return 1 if element is queued and if else 0*/
void enqueue(Queue q, int* data){
    if(!qIsFull(q)){
        if(q.rear == q.capacity-1) {
          q.rear = -1;            
        }  
        q.q[++q.rear]=data;
        q.elements++;
    }
}
int* dequeue(Queue q){
    int* data=(q.q[q.front++]);
    if(q.front==q.capacity){ q.front=0; }
    q.elements--;
    return data;
}



