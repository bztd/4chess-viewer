#ifndef CHESSNOTATION_H_INCLUDED
#define CHESSNOTATION_H_INCLUDED

#include <iostream>
#include <vector>

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/textfile.h>

#include "Common.h"
#include "ChessParameters.h"

enum class Type{//ElementType
    MOVEMENT,EMPTY,NO_PARSEBLE
};

struct Element{
    int line = -1;
    int turno = -1;//no representa el turno, es el indice del elemento en cada linea
    int init_m=-1;//cordenada en archivo
    int end_m=-1;
    Type type;
    Corxy corI;
    Corxy corE;

    std::vector<int> g_move={-1,-1,-1,-1};
    wxColor color;

    Element(){type=Type::EMPTY;}
    bool isMovementComplete(){
        return (corE.x != -1) && (corE.y != -1) &&
               (corI.x != -1) && (corI.y != -1);
    }
    void setMovementColor(){
        color.Set("#56f432");
    }
    void setRondaColor(){//1.  2.
        color.Set("#e6f4a2");
    }
    void restart(){
        line=-1;
        init_m=-1;//cordenada en archivo
        end_m=-1;
        type=Type::EMPTY;
        corI=Corxy();
        corE=Corxy();
    }
};

struct Numero{
    int numero=0;
    int digitos=0;

    void addDigito(int d){
        if(digitos>=3){wxLogError(_("Conversion de PGN erronea: %d"), 380311);return;}
        if(digitos==1){
            numero=10+d;
        }else{
            numero=d;
        }

        digitos++;
    }
    int getNumero(){return numero;}
    void restart(){
        numero=0;
        digitos=0;
    }
};

class ChessNotation : public wxRichTextCtrl{
public:
    ChessNotation(wxWindow* parent,
                   wxWindowID id = wxID_ANY,
                   const wxString& value = wxEmptyString,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxRE_MULTILINE);

    virtual ~ChessNotation();

    std::vector<int> getNextMovement();

    void Reset();
    void setPosInicial();

    void setFCBrestorePosition(std::function<void(long)> f);
    void setFCFloadGameCB(std::function<void()> f);
    void InsertTextoFormateado(const wxString& texto);
    void LoadPGN(wxString path);
    bool InterpretPgn(wxString path);
    void AplicarEstiloTitulo();
    wxPoint ConvertPosition(wxString pos);
    void setStyleParameter(ParametrosDeEstilo *p);
    long goToNextMove(bool show_mark=true);
    void goToLastMovimiento();//ChessFrame

    bool setJugada(size_t n);//para ChessFrame

private:
    size_t linea = 0;
    size_t turno = 0;

    Element* E_actual=nullptr;
    Element* E_anterior=nullptr;
    ParametrosDeEstilo *P=nullptr;

    std::vector<std::vector<Element>> plays;
    std::function<void(long)> CBrestorePosition;
    std::function<void()> CFloadGameCB;

    wxRichTextAttr estilo_global;
    wxRichTextAttr estilo_jugadas;
    wxRichTextAttr estilo_jugada_actual;

    void DrawMark();//resaltado de jugada actual en pgn
    void setMovementStyle();
    void OnKeyDown(wxKeyEvent& event);
    void OnTextChanged(wxCommandEvent& event);//borrar
    void OnPGNClick(wxMouseEvent& event);
    void goTofirstMovimiento();

    void setStyle();
    void RevertDrawMark();
    void setValuesForPosition(const long& pos);

};

#endif
