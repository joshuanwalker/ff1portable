/** \file allegvn.c
*	\brief  Allegro functions and startup
*/

#include "includes.h"


void initColors(void);


ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_CONFIG* config = NULL;
ALLEGRO_FILE* configFile = NULL;
ALLEGRO_PATH* configPath = NULL;
ALLEGRO_PATH* savePath = NULL;
ALLEGRO_PATH* themePath = NULL;
ALLEGRO_PATH* romPath = NULL;
ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* e_queue;
ALLEGRO_EVENT event;

ALLEGRO_COLOR nesPallette[64];

ALLEGRO_COLOR transparent;
ALLEGRO_COLOR black;
ALLEGRO_COLOR blueDark;
ALLEGRO_COLOR greenDark;
ALLEGRO_COLOR cyanDark;
ALLEGRO_COLOR redDark;
ALLEGRO_COLOR magentaDark;
ALLEGRO_COLOR brown;
ALLEGRO_COLOR grey;
ALLEGRO_COLOR greyDark;
ALLEGRO_COLOR blueBright;
ALLEGRO_COLOR greenBright;
ALLEGRO_COLOR cyanBright;
ALLEGRO_COLOR redBright;
ALLEGRO_COLOR magentaBright;
ALLEGRO_COLOR yellow;
ALLEGRO_COLOR white;



int resolutionMode;
int resx;
int resy;
int gameHeight;
int gameWidth;

const char* theme;



void  initConfig(void)
{
	int error;
	if (!al_filename_exists(al_path_cstr(configPath, ALLEGRO_NATIVE_PATH_SEP))) {
		printf("trying to make configuration folder\n");
		if (!al_make_directory(al_path_cstr(configPath, ALLEGRO_NATIVE_PATH_SEP))) {
			printf("trying to make configuration folder failed!\n");
			error = al_get_errno();
			printf("error was %d\n", error);
			exit(error);
		}
	}
	//create config file if one does not exist
	al_set_path_filename(configPath, "config.cfg");
	if (!al_filename_exists(al_path_cstr(configPath, ALLEGRO_NATIVE_PATH_SEP))) {
		configFile = al_fopen(al_path_cstr(configPath, ALLEGRO_NATIVE_PATH_SEP), "w");
		if (configFile != NULL) {
			al_fprintf(configFile, "scale = 1\nfullscreen = 0\ncustomResX = 1024\ncustomResY = 960\ntheme = original");
			al_fclose(configFile);
		}
	}
}


void initPaths(void)
{
	int error;
	configPath = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
	al_replace_path_component(configPath, -1, "ff1portable");
	savePath = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
	al_replace_path_component(savePath, -1, "ff1portable");
	themePath = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
	al_replace_path_component(themePath, -1, "ff1portable");
	al_append_path_component(themePath, "themes");
	romPath = al_get_standard_path(ALLEGRO_USER_DATA_PATH);
	al_replace_path_component(romPath, -1, "ff1portable");
	al_append_path_component(romPath, "rom");

	//Make theme folder if one doesn't exist
	if (!al_filename_exists(al_path_cstr(themePath, ALLEGRO_NATIVE_PATH_SEP))) {
		printf("trying to make theme folder\n");
		if (!al_make_directory(al_path_cstr(themePath, ALLEGRO_NATIVE_PATH_SEP))) {
			printf("trying to make theme folder failed!\n");
			error = al_get_errno();
			printf("error was %d\n", error);
			exit(error);
		}
	}

	//Make rom folder if one doesn't exist
	if (!al_filename_exists(al_path_cstr(romPath, ALLEGRO_NATIVE_PATH_SEP))) {
		printf("trying to make rom folder\n");
		if (!al_make_directory(al_path_cstr(romPath, ALLEGRO_NATIVE_PATH_SEP))) {
			printf("trying to make rom folder failed!\n");
			error = al_get_errno();
			printf("error was %d\n", error);
			exit(error);
		}
	}


	//printf("Config located at %s\n", al_path_cstr(configPath, ALLEGRO_NATIVE_PATH_SEP));
	//printf("Save Data located at %s\n", al_path_cstr(savePath, ALLEGRO_NATIVE_PATH_SEP));
	//printf("Themes located at %s\n", al_path_cstr(themePath, ALLEGRO_NATIVE_PATH_SEP));
	//printf("rom path located at %s\n", al_path_cstr(romPath, ALLEGRO_NATIVE_PATH_SEP));

}

