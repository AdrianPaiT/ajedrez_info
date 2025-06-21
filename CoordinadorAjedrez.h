#pragma once
#include "Tablero.h"
#include "ETSIDI.h"


class coordinador {
    Mundo mundo;        // Objeto que representa el mundo/entorno del juego
    
public:

    void dibuja();      // Método para renderizar/dibujar el juego
    void tecla(unsigned char key);  // Maneja eventos de teclado
    void Boton_Raton(int num_rival, int x, int y, int boton, bool abajo, bool espacio, bool ref_tecla);  // Maneja eventos del ratón


    //Gestión de músicas
    bool musicaJuegoActiva;
    bool musicaMenuActiva;

};
// Enumeración que define los posibles estados del juego
enum {
    JUEGO = 0,      // Estado principal de juego
    START,          // Pantalla de inicio
    MENU_1C1,       // Menú para 1 jugador contra 1 jugador
    INSTRUCC_1C1,   // Instrucciones para el modo 1C1
    MENU_BOT,       // Menú para jugar contra la máquina
    INSTRUCC_BOT,   // Instrucciones para el modo contra bot
    RATON,          // Estado relacionado con interacción por ratón
    GANA_BLANCAS,   // Estado cuando ganan las blancas
    GANA_NEGRAS,    // Estado cuando ganan las negras
    TABLAS          // Estado cuando hay empate
};
