#ifndef CHESSSTYLEBOARD_H_INCLUDED
#define CHESSSTYLEBOARD_H_INCLUDED

#include <wx/clrpicker.h>

#include "ChessParameters.h"
#include "Common.h"

#define C_TAM wxSize(40, 40)



class StyleBoard : public wxDialog {
public:
    StyleBoard (wxWindow* parent, ChessParameters& c_p)
        : wxDialog(parent, wxID_ANY, _("Estilo Tablero"), wxDefaultPosition, wxSize(320, 200)){

        ptr_c_p = &c_p;

        wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);


        wxStaticText* title_color_casillas = new wxStaticText(this, wxID_ANY, _("Color de las casillas"));
        main_sizer->Add(title_color_casillas, 0, wxALL, 10);
        wxBoxSizer* sizer_casillas = new wxBoxSizer(wxHORIZONTAL);
        s_c_casilla_black = new wxColourPickerCtrl(
                                                   this, wxID_ANY,
                                                   c_p.getColorCasillaWhite(),
                                                   wxDefaultPosition,
                                                   C_TAM
                                                   );
        s_c_casilla_white = new wxColourPickerCtrl(
                                                   this, wxID_ANY,
                                                   c_p.getColorCasillaBlack(),
                                                   wxDefaultPosition,
                                                   C_TAM
                                                   );
        sizer_casillas->Add(s_c_casilla_black, 0, wxALL, 0);
        sizer_casillas->Add(s_c_casilla_white, 0, wxALL, 0);
        main_sizer->Add(sizer_casillas, 1, wxEXPAND | wxALL, 5);


        wxStaticText* title_color_pieces = new wxStaticText(this, wxID_ANY, _("Color de las piezas"));
        main_sizer->Add(title_color_pieces, 0, wxALL, 10);
        wxBoxSizer* sizer_pieces = new wxBoxSizer(wxHORIZONTAL);
        s_c_piece_black = new wxColourPickerCtrl(
                                                   this, wxID_ANY,
                                                   c_p.getColorPieceBlack(),
                                                   wxDefaultPosition,
                                                   C_TAM
                                                   );
        s_c_piece_white = new wxColourPickerCtrl(
                                                   this, wxID_ANY,
                                                   c_p.getColorPieceWhite(),
                                                   wxDefaultPosition,
                                                   C_TAM
                                                   );
        s_c_piece_red = new wxColourPickerCtrl(
                                                   this, wxID_ANY,
                                                   c_p.getColorPieceRed(),
                                                   wxDefaultPosition,
                                                   C_TAM
                                                   );
        s_c_piece_yellow = new wxColourPickerCtrl(
                                                   this, wxID_ANY,
                                                   c_p.getColorPieceYellow(),
                                                   wxDefaultPosition,
                                                   C_TAM
                                                   );
        sizer_pieces->Add(s_c_piece_white, 0, wxALL, 0);
        sizer_pieces->Add(s_c_piece_red, 0, wxALL, 0);
        sizer_pieces->Add(s_c_piece_black, 0, wxALL, 0);
        sizer_pieces->Add(s_c_piece_yellow, 0, wxALL, 0);
        main_sizer->Add(sizer_pieces, 1, wxEXPAND | wxALL, 5);



        wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
        main_sizer->Add(buttonSizer, 0, wxALL | wxALIGN_RIGHT, 10);

        SetSizerAndFit(main_sizer);
        CenterOnParent();

        Bind(wxEVT_BUTTON, &StyleBoard::onAccept, this, wxID_OK);
    }

private:
    ChessParameters* ptr_c_p;

    wxColourPickerCtrl* s_c_casilla_black;
    wxColourPickerCtrl* s_c_casilla_white;
    wxColourPickerCtrl* s_c_piece_white;
    wxColourPickerCtrl* s_c_piece_red;
    wxColourPickerCtrl* s_c_piece_black;
    wxColourPickerCtrl* s_c_piece_yellow;

    void onAccept(wxCommandEvent& event){
        ptr_c_p->setColorCasillaBlack(s_c_casilla_black->GetColour().GetAsString(wxC2S_HTML_SYNTAX));
        ptr_c_p->setColorCasillaWhite(s_c_casilla_white->GetColour().GetAsString(wxC2S_HTML_SYNTAX));

        ptr_c_p->setColorPieceWhite(s_c_piece_white->GetColour().GetAsString(wxC2S_HTML_SYNTAX));
        ptr_c_p->setColorPieceBlack(s_c_piece_black->GetColour().GetAsString(wxC2S_HTML_SYNTAX));
        ptr_c_p->setColorPieceRed(s_c_piece_red->GetColour().GetAsString(wxC2S_HTML_SYNTAX));
        ptr_c_p->setColorPieceYellow(s_c_piece_yellow->GetColour().GetAsString(wxC2S_HTML_SYNTAX));

        ptr_c_p->saveConfiguracion();

        event.Skip();
    }
};

#endif // CHESSSTYLEBOARD_H_INCLUDED
