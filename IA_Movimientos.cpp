#pragma once
#include "IA_Movimientos.h"
#include "freeglut.h"
#include "Tablero.h"
#include "mundo.h"
#include <iostream>
#include <chrono>
#include <thread>  
#include <alghoritm>
#include <ctime>

/*-------------------------------------------------DIFICULTAD FÁCIL---------------------------------------------------------*/

void IA_Facil::mover(Tablero& tablero) {

    verifica_mov = false;

    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();

    //tuple: para agrupar en un sólo objeto los valores de movimeinto, en lugar de usar una estructura
    vector<tuple<int, int, int, int, int>> allMoves;  // (pieza_idx, origen_x, origen_y, dest_x, dest_y)

    //recopilar TODOS los movimientos válidos
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            if (matriz[i][j] < 0) { //piezas negras (IA)

                //guardar posición actual
                int temp_pos_x = tablero.getPosX();
                int temp_pos_y = tablero.getPosY();

                //establecer posición actual para validación
                tablero.setPosicionSeleccionada(i, j);

                //buscar índice de la pieza
                int piece_index = -1;
                for (int z = 0; z < piezas.size(); z++) {
                    if (piezas[z]->Get_Posicion().x == i &&
                        piezas[z]->Get_Posicion().y == j) {
                        piece_index = z;
                        break;
                    }
                }
                if (piece_index == -1) continue;

                //buscar movimientos válidos
                for (int l = 0; l < 8; l++) {
                    for (int k = 0; k < 10; k++) {
                        if (tablero.Selec_Mover(l, k, true)) {
                            allMoves.push_back(std::make_tuple(piece_index, i, j, l, k));
                        }
                    }
                }

                //restaurar posición original
                tablero.setPosicionSeleccionada(temp_pos_x, temp_pos_y);
            }
        }
    }

    //seleccionar movimiento aleatorio
    if (!allMoves.empty()) {
        int randomIndex = rand() % allMoves.size();
        auto& move = allMoves[randomIndex];

        posicion_selecc = std::get<0>(move);
        int origen_x = std::get<1>(move);
        int origen_y = std::get<2>(move);
        mov_x_IA = std::get<3>(move);
        mov_y_IA = std::get<4>(move);
        verifica_mov = true;

        //establecer posición de origen para el movimiento
        tablero.setPosicionSeleccionada(origen_x, origen_y);

        //reproducir sonido de selección de pieza
        ETSIDI::play("sonidos/mover_bot.wav");   

        tablero.RealizarMovimientoIA(mov_x_IA, mov_y_IA, posicion_selecc);
    }
}


int IA_Facil::getMovX() const { return mov_x_IA; }
int IA_Facil::getMovY() const { return mov_y_IA; }
bool IA_Facil::getVerificaMov() const { return verifica_mov; }
int IA_Facil::getPosicionSelecc() const { return posicion_selecc; }




/*-------------------------------------------------DIFICULTAD MEDIA--------------------------------------------------------*/

    //El criterio que seguirá ahora la IA para comer piezas dependerá de una escala de jerarquías
    //La idea es que a mayor valor tenga la pieza, más prioritario será para el bot comerse dicha pieza
    //Comerse una pieza propia será muy negativo para la puntuación considerada, por lo que será la última opción
    //Poner piezas propias en riesgo de ser comidas por el jugador humano también será penalizado


