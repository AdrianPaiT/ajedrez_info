#pragma once
#include "IA_Movimientos.h"
#include "freeglut.h"
#include "Tablero.h"
#include "mundo.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <algorithm> 
#include <tuple>
#include <cstdlib> 
#include <ctime>  
#include <set>


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
                int indice_pieza = -1;
                for (int z = 0; z < piezas.size(); z++) {
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


/*-------------------------------------------------DIFICULTAD DIFÍCIL--------------------------------------------------------*/

    //Partiendo como base de todo lo que se tenía en el nivel medio de dificultad, se añaden una serie de implementaciones extra:
    
    /*
     - Ahora la IA considera también la posición en el tablero (proriza el control de las piezas que ocupan posiciones centrales), 
     no sólo evalúa la posibilidad de comer.  
     
     - Hace una consideración de los 3 mejores movimientos en cada situación, y elige aleatoriamente entre ellos con cierto 
     porcentaje de probabilidad

     - Evalúa cómo de expuestas pueden quedar las piezas después de comer

     - La IA además es capaz de mirar 2 movimientos hacia adelante: su movimiento y la posible respuesta del oponente, simulando
     todas las posibilidades de respuesta que puede teenr el jugador humano (mejorable en un futuro ampliando a una capacidad de 
     visualización de movimientos mayor)

    */


//ITERACIÓN 1

/*

void IA_Dificil::mover(Tablero& tablero) {
    contadorTurnos++;
    actualizarFaseJuego(tablero);
    const bool esNegras = true;  // La IA siempre juega con piezas negras

    verifica_mov = false;
    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();
    std::vector<tuple<int, int, int, int, int>> allMoves;
    std::vector<float> puntuaciones;
    std::vector<std::tuple<int, int, int, int>> historialMovimientos;

    // 1. Generar todos los movimientos válidos
    allMoves = generarTodosMovimientos(tablero, true);

    if (allMoves.empty()) {
        verifica_mov = false;
        return;
    }

    puntuaciones.resize(allMoves.size(), 0.0f);

    // 2. Evaluar cada movimiento
    for (size_t i = 0; i < allMoves.size(); i++) {
        auto& move = allMoves[i];
        int indice_pieza = std::get<0>(move);
        int origen_x = std::get<1>(move);
        int origen_y = std::get<2>(move);
        int dest_x = std::get<3>(move);
        int dest_y = std::get<4>(move);
        int tipoPieza = abs(matriz[origen_x][origen_y]);
        int valorPieza = obtenerValorPieza(tipoPieza);

        float puntuacion = 0.0f;

        // A. Prioridad ABSOLUTA: Capturar piezas enemigas
        if (matriz[dest_x][dest_y] > 0) { // Pieza blanca (enemiga)
            int valorCaptura = obtenerValorPieza(matriz[dest_x][dest_y]);

            // Bonus enorme por capturar (especialmente piezas valiosas)
            puntuacion += 100 + valorCaptura * 10;

            // Bonus adicional por capturar con pieza de menor valor
            if (valorCaptura > valorPieza) {
                puntuacion += (valorCaptura - valorPieza) * 20;
            }
        }

        // B. Penalización MUY severa por movimientos repetitivos
        if (contadorTurnos > 10) { // Solo aplicar después de la apertura
            for (const auto& mov : historialMovimientos) {
                if (mov == std::make_tuple(origen_x, origen_y, dest_x, dest_y)) {
                    puntuacion -= 200; // Penalización extrema
                    break;
                }
            }
        }

        // C. Bonus por desarrollo en apertura
        if (faseActual == APERTURA) {
            // Bonus por sacar piezas del fondo
            if (origen_x == 0 && dest_x > 1) {
                puntuacion += 30;
            }

            // Bonus por control del centro
            if (dest_x >= 3 && dest_x <= 4 && dest_y >= 4 && dest_y <= 5) {
                puntuacion += 20;
            }
        }

        // D. Penalización por pasividad
        if (dest_x == origen_x && abs(dest_y - origen_y) < 2) {
            puntuacion -= 15; // Movimientos laterales cortos
        }

        // E. Bonus por avanzar piezas en medio juego y final
        if (faseActual != APERTURA) {
            if (dest_x > origen_x) { // Avanzar hacia el territorio enemigo
                puntuacion += (dest_x - origen_x) * 5;
            }
        }

        puntuaciones[i] = puntuacion;
    }

    // 3. Seleccionar el mejor movimiento
    int indiceElegido = 0;
    float maxPuntuacion = puntuaciones[0];
    for (size_t i = 1; i < puntuaciones.size(); i++) {
        if (puntuaciones[i] > maxPuntuacion) {
            maxPuntuacion = puntuaciones[i];
            indiceElegido = i;
        }
    }

    // 4. Actualizar historial
    auto& mejorMove = allMoves[indiceElegido];
    int origen_x = std::get<1>(mejorMove);
    int origen_y = std::get<2>(mejorMove);
    int dest_x = std::get<3>(mejorMove);
    int dest_y = std::get<4>(mejorMove);

    historialMovimientos.push_back(std::make_tuple(origen_x, origen_y, dest_x, dest_y));
    if (historialMovimientos.size() > 10) {
        historialMovimientos.erase(historialMovimientos.begin());
    }

    // 5. Realizar movimiento
    int piece_index = std::get<0>(mejorMove);
    tablero.setPosicionSeleccionada(origen_x, origen_y);
    ETSIDI::play("sonidos/mover_bot.wav");
    tablero.RealizarMovimientoIA(dest_x, dest_y, piece_index);
}
*/


//ITERACIÓN 2

/*

void IA_Dificil::mover(Tablero& tablero) {
    verifica_mov = false;
    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();
    std::vector<std::tuple<int, int, int, int>> historialMovimientos;

    // 1. Generar todos los movimientos válidos
    vector<tuple<int, int, int, int, int>> allMoves = generarTodosMovimientos(tablero, true);

    if (allMoves.empty()) {
        verifica_mov = false;
        return;
    }

    // 2. Evaluar cada movimiento con criterios más estrictos
    vector<float> puntuaciones;
    for (size_t i = 0; i < allMoves.size(); i++) {
        auto& move = allMoves[i];
        int indice_pieza = std::get<0>(move);
        int origen_x = std::get<1>(move);
        int origen_y = std::get<2>(move);
        int dest_x = std::get<3>(move);
        int dest_y = std::get<4>(move);

        int tipoPieza = abs(matriz[origen_x][origen_y]);
        int valorPieza = obtenerValorPieza(tipoPieza);
        int valorCaptura = (matriz[dest_x][dest_y] > 0) ?
            obtenerValorPieza(matriz[dest_x][dest_y]) : 0;

        float puntuacion = 0.0f;
        bool estaAmenazada = estaBajoAtaque(tablero, dest_x, dest_y, true);

        // A. EVALUACIÓN DE CAPTURAS (con protección de piezas valiosas)
        if (valorCaptura > 0) {
            // Bonus por capturar (mayor para piezas menores)
            float factorCaptura = (valorPieza < VALOR_TORRE) ? 1.8f : 1.2f;
            puntuacion += valorCaptura * factorCaptura;

            // Penalización EXTREMA si pieza valiosa queda expuesta
            if (valorPieza > VALOR_TORRE && estaAmenazada) {
                puntuacion -= valorPieza * 2.0f;  // Penalización doble del valor
            }
        }

        // B. EVALUACIÓN DE RIESGO PARA PIEZAS VALIOSAS
        if (valorPieza > VALOR_CABALLO) {
            // Penalización fuerte por exponer piezas importantes
            if (estaAmenazada) {
                puntuacion -= valorPieza * 1.5f;

                // Penalización adicional si la amenaza viene de pieza menor
                if (valorPieza > VALOR_TORRE) {
                    int valorAmenaza = valorAmenazaMinima(tablero, dest_x, dest_y);
                    if (valorAmenaza < valorPieza) {
                        puntuacion -= (valorPieza - valorAmenaza) * 1.2f;
                    }
                }
            }
        }

        // C. ESTRATEGIA PARA PEONES (avance seguro)
        if (tipoPieza == 1) {
            // Bonus por avance controlado
            int avance = dest_x - origen_x;
            puntuacion += avance * 2;

            // Penalización MUY FUERTE si queda expuesto a captura por peón enemigo
            if (esVulnerableAPeon(tablero, dest_x, dest_y)) {
                puntuacion -= 100;
            }

            // Bonus extra si está protegido por otro peón
            if (estaProtegidoPorPeon(tablero, dest_x, dest_y)) {
                puntuacion += 30;
            }
        }

        // D. ESTRATEGIA DE APERTURA
        if (contadorTurnos < 10) {
            // Bonus por desarrollar piezas menores
            if (origen_x == 0 && (tipoPieza == 2 || tipoPieza == 3)) {
                puntuacion += 40;
            }

            // Penalización por mover dama temprano
            if (tipoPieza == 5) {
                puntuacion -= 80;
            }
        }

        // E. CONTROL POSICIONAL
        // Bonus por control central (reducido para peones)
        if ((dest_x == 3 || dest_x == 4) && (dest_y == 4 || dest_y == 5)) {
            puntuacion += (tipoPieza == PEON) ? 5 : 15;
        }

        // F. EVITAR REPETICIÓN
        for (const auto& mov : historialMovimientos) {
            if (mov == std::make_tuple(origen_x, origen_y, dest_x, dest_y)) {
                puntuacion -= 200;
                break;
            }
        }

        puntuaciones.push_back(puntuacion);
    }

    // 3. Seleccionar el mejor movimiento
    int mejorIndice = 0;
    float mejorPuntuacion = puntuaciones[0];

    for (size_t i = 1; i < puntuaciones.size(); i++) {
        if (puntuaciones[i] > mejorPuntuacion) {
            mejorPuntuacion = puntuaciones[i];
            mejorIndice = i;
        }
    }

    // 4. Realizar movimiento
    auto& mejorMove = allMoves[mejorIndice];
    int indice_pieza = std::get<0>(mejorMove);
    int origen_x = std::get<1>(mejorMove);
    int origen_y = std::get<2>(mejorMove);
    int dest_x = std::get<3>(mejorMove);
    int dest_y = std::get<4>(mejorMove);

    // Actualizar historial
    historialMovimientos.push_back(std::make_tuple(origen_x, origen_y, dest_x, dest_y));
    if (historialMovimientos.size() > 8) {
        historialMovimientos.erase(historialMovimientos.begin());
    }

    contadorTurnos++;

    // Ejecutar movimiento
    tablero.setPosicionSeleccionada(origen_x, origen_y);
    ETSIDI::play("sonidos/mover_bot.wav");
    tablero.RealizarMovimientoIA(dest_x, dest_y, indice_pieza);
    verifica_mov = true;
}
*/


//ITERACIÓN 3
/*
void IA_Dificil::mover(Tablero& tablero) {
    verifica_mov = false;
    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();

    // 1. Generar todos los movimientos válidos
    vector<tuple<int, int, int, int, int>> allMoves = generarTodosMovimientos(tablero, true);
    std::vector<std::tuple<int, int, int, int>> historialMovimientos;

    if (allMoves.empty()) {
        verifica_mov = false;
        return;
    }

    // 2. Evaluar cada movimiento con criterios mejorados
    vector<float> puntuaciones;
    for (size_t i = 0; i < allMoves.size(); i++) {
        auto& move = allMoves[i];
        int indice_pieza = std::get<0>(move);
        int origen_x = std::get<1>(move);
        int origen_y = std::get<2>(move);
        int dest_x = std::get<3>(move);
        int dest_y = std::get<4>(move);

        int tipoPieza = abs(matriz[origen_x][origen_y]);
        int valorPieza = obtenerValorPieza(tipoPieza);
        int valorCaptura = (matriz[dest_x][dest_y] > 0) ?
            obtenerValorPieza(matriz[dest_x][dest_y]) : 0;
        bool esPropia = (matriz[dest_x][dest_y] < 0); // ¿Captura pieza propia?

        float puntuacion = 0.0f;
        bool estaAmenazada = estaBajoAtaque(tablero, dest_x, dest_y, true);

        // A. PENALIZACIÓN EXTREMA POR COMER PROPIOS
        if (esPropia) {
            puntuacion -= 1000;  // Penalización enorme para evitar comer propias
        }

        // B. ESTRATEGIA PARA PIEZAS VALIOSAS
        else if (tipoPieza == 5 || tipoPieza == 4 || tipoPieza == 7 || tipoPieza == 8) {
            // 1. Penalización por mover en apertura
            if (contadorTurnos < 12) {
                puntuacion -= 150;
            }

            // 2. Penalización extrema por exponer
            if (estaAmenazada) {
                puntuacion -= valorPieza * 3.0f;

                // Penalización adicional si la amenaza es de pieza menor
                if (valorAmenazaMinima(tablero, dest_x, dest_y) < valorPieza) {
                    puntuacion -= valorPieza * 2.0f;
                }
            }

            // 3. Bonus por captura segura
            if (valorCaptura > 0 && !estaAmenazada) {
                puntuacion += valorCaptura * 1.5f;
            }
        }

        // C. ESTRATEGIA MEJORADA PARA PEONES
        else if (tipoPieza == 1) {
            // 1. Bonus agresivo por capturas
            if (valorCaptura > 0) {
                puntuacion += valorCaptura * 4.0f;  // Bonus muy alto
            }

            // 2. Avance seguro (solo si no hay amenaza)
            int avance = dest_x - origen_x;
            if (!esVulnerableAPeon(tablero, dest_x, dest_y)) {
                puntuacion += avance * 3;
            }

            // 3. Bonus por romper estructuras enemigas
            if (dest_y >= 4 && dest_y <= 5) {  // Centro
                puntuacion += 20;
            }
        }

        // D. ESTRATEGIA PARA PIEZAS MENORES (Caballo, Alfil)
        else if (tipoPieza == 2 || tipoPieza == 3) {
            // Bonus por desarrollo en apertura
            if (contadorTurnos < 10 && origen_x == 0) {
                puntuacion += 60;
            }

            // Bonus por control central
            if ((dest_x == 3 || dest_x == 4) && (dest_y == 4 || dest_y == 5)) {
                puntuacion += 35;
            }
        }

        // E. EVITAR REPETICIÓN
        for (const auto& mov : historialMovimientos) {
            if (mov == std::make_tuple(origen_x, origen_y, dest_x, dest_y)) {
                puntuacion -= 1000;  // Penalización extremadamente alta
                break;
            }
        }

        puntuaciones.push_back(puntuacion);
    }

    // 3. Seleccionar el mejor movimiento
    int mejorIndice = 0;
    float mejorPuntuacion = puntuaciones[0];

    for (size_t i = 1; i < puntuaciones.size(); i++) {
        if (puntuaciones[i] > mejorPuntuacion) {
            mejorPuntuacion = puntuaciones[i];
            mejorIndice = i;
        }
    }

    // 4. Realizar movimiento
    auto& mejorMove = allMoves[mejorIndice];
    int indice_pieza = std::get<0>(mejorMove);
    int origen_x = std::get<1>(mejorMove);
    int origen_y = std::get<2>(mejorMove);
    int dest_x = std::get<3>(mejorMove);
    int dest_y = std::get<4>(mejorMove);

    // Actualizar historial
    historialMovimientos.push_back(std::make_tuple(origen_x, origen_y, dest_x, dest_y));
    if (historialMovimientos.size() > 10) {
        historialMovimientos.erase(historialMovimientos.begin());
    }

    contadorTurnos++;

    // Ejecutar movimiento
    tablero.setPosicionSeleccionada(origen_x, origen_y);
    ETSIDI::play("sonidos/mover_bot.wav");
    tablero.RealizarMovimientoIA(dest_x, dest_y, indice_pieza);
    verifica_mov = true;
}
*/


//ITERACIÓN 4

void IA_Dificil::mover(Tablero& tablero) {
    verifica_mov = false;
    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();
    std::vector<std::tuple<int, int, int, int>> historialMovimientos;

    // 1. Generar todos los movimientos válidos
    vector<tuple<int, int, int, int, int>> allMoves = generarTodosMovimientos(tablero, true);

    if (allMoves.empty()) {
        verifica_mov = false;
        return;
    }

    // 2. Evaluar cada movimiento con protección reforzada
    vector<float> puntuaciones;
    for (size_t i = 0; i < allMoves.size(); i++) {
        auto& move = allMoves[i];
        int indice_pieza = std::get<0>(move);
        int origen_x = std::get<1>(move);
        int origen_y = std::get<2>(move);
        int dest_x = std::get<3>(move);
        int dest_y = std::get<4>(move);

        int tipoPieza = abs(matriz[origen_x][origen_y]);
        int valorPieza = obtenerValorPieza(tipoPieza);
        int valorCaptura = (matriz[dest_x][dest_y] > 0) ?
            obtenerValorPieza(matriz[dest_x][dest_y]) : 0;
        bool esPropia = (matriz[dest_x][dest_y] < 0);
        bool estaAmenazada = estaBajoAtaque(tablero, dest_x, dest_y, true);
        bool capturaAltoValor = (valorCaptura >= VALOR_TORRE);
        bool esPiezaValiosa = (tipoPieza == 5 || tipoPieza == 4 || tipoPieza == 7 || tipoPieza == 8);
        bool esApertura = (contadorTurnos < 10);

        float puntuacion = 0.0f;
        float factorAleatorio = (rand() % 10) * 0.1f;

        // A. PENALIZACIÓN EXTREMA POR COMER PROPIOS
        if (esPropia) {
            puntuacion -= 10000;
        }
        // B. ESTRATEGIA PARA CAPTURAS DE ALTO VALOR
        else if (capturaAltoValor) {
            puntuacion += valorCaptura * 3.0f;

            // Bonus adicional solo si no hay riesgo
            if (!estaAmenazada) {
                puntuacion += valorCaptura * 1.5f;
            }
        }
        // C. PROTECCIÓN REFORZADA PARA PIEZAS VALIOSAS EN APERTURA
        else if (esPiezaValiosa && esApertura) {
            // Penalización base muy fuerte por mover
            puntuacion -= 150;

            // Penalización adicional exponencial si hay amenaza
            if (estaAmenazada) {
                float factorRiesgo = (valorAmenazaMinima(tablero, dest_x, dest_y) < valorPieza) ? 3.5f : 2.0f;
                puntuacion -= valorPieza * factorRiesgo;
            }

            // Bonus mínimo por capturas en apertura
            if (valorCaptura > 0) {
                puntuacion += valorCaptura * 0.3f; // Bonus muy reducido
            }
        }
        // D. ESTRATEGIA MEJORADA PARA PEONES
        else if (tipoPieza == 1) {
            // Bonus agresivo por capturas
            if (valorCaptura > 0) {
                puntuacion += (3.5f + factorAleatorio) * valorCaptura;
            }

            // Avance seguro con variación
            int avance = dest_x - origen_x;
            if (!esVulnerableAPeon(tablero, dest_x, dest_y)) {
                puntuacion += (2.0f + factorAleatorio) * avance;
            }
        }
        // E. ESTRATEGIA PARA CABALLOS Y ALFILES
        else if (tipoPieza == 2 || tipoPieza == 3) {
            // Bonus por desarrollo temprano
            if (esApertura && origen_x == 0) {
                puntuacion += 50 + factorAleatorio * 20;
            }

            // Bonus por control central
            if ((dest_x >= 3 && dest_x <= 4) && (dest_y >= 4 && dest_y <= 5)) {
                puntuacion += 30;
            }
        }

        // F. EVITAR REPETICIÓN
        for (const auto& mov : historialMovimientos) {
            if (mov == std::make_tuple(origen_x, origen_y, dest_x, dest_y)) {
                puntuacion -= 1000;
                break;
            }
        }

        puntuaciones.push_back(puntuacion);
    }

    // 3. Selección con protección de piezas valiosas
    float mejorPuntuacion = -100000;
    std::vector<int> mejoresIndices;

    // Encontrar la mejor puntuación segura
    for (size_t i = 0; i < puntuaciones.size(); i++) {
        if (puntuaciones[i] > mejorPuntuacion) {
            mejorPuntuacion = puntuaciones[i];
        }
    }

    // Filtrar movimientos peligrosos para piezas valiosas
    for (size_t i = 0; i < puntuaciones.size(); i++) {
        auto& move = allMoves[i];
        int tipoPieza = abs(matriz[std::get<1>(move)][std::get<2>(move)]);
        bool esValiosa = (tipoPieza == 5 || tipoPieza == 4 || tipoPieza == 7 || tipoPieza == 8);

        // Solo considerar movimientos seguros para piezas valiosas
        if (esValiosa && contadorTurnos < 10) {
            if (puntuaciones[i] >= mejorPuntuacion - 50.0f) {
                mejoresIndices.push_back(i);
            }
        }
        else if (puntuaciones[i] >= mejorPuntuacion - 20.0f) {
            mejoresIndices.push_back(i);
        }
    }

    // Selección aleatoria entre los mejores movimientos seguros
    int indiceElegido = mejoresIndices[rand() % mejoresIndices.size()];
    auto& mejorMove = allMoves[indiceElegido];

    // 4. Realizar movimiento
    int indice_pieza = std::get<0>(mejorMove);
    int origen_x = std::get<1>(mejorMove);
    int origen_y = std::get<2>(mejorMove);
    int dest_x = std::get<3>(mejorMove);
    int dest_y = std::get<4>(mejorMove);

    // Actualizar historial
    historialMovimientos.push_back(std::make_tuple(origen_x, origen_y, dest_x, dest_y));
    if (historialMovimientos.size() > 10) {
        historialMovimientos.erase(historialMovimientos.begin());
    }

    contadorTurnos++;

    // Ejecutar movimiento
    tablero.setPosicionSeleccionada(origen_x, origen_y);
    ETSIDI::play("sonidos/mover_bot.wav");
    tablero.RealizarMovimientoIA(dest_x, dest_y, indice_pieza);
    verifica_mov = true;
}



//ITERACIÓN 5  (Basura)

/*
void IA_Dificil::mover(Tablero& tablero) {
    verifica_mov = false;
    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();
    std::vector<std::tuple<int, int, int, int>> historialMovimientos;

    // 1. Generar todos los movimientos válidos
    vector<tuple<int, int, int, int, int>> allMoves = generarTodosMovimientos(tablero, true);

    if (allMoves.empty()) {
        verifica_mov = false;
        return;
    }

    // 2. Evaluar cada movimiento con búsqueda a futuro
    vector<float> puntuaciones;
    for (size_t i = 0; i < allMoves.size(); i++) {
        auto& move = allMoves[i];
        int indice_pieza = std::get<0>(move);
        int origen_x = std::get<1>(move);
        int origen_y = std::get<2>(move);
        int dest_x = std::get<3>(move);
        int dest_y = std::get<4>(move);

        int tipoPieza = abs(matriz[origen_x][origen_y]);
        int valorPieza = obtenerValorPieza(tipoPieza);
        int valorCaptura = (matriz[dest_x][dest_y] > 0) ?
            obtenerValorPieza(matriz[dest_x][dest_y]) : 0;
        bool esPropia = (matriz[dest_x][dest_y] < 0);
        bool estaAmenazada = estaBajoAtaque(tablero, dest_x, dest_y, true);
        bool capturaAltoValor = (valorCaptura >= VALOR_TORRE);
        bool esPiezaValiosa = (tipoPieza == 5 || tipoPieza == 4 || tipoPieza == 7 || tipoPieza == 8);
        bool esApertura = (contadorTurnos < 10);

        float puntuacion = 0.0f;
        float factorAleatorio = (rand() % 10) * 0.1f;

        // A. PENALIZACIÓN EXTREMA POR COMER PROPIOS
        if (esPropia) {
            puntuacion -= 10000;
        }
        // B. ESTRATEGIA PARA CAPTURAS DE ALTO VALOR
        else if (capturaAltoValor) {
            puntuacion += valorCaptura * 3.0f;

            if (!estaAmenazada) {
                puntuacion += valorCaptura * 1.5f;
            }
        }
        // C. PROTECCIÓN PARA PIEZAS VALIOSAS EN APERTURA
        else if (esPiezaValiosa && esApertura) {

            // PENALIZACIÓN EXTRA ESPECÍFICA PARA LA DAMA
            if (tipoPieza == 5) {  // Dama
                puntuacion -= 500;  // Penalización base enorme

                // Penalización adicional si se mueve a territorio enemigo
                if (dest_x > 3) {
                    puntuacion -= 300;
                }

                // Penalización extra si captura piezas pequeñas
                if (valorCaptura < VALOR_CABALLO) {
                    puntuacion -= 200;
                }
            }
            else {  // Otras piezas valiosas
                puntuacion -= 150;
            }

            // Penalización por amenazas (se mantiene)
            if (estaAmenazada) {
                float factorRiesgo = (valorAmenazaMinima(tablero, dest_x, dest_y) < valorPieza) ? 4.0f : 2.5f;
                puntuacion -= valorPieza * factorRiesgo;
            }

            // Bonus mínimo por capturas
            if (valorCaptura > 0) {
                puntuacion += valorCaptura * 0.1f;
            }
        }
        // D. ESTRATEGIA PARA PEONES
        else if (tipoPieza == 1) {
            if (valorCaptura > 0) {
                puntuacion += (3.5f + factorAleatorio) * valorCaptura;
            }

            int avance = dest_x - origen_x;
            if (!esVulnerableAPeon(tablero, dest_x, dest_y)) {
                puntuacion += (2.0f + factorAleatorio) * avance;
            }
        }
        // E. ESTRATEGIA PARA CABALLOS Y ALFILES
        else if (tipoPieza == 2 || tipoPieza == 3) {
            if (esApertura && origen_x == 0) {
                puntuacion += 50 + factorAleatorio * 20;
            }

            if ((dest_x >= 3 && dest_x <= 4) && (dest_y >= 4 && dest_y <= 5)) {
                puntuacion += 30;
            }
        }

        // F. BÚSQUEDA A FUTURO DE 2 MOVIMIENTOS 
        if (puntuacion > -5000) {  // Solo para movimientos no prohibidos
            // 1. Guardar estado original
            int pieza_original = matriz[dest_x][dest_y];
            int pieza_movida = matriz[origen_x][origen_y];

            // 2. Simular movimiento actual
            matriz[dest_x][dest_y] = pieza_movida;
            matriz[origen_x][origen_y] = 0;

            // 3. Evaluar mejor respuesta del oponente
            float mejorRespuestaOponente = 0.0f;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 10; j++) {
                    if (matriz[i][j] > 0) {  // Piezas blancas (oponente)
                        // Verificar si puede capturar nuestra pieza movida
                        if (tablero.Selec_Mover(dest_x, dest_y, false)) {
                            int valorPiezaOponente = obtenerValorPieza(matriz[i][j]);
                            int gananciaNeta = valorPieza - valorPiezaOponente;

                            if (gananciaNeta > mejorRespuestaOponente) {
                                mejorRespuestaOponente = gananciaNeta;
                            }
                        }
                    }
                }
            }

            // 4. Penalizar si el oponente puede capturar con ganancia
            if (mejorRespuestaOponente > 0) {
                puntuacion -= mejorRespuestaOponente * 1.2f;
            }

            // 5. Restaurar estado original
            matriz[origen_x][origen_y] = pieza_movida;
            matriz[dest_x][dest_y] = pieza_original;
        }

        // G. EVITAR REPETICIÓN
        for (const auto& mov : historialMovimientos) {
            if (mov == std::make_tuple(origen_x, origen_y, dest_x, dest_y)) {
                puntuacion -= 1000;
                break;
            }
        }

        puntuaciones.push_back(puntuacion);
    }

    // 3. Selección con protección de piezas valiosas
    float mejorPuntuacion = -100000;
    std::vector<int> mejoresIndices;

    for (size_t i = 0; i < puntuaciones.size(); i++) {
        if (puntuaciones[i] > mejorPuntuacion) {
            mejorPuntuacion = puntuaciones[i];
        }
    }

    for (size_t i = 0; i < puntuaciones.size(); i++) {
        auto& move = allMoves[i];
        int tipoPieza = abs(matriz[std::get<1>(move)][std::get<2>(move)]);
        bool esValiosa = (tipoPieza == 5 || tipoPieza == 4 || tipoPieza == 7 || tipoPieza == 8);

        if (esValiosa && contadorTurnos < 10) {
            if (puntuaciones[i] >= mejorPuntuacion - 50.0f) {
                mejoresIndices.push_back(i);
            }
        }
        else if (puntuaciones[i] >= mejorPuntuacion - 20.0f) {
            mejoresIndices.push_back(i);
        }
    }

    int indiceElegido = mejoresIndices[rand() % mejoresIndices.size()];
    auto& mejorMove = allMoves[indiceElegido];

    // 4. Realizar movimiento
    int indice_pieza = std::get<0>(mejorMove);
    int origen_x = std::get<1>(mejorMove);
    int origen_y = std::get<2>(mejorMove);
    int dest_x = std::get<3>(mejorMove);
    int dest_y = std::get<4>(mejorMove);

    historialMovimientos.push_back(std::make_tuple(origen_x, origen_y, dest_x, dest_y));
    if (historialMovimientos.size() > 10) {
        historialMovimientos.erase(historialMovimientos.begin());
    }

    contadorTurnos++;

    tablero.setPosicionSeleccionada(origen_x, origen_y);
    ETSIDI::play("sonidos/mover_bot.wav");
    tablero.RealizarMovimientoIA(dest_x, dest_y, indice_pieza);
    verifica_mov = true;
}
*/






bool IA_Dificil::esVulnerableAPeon(Tablero& tablero, int x, int y) {
    auto& matriz = tablero.getMatriz();

    // Para piezas negras (IA), los peones blancos están abajo (x+1)
    if (x < 7) {
        // Comprobar captura por izquierda
        if (y > 0 && matriz[x + 1][y - 1] == 1) return true;

        // Comprobar captura por derecha
        if (y < 9 && matriz[x + 1][y + 1] == 1) return true;
    }
    return false;
}



bool IA_Dificil::estaProtegidoPorPeon(Tablero& tablero, int x, int y) {
    auto& matriz = tablero.getMatriz();

    // Para piezas negras (IA), los peones protectores están arriba (x-1)
    if (x > 0) {
        // Protección por izquierda
        if (y > 0 && matriz[x - 1][y - 1] == -1) return true;

        // Protección por derecha
        if (y < 9 && matriz[x - 1][y + 1] == -1) return true;
    }
    return false;
}


int IA_Dificil::valorAmenazaMinima(Tablero& tablero, int x, int y) {
    int minValor = 1000;
    auto& matriz = tablero.getMatriz();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            if (matriz[i][j] > 0) {  // Piezas blancas (enemigas para la IA)
                int tempX = tablero.getPosX();
                int tempY = tablero.getPosY();

                tablero.setPosicionSeleccionada(i, j);
                if (tablero.Selec_Mover(x, y, false)) {
                    int valorAtacante = obtenerValorPieza(matriz[i][j]);
                    if (valorAtacante < minValor) {
                        minValor = valorAtacante;
                    }
                }
                tablero.setPosicionSeleccionada(tempX, tempY);
            }
        }
    }
    return minValor;
}


