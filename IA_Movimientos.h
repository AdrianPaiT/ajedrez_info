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
