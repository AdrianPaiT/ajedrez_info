#pragma once
#include <iostream>
#include <vector>
#include "ETSIDI.h"
#include "Pieza.h"
#include <Vector.h>



struct CasillA
{
	// Parámetros gráficos de casilla
	int param_x;
	int param_y;
	int colorR, colorG, colorB;
};

class Tablero
{
private:

	int tipo_juego;    // Modo de juego seleccionado
	int tipo_oponente; // 0=Humano, 1=IA

	CasillA Casillero[8][10];      // Almacena propiedades gráficas de cada casilla
	Vector Posic_Casillas[8][10];   // Posiciones centrales de las casillas para interacción
	std::vector<Vector> movimientos_posibles; // Almacena casillas destino posibles
	
	// Estructuras de datos del juego
	std::vector<std::vector<int>> matriz;  // Matriz lógica del tablero (8x10)
	std::vector<Pieza*> piezas;            // Vector de punteros a las piezas

	// Variables de estado del juego
	int mov_x_IA = -1, mov_y_IA = -1;      // Movimiento planeado por la IA
	int posicion_selecc = -1;               // Índice de pieza seleccionada (-1=nada)
	int pos_x = -1, pos_y = -1;            // Posición de pieza seleccionada (jugador)
	int pos_x_IA = 0, pos_y_IA = 0;        // Posición de pieza seleccionada (IA)
	bool color = true;                      // Turno (true=blancas, false=negras)

	bool jaqblancas = false;               // Rey blanco en jaque
	bool jaqmateblancas = false;           // Rey blanco en jaque mate
	bool jaqnegras = false;                // Rey negro en jaque
	bool jaqmatenegras = false;            // Rey negro en jaque mate
	bool tablas = false;                   // Estado de empate

public:
	void dibuja();

	void Seleccionar_Pieza_1VS1(Vector origen);  // Selecciona pieza en coordenadas
	void Mover_Pieza_1VS1(Vector destino);       // Mueve pieza a coordenadas
	void inicializa(const int& tipojuego); // Inicializa tablero según modo de juego

        //Logica de movimientos por tipo de pieza
	bool Selec_Peon(int, int);            // Valida movimiento de peón
	bool Selec_Rey(int, int);             // Valida movimiento de rey
	bool Selec_Alfil(int, int);           // Valida movimiento de alfil
	

};
