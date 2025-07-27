//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "screen.hpp"
#include "widgets.hpp"

enum MENU {

	MENU_MAIN,
    MENU_SETTINGS,
	MENU_EPISODES,
	MENU_CONTROLS,
	MENU_GRAPHICS,
	MENU_SOUNDS,
	MENU_NAME,
	MENU_LOAD,
	MENU_SAVE,
	MENU_LANGUAGE,
    MENU_LINKS
};

class MenuScreen: public Screen{
public:
    MenuScreen()
    :playerNameEdit(19, "/\\~:"){

    }

    void Init();
    void Loop();
    void Draw();
private:
    class MenuRect{
    public:
        int left = 0;
        int right = 0;
        int top = 0;
        int bottom = 0;
    }; 

    MenuRect bg_square;
    int menu_nyt = MENU_MAIN;
    int menu_lue_kontrollit = 0;
 
    PK2gui::TextInput playerNameEdit;

    uint episode_page = 0;
    uint langlistindex = 0;
    void Draw_BGSquare(int left, int top, int right, int bottom, u8 pvari);
    int  Draw_BackNext(int x, int y);
    int  Draw_Radio(int x, int y, int num, int sel);

    int  my = 0;
    void drawBoolBoxGroup(bool& value, bool& changed, const std::string& text_true, const std::string& text_false);

    PK2gui::LinksMenu bottomLinksMenu;

    void Draw_Menu_Main();
    void Draw_Menu_Name();
    void Draw_Menu_Load();
    void Draw_Menu_Save();
    void Draw_Menu_Graphics();
    void Draw_Menu_Sounds();
    void Draw_Menu_Controls();
    void Draw_Menu_Episodes();
    void Draw_Menu_Language();
    void Draw_Menu_Settings();
    void Draw_Menu_Links();
};