#include "freeglut.h"
#include "ETSIDI.h"
#include "Tablero.h"


//Constructor del tablero
Tablero::Tablero()
{
}

//Punto de vista
void Tablero::inicializa()
{
	x_ojo = 0.0;
	y_ojo = -0.01;
	z_ojo = 23;

//Inicializacion de las diferentes piezas
	
	piezas.eliminarContenido();
	
	
	for (int fila = 0; fila < 10; fila++) {
		piezas.agregar(new Peon(fila, 2, BLANCO));
		piezas.agregar(new Peon(fila, 7, NEGRO));
	}
	piezas.agregar(new Rey(5, 1, BLANCO));
	piezas.agregar(new Rey(5, 8, NEGRO));
	piezas.agregar(new Dama(5, 1, BLANCO));
	piezas.agregar(new Dama(5, 8, NEGRO));
	piezas.agregar(new Alfil(4, 1, BLANCO));
	piezas.agregar(new Alfil(7, 1, BLANCO));
	piezas.agregar(new Alfil(4, 8, NEGRO));
	piezas.agregar(new Alfil(7, 8, NEGRO));
	piezas.agregar(new Torre(1, 1, BLANCO));
	piezas.agregar(new Torre(10, 1, BLANCO));
	piezas.agregar(new Torre(1, 8, NEGRO));
	piezas.agregar(new Torre(10, 8, NEGRO));
	piezas.agregar(new Caballo(2, 1, BLANCO));
	piezas.agregar(new Caballo(9, 1, BLANCO));
	piezas.agregar(new Caballo(2, 8, NEGRO));
	piezas.agregar(new Caballo(9, 8, NEGRO));
	piezas.agregar(new Arzobispo(3, 1, BLANCO));
	piezas.agregar(new Arzobispo(3, 8, NEGRO));
	piezas.agregar(new Canciller(8, 1, BLANCO));
	piezas.agregar(new Canciller(8, 8, NEGRO));


//Dibujar tablero

void Tablero::dibuja() {

	//Pto. de vista de la camara

	gluLookAt(x_ojo, y_ojo, z_ojo,  // posicion del ojo
		0.0, 0.0, 0.0,    // hacia que punto mira  (0,0,0) 
		0.0, 0.0, 1.0);      // definimos hacia arriba (eje Z)    

//Representacion del tablero

glClear(GL_COLOR_BUFFER_BIT);
	

	for (int columna = 1; columna <= 10; columna++) {
		for (int fila = 1; fila <= 8; fila++) {
			posicion.setX(columna * 1.5 - 6);
			posicion.setY(fila * 1.5 - 7.5);

			if (columna == selector.getFila() && fila == selector.getColumna()) {

				casillas[columna - 1][fila - 1].setColor(0, 255, 0);

			}
			else
			{
				if ((columna + fila) & 1)
				{
					casillas[columna - 1][fila - 1].setColor(255, 255, 255);
					casillas[columna - 1][fila - 1].setCasillas(columna, fila);
				}

				else
				{
					casillas[columna - 1][fila - 1].setColor(0, 0, 0);
					casillas[columna - 1][fila - 1].setCasillas(columna, fila);
				}
			}

			casillas[columna - 1][fila - 1].cuadrado(posicion.getX(), posicion.getY(), 1.5);
			posf[columna - 1] = posicion.getX();
			posc[fila - 1] = posicion.getY();

		}
	}
	 
	 //Dibujo del contorno del tablero
	 
	glColor3f(0.2f, 0.05f, 0.05f); // Marrón oscuro en RGB normalizado

	// Configura el grosor de la línea
	 glLineWidth(15.0f); // Grosor del borde (en píxeles)

	 // Dibujar el borde del cuadrado
	 glBegin(GL_LINE_LOOP); // Dibuja solo el contorno
	 glVertex2f(-5.3, -6.8); // Vértice inferior izquierdo
	 glVertex2f(9.75, -6.8); // Vértice inferior derecho
	 glVertex2f(9.75, 5.3); // Vértice superior derecho
	 glVertex2f(-5.3, 5.3); // Vértice superior izquierdo
	 glEnd();

