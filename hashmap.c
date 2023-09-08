#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  long posicion = hash(key, map->capacity);
  
  long copiaPos = posicion;

  while(true){
    Pair * aux = map-> buckets[posicion];
    if(aux == NULL || aux->key == NULL){
      Pair * nuevo = createPair(key, value);
      map->buckets[posicion] = nuevo;
      map->size++;
      map->current = posicion;
      return;
    }
    posicion = (posicion + 1) % map->capacity;
    if(copiaPos == posicion) return;
  }
  
}

/*6.- Implemente la función void enlarge(HashMap * map). Esta función agranda la capacidad del arreglo buckets y reubica todos sus elementos. Para hacerlo es recomendable mantener referenciado el arreglo *actual/antiguo* de la tabla con un puntero auxiliar. Luego, los valores de la tabla se reinicializan con un nuevo arreglo con el **doble de capacidad**. Por último los elementos del arreglo antiguo se insertan en el mapa *vacío* con el método *insertMap*.
Puede seguir los siguientes pasos:

a - Cree una variable auxiliar de tipo Pair** para matener el arreglo map->buckets (*old_buckets*);

b - Duplique el valor de la variable capacity.

c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

d - Inicialice size a 0.

e - Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó).*/


void enlarge(HashMap * map) 
{
    enlarge_called = 1; //no borrar (testing purposes)
  
  long dobleCapacidad = (map->capacity)*2; //Duplique el valor de la variable capacity. 
  //Pair ** old_buckets = map->buckets; //mantener referenciado el arreglo *actual/antiguo* de la tabla con un puntero auxiliar.
  map->capacity = dobleCapacidad;
  map->size = 0;
  
  /*for(long i = 0 ; i < dobleCapacidad ; i++)
    {
      Pair * dato = old_buckets[i];
      if(dato != NULL && dato->key != NULL && dato->value!=NULL) 
        insertMap(map, dato->key, dato->value);
    }*/
}


HashMap * createMap(long capacity) 
{
  HashMap * map = (HashMap*) malloc(sizeof(HashMap));
  if(map == NULL) return NULL;

  map->buckets = (Pair **) calloc(sizeof(Pair), 100);
  map->capacity = capacity;
  map->current = -1;
  map->size = 0;
  return map;
}

void eraseMap(HashMap * map,  char * key)
{    
  long posicion = hash(key, map->capacity);
  long auxPos = posicion;

  while(true)
    {
      Pair * aux = map->buckets[posicion];
      if(aux == NULL) return;

      if(strcmp(aux->key, key) == 0){
        aux->key = NULL;
        map->size--;
        return;
      }

      posicion = (posicion + 1)% map->capacity;
      if(posicion == auxPos) return;
    }
  
}

Pair * searchMap(HashMap * map,  char * key) 
{   
  long posicion = hash(key, map->capacity);
  long auxPos = posicion;
  
  while(true)
  {
    if(map->buckets[posicion] == NULL) return NULL;
    
    if(map->buckets[posicion]->key != NULL && strcmp(map->buckets[posicion]->key, key) == 0)
    {
      //Pair * nuevo_par = createPair(key, map->buckets[posicion]->value);
      map->current = posicion;
      return map->buckets[posicion];
    }
    posicion = (posicion + 1) % map->capacity;
    if(posicion == auxPos) return NULL;
  }

  return NULL;
}

Pair * firstMap(HashMap * map)
{
  if(map == NULL ) return NULL;
  for(long i = 0 ; i < map->capacity ; i++)
    {
      if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
        map->current = i;
        return map->buckets[i];
      }
    }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{
  if(map == NULL) return NULL;
  
  long posicion = map->current;
  long i = posicion;
  while(true)
    {
      i = (i + 1) % map->capacity;
      if(i == posicion) return NULL;
   
      if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
        map->current = i;
        return map->buckets[i];
      }
    }
  
  
  return NULL;
}
