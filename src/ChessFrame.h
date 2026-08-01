#ifndef CHESSFRAME_H_INCLUDED
#define CHESSFRAME_H_INCLUDED

#include <wx/timer.h>

#include "ChessBoard.h"
#include "ChessNotation.h"
#include "ChessFiles.h"
#include "ChessParameters.h"
#include "ChessStyleBoard.h"

#define TIME_CLEAN_STATUS 2000


class ChessFrame : public wxFrame {
public:
    ChessFrame();
    void loadGameOnCB();
    void Reset();

private:

    wxTimer temporizador;
    wxTimer timer_clean_status;

    wxBoxSizer* sizerDespliege;
    wxButton* btnPGN;
    wxButton* btnPartidas;

    ChessBoard *chessBoard;
    ChessNotation *chessNotation;
    ChessFiles *chessFiles;
    ChessParameters chessParameters;

    ParametrosDeEstilo P_D_E;
    wxString Path;//juego actual
    std::vector<wxString> *games;//Paths

    void setEventos();

    void onAutoPlay(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnLoadFile(wxCommandEvent& event);
    void onKeyDown(wxKeyEvent& event);
    void onTimer(wxTimerEvent& event);
    void onOpenOptionStyleBoardDialog(wxCommandEvent& event);
    void onGoToInicioDePartida(wxCommandEvent& event);
    void onPreviousJugada(wxCommandEvent& event);
    void onNextJugada(wxCommandEvent& event);
    void onGoToFinDePartida(wxCommandEvent& event);
    void onCleanStatus(wxTimerEvent& event);

    void runTimer();
    bool nextMovement();
    void jumpToPosition(long pos_in);
    void OnClickBtnPartidasPGN(wxCommandEvent& event);
    void SavePath(wxString path);
    void setInteroperatividadEntreClases();
    void _Status(wxString msg);

};


#endif // CHESSFRAME_H_INCLUDED
