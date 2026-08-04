#include "ChessFiles.h"

ChessFiles::ChessFiles(
                   wxWindow* parent,
                   wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& _size
                  )
                  :wxDataViewListCtrl(parent,id,pos,_size,wxDV_ROW_LINES | wxDV_VERT_RULES){
    wxDataViewColumn *p=AppendTextColumn(_("Archivo"), wxDATAVIEW_CELL_INERT, 150);
    //AppendTextColumn(_("Ruta"), wxDATAVIEW_CELL_INERT, 150);

    p->SetMinWidth(90);
    //wxVector<wxVariant> data;

    wxString dirConfig = wxStandardPaths::Get().GetUserLocalDataDir();
    wxString nombreArchivo = "data";
    wxFileName fn(dirConfig, nombreArchivo);
    path_data_file = fn.GetFullPath();
    LoadSavedRoutes();

    Bind(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, &ChessFiles::MenuDesplegable, this);
    Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &ChessFiles::CargarItemEnBoardYPGN, this);
    //data={wxVariant("1eefefefef"),wxVariant("1eefefefef"),wxVariant("1eefefefef")};

    //SetTextValue("Nue", 1, 0);
    //SetTextValue("Jua", 1, 1);
    //Update();
}
void ChessFiles::CargarItemEnBoardYPGN(wxDataViewEvent& event){//doble click
    wxDataViewItem item = event.GetItem();

    if (!item.IsOk()) return;

    size_t fila = ItemToRow(item);

    if(paths.size()<=fila){
        _Status(_("Error, fila seleccionada por fuera del limite del arreglo"));
        return;
    }
    _Status(_("Cargando partida..."));

    GeneralReset();//board,notation
    LoadPGN(paths[fila]);

    event.Skip();
}

void ChessFiles::MenuDesplegable(wxDataViewEvent& event){
    wxDataViewItem item = event.GetItem();

    if (!item.IsOk()) return;

    size_t fila = ItemToRow(item);

    if(paths.size()<=fila){
        _Status(_("Error, fila seleccionada por fuera del limite del arreglo"));
        return;
    }

    wxMenu menu;
    wxMenuItem* eliminarItem=menu.Append(wxID_ANY, _("Eliminar elemento"));
    //menu.AppendSeparator();

    menu.Bind(wxEVT_MENU, [this, fila](wxCommandEvent&) {
        this->DeleteItem(fila);
        this->_Status("Borrando: "+this->paths[fila]);
        this->paths.erase(this->paths.begin()+fila);
        this->OrdenarPosiciones();

    }, eliminarItem->GetId());

    PopupMenu(&menu);
}
void ChessFiles::PrintPath(wxString path){
    wxVector<wxVariant> data;
    data.push_back(wxVariant(path));
    AppendItem(data);
}
std::vector<wxString>* ChessFiles::getGames(){
    return &paths;
}
void ChessFiles::SavePath(wxString path){
    wxFileConfig *data_file = new wxFileConfig("", "", path_data_file);
    data_file->SetPath("/Path/Files");
    wxString clave = wxString::Format("Path_%zu", path_counter++);
    data_file->Write(clave, path);
    data_file->Flush();
    delete data_file;
}
void ChessFiles::LoadPath(wxString path){
    PrintPath(path);
    SavePath(path);

}

void ChessFiles::DeleteEntriesFileConfig(){
    wxFileConfig *data_file = new wxFileConfig("", "", path_data_file);
    data_file->SetPath("/Path");
    if(data_file->HasGroup("Files")){
        data_file->DeleteGroup("Files");
    }
    data_file->Flush();
    delete data_file;
}
void ChessFiles::OrdenarPosiciones(){
    //_Status(_("Ordenando lista de archivos..."),0);
    DeleteEntriesFileConfig();

    size_t cc=0;
    wxFileConfig *_data_file = new wxFileConfig("", "", path_data_file);
    _data_file->SetPath("/Path/Files");
    wxString _clave;
    for(const wxString& s:paths){
        _clave = wxString::Format("Path_%zu", cc++);
        _data_file->Write(_clave, s);
    }
    _data_file->Flush();
    delete _data_file;

    path_counter=cc;
}
void ChessFiles::LoadSavedRoutes(){
    long index;
    wxFileConfig *data_file = new wxFileConfig("", "", path_data_file);
    data_file->SetPath("/Path/Files");

    wxString clave;//wxString::Format("Path_%zu", path_counter++);
    bool tieneDatos = data_file->GetFirstEntry(clave, index);

    bool duplicados_rotos=false;
    while (tieneDatos) {
        wxString valor = data_file->Read(clave, "");//archivo de configuracion
        tieneDatos = data_file->GetNextEntry(clave, index);

        bool is_item_valid=true;
        if(wxFileName::FileExists(valor)){
            for(size_t w=0; w<paths.size(); w++){
                if(paths[w]==valor){
                    duplicados_rotos=true;
                    is_item_valid=false;
                }
            }
        }else{
            duplicados_rotos=true;
            is_item_valid=false;
        }

        if(is_item_valid){
            paths.push_back(valor);
            PrintPath(valor);
            path_counter++;
        }
    }
    delete data_file;

    if(duplicados_rotos){
        OrdenarPosiciones();
    }

}//void SetStatusText(const wxString &text, int number = 0);
void ChessFiles::setFunctionStatus(std::function<void(const wxString&)> f){
    _Status=f;
}
void ChessFiles::setFunctionLoadPGN(std::function<void(const wxString&)> f){
    LoadPGN=f;
}
void ChessFiles::setFunctionGeneralReset(std::function<void()> f){
    GeneralReset=f;
}
