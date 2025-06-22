#pragma once
#include "IA_Movimientos.h"
#include "freeglut.h"
#include "Tablero.h"
#include "mundo.h"
#include <iostream>
#include <chrono>
#include <thread>  



void IA_Facil::mover(Tablero& tablero) {

    bool verifica_mov = false;
    int mov_x_IA = -1, mov_y_IA = -1;
    int pos_x = -1, pos_y = -1;
    int posicion_selecc = -1;

    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();

    for (int i = 0; i < 8 && !verifica_mov; i++) {
        for (int j = 0; j < 10 && !verifica_mov; j++) {
            if (matriz[i][j] < 0) {
                pos_x = i;
                pos_y = j;

                for (int z = 0; z < static_cast<int>(piezas.size()); z++) {
                    if (piezas[z]->Get_Posicion().x == pos_x &&
                        piezas[z]->Get_Posicion().y == pos_y) {
                        posicion_selecc = z;
                        break;
                    }
                }

                // Guardar posición temporalmente
                int temp_pos_x = tablero.getPosX();
                int temp_pos_y = tablero.getPosY();

                // Establecer posición para validación
                tablero.setPosicionSeleccionada(pos_x, pos_y);

                for (int l = 0; l < 8 && !verifica_mov; l++) {
                    for (int k = 0; k < 10 && !verifica_mov; k++) {
                        if (tablero.Selec_Mover(l, k, true)) {
                            mov_x_IA = l;
                            mov_y_IA = k;
                            verifica_mov = true;
                        }
                    }
                }

                // Restaurar posición original
                tablero.setPosicionSeleccionada(temp_pos_x, temp_pos_y);
            }
        }
    }

    if (verifica_mov) {
        std::this_thread::sleep_for(std::chrono::seconds(1));   //Delay de 1 segundo
        tablero.RealizarMovimientoIA(mov_x_IA, mov_y_IA, posicion_selecc);
    }
}


int IA_Facil::getMovX() const { return mov_x_IA; }
int IA_Facil::getMovY() const { return mov_y_IA; }
bool IA_Facil::getVerificaMov() const { return verifica_mov; }
int IA_Facil::getPosicionSelecc() const { return posicion_selecc; }
