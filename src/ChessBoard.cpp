#include "ChessBoard.h"

ChessBoard::ChessBoard(wxFrame* parent)
    : wxPanel(parent, wxID_ANY),
      cellSize(40),
      selectedCell(-1, -1),
      pieceSelected(false) {

    board.resize(BOARD_SIZE, std::vector<size_t>(BOARD_SIZE));
    CreaJugadores();
    p_empty=Piece();
    InitializeBoard();

    SetMinSize(wxSize(560, 560));

    setEventos();
}
void ChessBoard::CreaJugadores(){
    jugadores.push_back(Jugador(PlayerColor::WHITE));
    jugadores.push_back(Jugador(PlayerColor::RED));
    jugadores.push_back(Jugador(PlayerColor::BLACK));
    jugadores.push_back(Jugador(PlayerColor::YELLOW));
}
void ChessBoard::setEventos(){
    Bind(wxEVT_PAINT, &ChessBoard::OnPaint, this);
    Bind(wxEVT_SIZE, &ChessBoard::OnSize, this);
    Bind(wxEVT_LEFT_DOWN, &ChessBoard::onMouseClick, this);
}
void ChessBoard::FillBoard(size_t i, size_t j,PieceType p, PlayerColor c){
    switch(c){
        case PlayerColor::BLACK:
            board[i][j]=jugadores[2].setPiece(i,j,p);
            break;
        case PlayerColor::WHITE:
            board[i][j]=jugadores[0].setPiece(i,j,p);
            break;
        case PlayerColor::RED:
            board[i][j]=jugadores[1].setPiece(i,j,p);
            break;
        case PlayerColor::YELLOW:
            board[i][j]=jugadores[3].setPiece(i,j,p);
            break;
        case PlayerColor::NONE:
            wxLogError("Error: Asignacion de color de jugador invalida.");
            break;
        default:
            break;
    }
}
void ChessBoard::InitializeBoard() {

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = NOPIEZA;
        }
    }

    for (int i = 3; i <= 10; i++) {
        FillBoard(12,i,PieceType::PAWN, PlayerColor::WHITE);
    }
    FillBoard(13,3,PieceType::ROOK, PlayerColor::WHITE);
    FillBoard(13,10,PieceType::ROOK, PlayerColor::WHITE);
    FillBoard(13,4,PieceType::KNIGHT, PlayerColor::WHITE);
    FillBoard(13,9,PieceType::KNIGHT, PlayerColor::WHITE);
    FillBoard(13,5,PieceType::BISHOP, PlayerColor::WHITE);
    FillBoard(13,8,PieceType::BISHOP, PlayerColor::WHITE);
    FillBoard(13,6,PieceType::QUEEN, PlayerColor::WHITE);
    FillBoard(13,7,PieceType::KING, PlayerColor::WHITE);

    for (int i = 3; i <= 10; i++) {
        FillBoard(1,i,PieceType::PAWN, PlayerColor::BLACK);
    }
    FillBoard(0,3,PieceType::ROOK, PlayerColor::BLACK);
    FillBoard(0,10,PieceType::ROOK, PlayerColor::BLACK);
    FillBoard(0,4,PieceType::KNIGHT, PlayerColor::BLACK);
    FillBoard(0,9,PieceType::KNIGHT, PlayerColor::BLACK);
    FillBoard(0,5,PieceType::BISHOP, PlayerColor::BLACK);
    FillBoard(0,8,PieceType::BISHOP, PlayerColor::BLACK);
    FillBoard(0,7,PieceType::QUEEN, PlayerColor::BLACK);
    FillBoard(0,6,PieceType::KING, PlayerColor::BLACK);

    for (int i = 3; i <= 10; i++) {
        FillBoard(i,1,PieceType::PAWN, PlayerColor::RED);
    }
    FillBoard(3,0,PieceType::ROOK, PlayerColor::RED);
    FillBoard(10,0,PieceType::ROOK, PlayerColor::RED);
    FillBoard(4,0,PieceType::KNIGHT, PlayerColor::RED);
    FillBoard(9,0,PieceType::KNIGHT, PlayerColor::RED);
    FillBoard(5,0,PieceType::BISHOP, PlayerColor::RED);
    FillBoard(8,0,PieceType::BISHOP, PlayerColor::RED);
    FillBoard(6,0,PieceType::QUEEN, PlayerColor::RED);
    FillBoard(7,0,PieceType::KING, PlayerColor::RED);

    for (int i = 3; i <= 10; i++) {
        FillBoard(i,12,PieceType::PAWN, PlayerColor::YELLOW);
    }
    FillBoard(3,13,PieceType::ROOK, PlayerColor::YELLOW);
    FillBoard(10,13,PieceType::ROOK, PlayerColor::YELLOW);
    FillBoard(4,13,PieceType::KNIGHT, PlayerColor::YELLOW);
    FillBoard(9,13,PieceType::KNIGHT, PlayerColor::YELLOW);
    FillBoard(5,13,PieceType::BISHOP, PlayerColor::YELLOW);
    FillBoard(8,13,PieceType::BISHOP, PlayerColor::YELLOW);
    FillBoard(7,13,PieceType::QUEEN, PlayerColor::YELLOW);
    FillBoard(6,13,PieceType::KING, PlayerColor::YELLOW);

    init_state_board = board;
}
void ChessBoard::Reset(){
    jugada_actual={-1,-1,-1,-1};
    p_huerfanas.clear();
    board = init_state_board;
    se_permite_dibujar = true;
    indice_de_jugada = 0;

    playerTurn.Reset();
    pieceSelected=false;

    for(Jugador& jj:jugadores){
        jj.Reset();
    }

    Refresh();
}