void IA_Dificil::actualizarFaseJuego(Tablero& tablero) {
    int totalPiezas = 0;
    auto& matriz = tablero.getMatriz();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            if (matriz[i][j] != 0) totalPiezas++;
        }
    }

    // Menos de 20 piezas = final de juego
    if (totalPiezas < 20) {
        faseActual = FINAL;
    }
    // Entre 20 y 28 piezas = medio juego
    else if (totalPiezas < 28) {
        faseActual = MEDIO_JUEGO;
    }
    // Más de 28 piezas = apertura
    else {
        faseActual = APERTURA;
    }
}


float IA_Dificil::evaluarApertura(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y) {
    float puntuacion = 0.0f;
    int tipoPieza = abs(tablero.getMatriz()[origen_x][origen_y]);

    // 1. Desarrollo muy conservador de piezas valiosas
    if (tipoPieza == VALOR_CABALLO || tipoPieza == VALOR_ALFIL ||
        tipoPieza == VALOR_ARZOBISPO || tipoPieza == VALOR_CANCILLER) {

        if (origen_x == 0 && dest_x > 1) {
            // Bonus reducido para piezas menores
            puntuacion += 8;

            // Penalización severa por avanzar demasiado
            if (dest_x > 3 && contadorTurnos < 6) {
                puntuacion -= 30;
            }

            // Penalización adicional si la casilla destino está atacada
            if (estaBajoAtaque(tablero, dest_x, dest_y, true)) {
                puntuacion -= 20;
            }
        }
    }

    // 2. Control del centro con peones
    if (abs(tablero.getMatriz()[origen_x][origen_y]) == 1) { //Peón
        if (dest_y >= 4 && dest_y <= 5 && dest_x == 3) {
            puntuacion += 15;
        }
    }

    // 4. Evitar exponer piezas valiosas
    if (estaBajoAtaque(tablero, dest_x, dest_y, true)) {
        int valorPieza = obtenerValorPieza(abs(tablero.getMatriz()[origen_x][origen_y]));
        puntuacion -= valorPieza * 0.7;
    }

    return puntuacion;
}

