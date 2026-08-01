
#include "ChessFrame.h"


ChessFrame::ChessFrame()
    : wxFrame(nullptr, wxID_ANY, PROGRAM_NAME,
              wxDefaultPosition, wxSize(700, 750)),
              temporizador(this),
              timer_clean_status(this){

    Maximize(true);
    SetMinSize(wxSize(400, 500));

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Load_File, "&Abrir Archivo\tCtrl-A", "Cargar Archivo");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, "&Salir\tCtrl-Q", "Salir del programa");

    wxMenu* menuGame = new wxMenu;
    menuGame->Append(ID_Auto_Play, _("Reproducir\tP"));
    menuGame->Append(ID_MenuGame_Next_Jugada, _("Siguiente jugada\tSpace"));
    menuGame->Append(ID_MenuGame_Anterior_Jugada, _("Anterior jugada\tB"));
    menuGame->Append(ID_MenuGame_Inicio_Game, _("Ir a inicio de partida\tI"));
    menuGame->Append(ID_MenuGame_Last_Jugada, _("Ir a final de partida\tE"));

    wxMenu* menuOpciones = new wxMenu;
    menuOpciones->Append(ID_Open_Option_Style_Board_Dialog, _("Estilo del Tablero"));

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, ("Acerca de..\tF1"), _("Acerca de " PROGRAM_NAME));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _("Archivo"));
    menuBar->Append(menuGame, _("Juego"));
    menuBar->Append(menuOpciones, _("Opciones"));
    menuBar->Append(menuHelp, _("Ayuda"));

    SetMenuBar(menuBar);

    CreateStatusBar();

    chessBoard = new ChessBoard(this);

    chessNotation = new ChessNotation(
                    this,
                    wxID_ANY,
                    wxEmptyString,
                    wxDefaultPosition,
                    wxSize(50, 10),
                    wxRE_MULTILINE | wxRE_READONLY
                    );

    btnPartidas = new wxButton(this, ID_Btn_Partidas, "Partidas");
    btnPGN = new wxButton(this, ID_Btn_PGN, "PGN");
    btnPGN->SetFont(btnPGN->GetFont().Bold());
    btnPGN->SetForegroundColour(*wxRED);
    btnPGN->SetBackgroundColour(wxColour(240, 240, 240));
    btnPartidas->SetForegroundColour(*wxLIGHT_GREY);
    btnPartidas->SetBackgroundColour(wxColour(240, 240, 240));

    wxBoxSizer* sizerPrimario = new wxBoxSizer(wxHORIZONTAL);
    sizerPrimario->Add(chessBoard, 1, wxEXPAND | wxALL, 5);//lo contiene todo

    wxBoxSizer* sizer_C_lateral = new wxBoxSizer(wxVERTICAL);
    sizerPrimario->Add(sizer_C_lateral, 1, wxEXPAND | wxALL, 5);

    wxBoxSizer* sizerEncabezado = new wxBoxSizer(wxHORIZONTAL);
    sizer_C_lateral->Add(sizerEncabezado, 0, wxEXPAND | wxALL, 0);

    sizerDespliege = new wxBoxSizer(wxVERTICAL);
    sizer_C_lateral->Add(sizerDespliege, 1, wxEXPAND | wxALL, 5);

    sizerEncabezado->Add(btnPGN, 0, wxALL, 0);
    sizerEncabezado->Add(btnPartidas, 0, wxALL, 0);

    sizerDespliege->Add(chessNotation, 1, wxEXPAND | wxALL, 5);

    chessFiles = new ChessFiles(this);

     //funciones
    setInteroperatividadEntreClases();

    games=chessFiles->getGames();
    chessFiles->Show(false);
    sizerDespliege->Add(chessFiles, 1, wxEXPAND | wxALL, 5);

    SetSizer(sizerPrimario);
    Centre();

    chessNotation->setStyleParameter(&P_D_E);

    setEventos();
}
void ChessFrame::runTimer(){
    if(temporizador.IsRunning()){
        temporizador.Stop();
    }else{
        temporizador.Start(chessParameters.getTimerTime());
    }
}
void ChessFrame::onAutoPlay(wxCommandEvent& event){
    wxUnusedVar(event);

    runTimer();
}
void ChessFrame::onTimer(wxTimerEvent& event) {
    if(!nextMovement()){
        temporizador.Stop();
    }

    event.Skip();
}
void ChessFrame::setEventos(){
    Bind(wxEVT_TIMER, &ChessFrame::onTimer, this,temporizador.GetId());
    Bind(wxEVT_TIMER, &ChessFrame::onCleanStatus, this, timer_clean_status.GetId());

    Bind(wxEVT_CHAR_HOOK, &ChessFrame::onKeyDown, this);

    Bind(wxEVT_MENU, &ChessFrame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &ChessFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &ChessFrame::OnLoadFile, this, ID_Load_File);
    Bind(wxEVT_MENU, &ChessFrame::onAutoPlay, this, ID_Auto_Play);
    Bind(wxEVT_MENU, &ChessFrame::onOpenOptionStyleBoardDialog, this, ID_Open_Option_Style_Board_Dialog);
    Bind(wxEVT_MENU, &ChessFrame::onGoToInicioDePartida, this, ID_MenuGame_Inicio_Game);
    Bind(wxEVT_MENU, &ChessFrame::onPreviousJugada, this, ID_MenuGame_Anterior_Jugada);
    Bind(wxEVT_MENU, &ChessFrame::onNextJugada, this, ID_MenuGame_Next_Jugada);
    Bind(wxEVT_MENU, &ChessFrame::onGoToFinDePartida, this, ID_MenuGame_Last_Jugada);

    Bind(wxEVT_BUTTON, &ChessFrame::OnClickBtnPartidasPGN, this, ID_Btn_Partidas);
    Bind(wxEVT_BUTTON, &ChessFrame::OnClickBtnPartidasPGN, this, ID_Btn_PGN);
}
void ChessFrame::loadGameOnCB(){//otro hilo?
    //se carga la partida
    bool control = false;
    _Status(_("Cargando partida."));
    chessBoard->anularDibujado(true);
    while(true){
        std::vector<int> m=chessNotation->getNextMovement();//x,y
        if(m[0]!=-1){
            if(chessBoard->saveBoard(m[1],m[0],m[3],m[2],m[4],m[5])){
                chessNotation->goToNextMove(false);
                control = true;
            }else{break;}
        }else{break;}
    }
    chessBoard->anularDibujado(false);
    if(control){
        chessBoard->restoreDesdePagina(0);
        chessNotation->setPosInicial();

        _Status(_("Carga de partida finalizada."));
    }

}
void ChessFrame::setInteroperatividadEntreClases(){
    chessFiles->setFunctionStatus([this](const wxString& msg) {
        this->_Status(msg);
    });
    chessFiles->setFunctionLoadPGN([this](const wxString& msg) {
        this->chessNotation->LoadPGN(msg);
    });
    chessFiles->setFunctionGeneralReset([this]() {
        this->Reset();
    });


    chessBoard->setFunctionStatus([this](const wxString& msg) {
        this->_Status(msg);
    });


    chessBoard->setChessParameters(&chessParameters);


    chessNotation->setFCFloadGameCB([this](){
        this->loadGameOnCB();
    });
    chessNotation->setFCBrestorePosition([this](long pos_in) {
        this->chessBoard->restorePosition(pos_in);
    });
}

