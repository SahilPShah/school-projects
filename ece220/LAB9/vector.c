#include "vector.h"
#include <stdlib.h>


vector_t * createVector(int initialSize)
{
  //allocate
  vector_t *avector=malloc(sizeof(vector_t));
  avector->array=malloc(initialSize*sizeof(int));
  //initialize
  avector->maxSize=initialSize;
  avector->size=0;
  return avector;
}

void destroyVector(vector_t * vector)
{
  //de-allocate
  free(vector->array);
  free(vector);
}

void resize(vector_t * vector)
{
  //reallocate -- double the size of the array
  vector->array=realloc(vector->array,2*vector->maxSize*sizeof(int));
  vector->maxSize*=2;
}

void push_back(vector_t * vector, int element)
{
  //if the size of the array is less than the max capacity
  if((vector->size)<(vector->maxSize)){
    vector->size++;
  }
  else{//resize the array if there isnt enough space
    resize(vector);
    vector->size++;
  }
  vector->array[vector->size-1]=element;
}

int pop_back(vector_t * vector)
{
  //check if the size is greater than zero
  if(vector->size>0){
    vector->size--;
    return vector->array[vector->size];
  }
  return 0;//if the array is empty
}

int access(vector_t * vector, int index)
{
  if(index>0 && index<vector->size){//index check
    return vector->array[index];
  }
  //the index is out of bounds
  return 0;
}
