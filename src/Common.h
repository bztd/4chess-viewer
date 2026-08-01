#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define PROGRAM_NAME "4ChessViewer"

enum {
    ID_Load_File = 5634,
    ID_TECLA_EPACIO,
    ID_Btn_PGN,
    ID_Btn_Partidas,
    ID_Auto_Play,
    ID_Open_Option_Style_Board_Dialog,
    ID_MenuGame_Inicio_Game,
    ID_MenuGame_Next_Jugada,
    ID_MenuGame_Anterior_Jugada,
    ID_MenuGame_Last_Jugada
};

enum class PlayerColor {
    WHITE,
    BLACK,
    RED,
    YELLOW,
    ZOMBI,
    NONE
};

enum class PieceType {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    NONE
};

enum class STKing{
    CHECK,CHECKMATE,FREE
};

struct Corxy{
    int x,y;

    Corxy(){x=-1;y=-1;}
    Corxy(int xx, int yy):x(xx),y(yy){}
    void setXY(int xx, int yy){x=xx;y=yy;}
    void addXY(int xx, int yy){x=xx+x;y=yy+y;}
};

struct Piece {
    PieceType type;
    PlayerColor color;
    bool initial_position = true;
    bool se_movio_dos_posiciones=false;//para peon, captura al paso
    bool is_an_orphan_piece=false;//implementar//cuando el jugador se rinde
    Corxy pos;//posicion en el tablero
    Corxy initial_pos;//cordenadas iniciales
    size_t id=0;

    Piece() : type(PieceType::NONE), color(PlayerColor::NONE),initial_pos(-1,-1) {}
    Piece(PieceType t, PlayerColor c) : type(t), color(c),initial_pos(-1,-1) {}

    bool isInitialPosition() const {return initial_position;}
    PlayerColor getPlayerColor() const {return color;}
    void setPlayerColor(PlayerColor pl){color=pl;}
    PieceType getPieceType() const {return type;}
    bool isEmpty() const { return type == PieceType::NONE; }
    void setId(size_t i_d){
        id = i_d;
    }
    size_t getId(){
        return id;
    }
    void setPosition(size_t i, size_t j){
        if(initial_pos.x==-1){
            initial_pos.x=i;
            initial_pos.y=j;
        }

        pos.x=i;
        pos.y=j;

    }
    void Reset(){
        se_movio_dos_posiciones=false;
        initial_position = true;
        is_an_orphan_piece=false;

        pos.x=initial_pos.x;
        pos.y=initial_pos.y;
    }
};

#endif // COMMON_H_INCLUDED