void ChessFrame::_Status(wxString msg){
    SetStatusText(msg);
    timer_clean_status.StartOnce(TIME_CLEAN_STATUS);
}
void ChessFrame::onCleanStatus(wxTimerEvent& event){
    wxUnusedVar(event);

    SetStatusText("");
}
void ChessFrame::onGoToInicioDePartida(wxCommandEvent& event){
    wxUnusedVar(event);

    chessBoard->restoreDesdePagina(0);
    chessNotation->setJugada(0);

    Refresh();
}
void ChessFrame::onGoToFinDePartida(wxCommandEvent& event){
    wxUnusedVar(event);

    chessBoard->goToLastPage();
    chessNotation->goToLastMovimiento();

    Refresh();
}
void ChessFrame::onPreviousJugada(wxCommandEvent& event){
    wxUnusedVar(event);

    size_t indice = chessBoard->getIndiceDeJugada();

    if(indice !=0){

        indice--;

        chessBoard->restoreDesdePagina(indice);
        chessNotation->setJugada(indice);

        Refresh();

    }
}

void ChessFrame::onNextJugada(wxCommandEvent& event){
    wxUnusedVar(event);

    nextMovement();
}

void ChessFrame::OnClickBtnPartidasPGN(wxCommandEvent& event){

    if (event.GetId() == ID_Btn_Partidas) {
        btnPartidas->SetFont(btnPGN->GetFont().Bold());
        btnPartidas->SetForegroundColour(*wxRED);
        btnPGN->SetForegroundColour(*wxLIGHT_GREY);
        btnPGN->SetBackgroundColour(wxColour(240, 240, 240));

        chessNotation->Show(false);
        chessFiles->Show(true);
    }else{
        btnPGN->SetFont(btnPGN->GetFont().Bold());
        btnPGN->SetForegroundColour(*wxRED);
        btnPartidas->SetForegroundColour(*wxLIGHT_GREY);
        btnPartidas->SetBackgroundColour(wxColour(240, 240, 240));

        chessFiles->Show(false);
        chessNotation->Show(true);
    }

    sizerDespliege->Layout();
}
bool ChessFrame::nextMovement(){
    bool control = false;
    size_t indice = chessBoard->getIndiceDeJugada();

    indice++;

    control = chessBoard->restoreDesdePagina(indice);
    chessNotation->setJugada(indice);

    Refresh();

    return control;
    /*
    long resultado=0;
    std::vector<int> m=chessNotation->getNextMovement();//x,y
    if(m[0]!=-1){
        if(chessBoard->MovePiece(m[1],m[0],m[3],m[2])){
            resultado=chessNotation->goToNextMove();
        }
    }
    return resultado;
    */
}
void ChessFrame::onKeyDown(wxKeyEvent& event) {

/*
    int key_code = event.GetKeyCode();

    switch (key_code) {
        case WXK_SPACE:
            nextMovement();
            return;

        case WXK_LEFT:
            //deshacerJugada();
            return;

        case 'P':
            runTimer();
            return;
    }
*/
    event.Skip();

}

