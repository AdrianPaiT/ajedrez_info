#include "CoordinadorAjedrez.h"

void coordinador::dibuja() {
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
			glTexCoord2d(0, 0); glVertex2d(-10, 45); //Esquina izquierda superior
			glTexCoord2d(0, 1); glVertex2d(-10, -5); //Esquina izquierda inferior
			glTexCoord2d(1, 1); glVertex2d(55, -5); //Esquina derecha inferior
			glTexCoord2d(1, 0); glVertex2d(55, 45); //Esquina derecha superior
			glEnd();
	
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			break;
	}
}