float IA_Dificil::evaluarMedioJuego(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y) {
    float puntuacion = 0.0f;

    // 1. Ataque coordinado
    puntuacion += evaluarAtaqueAlRey(tablero, dest_x, dest_y);

    // 2. Movilidad de piezas
    int movilidad = 0;
    int temp_pos_x = tablero.getPosX();
    int temp_pos_y = tablero.getPosY();

    tablero.setPosicionSeleccionada(dest_x, dest_y);
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 10; y++) {
            if (tablero.Selec_Mover(x, y, true)) {
                movilidad++;
            }
        }
    }
    tablero.setPosicionSeleccionada(temp_pos_x, temp_pos_y);

    puntuacion += movilidad * 0.5;

    // 3. Control de columnas abiertas para torres
    if (abs(tablero.getMatriz()[origen_x][origen_y]) == 4) {    //Torre
        bool columnaAbierta = true;
        for (int i = 0; i < 8; i++) {
            if (tablero.getMatriz()[i][dest_y] == 1 ||   //peón blanco
                tablero.getMatriz()[i][dest_y] == -1) {  //peón negro
                columnaAbierta = false;
                break;
            }

        }
        if (columnaAbierta) puntuacion += 20;
    }

    return puntuacion;
}

float IA_Dificil::evaluarFinal(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y) {
    float puntuacion = 0.0f;

    // 1. Activar rey en el final
    if (abs(tablero.getMatriz()[origen_x][origen_y]) == 6) {

        int centroX = 3.5;
        int centroY = 4.5;
        float distanciaCentro = sqrt(pow(dest_x - centroX, 2) + pow(dest_y - centroY, 2));
        puntuacion += (10 - distanciaCentro) * 2;
    }

    // 2. Avanzar peones pasados
    if (abs(tablero.getMatriz()[origen_x][origen_y]) == 1) {    //peón
        int avance = (tablero.Consultar_Turno() == true) ? dest_x : (7 - dest_x);
        puntuacion += avance * 5;
    }

    return puntuacion;
}

