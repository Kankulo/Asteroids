#include <allegro.h>
#include <math.h>

//Constantes basicas del juego
#define INC 5
#define ANCHO 1200
#define ALTO 800
#define CONT 10

//Estructura de datos del Asteroide
typedef struct Asteroid{
	float posX;
	float posY;
	float incX;
	float incY;
	float ang;
	int tam;
	int activo;
};

#define PI 3.1416

float asteroide [2][12] = { {0, 5, 4, 4, 3, -1, -2, -4, -7.5, -6, -5, 0}, //Idem que con la nave.
								{6, 4, 3, 0, -3, -4, -1, -2, 1, 2, 5, 6}};
//Rutina que actualiza las coordenadas del asteroide, lo dibuja y hace que gire
void dibujar_asteroide (float asteroide[][12], BITMAP *buffer, struct Asteroid ast){
	
	int tam = ast.tam;
	int x = ast.posX;
	int y = ast.posY;

	float ang = ast.ang*(PI/180); // Determina la velocidad del giro del asteriode tanto rotacion como traslacion
	float n_x, n_y;//variables que alojaran los datos temporalmente
	int i, cont = 1;
	 //centro de rotacion del asteroide. Mas tarde meterle incrementos para que esl asteroide se mueva

	for (i = 0; i <= 11; i++){
		n_x = asteroide[0][i] * cos(ang) - asteroide[1][i] * sin(ang); //calculo de las nuevas posiciones de todos los puntos 
		n_y = asteroide[0][i] * sin(ang) + asteroide[1][i] * cos(ang);
		
		asteroide[0][i] = n_x; //si se suma o resta una constante al n_x y n_y lo que se conseguira 
		asteroide[1][i] = n_y; // un movimiento de traslacion al rededor de un punto. El sentido depende de sigo de la constante
	}
	
	//dibujo el asteroide recorriedo las coordenadas de la matriz y dibujando una linea. Es como hacer un dibujo uniendo los puntos
	while (cont <= 11){
		line(buffer, asteroide[0][cont-1]*tam+x, asteroide[1][cont-1]*tam+y, asteroide[0][cont]*tam+x, asteroide[1][cont]*tam+y, pallete_color[15]);
		cont ++;
	}
	
}

