#include <allegro.h>
#include <math.h>

//Estructura de datos del disparo
typedef struct Blast {
	float posY;
	float posX;
	float incX;
	float incY;
	int activo;	
};

void dibujar_blast (int cant_bla, BITMAP *buffer, struct Blast bla[]){ //cant_bla es el numero de disparos que hay activos
	int i;
	for (i=0; i < cant_bla; i++){
		if (bla[i].activo == 0){ //comprobamos de que el disparo este activo y si no es asi ordenamos el vector de Blasters para que este ordenado
			int x = i;
			while (x < cant_bla){ 
				bla[x] = bla[x+1];
				x++;
			}
			
		}
		else {
			bla[i].posX += bla[i].incX;
			bla[i].posY += bla[i].incY;
			putpixel (buffer, bla[i].posX, bla[i].posY, pallete_color[3]);
		}
	}
}

//Esta funcion ordena los elementos del vectro de tal manera que los que esten activos esten en posiciones contiguas
void ordena_blast (struct Blast bla[], int cant_bla){
	int i;
	int cont=0;
	while (cont < cant_bla){
		if (bla[cont].activo == 0){
			for(i = cont; i < cant_bla-1; i++)
				bla[i] = bla[i+1];
			bla[cant_bla].activo =0;
		}
		cont++;
	}
}