#include "freeglut.h"
#include "ETSIDI.h"
#include "Tablero.h"


//Dibujar tablero

void Tablero::dibuja()
{

	//GESTIÓN DE LAS CASILLAS:
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 10; j++) {
			glBegin(GL_POLYGON);
			if (posicion_selecc >= 0 &&
				piezas[posicion_selecc]->Get_Posicion().x == i &&
				piezas[posicion_selecc]->Get_Posicion().y == j) {
				// Resaltado según color de la pieza
				if (piezas[posicion_selecc]->Get_Valor() > 0) { // Pieza blanca
					glColor3ub(143, 245, 244); // Azul claro para las blancas
				}
				else { // Pieza negra
					glColor3ub(241, 137, 113); // Rojo claro para las negras
				}
			}
			else {
				// Color normal de la casilla
				glColor3ub(Casillero[i][j].colorR, Casillero[i][j].colorG, Casillero[i][j].colorB);
			}
			glVertex3f(Casillero[i][j].param_x - 9, Casillero[i][j].param_y, 0.0f);
			glVertex3f(Casillero[i][j].param_x - 9, Casillero[i][j].param_y + 4, 0.0f);
			glVertex3f(Casillero[i][j].param_x - 5, Casillero[i][j].param_y + 4, 0.0f);
			glVertex3f(Casillero[i][j].param_x - 5, Casillero[i][j].param_y, 0.0f);
			glEnd();
			glLineWidth(1.0f);
		}
	}

	// GESTIÓN DEL BACKGROUND:

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,
		ETSIDI::getTexture("imagenes/FONDO.png").id);
	glDisable(GL_LIGHTING);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glTexCoord2d(0, 0); glVertex2d(-15, 41); //Esquina izquierda superior
	glTexCoord2d(0, 1); glVertex2d(-15, -4.5); //Esquina izquierda inferior
	glTexCoord2d(1, 1); glVertex2d(44.75, -4.5); //Esquina derecha inferior
	glTexCoord2d(1, 0); glVertex2d(44.75, 41); //Esquina derecha superior
	glEnd();
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);



	// GESTIÓN DE LOS MOVIMIENTOS POSIBLES
	DibujarMovimientosPosibles();



	// GESTIÓN DE TODAS LAS PIEZAS:
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 10; j++) {
			if (matriz[i][j] == -REY) {
				ETSIDI::Sprite sprite = { "imagenes/rey_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 1.3f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == -DAMA) {
				ETSIDI::Sprite sprite = { "imagenes/dama_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 1.3f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == -ALFIL) {
				ETSIDI::Sprite sprite = { "imagenes/alfil_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.0f, Posic_Casillas[i][j].y - 1.3f, 1);
				sprite.setSize(3.15, 3.15);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == -CABALLO) {
				ETSIDI::Sprite sprite = { "imagenes/caballo_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 1.3f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == -TORRE) {
				ETSIDI::Sprite sprite = { "imagenes/torre_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.0f, Posic_Casillas[i][j].y - 1.3f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == -PEON) {
				ETSIDI::Sprite sprite = { "imagenes/peon_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 1.3f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == -CANCILLER) {
				ETSIDI::Sprite sprite = { "imagenes/canciller_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 11.1f, Posic_Casillas[i][j].y - 1.85f, 1);
				sprite.setSize(5.2, 4.5);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == -ARZOBISPO) {
				ETSIDI::Sprite sprite = { "imagenes/arzobispo_b.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.0f, Posic_Casillas[i][j].y - 1.3f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == REY) {
				ETSIDI::Sprite sprite = { "imagenes/rey_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 0.9f, 1);
				sprite.setSize(3.5, 3.5);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == DAMA) {
				ETSIDI::Sprite sprite = { "imagenes/dama_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 0.9f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == ALFIL) {
				ETSIDI::Sprite sprite = { "imagenes/alfil_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 0.9f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == CABALLO) {
				ETSIDI::Sprite sprite = { "imagenes/caballo_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 0.9f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == TORRE) {
				ETSIDI::Sprite sprite = { "imagenes/torre_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 0.9f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == PEON) {
				ETSIDI::Sprite sprite = { "imagenes/peon_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 1.1f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == CANCILLER) {
				ETSIDI::Sprite sprite = { "imagenes/canciller_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.1f, Posic_Casillas[i][j].y - 0.9f, 1);
				sprite.setSize(3.3, 3.3);
				sprite.draw();
				glPopMatrix();
			}
			if (matriz[i][j] == ARZOBISPO) {
				ETSIDI::Sprite sprite = { "imagenes/arzobispo_w.png" };
				glPushMatrix();
				glTranslatef(Posic_Casillas[i][j].x - 10.5f, Posic_Casillas[i][j].y - 0.6f, 1);
				sprite.setSize(4.0, 4.0);
				sprite.draw();
				glPopMatrix();
			}

		}
	}

}



// Selecciona una pieza en la posición del ratón para moverla
void Tablero::Seleccionar_Pieza_1VS1(Vector origen) 
{
	posicion_selecc = -1;	  // Resetear selección previa

	if (matriz[origen.x][origen.y] != 0) {	
		// Si hay pieza en la casilla, se busca la pieza en el vector de piezas

		for (int z = 0; z < static_cast<int>(piezas.size()); z++) {
			if (piezas[z]->Get_Posicion().x == origen.x && piezas[z]->Get_Posicion().y == origen.y) {
				posicion_selecc = z;
				break;
			}
		}

		//Si la pieza no corresponde con el color del turno
		if (posicion_selecc != -1) {
			// Verificar turno correcto (valor negativo = negras, positivo = blancas)
			if ((color && piezas[posicion_selecc]->Get_Valor() < 0) || (!color && piezas[posicion_selecc]->Get_Valor() > 0)) { 
				posicion_selecc = -1;			
			}

			if (posicion_selecc != -1) {
				pos_x = origen.x;
				pos_y = origen.y;
				CalcularMovimientosPosibles();
			}
		}
	}
}



void Tablero::Mover_Pieza_1VS1(Vector destino) //posición del ratón -> destino
{

	if (posicion_selecc != -1) { // Si es una casilla permitida

		//Deshabilitación de las limitaciones de movimiento
		if (Selec_Mover(destino.x, destino.y, true)) { 

			// Algoritmo de comer: se elimina la pieza si hay otra del oponente en el destino
			if ((color && matriz[destino.x][destino.y] != 0) || (!color && matriz[destino.x][destino.y] != 0)) {

				for (int z = 0; z < static_cast<int>(piezas.size()); z++) {
					if (piezas[z]->Get_Posicion().x == destino.x && piezas[z]->Get_Posicion().y == destino.y) {

						delete piezas[z];
						if (z < posicion_selecc) posicion_selecc--;
						piezas.erase(piezas.begin() + z);
					}
				}

			}


			piezas[posicion_selecc]->Set_Posicion(destino.x, destino.y);

			//Actualización de los valores
			matriz[destino.x][destino.y] = matriz[pos_x][pos_y];
			matriz[pos_x][pos_y] = 0;

			//Cambio de turno
			if (color) color = false;		// Turno de las Negras
			else color = true;			// Turno de las Blancas

			

		}
		
	posicion_selecc = -1;
	movimientos_posibles.clear(); // Limpiar movimientos posibles
}

void Tablero::inicializa(const int& tipojuego)
{
	tipo_juego = tipojuego;

	static GLuint fondoTexture = 0;
	if (fondoTexture == 0) {
		fondoTexture = ETSIDI::getTexture("imagenes/FONDO.png").id;
		glBindTexture(GL_TEXTURE_2D, fondoTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	// Configuración gráfica del tablero (8x10 casillas)
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 10; j++) {
			Casillero[i][j].param_x = { j * 4 };
			Casillero[i][j].param_y = { i * 4 };
			Posic_Casillas[i][j] = { Casillero[i][j].param_x + 1, Casillero[i][j].param_y + 1 }; //para hallar el centro de las casillas
			if ((i + j) & 1) {
				// Color madera oscura para las "casillas negras"
				Casillero[i][j].colorR = 101;
				Casillero[i][j].colorG = 67;
				Casillero[i][j].colorB = 33;
			}
			else {
				// Color crema para las "casillas blancas"
				Casillero[i][j].colorR = 245;
				Casillero[i][j].colorG = 222;
				Casillero[i][j].colorB = 179;
			}
		}
	}

	// Inicialización de piezas según tipo de juego
	if (tipo_juego == 0) { 
		matriz =
		{
			{ TORRE, CABALLO, ARZOBISPO, ALFIL, DAMA, REY, ALFIL, CANCILLER, CABALLO, TORRE },				//Piezas blancas
			{ PEON, PEON, PEON, PEON, PEON , PEON, PEON, PEON, PEON, PEON},									
	
			// Filas vacías al inicio de la partida
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},		   
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

			{ -PEON, -PEON, -PEON, -PEON, -PEON, -PEON, -PEON, -PEON, -PEON, -PEON },						//Piezas negras
			{ -TORRE, -CABALLO, -ARZOBISPO, -ALFIL, -DAMA, -REY, -ALFIL, -CANCILLER, -CABALLO, -TORRE}		
		};
	}

	// Crear objetos Pieza según la matriz
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (matriz[i][j] != 0) {
				Pieza* p = new Pieza(i, j, matriz[i][j]); 
				piezas.push_back(p);
			}
		}
	}

	// Inicialización de las variables
	mov_x_IA = -1, mov_y_IA = -1;
	posicion_selecc = -1;
	pos_x = -1, pos_y = -1;
	pos_x_IA = 0, pos_y_IA = 0;
	color = true;	    // INSTRUCCIÓN PARA QUE LAS BLANCAS EMPIECEN
	jaqblancas = jaqmateblancas =  jaqnegras = jaqmatenegras = tablas = false;

}




void Tablero::Comprobar_JaqueMate() {
	bool esJaqueMate = true;
	bool escapar = false;

	for (int i = 0; i < 8 && !escapar; i++) {
		for (int j = 0; j < 10 && !escapar; j++) {
			//l y k deberían ser las variables de un segundo par de bucles for anidados
			//Pendiente verificar lógica de movimientos en visual
		for (int l = 0; l < 8 && !escapar; l++) {
			for (int k = 0; k < 10 && !escapar; k++) {
				if (Selec_Mover(l, k, true)) {  //Mov permitido???
					esJaqueMate = false;
					escapar = true;
				}
			}
		}
	}
	if (esJaqueMate) {
		if (color) {
		jaqmateblancas = true;
		}
	else {
		jaqmateblancas = false;
	}
}
