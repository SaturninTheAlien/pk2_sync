#include "screen.hpp"
#include "engine/Piste.hpp"
#include "sfx.hpp"
#include "system.hpp"
#include "gfx/text.hpp"
#include "language.hpp"


unsigned int Screen::chosen_menu_id = 0;
unsigned int Screen::selected_menu_id = 0;
bool Screen::closing_game = false;
int Screen::next_screen = 0;

void Screen::Fade_Quit() {

	if(!closing_game) Fade_out(FADE_FAST);
	closing_game = true;
	PSound::set_musicvolume(0);
	
}

bool Screen::Draw_Menu_Text(int id, int x, int y){
	return Draw_Menu_Text(tekstit->Get_Text(id), x, y);
}

bool Screen::Draw_Menu_Text(const std::string& text, int x, int y) {

	const int TEXT_H = 20; 

	int length = text.size() * 15;

	bool mouse_on = PInput::mouse_x > x && PInput::mouse_x < x + length 
		&& PInput::mouse_y > y && PInput::mouse_y < y + TEXT_H
		&& !mouse_hidden;

	if ( mouse_on || (chosen_menu_id == selected_menu_id) ) {

		chosen_menu_id = selected_menu_id;
		Wavetext_Draw(text.c_str(), fontti3, x, y);//

		int c = Clicked();
		if ( (c == 1 && mouse_on) || (c > 1) ) {

			Play_MenuSFX(sfx_global.menu_sound, 100);
			key_delay = 20;
			selected_menu_id++;
			
			return true;

		}
	} else {
	
		WavetextSlow_Draw(text.c_str(), fontti2, x, y);
	
	}

	selected_menu_id++;

	return false;
}