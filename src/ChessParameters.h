#pragma once

#include <wx/stdpaths.h>
#include <wx/fileconf.h>

#define C_CASILLA_BLACK "c-c-black"
#define C_CASILLA_WHITE "c-c-white"

#define C_PIECE_WHITE "c-p-white"
#define C_PIECE_BLACK "c-p-black"
#define C_PIECE_RED "c-p-red"
#define C_PIECE_YELLOW "c-p-yellow"

struct ParametrosDeEstilo{
    wxColor movement_mark;
    ParametrosDeEstilo(){
        movement_mark.Set("#fd1f6e");
    }
};

class ChessParameters{
public:
    ChessParameters(){

        wxString data_config = wxStandardPaths::Get().GetUserLocalDataDir();
        wxFileName fn(data_config, nombre_archivo);

        path_data_config = fn.GetFullPath();

        loadConfiguracion();
    }

    void saveConfiguracion(){

        wxFileConfig *data_config = new wxFileConfig("", "", path_data_config);
        data_config->SetPath(config_tag);

        data_config->Write(C_CASILLA_BLACK, c_casilla_black);
        data_config->Write(C_CASILLA_WHITE, c_casilla_white);

        data_config->Write(C_PIECE_WHITE, c_piece_white);
        data_config->Write(C_PIECE_BLACK, c_piece_black);
        data_config->Write(C_PIECE_RED, c_piece_red);
        data_config->Write(C_PIECE_YELLOW, c_piece_yellow);

        data_config->Flush();

        delete data_config;
    }

    int getTimerTime(){
        return timer_time;
    }
    wxString getColorCasillaWhite(){
        return c_casilla_white;
    }
    wxString getColorCasillaBlack(){
        return c_casilla_black;
    }

    void setColorCasillaWhite(wxString c){
        c_casilla_white = c;
    }
    void setColorCasillaBlack(wxString c){
        c_casilla_black = c;
    }

    wxString getColorPieceBlack(){
        return c_piece_black;
    }
    wxString getColorPieceWhite(){
        return c_piece_white;
    }
    wxString getColorPieceRed(){
        return c_piece_red;
    }
    wxString getColorPieceYellow(){
        return c_piece_yellow;
    }

    void setColorPieceBlack(wxString c){
        c_piece_black = c;
    }
    void setColorPieceWhite(wxString c){
        c_piece_white = c;
    }
    void setColorPieceRed(wxString c){
        c_piece_red = c;
    }
    void setColorPieceYellow(wxString c){
        c_piece_yellow = c;
    }

    void Reset(){

        c_casilla_black = "#000000";
        c_casilla_white = "#FFFFFF";
    }
private:

    const wxString nombre_archivo = "4chess_config";
    const wxString config_tag = "/config/";

    wxString path_data_config = "";

    int timer_time = 1500;

    wxString c_casilla_black = "#FFFFFF";
    wxString c_casilla_white = "#C0BFBC";

    wxString c_piece_black = "#F5C211";
    wxString c_piece_white = "#FF0303";
    wxString c_piece_red = "#1A5FB4";
    wxString c_piece_yellow = "#33D17A";


    void loadConfiguracion(){

        long index;
        wxFileConfig *config_file = new wxFileConfig("", "", path_data_config);
        config_file->SetPath(config_tag);

        wxString clave;
        bool tiene_datos = config_file->GetFirstEntry(clave, index);

        if(tiene_datos){
            while (tiene_datos){
                wxString valor = config_file->Read(clave, "");

                if (clave == C_CASILLA_WHITE){
                    c_casilla_white = valor;
                } else if(clave == C_CASILLA_BLACK){
                    c_casilla_black = valor;
                } else if(clave == C_PIECE_BLACK){
                    c_piece_black = valor;
                }else if(clave == C_PIECE_WHITE){
                    c_piece_white = valor;
                }else if(clave == C_PIECE_RED){
                    c_piece_red = valor;
                }else if(clave == C_PIECE_YELLOW){
                    c_piece_yellow = valor;
                }

                tiene_datos = config_file->GetNextEntry(clave, index);
            }
        }

        delete config_file;

    }
};

