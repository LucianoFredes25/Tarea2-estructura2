#include "list.h"
#include "hashmap.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  char nombre[50];
  int cantidad;
}Item;

typedef struct{
  char nombre[50];
  int PA; //puntos de habilidad
  int CantidadItems;
  HashMap* mapItems;
}Jugador;

typedef struct{
  int tipo;
  char nomAux[50];
  char itemAux[50];
  int PAux;
}Accion;


void mostrarMenu() {
  printf("\n---  Juego?  ---\n");
  printf("[1] Registrar Jugador.\n");
  printf("[2] Mostrar perfil de Jugador.\n");
  printf("[3] Agregar item a Jugador.\n");
  printf("[4] Eliminar item a Jugador.\n");
  printf("[5] Agregar puntos de habilidad a Jugador.\n");
  printf("[6] Mostrar jugadores con un item especifico.\n");
  printf("[7] Deshacer ultima accion del jugador.\n");
  printf("[8] Exportar Jugadores.\n");
  printf("[9] Cargar datos de Jugadores.\n");
  printf("[10] Salir.\n");
  printf("Seleccione una opción: ");
}

Jugador* obtenerDatosJugador(){
  
  Jugador* newJ = (Jugador *) calloc(1, sizeof(Jugador));
  printf("Nombre : ");
  scanf("%s", newJ->nombre);
  getchar();
  newJ->PA = 0;
  newJ->CantidadItems = 0;
  newJ->mapItems = createMap(100);

  return newJ;
}

void mostrarItem(Item* item){
  printf("%s: %d", item->nombre, item->cantidad);
  //printf("%s", item->nombre);
}

Accion* createAccion(int tipo, char nombre[50], char item[50], int PA){
  Accion* newA = calloc(1, sizeof(Accion));
  newA->tipo = tipo;
  strcpy(newA->nomAux, nombre);
  strcpy(newA->itemAux, item);
  newA->PAux = PA;

  return newA;
}

void imprimirDatosJugador(Jugador* jugador){
  printf("\nPerfil de %s :\n\nPuntos de Habilidad = [%d] \nCantidad de Items = [%d]\n\n", jugador->nombre, jugador->PA , jugador->CantidadItems);
  if(jugador->CantidadItems == 0)
    printf("No hay items asignados.\n");
  else{
    Pair* it = firstMap(jugador->mapItems);
    while(it != NULL){
      mostrarItem(it->value);
      it = nextMap(jugador->mapItems);
      if(it != NULL) printf("\n");
    }
  }
}

HashMap* getMapJ(Jugador* iyo){
  return iyo->mapItems;
}

//Opcion 1 Registrar jugador

void registrarJugador(HashMap* mapJugadores){
  Jugador* newJ = obtenerDatosJugador();
  insertMap(mapJugadores, newJ->nombre, newJ);
  printf("Jugador registrado correctamente!\n");
}

//Opcion 2 Mostrar perfil jugador

void mostrarJugador(HashMap* mapJugadores){

  char key[50];
  printf("Ingrese el nombre del jugador: ");
  scanf("%s", key);
  
  Pair* player = searchMap(mapJugadores, key);
  
  if(player == NULL){
    printf("--- NO se encontro al jugador --- \n");
  }else{
    imprimirDatosJugador(player->value);
  }
  
}

//Opcion 3 agregar item
void agregarItem(HashMap* mapJugadores, List* listaAcciones, int reverse, char nomAux[50], char itemAux[50])
{
  char blank[50];
  char keyItem[50];
  char key[50];
  if(!reverse){
    printf("Ingrese el nombre del jugador: ");
    scanf("%s", key);
  }else strcpy(key, nomAux);
  
  Pair* player = searchMap(mapJugadores, key);

  if(player == NULL){
    printf("--- NO se encontro al jugador --- \n");
    return;
  }
  
  if(!reverse){
    printf("Nombre del item: ");
    fgets(blank, 50, stdin);
    fgets(keyItem, 50, stdin);
    keyItem[strcspn(keyItem, "\n")] ='\0';
  }else strcpy(keyItem, itemAux);

  HashMap* mapita = getMapJ(player->value);

  Pair* bPar = searchMap(mapita, keyItem);
  
  if(bPar == NULL){
    Item* new = calloc(1, sizeof(Item));
    strcpy(new->nombre, keyItem);
    new->cantidad =  1;
    insertMap( ((Jugador*)(player->value))->mapItems, new->nombre, new);
  }else{
    ((Item*)(bPar->value))->cantidad++;
  }

  if(!reverse){
    Accion* newA = createAccion(1, key, keyItem,0);
    pushBack(listaAcciones, newA);
  }
  
  ((Jugador*)(player->value))->CantidadItems++;

  if(!reverse) printf("--- Se agrego correctamente el item!!! --- \n"); 
}