float IA_Dificil::evaluarAtaqueAlRey(Tablero& tablero, int dest_x, int dest_y) {
    float puntuacion = 0.0f;

    // Encontrar posición del rey enemigo
    int reyX = -1, reyY = -1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            if (tablero.getMatriz()[i][j] == 6) { // Rey blanco
                reyX = i;
                reyY = j;
                break;
            }
        }
    }

    if (reyX != -1) {
        // Bonus por acercarse al rey enemigo
        float distancia = sqrt(pow(dest_x - reyX, 2) + pow(dest_y - reyY, 2));
        puntuacion += (10 - distancia) * 0.5;

        // Bonus por atacar casillas alrededor del rey
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                if (dest_x == reyX + dx && dest_y == reyY + dy) {
                    puntuacion += 15;
                }
            }
        }
    }

    return puntuacion;
}


float IA_Dificil::calcularRiesgo(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y) {
    float riesgo = 0.0f;
    int valorPieza = obtenerValorPieza(abs(tablero.getMatriz()[origen_x][origen_y]));

    // 1. Riesgo base por estar bajo ataque
    if (estaBajoAtaque(tablero, dest_x, dest_y, true)) {
        riesgo += valorPieza * 0.8f;
    }

    // 2. Riesgo por capturas en cadena
    if (tablero.getMatriz()[dest_x][dest_y] > 0) {
        int valorCaptura = obtenerValorPieza(tablero.getMatriz()[dest_x][dest_y]);

        // Calcular posibles contra-capturas
        if (estaBajoAtaque(tablero, dest_x, dest_y, true)) {
            // Encontrar el atacante de menor valor
            int minValorAtacante = 1000;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 10; j++) {
                    if (tablero.getMatriz()[i][j] > 0) {
                        int tempX = tablero.getPosX();
                        int tempY = tablero.getPosY();
                        tablero.setPosicionSeleccionada(i, j);
                        if (tablero.Selec_Mover(dest_x, dest_y, false)) {
                            int valorAtacante = obtenerValorPieza(tablero.getMatriz()[i][j]);
                            if (valorAtacante < minValorAtacante) {
                                minValorAtacante = valorAtacante;
                            }
                        }
                        tablero.setPosicionSeleccionada(tempX, tempY);
                    }
                }
            }

            // Calcular riesgo de intercambio
            if (minValorAtacante < valorPieza) {
                riesgo += (valorPieza - minValorAtacante) * 1.2f;
            }
        }
    }

    return riesgo;
}


