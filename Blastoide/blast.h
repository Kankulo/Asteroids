//Estructura de datos del disparo
typedef struct Blast {
	float posY;
	float posX;
	float incX;
	float incY;
	int activo;	
};

void dibujar_blast (int cant_bla, BITMAP *buffer, struct Blast bla[]);

void ordena_blast (struct Blast bla[], int cant_bla);