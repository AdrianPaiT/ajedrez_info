#include "CoordinadorAjedrez.h"

void coordinador::dibuja() {


	// Gestión de la música de fondo
if (!musicaJuegoActiva && !musicaMenuActiva) {
	ETSIDI::playMusica("sonidos/MusicaFondo.wav", true);
	musicaMenuActiva = true;
}

// Soundtracks distintos en la interfaz de inicio y durante el gameplay
if (estado == JUEGO && !musicaJuegoActiva) {
	ETSIDI::stopMusica();
	ETSIDI::playMusica("sonidos/gameplay.mp3", true);
	musicaJuegoActiva = true;
	musicaMenuActiva = false;
}
else if (estado != JUEGO && !musicaMenuActiva) {
	ETSIDI::stopMusica();
	ETSIDI::playMusica("sonidos/MusicaFondo.wav", true);
	musicaMenuActiva = true;
	musicaJuegoActiva = false;
}



	
	gluLookAt(15, 18, 48,    // ubicación inicial del ojo (perspectiva)
		15, 18, 0,			 // punto hacia el que mira el ojo
		0.0, 1.0, 0.0);      // orientación hacia el eje Y+ 

//ESTADOS
	switch(estado){
		case START:
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
	
			glBegin(GL_POLYGON);
			glColor3f(1, 1, 1);
			
			glTexCoord2d(0, 0); glVertex2d(-15, 41); //Esquina izquierda superior
			glTexCoord2d(0, 1); glVertex2d(-15, -4.5); //Esquina izquierda inferior
			glTexCoord2d(1, 1); glVertex2d(45, -4.5); //Esquina derecha inferior
			glTexCoord2d(1, 0); glVertex2d(45, 41); //Esquina derecha superior
			glEnd();
	
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			break;
		
		case MENU_1C1:
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
	
			glBegin(GL_POLYGON);
			glColor3f(1, 1, 1);
			
			//JUGAR CON LOS VÉRTICES HASTA ATINAR
			glTexCoord2d(0, 0); glVertex2d(-15, 41); //Esquina izquierda superior
			glTexCoord2d(0, 1); glVertex2d(-15, -4.5); //Esquina izquierda inferior
			glTexCoord2d(1, 1); glVertex2d(45, -4.5); //Esquina derecha inferior
			glTexCoord2d(1, 0); glVertex2d(45, 41); //Esquina derecha superior
			glEnd();
	
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			break;
		
		case RATON:
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
	
			glBegin(GL_POLYGON);
			glColor3f(1, 1, 1);
			
			glTexCoord2d(0, 0); glVertex2d(-15, 41); //Esquina izquierda superior
			glTexCoord2d(0, 1); glVertex2d(-15, -4.5); //Esquina izquierda inferior
			glTexCoord2d(1, 1); glVertex2d(45, -4.5); //Esquina derecha inferior
			glTexCoord2d(1, 0); glVertex2d(45, 41); //Esquina derecha superior
			glEnd();
	
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			break;
		
		case GANA_NEGRAS:
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
	
			glBegin(GL_POLYGON);
			glColor3f(1, 1, 1);
			
			glTexCoord2d(0, 0); glVertex2d(-15, 41); //Esquina izquierda superior
			glTexCoord2d(0, 1); glVertex2d(-15, -4.5); //Esquina izquierda inferior
			glTexCoord2d(1, 1); glVertex2d(45, -4.5); //Esquina derecha inferior
			glTexCoord2d(1, 0); glVertex2d(45, 41); //Esquina derecha superior
			glEnd();
	
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			break;
				case GANA_BLANCAS:
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
	
			glBegin(GL_POLYGON);
			glColor3f(1, 1, 1);
			
			glTexCoord2d(0, 0); glVertex2d(-15, 41); //Esquina izquierda superior
			glTexCoord2d(0, 1); glVertex2d(-15, -4.5); //Esquina izquierda inferior
			glTexCoord2d(1, 1); glVertex2d(45, -4.5); //Esquina derecha inferior
			glTexCoord2d(1, 0); glVertex2d(45, 41); //Esquina derecha superior
			glEnd();
	
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			break;
		
		case TABLAS:
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
	
			glBegin(GL_POLYGON);
			glColor3f(1, 1, 1);
			
			glTexCoord2d(0, 0); glVertex2d(-15, 41); //Esquina izquierda superior
			glTexCoord2d(0, 1); glVertex2d(-15, -4.5); //Esquina izquierda inferior
			glTexCoord2d(1, 1); glVertex2d(45, -4.5); //Esquina derecha inferior
			glTexCoord2d(1, 0); glVertex2d(45, 41); //Esquina derecha superior
			glEnd();
	
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			break;

		case JUEGO:

			mundo.dibuja();

			if (mundo.get_Jaques() != 0) {

				mundo.borrar();

			switch (mundo.get_Jaques()) {
				case 1:
				estado = GANA_NEGRAS;
				break;
			case 2:
				estado = GANA_BLANCAS;
				break;
			case 3:
				estado = TABLAS;
				break;
		}
	}

			break;

		default:
			break;
		
	}
}
