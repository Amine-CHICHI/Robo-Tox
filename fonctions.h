#ifndef INTI_H
#define INTI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window *createWindow();

SDL_Renderer *createRenderer(SDL_Window *window);

SDL_Surface *createSurface(SDL_Window *window, SDL_Renderer *rend);

SDL_Texture *createTexture(SDL_Window *window, SDL_Renderer *rend, SDL_Surface *surface);

int handleEvents(SDL_Window *window, int *w, int *h, int *haut, int *droite, int *bas, int *gauche, SDL_Rect *position, SDL_Rect FormesGeo[], int randoms[], int *Num_Carre, int tab[], int *menu, int Zone_2[]);

void controlRobot(int haut, int bas, int droite, int gauche, int speed, int *vitesse_x, int *vitesse_y);

void rotationRobot(int haut, int bas, int droite, int gauche, int *rot);

void PositionUpdate(SDL_Rect *position, int vitesse_x, int vitesse_y, int w, int h);

void checkCircle(SDL_Rect *position, SDL_Rect FormesGeo[], int randoms[], int *Num_Carre);

void putCircle(SDL_Rect *position, SDL_Rect FormesGeo[], int randoms[], int *Num_Carre, int tab[]);

void clearGame(SDL_Rect *position, SDL_Rect FormesGeo[], int randoms[], int *Num_Carre, int w, int h, int Zone_2[]);

#endif
