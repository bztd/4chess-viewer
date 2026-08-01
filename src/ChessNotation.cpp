#include "ChessNotation.h"

ChessNotation::ChessNotation(wxWindow* parent,
                                wxWindowID id,
                                const wxString& value,
                                const wxPoint& pos,
                                const wxSize& _size,
                                long style)
    : wxRichTextCtrl(parent, id, value, pos, _size, style){

    SetBackgroundColour(*wxBLACK);
    Bind(wxEVT_LEFT_DOWN, &ChessNotation::OnPGNClick, this);

}

long ChessNotation::goToNextMove(bool show_mark){//primero goToNextMove() y luego getNextMovement()
    turno++;

    if(show_mark){
        DrawMark();
    }

    E_anterior=E_actual;

    return XYToPosition(E_anterior->init_m,E_anterior->line);
}
std::vector<int> ChessNotation::getNextMovement(){
    std::vector<int> m;

    while(true){
        if(linea>=plays.size()){return {-1};}
        if(turno>=plays[linea].size()){turno=0;linea++;}
        if(linea>=plays.size()){return {-1};}
        if(turno>=plays[linea].size()){return {-1};}

        Element& jugada=plays[linea][turno];

        if(jugada.type!=Type::MOVEMENT){
            turno=0;
            linea++;
            continue;
        }

        E_actual=&plays[linea][turno];

        long i_cor=XYToPosition(jugada.init_m,linea);
        long e_cor=XYToPosition(jugada.end_m,linea);

        m.push_back(jugada.corI.x);
        m.push_back(jugada.corI.y);
        m.push_back(jugada.corE.x);
        m.push_back(jugada.corE.y);
        m.push_back(i_cor);
        m.push_back(e_cor);

        return m;
    }
    return {-1};
}

wxPoint ConvertPosition(wxString pos){//borrar
    int i,j;
    long t_j;

    wxChar pos_i= wxTolower(pos[0]);
    i=static_cast<int>(pos_i)-97;

    wxString pos_j = pos.Mid(1);
    pos_j.ToLong(&t_j);
    j=14-(int)t_j;

    return wxPoint(i,j);
}
bool ChessNotation::InterpretPgn(wxString path){

    wxTextFile file(path);

    if (file.Open()) {
        bool was_numer_read=false;
        bool was_letter_read=false;
        bool is_castling=false;
        size_t contador_para_jugada=0;//ayuda a distinguir el orden de las cordenadas
        size_t turno = 0; //0..3
        int line=-1;

        Numero numero;
        Element e;

        for (wxString str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine()) {
            std::vector<Element> vE;
            line++;
            turno = 0;

            if((int)str[0]<48 || (int)str[0]>57){ //primer caracter diferente de numero
                e.line=line;
                vE.push_back(e);
                plays.push_back(vE);

                e.restart();
                continue;
            }

            for(size_t k=0;k<=str.Length();k++){
                int caracter;
                if(k!=str.Length()){//Para procesar ultimo caracter
                    caracter=static_cast<int>(str[k]);
                }else{
                    caracter=64;
                }

                if(caracter>96 && caracter<111){//letra a=97;n=110
                    was_letter_read=true;
                    if(contador_para_jugada==0){
                        e.init_m=k;
                        e.corI.x=caracter-97;
                    }else{
                        e.corE.x=caracter-97;
                    }
                    contador_para_jugada++;


                    continue;
                }
                if(was_letter_read&&caracter>47 && caracter<58){//number
                    numero.addDigito(caracter-49);
                    was_numer_read=true;

                    continue;
                }


                if(was_numer_read){
                    if(contador_para_jugada==1){
                        e.corI.y=13-numero.getNumero();
                    }

                    if(contador_para_jugada==3){
                        e.corE.y=13-numero.getNumero();
                    }
                    was_numer_read=false;
                    was_letter_read=false;
                    numero.restart();
                    contador_para_jugada++;

                }

                if(caracter==79){//Enrroque 79=O
                    if(contador_para_jugada==0){
                        e.init_m=k;
                        is_castling=true;
                    }
                    contador_para_jugada++;
                    continue;
                }
                if(is_castling && caracter!=45){//45=-
                    if(contador_para_jugada==2){//corto
                        e.corE.x=200;
                        e.corE.y=200;
                        e.corI.x=200;
                        e.corI.y=200;
                    }else{//largo
                        e.corE.x=300;
                        e.corE.y=300;
                        e.corI.x=300;
                        e.corI.y=300;
                    }
                    is_castling=false;
                }

                //si jugador se rinde y sus piezas zombi lo colocan en mate
                //no se envia movimiento, solo estado del rey
                if( str[k-1] == 'R' && caracter == 35){//35=#
                    e.init_m=k-1;

                    e.corI.x=3000;
                    e.corI.y=3000;
                    e.corE.x=3000;
                    e.corE.y=3000;
                }

                if(e.isMovementComplete()){

                    if(caracter==84 || caracter==82){//84=T rendicion 82=R
                        e.corI.x=500+e.corI.x;
                    }

                    e.line = line;
                    e.turno = turno;
                    e.end_m = k;
                    e.type = Type::MOVEMENT;
                    vE.push_back(e);

                    contador_para_jugada=0;
                    e.restart();
                    turno++;
                }

            }

            plays.push_back(vE);

        }
        file.Close();
    }
    return true;
}
void ChessNotation::LoadPGN(wxString path){

    Reset();
    LoadFile(path,wxRICHTEXT_TYPE_TEXT);
    InterpretPgn(path);
    setMovementStyle();

    //carga la partida en el tablero
    CFloadGameCB();

    //LayoutContent(); // Recalcula la disposición de párrafos y líneas
    //Refresh();       // Solicita que el control se vuelva a dibujar
    //Update();

}
void ChessNotation::setPosInicial(){
    linea=0;
    turno = 0;
    E_actual=nullptr;
    E_anterior=nullptr;

    P=nullptr;
}
void ChessNotation::Reset(){
    setPosInicial();
    plays.clear();

}
void ChessNotation::setMovementStyle(){
    Freeze();
    for(size_t l=0; l<plays.size();l++){
        for(const auto& e : plays[l]){
            if(e.type==Type::MOVEMENT){

                SetStyle( XYToPosition(e.init_m,e.line),
                          XYToPosition(e.end_m,e.line),
                          estilo_jugadas
                        );
            }
        }
    }
    Thaw();//refresca, para que se carge con "estilo"
}