bool ChessBoard::IsPlayableCell(int row, int col) const {

    if (row >= 3 && row <= 10 && col >= 3 && col <= 10) {
        return true;
    }

    if ((row >= 0 && row <= 2) || (row >= 11 && row <= 13)) {
        if (col >= 3 && col <= 10) {
            return true;
        }
    }

    if ((col >= 0 && col <= 2) || (col >= 11 && col <= 13)) {
        if (row >= 3 && row <= 10) {
            return true;
        }
    }

    return false;
}

void ChessBoard::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    wxSize size = GetClientSize();
    wxBitmap bitmap(size.GetWidth(), size.GetHeight());
    wxMemoryDC memDC(bitmap);

    DrawBoard(memDC);
    DrawPieces(memDC);
    DrawTurnIndicator(memDC);

    // Resaltar celda seleccionada
    if (pieceSelected && selectedCell.x >= 0 && selectedCell.y >= 0) {
        memDC.SetPen(wxPen(*wxRED, 3));
        memDC.SetBrush(*wxTRANSPARENT_BRUSH);
        memDC.DrawRectangle(selectedCell.y * cellSize, selectedCell.x * cellSize,
                        cellSize, cellSize);
    }

    DrawMotionHighlighter(memDC);//dibuja recuadros para origen y destino de movimiento

    dc.Blit(0, 0, size.GetWidth(), size.GetHeight(), &memDC, 0, 0);
}
std::tuple<bool,int,int,int,int> ChessBoard::IsCastling(Corxy posInit, Corxy posEnd){
    std::tuple<bool,int,int,int,int> res={false,0,0,0,0};
    int diffx=posEnd.x-posInit.x;
    int diffy=posEnd.y-posInit.y;

    Piece* p_init = getPiece(board[posInit.x][posInit.y]);
    Piece* p_end = getPiece(board[posEnd.x][posEnd.y]);

    if(
        p_init->getPieceType()==PieceType::KING &&
        p_init->isInitialPosition() &&
        p_end->isEmpty() &&
        std::abs(diffy+diffx)==2 &&
        p_init->getPlayerColor()==playerTurn.getCurrent()
       ){
            int i=posInit.x,j=posInit.y;
            int *p=nullptr;
            int increase=0;
            int new_x_rook=0,new_y_rook=0;
            if(diffx==0){
                increase=diffy/std::abs(diffy);
                p=&j;
                new_x_rook=posEnd.x;
                new_y_rook=posEnd.y-increase;
            }
            if(diffy==0){
                increase=diffx/std::abs(diffx);
                p=&i;
                new_x_rook=posEnd.x-increase;
                new_y_rook=posEnd.y;
            }

            while(i>=0 && i<14 && j>=0 && j<14){
                *p=(*p)+increase;
                if(!IsPlayableCell(i, j)){break;}

                Piece* p_test = getPiece(board[i][j]);

                if(board[i][j] != NOPIEZA){
                    if(p_test->getPieceType()!=PieceType::ROOK){
                        break;
                    }else{
                        if(p_test->isInitialPosition()){
                            res = std::make_tuple(true, i,j,new_x_rook,new_y_rook);
                            break;
                        }else{
                            break;
                        }
                    }
                }

            }
       }

       return res;
}
bool ChessBoard::IsValidMove(Corxy posInit, Corxy posEnd, bool validate_turn){

    if(!IsPlayableCell(posEnd.x,posEnd.y)){return false;}

    Piece* p_movida = getPiece(board[posInit.x][posInit.y]);
    Piece* p_destino = getPiece(board[posEnd.x][posEnd.y]);

    if(validate_turn){
        if(p_movida->getPlayerColor()!= playerTurn.getCurrent()){
            return false;
        }
    }

    if( p_destino->getPlayerColor() == p_movida->getPlayerColor() ){
        return false;
    }

    int invalidMove=1;
    PieceType piece=p_movida->getPieceType();
    switch(piece){
        case PieceType::PAWN:
            size_t moveLength;
            moveLength=std::abs(posInit.x-posEnd.x)+std::abs(posInit.y-posEnd.y);
            if(moveLength==1){
                switch(playerTurn.getCurrent()){
                    case PlayerColor::WHITE:
                        if( posInit.y==posEnd.y && posInit.x > posEnd.x){
                            if(p_destino->isEmpty()){
                                invalidMove=0;
                            }
                        }
                        break;
                    case PlayerColor::BLACK:
                        if( posInit.y==posEnd.y && posInit.x < posEnd.x){
                            if(p_destino->isEmpty()){
                                invalidMove=0;
                            }
                        }
                        break;
                    case PlayerColor::RED:
                        if( posInit.x==posEnd.x && posInit.y < posEnd.y){
                            if(p_destino->isEmpty()){
                                invalidMove=0;
                            }
                        }
                        break;
                    case PlayerColor::YELLOW:
                        if( posInit.x==posEnd.x && posInit.y > posEnd.y){
                            if(p_destino->isEmpty()){
                                invalidMove=0;
                            }
                        }
                        break;
                    case PlayerColor::ZOMBI:
                    case PlayerColor::NONE:
                        invalidMove++;
                }

            }else{
                //Peon mueve dos y capturas
                if(moveLength==2){
                    switch(p_movida->getPlayerColor()){
                        case PlayerColor::WHITE:
                            if(
                                posInit.y==posEnd.y &&
                                posInit.x > posEnd.x &&
                                p_destino->isEmpty() &&
                                p_movida->isInitialPosition()
                               ){
                                if(p_destino->isEmpty()){/////borrrar
                                    invalidMove=0;
                                }
                            }
                            //captura
                            if(
                                posInit.x > posEnd.x &&
                                posInit.y != posEnd.y &&
                                !p_destino->isEmpty() &&
                                p_destino->getPlayerColor() != PlayerColor::WHITE
                               ){
                                invalidMove=0;
                            }
                            break;
                        case PlayerColor::BLACK:
                            if(
                                posInit.y==posEnd.y &&
                                posInit.x < posEnd.x &&
                                p_destino->isEmpty() &&
                                p_movida->isInitialPosition()
                              ){
                                if(p_destino->isEmpty()){
                                    invalidMove=0;
                                }
                            }
                            if(
                                posInit.x < posEnd.x &&
                                posInit.y != posEnd.y &&
                                !p_destino->isEmpty() &&
                                p_destino->getPlayerColor() != PlayerColor::BLACK
                               ){
                                invalidMove=0;
                            }
                            break;
                        case PlayerColor::RED:
                            if(
                                posInit.x==posEnd.x &&
                                posInit.y < posEnd.y &&
                                p_destino->isEmpty() &&
                                p_movida->isInitialPosition()
                              ){
                                if(p_destino->isEmpty()){
                                    invalidMove=0;
                                }
                            }
                            if(
                                posInit.y < posEnd.y &&
                                posInit.x != posEnd.x &&
                                !p_destino->isEmpty() &&
                                p_destino->getPlayerColor() != PlayerColor::RED
                               ){
                                invalidMove=0;
                            }

                            break;
                        case PlayerColor::YELLOW:
                            if(
                                posInit.x==posEnd.x &&
                                posInit.y > posEnd.y &&
                                p_destino->isEmpty() &&
                                p_movida->isInitialPosition()
                              ){
                                if(p_destino->isEmpty()){
                                    invalidMove=0;
                                }
                            }
                            if(
                                posInit.y > posEnd.y &&
                                posInit.x != posEnd.x &&
                                !p_destino->isEmpty() &&
                                p_destino->getPlayerColor() != PlayerColor::YELLOW
                               ){
                                invalidMove=0;
                            }
                            break;
                        case PlayerColor::ZOMBI:
                        case PlayerColor::NONE:
                            invalidMove++;
                    }

                }else{
                    invalidMove++;
                }
            }

            break;
        case PieceType::QUEEN:
        case PieceType::ROOK:
            if(
                (posEnd.x==posInit.x && std::abs(posEnd.y-posInit.y)>=1) ||
                (posEnd.y==posInit.y && std::abs(posEnd.x-posInit.x)>=1)
               ){
                int i,j;
                int *p=nullptr;
                int ult=0;

                if(posEnd.x==posInit.x){//misma fila
                    j=posEnd.x;
                    if(posInit.y>posEnd.y){
                        i=posEnd.y+1;
                        ult=posInit.y;
                    }else{
                        i=posInit.y+1;
                        ult=posEnd.y;
                    }
                    p=&i;
                }else{
                    i=posEnd.y;
                    if(posInit.x>posEnd.x){
                        j=posEnd.x+1;
                        ult=posInit.x;
                    }else{
                        j=posInit.x+1;
                        ult=posEnd.x;
                    }
                    p=&j;
                }

                invalidMove=0;
                for(;*p<ult;(*p)++){

                    Piece* p_temp = getPiece(board[j][i]);
                    if(!p_temp->isEmpty()){
                        invalidMove=1;
                        break;
                    }
                }

            }
            if(piece==PieceType::ROOK){break;}
            if(invalidMove==0){break;}

        case PieceType::BISHOP:
            if(std::abs(posEnd.x-posInit.x)==std::abs(posEnd.y-posInit.y)){
                int im,jm;
                if((posEnd.x-posInit.x)>0){jm=1;}else{jm=-1;}
                if((posEnd.y-posInit.y)>0){im=1;}else{im=-1;}
                int i=posInit.y,j=posInit.x;

                invalidMove=0;
                while(
                        j<14 && j>-1 &&
                        i<14 && i>-1
                    ){
                    i=i+im;
                    j=j+jm;
                    if(i==posEnd.y){break;}
                    //std::cout<<GetPieceSymbol(board[j][i])<<" "<<i<<" "<<j<<"\n"<<std::flush;
                    Piece* p_temp2 = getPiece(board[j][i]);
                    if(p_temp2->getPieceType()!=PieceType::NONE){
                        invalidMove=1;
                        break;
                    }
                }

            }
            break;
        case PieceType::KNIGHT:
            if(
                (std::abs(posEnd.x-posInit.x)==1 && std::abs(posEnd.y-posInit.y)==2) ||
                (std::abs(posEnd.x-posInit.x)==2 && std::abs(posEnd.y-posInit.y)==1)
               ){
                invalidMove=0;
            }
            break;
        case PieceType::KING:
            if(std::abs(posEnd.x-posInit.x)<=1 && std::abs(posEnd.y-posInit.y)<=1){
                invalidMove=0;
            }
            break;
        case PieceType::NONE:
            invalidMove++;
            break;
    }

    if(invalidMove==0){
        return true;
    }else{
        return false;
    }
}

