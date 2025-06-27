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