void initAllegro(void)
{

	int fullscreen = 0;

	if (!al_init()) {
		printf("Could not init Allegro.\n");
		exit(1);
	}
	if (!al_install_keyboard()) {
		printf("Error installing keyboard.\n");
		exit(1);
	}
	if (!al_init_image_addon()) {
		printf("Could not init Allegro Image Loader.\n");
		exit(1);
	}
	//if (!al_init_font_addon()) {
	//	printf("Could not init Allegro Font System.\n");
	//	exit(1);
	//}
	//if (!al_init_ttf_addon()) {
	//	printf("Could not init Allegro TrueTypeFont Extention.\n");
	//	exit(1);
	//}



	timer = al_create_timer(.7);
	e_queue = al_create_event_queue();
	al_register_event_source(e_queue, al_get_keyboard_event_source());
	al_register_event_source(e_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	initPaths();
	initConfig();
	initColors();
	config = al_load_config_file(al_path_cstr(configPath, ALLEGRO_NATIVE_PATH_SEP));
	if (!config)
	{
		printf("Failed to read settings!\n");
		exit(1);
	}
	resx = atoi(al_get_config_value(config, NULL, "customResX"));
	resy = atoi(al_get_config_value(config, NULL, "customResY"));
	fullscreen = atoi(al_get_config_value(config, NULL, "fullscreen"));
	resolutionMode = atoi(al_get_config_value(config, NULL, "scale"));
	theme = al_get_config_value(config, NULL, "theme");

	//if the theme is original, see if rom has been extracted to the folder
	al_append_path_component(themePath, theme);
	if (!strcmp(theme, "original"))
	{
		//if the folder doesn't exist, go extract the rom
		if (!al_filename_exists(al_path_cstr(themePath, ALLEGRO_NATIVE_PATH_SEP))) { romExtract(); }
	}

	//printf("Themes located at %s\n", al_path_cstr(themePath, ALLEGRO_NATIVE_PATH_SEP));
	

	al_destroy_config(config);

	switch (resolutionMode)
	{
	case 0:
		gameWidth = resx; gameHeight = resy;
		break;
	case 1:
		gameWidth = 256; gameHeight = 240;
		break;
	case 2:
		gameWidth = 512; gameHeight = 480;
		break;
		//16:10
	case 3:
		gameWidth = 768; gameHeight = 720;
		break;
	case 4:
		gameWidth = 1024; gameHeight = 960;
		break;
	case 5:
		gameWidth = 1200; gameHeight = 1200;
		break;
	default:
		gameWidth = 256; gameHeight = 240;
	}

	//override
	//gameWidth = 1024; gameHeight = 960;

	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR);

	if (fullscreen)
	{
		al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	}
	else
	{
		al_set_new_display_flags(ALLEGRO_WINDOWED);
	}
	display = al_create_display(gameWidth, gameHeight);
	if (!display)
	{
		printf("Failed to create display!\n");
		exit(1);
	}

	//This scales the display to any resolution (Native is 256x240)
	float scaleX = al_get_display_width(display) / (float)256;
	float scaleY = al_get_display_height(display) / (float)240;
	ALLEGRO_TRANSFORM trans;
	al_identity_transform(&trans);
	al_scale_transform(&trans, scaleX, scaleY);
	al_use_transform(&trans);
	al_set_target_backbuffer(display);
	al_clear_to_color(black);
}


void initColors(void)
{
	int colorIndex;
	uint8_t palRed, palGreen, palBlue;

	int i;

	for (i = 0; i <= 191; i+=3)
	{
		colorIndex = i / 3;
		palRed = PALETTE_DATA[i];
		palGreen = PALETTE_DATA[i + 1];
		palBlue = PALETTE_DATA[i + 2];
		//printf("read for color %x -> %x, %x, %x\n", colorIndex, palRed, palGreen, palBlue);
		nesPallette[colorIndex] = al_map_rgb(palRed, palGreen, palBlue);
	}

	nesPallette[0x0D] = al_map_rgba(0, 0, 0, 0); //Hijacking invalid color 0x0D for trasparent. 
	transparent = nesPallette[0x0D];

	black = nesPallette[0x0F];
	blueDark = nesPallette[0x02];
	greenDark = nesPallette[0x1A];
	cyanDark = nesPallette[0x1C];
	redDark = nesPallette[0x06];
	magentaDark = nesPallette[0x14];
	brown = nesPallette[0x18];
	grey = nesPallette[0x10];
	greyDark = nesPallette[0x00];
	blueBright = nesPallette[0x12];
	greenBright = nesPallette[0x2A];
	cyanBright = nesPallette[0x2C];
	redBright = nesPallette[0x16];
	magentaBright = nesPallette[0x24];
	yellow = nesPallette[0x28];
	white = nesPallette[0x20];
}