void ChessBoard::OnSize(wxSizeEvent& event) {
    wxSize size = GetClientSize();
    int minDim = std::min(size.GetWidth(), size.GetHeight());
    cellSize = minDim / BOARD_SIZE;

    Refresh();
    event.Skip();
}

void ChessBoard::DrawTurnIndicator(wxDC& dc){
    Corxy indicator=playerTurn.getCorsIndicator();

    int centerX = indicator.x * cellSize + cellSize / 2;
    int centerY = indicator.y * cellSize + cellSize / 2;
    dc.SetPen(*wxTRANSPARENT_PEN); // Elimina el borde
    dc.SetBrush(wxBrush(wxColour(0, 0, 255))); // Azul sólido
    dc.DrawRectangle(centerX, centerY, 10, 10);
}

void ChessBoard::DrawMotionHighlighter(wxDC& dc){
    if(jugada_actual[1]==-1){return;}

    dc.SetPen(wxPen(wxColor("#fd1f6e"), 3));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    //origen
    dc.DrawRectangle(jugada_actual[1]*cellSize, jugada_actual[0]*cellSize,
                    cellSize, cellSize);

    //destino
    dc.DrawRectangle(jugada_actual[3]*cellSize, jugada_actual[2]*cellSize,
                    cellSize, cellSize);
}

