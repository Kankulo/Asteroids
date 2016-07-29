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

void dibujar_asteroide (float asteroide[][12], BITMAP *buffer, struct Asteroid ast);

