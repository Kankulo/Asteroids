#include <allegro.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


// Macro para calcular la distancia entre las coordenadas del raton  el punto base
#define distancia(x1, y1, x2, y2) sqrt(pow(x1-x2,2)+pow(y1-y2,2))

#define RAN_FLOAT ((float)rand()/(float)RAND_MAX)

//Constantes basicas del juego
#define INC 5
#define ANCHO 1200
#define ALTO 800
#define CONT 10
#define PI 3.1416

//defino los botones
#define ESC KEY_ESC
#define AR KEY_W
#define AB KEY_S
#define DR KEY_A
#define IZ KEY_D
#define UP KEY_UP
#define DW KEY_DOWN


//Estructura de datos del Asteroide
struct Asteroid{
	float posX;
	float posY;
	float incX;
	float incY;
	float ang;
	int tam;
	int activo;
};

//Estructura de datos del disparo
struct Blast {
	float posY;
	float posX;
	float incX;
	float incY;
	int activo;	
};

//Estructura de datos de la nave
struct Nave {
	int posY;
	int posX;
	float ang;
	int activo;
	int vidas;
};

//rutina que se actualiza las coordenadas de la nave y los puntos que la forman y la dibuja
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

void dibujar_blast (int cant_bla, BITMAP *buffer, struct Blast bla[]){ //cant_bla es el numero de disparos que hay activos
	int i;
	for (i=0; i < cant_bla; i++){
		if (bla[i].activo == 0){ //comprobamos de que el asteroide este activo y si no es asi ordenamos el vector de Blasters para que este ordenado
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

void dir_blast (struct Nave nav, struct Blast bla){ //Rutina que establece la direccion y los incrementos del disparo
	bla.incX = sin(nav.ang);
	bla.incY = cos(nav.ang);
	bla.posX = nav.posX;
	bla.posY = nav.posX;
}

int main (){

	
 	srand (time(0));
 	int cant_ast = 10, cant_bla = 10, bla_activo = -1; //Establezco la cantidad de asteroides y disparos que pueden haber al msimo tiempo
	struct Asteroid ast[cant_ast];
	struct Nave nav;
	struct Blast bla[cant_bla];
	int i=0;
	int paso_x, paso_y; //serviran para guardar el numero de "pasos" que tendran que dar los asteroides para estar en el otro lado de la pantalla
	
	//Inicializacion de la libreria allegro. A partir de aqui no se podran usar las rutinas estandar para imprimir algo por pantalla.
	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();

	float nave [2][5] = { {0, -8, 0, 8, 0},   //Ordenada x de los puntos que forman la nave
						  {0, -5, 10, -5, 0}}; //Coordenada y de los puntos que forman la nave
	
	float asteroide [2][12] = { {0, 5, 4, 4, 3, -1, -2, -4, -7.5, -6, -5, 0}, //Idem que con la nave.
								{6, 4, 3, 0, -3, -4, -1, -2, 1, 2, 5, 6}};
	
	//Asignamos valores para cada asteroide aleatoriamente
	for (i = 0; i <= cant_ast; i++){
		ast[i].posX = rand()%ANCHO;
		ast[i].posY = rand()%ALTO;
		ast[i].incX = rand()%5;
		ast[i].incY = rand()%5;
		ast[i].ang = rand()%2;
		ast[i].tam = rand()%11+1;
	}

	BITMAP *buffer = create_bitmap(ANCHO, ALTO); //bitmap en el cual dibujaremos todos los elemnetos antes de volvcarlos a la pantalla
	
	int x1 = 320, y1 = 200; //Posicion del punto inicial de la nave
	float ang=0, ang2=0;
	

	//Entramos en modo grafico
	set_window_title("Asteroid");//Le pongo un titulo a la ventana

	if(set_gfx_mode(GFX_SAFE, ANCHO, ALTO, 0, 0) !=0){
		set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Incapaz de entrar a modod grafico\n%s\n", allegro_error);
		return 1;
	}
	
	clear_bitmap(buffer);
	
	
	while(!key[ESC]){
		

		clear_bitmap(buffer);
		show_mouse(NULL);

		//Lectura del teclado para cambiar del lugar de na nave
		if(key[AR])
			y1 -= INC;
		else if(key[AB])
			y1 += INC;
		else if (key[IZ])
			x1 +=INC;
		else if(key[DR])
			x1 -= INC;

		nav.posX = x1;
		nav.posY = y1;

		//recalculamos la posicion de los asteroides una vez que hayan salido de la pantalla. Pendiente mejora
		for (i=0; i<=cant_ast; i++){
			int x, y; //variables locales del bucle a las cuales les asigno los valores de la posicion del asteroide
			x = ast[i].posX;
			y = ast[i].posY;
			// Compruebo solo los asteroides que se hayan salido de la pantalla en la direccion y
			if (x > 0 && x < ANCHO && (y < 0 || y > ALTO)){
				//no necesita calcular el paso del x
				paso_y = ALTO/ast[i].incY;
				//calculo de la nueva coordenada y para el asteroide
				y = paso_y * ast[i].incY;
				//x = 5 + x;
			}
			// Compruebo solo los asteroides que se hayan salido de la pantalla en la direccion x
			if (y > 0 && y < ALTO && (x > ANCHO || x < 0)){ //
				//no necesita calcular el paso y
				paso_x = ANCHO/ast[i].incX;
				x = paso_x * ast[i].incX;
				//y = 5 + y;
			}
			//En este ultimo if compruebo los asteroides que se hayan salido de la pantalla por las esquinas
			if ((x < 0 && y < 0) || (x > ANCHO && y < 0) || (x > ANCHO && y > ALTO) || (x < 0 && y > ALTO)){
				paso_y = ALTO/ast[i].incY;
				ast[i].posY = paso_y * ast[i].incY;
				paso_x = ANCHO/ast[i].incX;
				x = paso_x * ast[i].incX;

			}
			ast[i].posX = x;
			ast[i].posY = y;
		}

		for (i = 0; i < cant_bla; i++){
			if (bla[i].posX > 0 && bla[i].posX < ANCHO && (bla[i].posY < 0 || bla[i].posY > ALTO)){
					bla[i].activo = 0;

			}
			// Compruebo solo los disparos que se hayan salido de la pantalla en la direccion x
			if (bla[i].posY > 0 && bla[i].posY < ALTO && (bla[i].posX > ANCHO || bla[i].posX < 0)){ //
					bla[i].activo = 0;
			}
			//En este ultimo if compruebo los disparos que se hayan salido de la pantalla por las esquinas
			if ((bla[i].posX < 0 && bla[i].posY < 0) || (bla[i].posX > ANCHO && bla[i].posY < 0) || (bla[i].posX > ANCHO && bla[i].posY > ALTO) || (bla[i].posX < 0 && bla[i].posY > ALTO)){
					bla[i].activo = 0;

			}
		}

		circle (buffer, mouse_x, mouse_y, 5, pallete_color[8]);
		circle (buffer, mouse_x, mouse_y, 3, pallete_color[7]);

		nav.ang = 0;
		ang = atan2((mouse_x-x1), (mouse_y-y1));
		if (ang != ang2){
			nav.ang = (ang2-ang);
			ang2=ang;
		}


		if (mouse_b & 1 && cant_bla >= bla_activo){ // en caso de que se pulse el boton izquierdo del raton se activa el disparo
			
			int cont = 0;
			while (bla[cont].activo == 1){
				
				cont++;
				
			}
			bla_activo++;
			bla[cont].activo = 1;
			bla[cont].incX = sin(ang2)*10;
			bla[cont].incY = cos(ang2)*10;
			bla[cont].posX = x1;
			bla[cont].posY = y1;
						
		} else if (bla_activo == cant_bla){
			bla_activo = 0;
		}
		//actualizacion de la posicion del disparo. Hay que meterlo en una funcion.
		for (i = 0; i < bla_activo; i++){
			if (bla[i].activo == 1){
				bla[i].posX += bla[i].incX;
				bla[i].posY += bla[i].incY;
				circle(buffer, bla[i].posX, bla[i].posY, 3, pallete_color[3]);
				putpixel(buffer, bla[i].posX, bla[i].posY, pallete_color[7]);
			}
		}

		//dir_blast(nav, bla); //calculo de la direccion del disparo

		dibuar_nave(nave, buffer, nav);
		

		for (i = 0; i < cant_ast; i++){
			ast[i].posX += ast[i].incX;
			ast[i].posY += ast[i].incY;
			dibujar_asteroide(asteroide, buffer, ast[i]);
		}

		blit (buffer, screen, 0, 0, 0, 0, ANCHO, ALTO);
		rest(CONT);

	}
	destroy_bitmap(buffer);
return 0;

}
END_OF_MAIN();
