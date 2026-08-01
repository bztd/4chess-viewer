#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <memory>
#include <cmath>
#include <tuple>
#include <vector>
#include <deque>

#include <wx/wx.h>

#include "Common.h"
#include "CBPagina.h"
#include "CBJugador.h"
#include "CBTurn.h"
#include "ChessParameters.h"

#define NOPIEZA 3000

class ChessBoard : public wxPanel {
public:
    ChessBoard(wxFrame* parent);

    void setFunctionStatus(std::function<void(const wxString&)> f);
    bool MovePiece(int xi,int yi,int xe,int ye);
    void Reset();
    bool restorePosition(long pos_click);
    bool restoreDesdePagina(size_t pg);
    bool saveBoard(int xi,int yi, int xe,int ye, int ic,int ec);
    void anularDibujado(bool dbj);
    void setChessParameters(ChessParameters* ptr);
    size_t getIndiceDeJugada();

    int goToLastPage();//para ChessFrame

private:
    static const int BOARD_SIZE = 14;
    std::vector<std::vector<size_t>> board;
    std::vector<std::vector<size_t>> init_state_board;
    wxMessageOutputStderr out_consola;//borrar?

    bool se_permite_dibujar = true;

    Turn playerTurn;
    ChessParameters* c_parameters = nullptr;

    int cellSize;
    wxPoint selectedCell;
    bool pieceSelected;
    std::vector<Jugador> jugadores;//w;R;B;Y
    Piece p_empty;
    std::vector<int> jugada_actual={-1,-1,-1,-1};
    size_t indice_de_jugada = 0; //para sincronizacion con ChessNotation en ChessFrame
    std::list<size_t> p_huerfanas;//piezas de jugadores con rey activo
                                  //pero movido por la maquina
    std::vector<std::shared_ptr<Pagina>> libro;

    std::function<void(const wxString&)> _Status=[](const wxString& msg) {
        wxLogMessage(msg);
    };

    void InitializeBoard();
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void onMouseClick(wxMouseEvent& event);

    void DrawBoard(wxDC& dc);
    void DrawPieces(wxDC& dc);
    void DrawPiece(wxDC& dc, int row, int col, const Piece *piece, bool is_activo);
    void DrawTurnIndicator(wxDC& dc);
    void DrawMotionHighlighter(wxDC& dc);

    void restoreBoard(std::shared_ptr<Pagina>& pg);
    void savePage(int ic, int ec);

    void setEventos();

    void CreaJugadores();
    void FillBoard(size_t i, size_t j,PieceType p, PlayerColor c);
    wxColor getCellColor(int row, int col) const;
    wxColor getPlayerColor(PlayerColor color) const;
    wxPoint GetCellFromPosition(const wxPoint& pos) const;
    bool IsValidCell(int row, int col);
    bool IsValidMove(Corxy posInit, Corxy posEnd,bool validate_turn=true);
    bool IsPlayableCell(int row, int col) const;
    int ProductoPunto(const Corxy& a,const Corxy& b) const;
    STKing StateOfTheKing(Jugador& j_g);
    void Rotar(Corxy& vect, size_t angle, float origen_x=7.5, float origen_y=7.5);
    void RotarV(auto &xx, auto &yy, const int angulo);
    std::tuple<bool,int,int,int,int> IsCastling(Corxy posInit, Corxy posEnd);
    std::vector<Piece*> quienControLaCasilla(Corxy& c,std::vector<Jugador*> jgds);
    std::vector<Corxy> PuntosEntreDosPuntos(Corxy a, Corxy b, PieceType p_t);
    Jugador* getJugador(PlayerColor pl_c);
    Piece* getPiece(size_t id);
    void MovimientosDeReyValidos(Piece* _King,
                                 std::vector<Corxy>& mv_R,
                                 std::vector<Jugador*>& jdres
                                );

    wxString getPieceSymbol(const Piece *piece,bool is_activo) const;
};

#endif
