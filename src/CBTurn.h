#ifndef CBTURN_H_INCLUDED
#define CBTURN_H_INCLUDED

struct Turn{
    size_t n=0;
    std::vector<size_t> jugadores_muertos;

    std::vector<PlayerColor> gP={
            PlayerColor::WHITE,PlayerColor::RED,
            PlayerColor::BLACK,PlayerColor::YELLOW
    };
    std::vector<Corxy> indicator_cors={
            Corxy(11,13),Corxy(0,11),
            Corxy(2,0),Corxy(13,2)
    };

    Turn(){}

    void Next(){
        while(true){
            n++;
            if(n>3){n=0;}
            bool finalizar_loop=true;
            for(size_t k=0; k<jugadores_muertos.size();k++){
                if(jugadores_muertos[k]==n){finalizar_loop=false;break;}
            }

            if (finalizar_loop) {break;}
        }

    }
    void JugadorActualEnMate(){
        jugadores_muertos.push_back(n);
        Next();
    }
    void JugadorEnMate(const PlayerColor& pl){
        size_t n = 0;
        for( ; n < gP.size(); n++ ){
            if( pl == gP[n] ){break;}
        }
        jugadores_muertos.push_back(n);
    }
    void setTurno(size_t tt){
        n=tt;
    }
    PlayerColor getCurrent() const {
        return gP[n];
    }
    size_t getTurno() const {
        return n;
    }
    size_t getTurno(PlayerColor p_c) const {
        for(size_t w=0; w<gP.size();w++){
            if(gP[w]==p_c){
                return w;
            }
        }
        wxLogError(_("Error en Turn::getTurno(..), elemento no valido"));
        return 9;
    }
    Corxy getCorsIndicator() const {
        return indicator_cors[n];
    }
    void Reset(){
        n=0;
        jugadores_muertos.clear();
    }

};

#endif // CBTURN_H_INCLUDED