//opcion 4 Eliminar item a jugador

void eliminarItem(HashMap* mapJugadores, List* listaAcciones, int reverse, char nomAux[50], char itemAux[50]){
  char blank[50];
  char keyItem[50];
  char key[50];
  if(!reverse){
    printf("Ingrese el nombre del jugador: ");
    scanf("%s", key);
  }else strcpy(key, nomAux);
  
  Pair* player = searchMap(mapJugadores, key);

  if(player == NULL){
    printf("--- NO se encontro al jugador --- \n");
    return;
  }
  
  if(!reverse){
    printf("Nombre del item: ");
    fgets(blank, 50, stdin);
    fgets(keyItem, 50, stdin);
    keyItem[strcspn(keyItem, "\n")] ='\0';
  }else strcpy(keyItem, itemAux);

  HashMap* mapita = getMapJ(player->value);

  Pair* bPar = searchMap(mapita, keyItem);
  
  
  if(bPar == NULL){
    printf("--- NO se encontro el item!!! --- \n");
    return; 
  }

  
  ((Item*)(bPar->value))->cantidad--;
  ((Jugador*)(player->value))->CantidadItems--;
  
  if(((Item*)(bPar->value))->cantidad == 0){
    eraseMap(((Jugador*)(player->value))->mapItems, keyItem);
  }

  if(!reverse){
    Accion* newA = createAccion(2, key, keyItem,0);
    pushBack(listaAcciones, newA);
  }
  
  if(!reverse) printf("--- Se borro correctamente el item!!! --- \n"); 
}

//opcion 5 Agregar PA
void asignarPuntos(HashMap * mapJugadores, List* listaAcciones, int reverse, char nomAux[50], int puAux){
  
  char key[50];
  int puntos;

  if(!reverse){
    printf("Ingrese el nombre del jugador: ");
    scanf("%s", key);
  }else strcpy(key, nomAux);

  Pair* player = searchMap(mapJugadores, key);

  if(player == NULL){
    printf("--- NO se encontro al jugador --- \n");
    return;
  }

  if(!reverse){
    printf("Ingrese puntos a añadir : ");
    scanf("%d", &puntos);
  }else puntos = puAux; 

  ((Jugador*)(player->value))->PA += puntos;

  if(!reverse){
    Accion* newA = createAccion(3, key, "", puntos*-1);
    pushBack(listaAcciones, newA);
  }
  
  if(!reverse) printf("Puntos añadidos correctamente!\n");
}
  

//opcion 6 mostrar jugadores con un item especifico

void itemEspecifico(HashMap* mapJugadores){
  char blank[50];
  char keyItem[50];
  
  printf("Nombre del item: ");
  fgets(blank, 50, stdin);
  fgets(keyItem, 50, stdin);
  keyItem[strcspn(keyItem, "\n")] ='\0';

  Pair* it = firstMap(mapJugadores);

  long cont = 0;

  
  while(it!=NULL){

    HashMap* mapita = getMapJ(it->value);

    Pair* bPar = searchMap(mapita, keyItem);

    if(bPar != NULL){
      printf("-> %s \n", ((Jugador*)(it->value))->nombre ); 
      cont++;
    }
    
    it = nextMap(mapJugadores);
  }

  printf("se encontraron %ld jugadores con el item.\n", cont);
  
}

//opcion 7 deshacer ultima accion
void deshacerAccion(HashMap* mapJugadores, List* listaAcciones){
  

  Accion* action = lastList(listaAcciones);

  if(action == NULL){
    printf("--- NO hay acciones para deshacer ---\n");
    return;
  }

  if(action->tipo == 1){
    eliminarItem(mapJugadores, listaAcciones, 1, action->nomAux, action->itemAux);
  }else if(action->tipo == 2){
    agregarItem(mapJugadores, listaAcciones, 1, action->nomAux, action->itemAux);
  }else{
    asignarPuntos(mapJugadores, listaAcciones, 1, action->nomAux , action->PAux); 
  }

  popBack(listaAcciones);
  printf("--- Accion deshecha correctamente ---\n");
    
}