void ChessFrame::OnQuit(wxCommandEvent& event) {
    wxUnusedVar(event);

    Close(true);
}

void ChessFrame::OnLoadFile(wxCommandEvent& event) {
    wxUnusedVar(event);

    wxFileDialog openFileDialog(this, _("Abrir archivo de texto"), "", "",
                       "Archivos de Ajedrez (*.pgn4)|*.pgn4|Texto plano (*.txt)|*.txt|Todos los archivos (*.*)|*.*",
                       wxFD_OPEN | wxFD_FILE_MUST_EXIST);//wxFD_MULTIPLE

    if(openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    Reset();

    wxString path = openFileDialog.GetPath();
    SavePath(path);
    chessNotation->LoadPGN(path);
}

void ChessFrame::SavePath(wxString path){
    Path=path;
    bool is_repeat=false;
    for(size_t n=0;n<games->size();n++){
        if(path==(*games)[n]){
            is_repeat=true;
            break;
        }
    }
    if(!is_repeat){games->push_back(path);chessFiles->LoadPath(path);}
}

void ChessFrame::OnAbout(wxCommandEvent& event) {
    wxUnusedVar(event);

    wxMessageBox("This program is licensed under the terms of the "
                 "GNU General Public License version 3.\n"
                 "Available online under:\n"
                 "http://www.gnu.org/licenses/gpl-3.0.html)",
                 PROGRAM_NAME,
                 wxOK | wxICON_INFORMATION);
}
void ChessFrame::Reset(){

    chessNotation->Reset();
    chessBoard->Reset();
}
void ChessFrame::onOpenOptionStyleBoardDialog(wxCommandEvent& event) {
    wxUnusedVar(event);

    StyleBoard  dlg(this, chessParameters);
    dlg.ShowModal();

    Refresh();

}
