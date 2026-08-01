#ifndef CBPJUGADOR_H_INCLUDED
#define CBPJUGADOR_H_INCLUDED

struct Jugador{
    PlayerColor color=PlayerColor::NONE;
    std::list<size_t> id_piezas;
    std::deque<Piece> d_piezas;//0..15
    std::deque<Piece> d_queens;//16..23
    bool is_an_active_player=true;
    size_t id_king=0;
    std::vector<Corxy> valid_movements;//para jaques, movimientos de rey
    size_t id_pieza_movida=6000; //para captura al paso
    size_t rotacion;//para calcular posicion de piezas
    Corxy direccion_de_coronacion;
    size_t id_base=0;

    Jugador(PlayerColor c){
        color=c;
        switch(c){
            case PlayerColor::WHITE:
                rotacion=270;
                direccion_de_coronacion.setXY(-1,0);
                break;
            case PlayerColor::RED:
                rotacion=180;
                direccion_de_coronacion.setXY(0,1);
                id_base = 24;
                break;
            case PlayerColor::BLACK:
                rotacion=90;
                direccion_de_coronacion.setXY(1,0);
                id_base = 48;
                break;
            case PlayerColor::YELLOW:
                rotacion=0;
                direccion_de_coronacion.setXY(0,-1);
                id_base = 72;
                break;
            case PlayerColor::ZOMBI:
            case PlayerColor::NONE:
                break;

        }
    }
    size_t getIndice(int id){
        size_t n = id % 24;

        if(n<16){
            return  n;
        }else{
            return n-16;
        }
    }

    Piece* getPiece(size_t id){
        size_t n = id % 24;

        if(n<16){
            return  &d_piezas[n];
        }else{
            return &d_queens[n-16];
        }

    }

    Corxy getDireccionDeCoronacion(){
        return direccion_de_coronacion;
    }
    size_t getRotacion(){
        return rotacion;
    }
    size_t PromotionToQueen(size_t id,int i=-1,int j=-1){
        Piece* p = getPiece(id);

        d_queens.emplace_back(PieceType::QUEEN, p->getPlayerColor());
        Piece* n_p = &d_queens.back();

        size_t id_q = id_base + d_queens.size() + 16 - 1;

        if(i!=-1){
            n_p->setPosition(i,j);
        }else{
            n_p->setPosition(p->pos.x,p->pos.y);
        }

        for(size_t& id_p:id_piezas){//cambia de pieza
            if(id_p==id){
                id_p=id_q;
                break;
            }
        }
        return id_q;
    }

    //se remueve las piezas de la lista para que no se consideren
    //en los calculos de jaque, menos el rey
    void Surrender(std::list<size_t>& id_p_h){//piezas huerfanas
        for(const size_t& id: id_piezas){
            if(id!=id_king){
                Piece* p = getPiece(id);

                p->setPlayerColor(PlayerColor::ZOMBI);
                id_p_h.push_back(id);
            }
        }

        id_piezas.clear();
        id_piezas.push_back(id_king);
    }

    size_t setPiece(size_t i,size_t j,PieceType pt){

        for(Piece& pc:d_piezas){  //si la pieza ya ha sido inicializada
            if(pc.getPieceType()==pt){
                if(pc.pos.x==(int)i && pc.pos.y==(int)j){
                    pc.Reset();
                    return pc.getId();
                }
            }
        }

        size_t id = id_base + d_piezas.size();

        d_piezas.emplace_back(pt,color);
        Piece* ptrP=&d_piezas.back();

        id_piezas.push_back(id);

        ptrP->setPosition(i,j);
        ptrP->setId(id);

        if(pt==PieceType::KING){id_king=id;}

        if(d_piezas.size()>16){
            wxLogDebug("Error en Jugador::setPiece, numero de piezas mayor al permitido.");
        }

        return id;
    }
    void RemovePiece(size_t id){
        id_piezas.remove(id);
        if(id == id_pieza_movida){id_pieza_movida=6000;}
    }

    PlayerColor getPlayerColor(){
        return color;
    }

    bool isAnActivePlayer(){
        return is_an_active_player;
    }
    void JaqueMate(){
        is_an_active_player=false;
    }
    void Reset(){
        is_an_active_player=true;
        id_pieza_movida=6000;

        id_piezas.clear();
        d_queens.clear();

        for(Piece& p:d_piezas){
            p.Reset();
            p.setPlayerColor(color);
            id_piezas.push_back(p.getId());
        }
    }
};

#endif // CBPJUGADOR_H_INCLUDED
