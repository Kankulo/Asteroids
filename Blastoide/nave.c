#include <allegro.h>
#include <math.h>

//Estructura de datos de la nave
typedef struct Nave {
	int posY;
	int posX;
	float ang;
	int activo;
	int vidas;
};
float nave [2][5] = { {0, -8, 0, 8, 0},   //Ordenada x de los puntos que forman la nave
						  {0, -5, 10, -5, 0}}; //Coordenada y de los puntos que forman la nave
void dibuar_nave(float nave[][5], BITMAP *buffer, struct Nave nav){
	int x = nav.posX;
	int y = nav.posY;
	float ang = nav.ang;
	float n_x, n_y; //Coordenadas x e y auxiliares, luego se volveran a volcar a la matriz origial
	int i, cont = 1;
	
	for(i = 0; i <= 4; i++){
		n_x = nave[0][i]*cos(ang) - nave[1][i]*sin(ang);
		n_y = nave[0][i]*sin(ang) + nave[1][i]*cos(ang);
		nave[0][i] = n_x;
		nave[1][i] = n_y;
	}
	
	while ( cont <= 4){

		line(buffer, nave[0][cont-1]+x, nave[1][cont-1]+y, nave[0][cont]+x, nave[1][cont]+y, pallete_color[5]);
		cont ++;

	}
	
}