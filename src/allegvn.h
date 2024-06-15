/** \file allegvn.h
*	\brief  Header for Allegro functions and startup
*/

#ifndef ALLGVN_H
#define ALLGVN_H


extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* e_queue;
extern ALLEGRO_EVENT event;

extern ALLEGRO_PATH* romPath;
extern ALLEGRO_PATH* themePath;

extern ALLEGRO_COLOR nesPallette[64];


extern ALLEGRO_COLOR transparent;
extern ALLEGRO_COLOR black;
extern ALLEGRO_COLOR blueDark;
extern ALLEGRO_COLOR greenDark;
extern ALLEGRO_COLOR cyanDark;
extern ALLEGRO_COLOR redDark;
extern ALLEGRO_COLOR magentaDark;
extern ALLEGRO_COLOR brown;
extern ALLEGRO_COLOR grey;
extern ALLEGRO_COLOR greyDark;
extern ALLEGRO_COLOR blueBright;
extern ALLEGRO_COLOR greenBright;
extern ALLEGRO_COLOR cyanBright;
extern ALLEGRO_COLOR redBright;
extern ALLEGRO_COLOR magentaBright;
extern ALLEGRO_COLOR yellow;
extern ALLEGRO_COLOR white;


extern void initAllegro(void);


#endif /*  ALLGVN_H */