void ChessBoard::DrawBoard(wxDC& dc) {
    dc.SetBackground(wxBrush(wxColor(100, 100, 100)));
    dc.Clear();

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            wxColor cellColor = getCellColor(row, col);
            dc.SetBrush(wxBrush(cellColor));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(col * cellSize, row * cellSize, cellSize, cellSize);
        }
    }
    /*
    // Dibujar líneas de cuadrícula
    dc.SetPen(wxPen(wxColor(50, 50, 50), 1));
    for (int i = 0; i <= BOARD_SIZE; i++) {
        dc.DrawLine(0, i * cellSize, BOARD_SIZE * cellSize, i * cellSize);
        dc.DrawLine(i * cellSize, 0, i * cellSize, BOARD_SIZE * cellSize);
    }
    */
}

wxColor ChessBoard::getCellColor(int row, int col) const {
    if (!IsPlayableCell(row, col)) {
        return wxColor(100, 100, 100); // Gris
    }

    bool isLight = (row + col) % 2 == 0;
    return isLight ? c_parameters->getColorCasillaWhite() : c_parameters->getColorCasillaBlack();
}

wxColor ChessBoard::getPlayerColor(PlayerColor color) const {
    switch (color) {
        case PlayerColor::WHITE:
            return c_parameters->getColorPieceWhite();
        case PlayerColor::BLACK:
            return c_parameters->getColorPieceBlack();
        case PlayerColor::RED:
            return c_parameters->getColorPieceRed();
        case PlayerColor::YELLOW:
            return c_parameters->getColorPieceYellow();
        default:
            return *wxBLUE;
    }
}

void ChessBoard::DrawPieces(wxDC& dc) {
    /*
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (!board[row][col]->isEmpty()) {
                DrawPiece(dc, row, col, board[row][col]);
            }
        }
    }
    */
    for(Jugador& j :jugadores){
        for(const size_t& id:j.id_piezas){
            Piece* p = getPiece(id);
            DrawPiece(dc,p->pos.x,p->pos.y,p,j.isAnActivePlayer());
        }
    }

    for(const size_t& id:p_huerfanas){
        Piece* p = getPiece(id);
        DrawPiece(dc,p->pos.x,p->pos.y,p,false);
    }
}

wxString ChessBoard::getPieceSymbol(const Piece *piece, bool is_activo) const {
    // Usar símbolos Unicode para las piezas de ajedrez
    switch (piece->type) {
        case PieceType::KING:
            return (piece->color==PlayerColor::WHITE ||
                   piece->color==PlayerColor::YELLOW) &&
                   is_activo
                   ? L"♔" : L"♚";
        case PieceType::QUEEN:
            return (piece->color == PlayerColor::WHITE ||
                   piece->color == PlayerColor::YELLOW) &&
                   is_activo
                   ? L"♕" : L"♛";
        case PieceType::ROOK:
            return (piece->color == PlayerColor::WHITE ||
                   piece->color == PlayerColor::YELLOW) &&
                   is_activo
                   ? L"♖" : L"♜";
        case PieceType::BISHOP:
            return (piece->color == PlayerColor::WHITE ||
                   piece->color == PlayerColor::YELLOW) &&
                   is_activo
                   ? L"♗" : L"♝";
        case PieceType::KNIGHT:
            return (piece->color == PlayerColor::WHITE ||
                   piece->color == PlayerColor::YELLOW) &&
                   is_activo
                   ? L"♘" : L"♞";
        case PieceType::PAWN:
            return (piece->color == PlayerColor::WHITE ||
                   piece->color == PlayerColor::YELLOW) &&
                   is_activo
                   ? L"♙" : L"♟";
        default:
            return "E";
    }
}

void ChessBoard::DrawPiece(wxDC& dc, int row, int col, const Piece* piece, bool is_activo) {
    wxColor pieceColor = getPlayerColor(piece->color);

    // Crear círculo de fondo para la pieza
    int centerX = col * cellSize + cellSize / 2;
    int centerY = row * cellSize + cellSize / 2;
    int radius = cellSize / 3;

    if(is_activo){
        dc.SetBrush(wxBrush(pieceColor));

        // Color del texto (contraste)
        if (piece->color == PlayerColor::WHITE || piece->color == PlayerColor::YELLOW) {
            dc.SetTextForeground(*wxBLACK);
        } else {
            dc.SetTextForeground(*wxWHITE);
        }
    }else{
        dc.SetBrush(wxBrush(wxColor("#6e6d68")));
        dc.SetTextForeground(wxColor("#ab9374"));
    }
    dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
    dc.DrawCircle(centerX, centerY, radius);

    // Dibujar símbolo de la pieza
    wxFont font(cellSize / 2, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    dc.SetFont(font);

    wxString symbol = getPieceSymbol(piece,is_activo);
    wxSize textSize = dc.GetTextExtent(symbol);
    int textX = centerX - textSize.GetWidth() / 2;
    int textY = centerY - textSize.GetHeight() / 2;

    dc.DrawText(symbol, textX, textY);
}

wxPoint ChessBoard::GetCellFromPosition(const wxPoint& pos) const {
    int row = pos.y / cellSize;
    int col = pos.x / cellSize;

    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return wxPoint(row, col);
    }

    return wxPoint(-1, -1);
}

