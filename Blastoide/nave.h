//Estructura de datos de la nave
typedef struct Nave {
	int posY;
	int posX;
	float ang;
	int activo;
	int vidas;
};
void dibuar_nave(float nave[][5], BITMAP *buffer, struct Nave nav);