void IA_Medio::mover(Tablero& tablero) {

    verifica_mov = false;

    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();

    vector<tuple<int, int, int,int, int>> allMoves;  // (pieza_idx, origen_x, origen_y, dest_x, dest_y)
    vector<float> puntuaciones;  // Almacena la puntuación de cada movimiento

    //recopilar TODOS los movimientos válidos
    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < 10; j++) {

            if (matriz[i][j] < 0) { // Piezas negras (IA)
                //guardar posición actual
                int temp_pos_x = tablero.getPosX();
                int temp_pos_y = tablero.getPosY();

                // Establecer posición actual para validación
                tablero.setPosicionSeleccionada(i, j);

                //establecer posición actual para validación
                int indice_pieza = -1;
                for (int z = 0; z < static_cast<int>(piezas.size()); z++) {
                    if (piezas[z]->Get_Posicion().x == i &&
                        piezas[z]->Get_Posicion().y == j) {
                        indice_pieza = z;
                        break;
                    }
                }
                if (indice_pieza == -1) continue;

                //buscar movimientos válidos
                for (int l = 0; l < 8; l++) {
                    for (int k = 0; k < 10; k++) {
                        if (tablero.Selec_Mover(l, k, true)) {
                            allMoves.push_back(std::make_tuple(indice_pieza, i, j, l, k));
                        }
                    }
                }

                //restaurar posición original
                tablero.setPosicionSeleccionada(temp_pos_x, temp_pos_y);
            }
        }
    }

    //Cuando no haya movs, se sale
    if (allMoves.empty()) {
        verifica_mov = false;
        return;
    }

    //criterio de puntuación para cada movimiento:
    puntuaciones.resize(allMoves.size(), 0.0f);

    for (size_t i = 0; i < allMoves.size(); i++) {
        auto& move = allMoves[i];
        int indice_pieza = std::get<0>(move);
        int origen_x = std::get<1>(move);
        int origen_y = std::get<2>(move);
        int dest_x = std::get<3>(move);
        int dest_y = std::get<4>(move);

        float puntuacion = 0.0f;

        //1) puntuación aumenta al comerse una pieza enemiga
        if (matriz[dest_x][dest_y] > 0) { // Pieza blanca (enemiga)
            puntuacion += obtenerValorPieza(matriz[dest_x][dest_y]);
        }

        //2) penalización de puntuación al comerse una pieza propia (x10)
        if (matriz[dest_x][dest_y] < 0) { // Pieza negra (propia)
            puntuacion -= 10 * obtenerValorPieza(abs(matriz[dest_x][dest_y]));
        }

        //3) penalización menor si al mover dejamos la pieza en posición vulnerable
        if (estaBajoAtaque(tablero, dest_x, dest_y, true)) { // true = pieza negra
            int valorPieza = obtenerValorPieza(abs(matriz[origen_x][origen_y]));
            puntuacion -= 0.5 * valorPieza;
        }

        puntuaciones[i] = puntuacion;
    }


    //FASE DE SELECCIÓN: Se escoge el mejor mov, o bien uno aleatorio entre los mejores
    float maxPuntuacion = *std::max_element(puntuaciones.begin(), puntuaciones.end());
    vector<int> mejoresIndices;

    for (size_t i = 0; i < puntuaciones.size(); i++) {
        if (puntuaciones[i] == maxPuntuacion) {
            mejoresIndices.push_back(i);
        }
    }

    int indiceElegido = mejoresIndices[rand() % mejoresIndices.size()];
    auto& mejorMove = allMoves[indiceElegido];

    posicion_selecc = std::get<0>(mejorMove);
    int origen_x = std::get<1>(mejorMove);
    int origen_y = std::get<2>(mejorMove);
    mov_x_IA = std::get<3>(mejorMove);
    mov_y_IA = std::get<4>(mejorMove);
    verifica_mov = true;



    //establecer posición de origen para el movimiento
    tablero.setPosicionSeleccionada(origen_x, origen_y);
    ETSIDI::play("sonidos/mover_bot.wav");

    tablero.RealizarMovimientoIA(mov_x_IA, mov_y_IA, posicion_selecc);
}


int IA_Medio::getMovX() const { return mov_x_IA; }
int IA_Medio::getMovY() const { return mov_y_IA; }
bool IA_Medio::getVerificaMov() const { return verifica_mov; }
int IA_Medio::getPosicionSelecc() const { return posicion_selecc; }


//función para obtener valor de pieza
int IA_Medio::obtenerValorPieza(int tipoPieza) {

    switch (tipoPieza) {

    case 1: return VALOR_PEON;       // PEON
    case 2: return VALOR_CABALLO;    // CABALLO
    case 3: return VALOR_ALFIL;      // ALFIL
    case 4: return VALOR_TORRE;      // TORRE
    case 5: return VALOR_DAMA;       // DAMA
    case 6: return VALOR_REY;        // REY
    case 7: return VALOR_ARZOBISPO;  // ARZOBISPO
    case 8: return VALOR_CANCILLER;  // CANCILLER

    default: return 0;
    }
}



//función para verificar si una posición está bajo ataque
bool IA_Medio::estaBajoAtaque(Tablero& tablero, int x, int y, bool esPiezaNegra) {
    auto& matriz = tablero.getMatriz();
    int temp_pos_x = tablero.getPosX();
    int temp_pos_y = tablero.getPosY();

    //verificar todas las piezas enemigas
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            
            //piezas del oponente
            if ((esPiezaNegra && matriz[i][j] > 0) || (!esPiezaNegra && matriz[i][j] < 0)) { 
                tablero.setPosicionSeleccionada(i, j);
                if (tablero.Selec_Mover(x, y, false)) {  //false = no validar jaque
                    tablero.setPosicionSeleccionada(temp_pos_x, temp_pos_y);
                    return true;
                }
            }
        }
    }

    tablero.setPosicionSeleccionada(temp_pos_x, temp_pos_y);
    return false;
}
