#pragma once
#include "Tablero.h"


class Tablero;   //Declaración anticipada

class IA_Movimientos {

public:

    virtual ~IA_Movimientos() = default;

    // Método para realizar movimiento
    virtual void mover(Tablero& tablero) = 0;

    // Métodos de acceso a miembros protegidos
    virtual int getMovX() const = 0;
    virtual int getMovY() const = 0;
    virtual bool getVerificaMov() const = 0;
    virtual int getPosicionSelecc() const = 0;
};




class IA_Facil : public IA_Movimientos {

    int mov_x_IA = -1;
    int mov_y_IA = -1;
    bool verifica_mov = false;
    int posicion_selecc = -1; 

public:

    void mover(Tablero& tablero) override;

    //Métodos de acceso
    int getMovX() const override;
    int getMovY() const override;
    bool getVerificaMov() const override;
    int getPosicionSelecc() const override;

};





 

class IA_Medio : public IA_Movimientos {

    int mov_x_IA;
    int mov_y_IA;
    bool verifica_mov;
    int posicion_selecc;

    //Valores de las piezas
    const int VALOR_PEON = 10;
    const int VALOR_CABALLO = 30;
    const int VALOR_ALFIL = 30;
    const int VALOR_ARZOBISPO = 200;
    const int VALOR_CANCILLER = 200;
    const int VALOR_TORRE = 50;
    const int VALOR_DAMA = 400;
    const int VALOR_REY = 1000;

public:

   void mover(Tablero& tablero) override;

   //Métodos de acceso
   int getMovX() const override;
   int getMovY() const override;
   bool getVerificaMov() const override;
   int getPosicionSelecc() const override;

   //Obtener valor de pieza para evaluación
   int obtenerValorPieza(int tipoPieza);

   //Verificar si una posición está bajo ataque
   bool estaBajoAtaque(Tablero& tablero, int x, int y, bool esPiezaNegra);

};



class IA_Dificil : public IA_Movimientos {

    int mov_x_IA;
    int mov_y_IA;
    bool verifica_mov;
    int posicion_selecc;

    //Valores de las piezas
    const int VALOR_PEON = 10;
    const int VALOR_CABALLO = 30;
    const int VALOR_ALFIL = 30;
    const int VALOR_ARZOBISPO = 200;
    const int VALOR_CANCILLER = 200;
    const int VALOR_TORRE = 50;
    const int VALOR_DAMA = 400;
    const int VALOR_REY = 1000;

    // Estrategia basada en la fase del juego
    enum FaseJuego { APERTURA, MEDIO_JUEGO, FINAL };
    FaseJuego faseActual = APERTURA;
    int contadorTurnos = 0;

public:

    void mover(Tablero& tablero) override;

    //Métodos de acceso
    int getMovX() const override;
    int getMovY() const override;
    bool getVerificaMov() const override;
    int getPosicionSelecc() const override;

    //Obtener valor de pieza para evaluación
    int obtenerValorPieza(int tipoPieza);

    //Verificar si una posición está bajo ataque
    bool estaBajoAtaque(Tablero& tablero, int x, int y, bool esPiezaNegra);

    //Función de evaluación mejorada
    float evaluarPosicion(Tablero& tablero, bool esNegras);

    //Generar todos los movimientos válidos
    std::vector<std::tuple<int, int, int, int, int>> generarTodosMovimientos(Tablero& tablero, bool esNegras);

    //Verificar si el movimiento pone una pieza valiosa en riesgo
    //PRESCINDIBLE
    float calcularRiesgo(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y);

    // Determinar la fase actual del juego
    void actualizarFaseJuego(Tablero& tablero);

    // Evaluación específica de apertura
    //PRESCINDIBLE
    float evaluarApertura(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y);

    // Evaluación específica de medio juego
     //PRESCINDIBLE
    float evaluarMedioJuego(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y);

    // Evaluación específica de final
     //PRESCINDIBLE
    float evaluarFinal(Tablero& tablero, int origen_x, int origen_y, int dest_x, int dest_y);

    // Buscar oportunidades de ataque al rey
    //PRESCINDIBLE
    float evaluarAtaqueAlRey(Tablero& tablero, int dest_x, int dest_y);

    // Comprueba si una posición es vulnerable a captura por peones enemigos
    bool esVulnerableAPeon(Tablero& tablero, int x, int y);

    // Verifica si una posición está protegida por otro peón aliado
    bool estaProtegidoPorPeon(Tablero& tablero, int x, int y);

    // Obtiene el valor de la amenaza más pequeña que ataca una casilla
    int valorAmenazaMinima(Tablero& tablero, int x, int y);
};

