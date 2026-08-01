
#include "ChessFrame.h"

class ChessApp : public wxApp {
public:
    virtual bool OnInit();
};

bool ChessApp::OnInit() {
    ChessFrame* frame = new ChessFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(ChessApp);

