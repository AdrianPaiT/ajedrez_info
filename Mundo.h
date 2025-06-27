#pragma once
#include "Vector.h"
#include "Tablero.h"

using namespace std;


class Mundo
{

	Tablero tablero;               // Gestión del tablero y piezas
	Vector Posic_Casillas[8][10];  // Matriz para posiciones gráficas de las casillas
	int tipo_juego = 0;            // Almacena el modo de juego (1vs1, vsIA, etc.)
	int tipo_oponente = 0;         // Dificultad del oponente (si es IA)

	
	//Variables que permiten la selección por ratón:
	float casillas_ancho = 4;      // Ancho en unidades de mundo para cada casilla
	Vector Casilla_ini;            // Almacena posición inicial al arrastrar pieza
	Vector Casilla_fin;            // Almacena posición final al soltar pieza
	

public:

	void inicializa(int tipo_juego, const int& num_rival);
	void dibuja();

	// Gestiona eventos de ratón para interacción con el tablero
	void Boton_Raton(int num_rival, int x, int y, int boton, bool abajo, bool espacio, bool ref_tecla);

	int get_opon() { return tablero.Get_Oponente(); }

	// Verifica estados de finalización de partida:
	// 1 = jaque mate a blancas, 2 = jaque mate a negras, 3 = tablas, 0 = juego activo
	int get_Jaques() {
		if (tablero.Get_JaqueMateBlancas()) {
			return 1;
		}
		else if (tablero.Get_JaqueMateNegras()) {
			return 2;
		}
		else if (tablero.Get_Tablas()) {
			return 3;
		}
		return 0;
	}

	// Reinicia el tablero para nueva partida
	void borrar() { tablero.Borrar(); }
};
