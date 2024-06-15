#include "includes.h"

ALLEGRO_BITMAP* font;


void initAssets(void)
{
	printf("Themes located at %s\n", al_path_cstr(themePath, ALLEGRO_NATIVE_PATH_SEP));
	al_set_path_filename(themePath, "font.png");

	//load font
	font = al_load_bitmap(al_path_cstr(themePath, ALLEGRO_NATIVE_PATH_SEP));
}
