#ifndef CBPAGINA_H_INCLUDED
#define CBPAGINA_H_INCLUDED

#include <tuple>
#include <list>

#include "Common.h"
#include "CBTurn.h"
#include "CBJugador.h"

class Pagina{
public:
    Pagina(int i, int j,const Turn& t){
        i_jugada=i;
        e_jugada=j;
        turno=t;
    }
    void setJugadores(std::vector<Jugador>& v_j){
        jugadores=v_j;
    }
    std::tuple<int,int> getCorJugadaNotacion(){
        std::tuple<int,int> r(i_jugada,e_jugada);
        return r;
    }
    std::vector<Jugador> getJugadores(){
        return jugadores;
    }
    std::vector<std::vector<size_t>> getBoard() const{
        return board;
    }
    std::vector<int> getvJugadaActual()const{
        return jugada_actual;
    }
    int getIndiceDeJugada()const{
        return indice_de_jugada;
    }
    std::list<size_t> getHuerfanas() const{
        return id_p_huerfanas;
    }
    Turn getTurno(){
        return turno;
    }
    void setBoard(std::vector<std::vector<size_t>>& b){
        board=b;
    }
    void setvJugadaActual(std::vector<int>& b){
        jugada_actual = b;
    }
    void setHuerfanas(std::list<size_t>& b){
        id_p_huerfanas=b;
    }
    void setIndiceDeJugada(int b){
        indice_de_jugada = b;
    }

private:
    int i_jugada;//para identificar notacion
    int e_jugada;
    Turn turno;
    int indice_de_jugada = -1;
    std::vector<int> jugada_actual={-1,-1,-1,-1};
    std::vector<Jugador> jugadores;
    std::vector<std::vector<size_t>> board;
    std::list<size_t> id_p_huerfanas;
};
#endif // CBPAGINA_H_INCLUDED