vector<tuple<int, int, int, int, int>> IA_Dificil::generarTodosMovimientos(Tablero& tablero, bool esNegras) {
    auto& matriz = tablero.getMatriz();
    auto& piezas = tablero.getPiezas();
    std::vector<tuple<int, int, int, int, int>> todosMovimientos;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            // Filtrar piezas del color correcto
            if ((esNegras && matriz[i][j] < 0) || (!esNegras && matriz[i][j] > 0)) {
                int temp_pos_x = tablero.getPosX();
                int temp_pos_y = tablero.getPosY();

                // Establecer posición actual para validación
                tablero.setPosicionSeleccionada(i, j);

                // Buscar índice de la pieza
                int indice_pieza = -1;
                for (int z = 0; z < static_cast<int>(piezas.size()); z++) {
                    if (piezas[z]->Get_Posicion().x == i &&
                        piezas[z]->Get_Posicion().y == j) {
                        indice_pieza = z;
                        break;
                    }
                }
                if (indice_pieza == -1) continue;

                // Buscar movimientos válidos
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 10; y++) {
                        if (tablero.Selec_Mover(x, y, true)) {
                            todosMovimientos.push_back(std::make_tuple(indice_pieza, i, j, x, y));
                        }
                    }
                }

                // Restaurar posición original
                tablero.setPosicionSeleccionada(temp_pos_x, temp_pos_y);
            }
        }
    }

    return todosMovimientos;
}



//función para verificar si una posición está bajo ataque
bool IA_Dificil::estaBajoAtaque(Tablero& tablero, int x, int y, bool esPiezaNegra) {
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



// función para obtener valor de pieza
int IA_Dificil::obtenerValorPieza(int tipoPieza) {

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

int IA_Dificil::getMovX() const { return mov_x_IA; }
int IA_Dificil::getMovY() const { return mov_y_IA; }
bool IA_Dificil::getVerificaMov() const { return verifica_mov; }
int IA_Dificil::getPosicionSelecc() const { return posicion_selecc; }