void ChessNotation::setValuesForPosition(const long& pos){//mover a frame junto con el libro de board

    Element* temp_E_anterior = nullptr;

    bool control = false;
    for(std::vector<Element>& ln: plays){
        for(Element& e: ln){

            int initM = XYToPosition(e.init_m,e.line);
            int endM = XYToPosition(e.end_m,e.line);

            if(control){
                linea = e.line;
                turno = e.turno;
                E_actual = &e;//movimiento siguiente
                E_anterior = temp_E_anterior;

                return;
            }

            if(pos>=initM && pos<=endM){
                temp_E_anterior = &e;
                E_actual = temp_E_anterior;
                DrawMark();

                control = true;
            }

        }
    }
}
void ChessNotation::goToLastMovimiento(){
    if(plays.size() == 0){return;}

    Element& e = plays.back().back();

    E_actual = &e;
    DrawMark();
    E_anterior = &e;

    linea = e.line;
    turno = e.turno;

}
void ChessNotation::goTofirstMovimiento(){
    if(plays.size() == 0){return;}

    Element& e = plays[0][0];

    E_actual = &e;
    RevertDrawMark();
    E_anterior = nullptr;

    linea = e.line;
    turno = e.turno;

}
bool ChessNotation::setJugada(size_t n){
    if(n == 0){
        goTofirstMovimiento();
        return true;
    }
    size_t j = 0;
    bool control = false;

    for(std::vector<Element>& l: plays){
        for(Element& e: l){
            if(e.type != Type::MOVEMENT){continue;}

            if(control){
                linea = e.line;
                turno = e.turno;

                E_actual = &e;

                return true;
            }
            if( (j+1) == n ){

                E_actual = &e;

                DrawMark();

                E_anterior = &e;
                control = true;
            }

            j++;
        }
    }

    return false;
}
void ChessNotation::OnPGNClick(wxMouseEvent& event){
    wxPoint pt = event.GetPosition();
    long pos = 0;

    wxTextCtrlHitTestResult result = HitTest(pt, &pos);

    if(result==wxTE_HT_ON_TEXT){
        CBrestorePosition(pos);
        setValuesForPosition(pos);
    }
}
// Evento: tecla presionada
void ChessNotation::OnKeyDown(wxKeyEvent& event){
    // Ejemplo: Ctrl+B para negrita
    if (event.ControlDown() && event.GetKeyCode() == 'B')
    {
        ApplyBoldToSelection();
        return; // No propagar el evento
    }
    event.Skip(); // Propagar el evento al control base
}
void ChessNotation::setStyle(){
    estilo_global.SetFontSize(12);
    estilo_global.SetLeftIndent(24);
    estilo_global.SetTextColour(*wxRED);
    estilo_global.GetTextBoxAttr().GetBorder().SetColour(*wxBLUE);
    estilo_global.GetTextBoxAttr().GetBorder().SetWidth(2, wxTEXT_ATTR_UNITS_PIXELS);
    estilo_global.GetTextBoxAttr().GetBorder().SetStyle(wxTEXT_BOX_ATTR_BORDER_SOLID);
    SetBasicStyle(estilo_global);

    estilo_jugadas.SetFlags(  wxTEXT_ATTR_TEXT_COLOUR |
                              wxTEXT_ATTR_FONT_WEIGHT |
                              wxTEXT_ATTR_BACKGROUND_COLOUR
                           );
    estilo_jugadas.SetTextColour(*wxBLUE);
    estilo_jugadas.SetFontWeight(wxFONTWEIGHT_BOLD);
    estilo_jugadas.SetBackgroundColour(*wxYELLOW);

    estilo_jugada_actual.SetBackgroundColour(P->movement_mark);
}
void ChessNotation::RevertDrawMark(){
    if(E_anterior!=nullptr){
         SetStyle(
                 XYToPosition(E_anterior->init_m,E_anterior->line),
                 XYToPosition(E_anterior->end_m,E_anterior->line),
                 estilo_jugadas
                );
    }
}
void ChessNotation::DrawMark(){//dibuja un resaltado que sigue las jugadas
    if(E_actual!=nullptr){
        RevertDrawMark();
        SetStyle(
             XYToPosition(E_actual->init_m,E_actual->line),
             XYToPosition(E_actual->end_m,E_actual->line),
             estilo_jugada_actual
            );

    }else{
        wxLogMessage(_("ChessNotation::DrawMark: Puntero E_actual no definido"));
    }
}
void ChessNotation::setStyleParameter(ParametrosDeEstilo *p){
    P=p;

    setStyle();
}

// se asigna la funcion de la clase madre para orquestar con las otras clases el evento
// de dar click ChessFrame::jumpToPosition
void ChessNotation::setFCBrestorePosition(std::function<void(long)> f){
    CBrestorePosition=f;
}
void ChessNotation::setFCFloadGameCB(std::function<void()> f){
    CFloadGameCB=f;
}
