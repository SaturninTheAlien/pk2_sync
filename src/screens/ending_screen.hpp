//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "screen.hpp"

class EndingScreen: public Screen{
public:
    void Init();
    void Loop();

private:
    void Draw_EndGame_Image(int x, int y, int tyyppi, int plus, int rapytys);
    void Draw_EndGame();


    u32 loppulaskuri = 0;
    bool change_to_next_screen = false;
};