//opcion 8 exportar
void exportarDatos(HashMap* mapJugadores){
  char nombreArchivo[50];

  printf("Ingrese el archivo a exportar: ");
  scanf("%s", nombreArchivo);

  FILE* archivo = 
    fopen(nombreArchivo, "w");
  if (archivo == NULL){
    printf("Error al abrir el archivo de exportación \n");
    return;
  }

  fprintf(archivo, "Nombre, Puntos de habilidad, #items\n");

  Pair* it = (Pair*) firstMap(mapJugadores);

  while (it != NULL){
    Jugador* jugador = (Jugador*)it->value;
    fprintf(archivo,"%s,%d,%d",jugador->nombre, jugador->PA, jugador->CantidadItems);

    Pair* parDelitem = (Pair*) firstMap(jugador->mapItems);
    while (parDelitem != NULL){
      fprintf(archivo, ",%s", parDelitem->key);
      parDelitem = nextMap(jugador->mapItems);
    }
    fprintf(archivo, "\n"); 
    it = nextMap(mapJugadores);
  }
  fclose(archivo);
  printf("Datos exportados correctamente en el archivo '%s'\n", nombreArchivo);
}
const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }

    return NULL;
}
//Opcion 9 importar 
void importarDatos(HashMap* mapJugadores) {
  FILE *archivo;
  char nombreArchivo[50];
  char linea[1024];
  Item *it;

  printf("Ingrese el nombre del archivo a importar: ");
  scanf("%s", nombreArchivo);
  archivo = fopen(nombreArchivo, "r");

  if (archivo == NULL) {
    printf("Error al abrir el archivo.\n");
    return;
  }
  
  fgets(linea, 1024, archivo); 

  while (fgets(linea, 1024, archivo)) {
    char *token;
    char *cualEs;
    int contCampos = 0;
    
    Jugador *jugador = (Jugador *)calloc(1, sizeof(Jugador));
    jugador->mapItems = createMap(100);
    token = get_csv_field(linea, contCampos);
    
    while (token != NULL) {
      cualEs = token;
      switch (contCampos) {
      case 0:
        strcpy(jugador->nombre, cualEs);
        break;
      case 1:
        jugador->PA = atoi(cualEs);
        break;
      case 2:
        jugador->CantidadItems = atoi(cualEs);
        break;
      default:
        it = (Item *) calloc(1, sizeof(Item));
        strcpy(it->nombre, cualEs);
        insertMap(jugador->mapItems, it->nombre, it); 
        it->cantidad++;       
        break;
      }
      contCampos++;
      token = get_csv_field(linea, contCampos);
    }
    insertMap(mapJugadores, jugador->nombre, jugador);
  }
  printf("Archivo importado correctamente\n");
  fclose(archivo);
}

int main() {

  int opcion = 10;
  
  HashMap* mapJugadores = createMap(5000);
  List* listaAcciones = createList();

  while (true) {

    mostrarMenu();
    
    scanf("%d", &opcion);
    printf("\n");
    
    switch(opcion){
      case 1:
        registrarJugador(mapJugadores);
        break;
      case 2:
        mostrarJugador(mapJugadores);
        break;
      case 3:
        agregarItem(mapJugadores, listaAcciones, 0, "", "");
        break;
      case 4:
        eliminarItem(mapJugadores, listaAcciones, 0, "", "");
        break;
      case 5:
        asignarPuntos(mapJugadores, listaAcciones, 0, "", 0);
        break;
      case 6:
        itemEspecifico(mapJugadores);
        break;
      case 7:
        deshacerAccion(mapJugadores, listaAcciones);
        break;
      case 8:
        exportarDatos(mapJugadores);
        break;
      case 9:
        importarDatos(mapJugadores);
        break;
      case 10:
        printf("Programa Finalizado\n");
        break;
      default:
        printf("Opcion Invalida, ingrese nueva opcion\n");
    }
    if(opcion == 10)
      break;
  }

  return 0;
}