bool ChessBoard::IsValidCell(int row, int col){
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

std::vector<Piece*> ChessBoard::quienControLaCasilla(
                    Corxy& c,std::vector<Jugador*> jgds){

    std::vector<Piece*> r_piezas;
    for(size_t k=0;k<jgds.size();k++){

        for(const size_t& id:jgds[k]->id_piezas){
            Piece* p = getPiece(id);
            if(IsValidMove(p->pos,c,false)){
                r_piezas.push_back(p);
            }
        }
    }
    return r_piezas;
}

void ChessBoard::MovimientosDeReyValidos(Piece* _King,
                                         std::vector<Corxy>& mv_R,//movimientos de rey validos, retorno
                                         std::vector<Jugador*>& jdres//rivales
                                         ){
    std::vector<Corxy> mKing={
                            Corxy(-1,-1),Corxy(0,-1),Corxy(1,1),Corxy(1,0),
                            Corxy(1,-1),Corxy(-1,0),Corxy(0,1),Corxy(-1,1)
                        };
    const size_t _x=_King->pos.x;
    const size_t _y=_King->pos.y;
    size_t xtem;
    size_t ytem;
    std::vector<Piece*> _p_tem;

    mv_R.clear();

    for(const Corxy &c: mKing){//movimientos rey
        xtem=_x+c.x;
        ytem=_y+c.y;

        Corxy tem_pos(xtem, ytem);

        if(IsValidMove(_King->pos,tem_pos,false)){
            //se realiza el movimiento de rey;
            size_t id_save_piece=board[tem_pos.x][tem_pos.y];
            board[tem_pos.x][tem_pos.y]=_King->getId();
            board[_King->pos.x][_King->pos.y]=NOPIEZA;

            _p_tem=quienControLaCasilla(tem_pos,jdres);
            if(_p_tem.size()==0){
                mv_R.push_back(tem_pos);
            }

            //se devuelve todo a su sitio
            board[tem_pos.x][tem_pos.y]=id_save_piece;
            board[_King->pos.x][_King->pos.y]=_King->getId();
        }
    }
}

int ChessBoard::goToLastPage(){
    int p = libro.size()-1;
    if(restoreDesdePagina(p)){
        return p;
    }
    return -1;
}

STKing ChessBoard::StateOfTheKing(Jugador& j_g) {

    if(!j_g.isAnActivePlayer()){
        return STKing::CHECKMATE;
    }

    Piece* King=j_g.getPiece(j_g.id_king);

    STKing state=STKing::FREE;
    std::vector<Jugador*> j_rivales;

    for(size_t k=0;k<jugadores.size();k++){
        if(j_g.getPlayerColor()==jugadores[k].getPlayerColor()){continue;}
        if(!jugadores[k].isAnActivePlayer()){continue;}
        j_rivales.push_back(&jugadores[k]);
    }
    std::vector<Piece*> p_rivales_q_hacen_jaque;
    p_rivales_q_hacen_jaque=quienControLaCasilla(King->pos,j_rivales);


    if(p_rivales_q_hacen_jaque.size()>0){//determina si es mate
        state=STKing::CHECK;

        MovimientosDeReyValidos(King,j_g.valid_movements,j_rivales);

        //si no hay movimiento de rey, se calcula
        //movimientos que interrumpan el jaque
        if(j_g.valid_movements.size()==0){
            if(p_rivales_q_hacen_jaque.size()>1){
                state=STKing::CHECKMATE;
            }else{
                std::vector<Corxy> _vP;
                _vP=PuntosEntreDosPuntos(
                            King->pos,
                            p_rivales_q_hacen_jaque[0]->pos,
                            p_rivales_q_hacen_jaque[0]->getPieceType()
                        );
                state=STKing::CHECKMATE;//temporal
                for(const Corxy& _punto:_vP){//obstaculizacion jaque
                    for(const size_t& id_piece: j_g.id_piezas){
                        Piece* _piece = j_g.getPiece(id_piece);
                        if(_piece->type==PieceType::KING){continue;}
                        if(IsValidMove(_piece->pos,_punto,false)){
                            state=STKing::CHECK;
                            break;
                        }
                    }
                    if(state==STKing::CHECK){break;}
                }
            }
        }

    }else{
        Jugador& j_g_t=jugadores[playerTurn.getTurno()];

        if(&j_g_t==&j_g && j_g_t.id_piezas.size()==1){//rey ahogado

            MovimientosDeReyValidos(King,j_g.valid_movements,j_rivales);
            if(j_g.valid_movements.size()==0){
                state=STKing::CHECKMATE;
            }
        }
    }
    return state;
}
//no se incluye "a", posicion del rey
//de a -> b
std::vector<Corxy> ChessBoard::PuntosEntreDosPuntos(Corxy a, Corxy b, PieceType p_t){
    std::vector<Corxy> puntos;
    if(p_t==PieceType::KNIGHT){
        puntos.push_back(b);
    }else{
        int inc_x=-(a.x-b.x);
        int inc_y=-(a.y-b.y);
        if(inc_x!=0){inc_x=inc_x/std::abs(a.x-b.x);}
        if(inc_y!=0){inc_y=inc_y/std::abs(a.y-b.y);}

        int _x=a.x,_y=a.y;
        bool salir_bucle=false;
        while(true){
            _x=_x+inc_x;
            _y=_y+inc_y;

            if(std::abs(_x-b.x)==0 && inc_x != 0){
                if(std::abs(_y-b.y)!=0){
                    wxLogDebug(_("ChessBoard::PuntosEntreDosPuntos error, discrepancia en _x y _y"));
                    puntos.clear();
                    return puntos;
                }

                salir_bucle=true;
            }
            if(std::abs(_y-b.y)==0 && inc_y != 0){
                if(std::abs(_x-b.x)!=0){
                    wxLogDebug(_("ChessBoard::PuntosEntreDosPuntos error, discrepancia en _x y _y"));
                    puntos.clear();
                    return puntos;
                }

                salir_bucle=true;
            }

            if(_x<0 || _y<0 || _x>13 || _y>13){//error
                wxLogDebug(_("ChessBoard::PuntosEntreDosPuntos error, parametros fuera de rango."));
                puntos.clear();
                return puntos;
            }

            puntos.push_back(Corxy(_x,_y));
            if(salir_bucle){break;}
        }
    }

    return puntos;
}
//rota vector
void ChessBoard::RotarV(auto &xx, auto &yy, const int angulo){//para captura al paso
    auto _x=xx;
    auto _y=yy;

    switch (angulo) {
        case 90:
            xx=-_y;
            yy=_x;
            break;
        case 180:
            xx=-_x;
            yy=-_y;
            break;
        case 270:
            xx=_y;
            yy=-_x;
            break;
        case 360:
        case 0:
            xx=_x;
            yy=_y;
            break;
        default:
            wxLogError(_("Error en ChessBoard::VRotar, angulo no definido"));
    }
}

void ChessBoard::Rotar(Corxy& vect, size_t degrees, float origen_x, float origen_y){
    float y=13.0-(float)vect.x;//de filas,.. a cordenadas cart..
    float x=(float)vect.y;

    x=x-origen_x+1;//base 1..14
    y=y-origen_y+1;

    RotarV(x,y,degrees);

    vect.y=(int)(x-1+origen_x);//retornando a 0..13 y pasando a filas,col
    vect.x=13-(int)(y-1+origen_y);

}
void ChessBoard::savePage(int ic, int ec){
    libro.push_back(
        std::make_shared<Pagina>(ic,ec,playerTurn)
    );

    Pagina& pg = *libro.back();

    pg.setJugadores(jugadores);//set - save
    pg.setBoard(board);
    pg.setHuerfanas(p_huerfanas);
    pg.setvJugadaActual(jugada_actual);
    pg.setIndiceDeJugada(indice_de_jugada);
}
//argumentos: cordenadas de movimiento y sus cordenadas dentro de la notacion
//se llama directamente desde ChessFrame
bool ChessBoard::saveBoard(int xi,int yi, int xe,int ye, int ic,int ec){

    if(libro.size() == 0){
        indice_de_jugada = 0;

        savePage(0,0);
    }

    bool control = MovePiece(xi, yi, xe, ye);
    if( control && ic!=-1){
        indice_de_jugada++;

        savePage(ic,ec);
    }

    return control;
}
size_t ChessBoard::getIndiceDeJugada(){
    return indice_de_jugada;
}
void ChessBoard::restoreBoard(std::shared_ptr<Pagina>& pg){
    jugadores = pg->getJugadores();
    board = pg->getBoard();
    playerTurn = pg->getTurno();
    p_huerfanas = pg->getHuerfanas();
    jugada_actual = pg->getvJugadaActual();
    indice_de_jugada = pg->getIndiceDeJugada();
}
bool ChessBoard::restorePosition(long pos_click){
    bool control = false;
    for(std::shared_ptr<Pagina>& pg : libro){
        std::tuple limites=pg->getCorJugadaNotacion();
        if(std::get<0>(limites)<=pos_click && std::get<1>(limites)>=pos_click){

            restoreBoard(pg);

            control = true;

            Refresh();
            _Status(_("Posicion cargada."));

            break;
        }
    }


    return control;
}

bool ChessBoard::restoreDesdePagina(size_t pg){

    if(pg >= libro.size()){
        return false;
    }

    restoreBoard(libro[pg]);

    return true;
}

Piece* ChessBoard::getPiece(size_t id){
    if(id>100){
        return &p_empty;
    }
    size_t n = id/24;
    Piece* ptr_p = jugadores[n].getPiece(id);
    return ptr_p;
}

bool ChessBoard::MovePiece(int xi,int yi, int xe,int ye){
    Jugador& j_turno=jugadores[playerTurn.getTurno()];

    bool was_the_piece_moved=false;//control para el retorno de la funcion
    bool is_capture=false;
    bool es_zombi_pieza_en_destino=false;

    size_t id_pieza_a_remover=3344;

    Piece* king_turno=getPiece(j_turno.id_king);

    if(xi==200){//se identifica enrroque desde notacion, corto
        Corxy e_corto(4,13);//cordenada de ajedrez, origen ezquina superior
                            //filas, col
        size_t angulo=j_turno.getRotacion();
        if(angulo!=0){
            Rotar(e_corto,angulo);
        }
        xi=king_turno->pos.x;
        yi=king_turno->pos.y;

        xe=e_corto.x;
        ye=e_corto.y;
    }
    if(xi==300){//largo
        Corxy e_largo(8,13);
        size_t angulo=j_turno.getRotacion();
        if(angulo!=0){
            Rotar(e_largo,angulo);
        }
        xi=king_turno->pos.x;
        yi=king_turno->pos.y;

        xe=e_largo.x;
        ye=e_largo.y;
    }

    if(yi>=500 && yi<514){//rendicion

        yi=yi-500;

        j_turno.Surrender(p_huerfanas);
        _Status(_("Jugador se rinde"));
    }

    if(yi==3000){//rendicion sin movimiento
                 //(rendicion conduce al mate al inutizar las piezas)

        j_turno.Surrender(p_huerfanas);
        _Status(_("Jugador se rinde"));
    }

    Piece* p_a_remover = nullptr;//captura

    bool control_while=true;
    while(control_while){

    if(xi==3000){
        was_the_piece_moved=true;
        break;
    }

    Piece* p_movida = getPiece(board[xi][yi]);
    Piece* p_destino = getPiece(board[xe][ye]);

    if(p_destino->getPieceType()!=PieceType::NONE){//captura de pieza
        is_capture=true;
        id_pieza_a_remover=board[xe][ye];
        p_a_remover = getPiece(id_pieza_a_remover);

        if(p_a_remover->getPlayerColor()==PlayerColor::ZOMBI){
            es_zombi_pieza_en_destino=true;
        }
    }

    std::tuple<bool,int,int,int,int> castling=IsCastling(
                Corxy(xi,yi),
                Corxy(xe,ye)
            );

    if(std::get<0>(castling)){//Enrroque

        int& pos_i_torre_x=std::get<1>(castling);
        int& pos_i_torre_y=std::get<2>(castling);
        int& pos_e_torre_x=std::get<3>(castling);
        int& pos_e_torre_y=std::get<4>(castling);

        //king,rook
        p_movida->initial_position=false;
        getPiece(board[pos_i_torre_x][pos_i_torre_y])->initial_position=false;

        //king,rook
        board[xe][ye] = board[xi][yi];
        p_movida->setPosition(xe,ye);
        board[xi][yi] = NOPIEZA;
        board[pos_e_torre_x][pos_e_torre_y] = board[pos_i_torre_x][pos_i_torre_y];
        Piece* torre_enroque=getPiece(board[pos_e_torre_x][pos_e_torre_y]);
        torre_enroque->setPosition(pos_e_torre_x,pos_e_torre_y);
        board[pos_i_torre_x][pos_i_torre_y] = NOPIEZA;

        pieceSelected = false;
        selectedCell = wxPoint(-1, -1);

        was_the_piece_moved=true;
    }

    if(
       !std::get<0>(castling) &&
       IsValidMove(Corxy(xi,yi),Corxy(xe,ye))
    ){

        if(p_movida->isInitialPosition()){
            p_movida->initial_position=false;
        }
        if(p_movida->getPieceType()!= PieceType::PAWN){
            board[xe][ye] = board[xi][yi];
            p_movida->setPosition(xe,ye);
            was_the_piece_moved=true;
        }else{
            //peon
            bool mark_switch=false;
            switch(p_movida->getPlayerColor()){//promotion
                case PlayerColor::BLACK:
                    if(xe==7){
                        board[xe][ye]=jugadores[
                            playerTurn.getTurno()
                            ].PromotionToQueen(board[xi][yi],xe,ye);
                        mark_switch=true;
                        was_the_piece_moved=true;
                    }
                break;
                case PlayerColor::RED:
                    if(ye==7){
                        board[xe][ye]=jugadores[
                            playerTurn.getTurno()
                            ].PromotionToQueen(board[xi][yi],xe,ye);
                        mark_switch=true;
                        was_the_piece_moved=true;
                    }
                break;
                case PlayerColor::WHITE:
                    if(xe==6){
                        board[xe][ye]=jugadores[
                            playerTurn.getTurno()
                            ].PromotionToQueen(board[xi][yi],xe,ye);
                        mark_switch=true;
                        was_the_piece_moved=true;
                    }

                break;
                case PlayerColor::YELLOW:
                    if(ye==6){
                        board[xe][ye]=jugadores[
                            playerTurn.getTurno()
                            ].PromotionToQueen(board[xi][yi],xe,ye);
                        mark_switch=true;
                        was_the_piece_moved=true;
                    }
                break;
                default:
                break;
            }
            if(!mark_switch){
                board[xe][ye] = board[xi][yi];
                p_movida->setPosition(xe,ye);
                was_the_piece_moved=true;
                if( std::abs(xe-xi) == 2 || std::abs(ye-yi) == 2 ){
                    p_movida->se_movio_dos_posiciones=true;
                }
            }
        }
        board[xi][yi] = NOPIEZA;
        pieceSelected = false;
        selectedCell = wxPoint(-1, -1);

    }

    //captura al paso
    if(
        p_movida->getPieceType()==PieceType::PAWN &&
        !was_the_piece_moved &&
        xi!=xe && yi!=ye
       ){

        Corxy quien_bloquea=j_turno.getDireccionDeCoronacion();

        int tem_xi=quien_bloquea.x+xi;
        int tem_yi=quien_bloquea.y+yi;

        Piece* p_q_bloquea = getPiece(board[tem_xi][tem_yi]);

        if(
           p_q_bloquea->getPieceType()==PieceType::PAWN&&
           p_q_bloquea->getPlayerColor()!=p_movida->getPlayerColor()
          ){//pieza que bloque es peon de otro bando no Zombi
            Jugador* j_rival=getJugador(p_q_bloquea->color);

            if(
               ProductoPunto(j_turno.getDireccionDeCoronacion(),
                             j_rival->getDireccionDeCoronacion()
               )==0 &&
               j_rival->isAnActivePlayer()
              ){//direccion de movimiento perpendicular
                //jugador activo

                if(
                    p_q_bloquea->se_movio_dos_posiciones  &&
                    j_rival->id_pieza_movida==board[tem_xi][tem_yi] &&
                    !p_q_bloquea->is_an_orphan_piece
                   ){
                     //si pieza que bloquea no es huerfana y
                     //se movio dos pasos hacia delante
                     //y es la píeza jugada en el turno respectivo

                    Corxy cor_peon(xi,yi);

                    Rotar(cor_peon, 360-j_turno.getRotacion());
                    bool es_fila_correcta=false;

                    Corxy v_movimiento(xe-xi,ye-yi);
                    RotarV(v_movimiento.x,v_movimiento.y,360-j_turno.getRotacion());

                    //se calcula movimiento de pieza y se compara con los esperados
                    if(cor_peon.x == 3 && v_movimiento.x == -1 && v_movimiento.y == -1){
                        es_fila_correcta=true;
                    }
                    if(cor_peon.x == 10 && v_movimiento.x == 1 && v_movimiento.y == -1){
                        es_fila_correcta=true;
                    }

                    if( es_fila_correcta ){
                        //peon bloqueado debe corresponder a las dos
                        //columnas factibles de ser bloqueadas, 3 y 10
                        //y al movimiento diagonal

                        p_movida->setPosition(xe,ye);
                        board[xe][ye] = board[xi][yi];
                        board[xi][yi] = NOPIEZA;

                        was_the_piece_moved=true;
                        is_capture=true;

                        id_pieza_a_remover = board[tem_xi][tem_yi];
                        p_a_remover = getPiece(board[tem_xi][tem_yi]);
                        board[tem_xi][tem_yi] = NOPIEZA;
                    }

                }
            }
        }
    }

    control_while=false;
    }//////fin while

    if(was_the_piece_moved){

        if(is_capture){
            if(!es_zombi_pieza_en_destino){
                jugadores[
                      playerTurn.getTurno(p_a_remover->getPlayerColor())
                     ].RemovePiece(id_pieza_a_remover);
            }else{
                p_huerfanas.remove(id_pieza_a_remover);
            }

        }

        if(xi>=0 && xi<14){//para no tomar indicador 3000
            jugada_actual={xi,yi,xe,ye};
            j_turno.id_pieza_movida = board[xe][ye];
        }

        playerTurn.Next();
        _Status(_("Pieza movida"));

        for(Jugador& jg: jugadores){//paralelizar
            //if(jg.getPlayerColor()==j_turno.getPlayerColor()){continue;} borrar?
            if(!jg.isAnActivePlayer()){continue;}

            STKing st=StateOfTheKing(jg);

            if(st==STKing::CHECKMATE){
                _Status(_("Jaque Mate"));
                jg.JaqueMate();
                playerTurn.JugadorEnMate(jg.getPlayerColor());

                if(&jugadores[playerTurn.getTurno()]==&jg){
                    playerTurn.Next();
                }
            }
            if(st==STKing::CHECK){
                _Status(_("Jaque"));
            }
        }

    }

    if(se_permite_dibujar){

        Refresh();//separar para eliminar: se_permite_dibujar
    }

    return was_the_piece_moved;
}

void ChessBoard::anularDibujado(bool dbj){//para anular dibujo en carga
    se_permite_dibujar = !dbj;
}

int ChessBoard::ProductoPunto(const Corxy& a,const Corxy& b) const {
    return a.x*b.x+b.y*a.y;
}

Jugador* ChessBoard::getJugador(PlayerColor pl_c){
    Jugador* r_j=nullptr;

    for(Jugador& jd:jugadores){
        if(jd.getPlayerColor()==pl_c){
            r_j=&jd;
            break;
        }
    }

    return r_j;
}

void ChessBoard::onMouseClick(wxMouseEvent& event) {
    wxPoint clickPos = event.GetPosition();
    wxPoint cell = GetCellFromPosition(clickPos);

    if (cell.x < 0 || cell.y < 0 || !IsPlayableCell(cell.x, cell.y)) {
        return;
    }

    Piece* p_select = getPiece(board[cell.x][cell.y]);
    if (!pieceSelected) {
        // Seleccionar una pieza
        if (!p_select->isEmpty()) {
            selectedCell = cell;
            pieceSelected = true;
            Refresh();
        }
    } else {
        bool mark_if=true;
        //selecion un toque
        if(p_select->getPlayerColor()==playerTurn.getCurrent()){
            mark_if=false;
            selectedCell = cell;
            Refresh();
        }
        // Mover la pieza
        if (cell == selectedCell && mark_if) {
            // Deseleccionar si se hace clic en la misma celda
            pieceSelected = false;
            selectedCell = wxPoint(-1, -1);

        } else {
            MovePiece(selectedCell.x,selectedCell.y,cell.x,cell.y);
        }
    }
}
void ChessBoard::setChessParameters(ChessParameters* ptr){
    c_parameters = ptr;
}
void ChessBoard::setFunctionStatus(std::function<void(const wxString&)> f){
    _Status=f;
}
