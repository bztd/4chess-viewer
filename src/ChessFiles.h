#pragma once

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>

class ChessFiles : public wxDataViewListCtrl{//ocupe resposivamente todo el espacio
public:
    ChessFiles(
                   wxWindow* parent,
                   wxWindowID id = wxID_ANY,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& _size = wxDefaultSize
                  );
    void LoadPath(wxString path);
    void setFunctionStatus(std::function<void(const wxString&)> f);
    void setFunctionLoadPGN(std::function<void(const wxString&)> f);
    void setFunctionGeneralReset(std::function<void()> f);
    std::vector<wxString>* getGames();

private:
    wxString path_data_file;
    size_t path_counter=0;
    std::vector<wxString> paths;
    std::function<void(const wxString&)> _Status=[](const wxString& msg) {
        wxLogMessage(msg);
    };
    std::function<void(const wxString&)> LoadPGN;
    std::function<void()> GeneralReset;

    void PrintPath(wxString path);
    void LoadSavedRoutes();
    void MenuDesplegable(wxDataViewEvent& event);
    void DeleteEntriesFileConfig();
    void SavePath(wxString path);
    void OrdenarPosiciones();
    void CargarItemEnBoardYPGN(wxDataViewEvent& event);
};

//sincronizar con Path y con el archivo, reorganizar al eliminar, verificar que los elementos existen

