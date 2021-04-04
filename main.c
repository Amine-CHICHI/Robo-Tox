#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "fonctions.h"


int main(int argc, char *argv[])
{

    
    // initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Erreur d'initialisation: %s", SDL_GetError());
        return 1;
    }

    // Creation de la fenetre 
    SDL_Window * window = createWindow();
    if(window == NULL) return 1;

    // Creation du rendu
    SDL_Renderer * rend = createRenderer(window);
    if(!rend) return 1;

    // Creation de la surface
    SDL_Surface* surface = createSurface(window, rend);
    if(!surface) return 1;
    // Creation de la texture
    SDL_Texture * Texture = createTexture(window, rend, surface);
    if(!Texture) return 1;
// ***************** CREATION DE L'ARRIERE PLAN ******************** //
    SDL_Surface* surface2 = IMG_Load("back.png"); 
    if(!surface2){
        printf("Erreur de creation de la surface \n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* Texture_Background = SDL_CreateTextureFromSurface(rend, surface2);
    SDL_FreeSurface(surface2); 
    if(!Texture_Background){
        printf("Erreur creation de la texture \n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Rect rectangle1={0,0,900,500};
    SDL_QueryTexture(Texture_Background, NULL, NULL, &rectangle1.w, &rectangle1.h); 
 //******************* CREATION DES MENUS *********************//

    SDL_Surface* Main_Menu = IMG_Load("MainMenu.png");
    if(!Main_Menu){
        printf("Erreur creation de la surface!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture * Texture_MainMenu = SDL_CreateTextureFromSurface(rend, Main_Menu);
    SDL_FreeSurface(Main_Menu);
    if(!Texture_MainMenu){
        printf("Erreur creation de la texture!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    } 

    SDL_Surface* PauseMenu = IMG_Load("PauseMenu.png");
    if(!PauseMenu){
        printf("Erreur creation de la surface!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture * Texture_PauseMenu = SDL_CreateTextureFromSurface(rend, PauseMenu);
    SDL_FreeSurface(PauseMenu);
    if(!Texture_PauseMenu){
        printf("Erreur creation de la texture!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    } 

// ***************** CREATION DU CERCLE ******************** //

    SDL_Surface* cercle = IMG_Load("cercle.png");
    if(!cercle){
        printf("Erreur de creation de la surface \n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture * Texture_Cercle = SDL_CreateTextureFromSurface(rend, cercle);
    SDL_FreeSurface(cercle);
    if(!Texture_Cercle){
        printf("Erreur creation de la texture!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    // *****************  CREATION DU RECTANGLE  ******************** //



    SDL_Surface* rectangle = IMG_Load("rectangle.png");
    if(!rectangle){
        printf("Erreur creation de la surface!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture * texRect = SDL_CreateTextureFromSurface(rend, rectangle);
    SDL_FreeSurface(rectangle);
    if(!texRect){
        printf("Erreur creation de la texture!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    // ***************** CREATION DU TRIANGLE ******************** //




    SDL_Surface* triangle = IMG_Load("triangle.png");
    if(!triangle){
        printf("Erreur creation de la surface!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture * Texture_Triangle = SDL_CreateTextureFromSurface(rend, triangle);
    SDL_FreeSurface(triangle);
    if(!Texture_Triangle){
        printf("Erreur creation de la texture!!\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }    

    SDL_Rect FormesGeo[6];        // position des formes geometriques chaque objet dans un carre de ( 100px * 100px )
    for(int i = 1; i<=6; i++){
        FormesGeo[i-1].x = i*100;
        FormesGeo[i-1].y = 0;
        FormesGeo[i-1].w = 100;
        FormesGeo[i-1].h = 100;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h); // dimension de la fenetre 
    
    SDL_Rect position,Taille_Fen;
    
    position.x = w -300;  // Position initial du Robot 
    position.y = h/2;
    position.w = 100;
    position.h = 100;

    Taille_Fen.x = 0; // va nous aider a afficher les menus apres 
    Taille_Fen.y = 0;
    Taille_Fen.w = w;
    Taille_Fen.h = h;

    //Parametre du Robot

    int  vitesse = 7 , haut = 0 , bas = 0 , droite = 0 , gauche = 0 , rotation = 0, k , randoms[6];

    srand(time(NULL)); // Permets à rand() de générer un Nbr aléatoire à chaque fois 
    for (int i = 0; i<6; i++){
        k = rand()%3;
        randoms[i] = k;
    }

    int Num_Carre = -1; // il recoit le numero du carre qui contient un cercle 
    int Zone_2[6]={0,0,0,0,0,0}; // zone ou on met les cercles 

    int Menu = 0;

    while(1){
        if(Menu==0){

            int ev = handleEvents(window, &w, &h, &haut, &droite, &bas, &gauche, &position, FormesGeo, randoms, &Num_Carre, Zone_2, &Menu, Zone_2);
            if(ev==1) break;

            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderClear(rend);
            SDL_RenderCopyEx(rend, Texture_MainMenu  , NULL, &Taille_Fen, 0, NULL, SDL_FLIP_NONE);
            SDL_RenderPresent(rend);

            SDL_Delay(1000/60);      // pour avoir 60 fps

        }else if(Menu==1){
            int ev = handleEvents(window, &w, &h, &haut, &droite, &bas, &gauche, &position, FormesGeo, randoms, &Num_Carre, Zone_2, &Menu, Zone_2);
            if(ev==1) break;

            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderClear(rend);
            SDL_RenderCopyEx(rend, Texture_PauseMenu, NULL, &Taille_Fen, 0, NULL, SDL_FLIP_NONE);
            SDL_RenderPresent(rend);

            
            SDL_Delay(1000/60);     // pour avoir 60 fps

        }else if(Menu==2){
            int ev = handleEvents(window, &w, &h, &haut, &droite, &bas, &gauche, &position, FormesGeo, randoms, &Num_Carre, Zone_2, &Menu, Zone_2);
            
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderClear(rend);
            SDL_RenderCopyEx(rend, Texture_MainMenu  , NULL, &Taille_Fen, 0, NULL, SDL_FLIP_NONE);
            SDL_RenderPresent(rend);

            
            SDL_Delay(1000/60);

        }else{
            int ev = handleEvents(window, &w, &h, &haut, &droite, &bas, &gauche, &position, FormesGeo, randoms, &Num_Carre, Zone_2, &Menu, Zone_2);
            if(ev==1) break;


            // determiner la vitesse
            int vitesse_x = 0;
            int vitesse_y = 0;
            controlRobot(haut, bas, droite, gauche, vitesse, &vitesse_x, &vitesse_y);

            //////////////////////   Rendu couleur  //////////////////
            SDL_SetRenderDrawColor(rend, 0, 0, 250, 255);
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, Texture_Background, NULL, &rectangle1);
            

            rotationRobot(haut, bas, droite, gauche, &rotation);

            for(int i = 0; i<6; i++){
                switch (randoms[i])
                {
                case 0:
                    SDL_RenderCopyEx(rend, Texture_Cercle  , NULL, &FormesGeo[i], 0, NULL, SDL_FLIP_NONE);
                    break; 
                case 1:
                    SDL_RenderCopyEx(rend, texRect, NULL, &FormesGeo[i], 0, NULL, SDL_FLIP_NONE);
                    break; 
                case 2:
                    SDL_RenderCopyEx(rend, Texture_Triangle, NULL, &FormesGeo[i], 0, NULL, SDL_FLIP_NONE);
                    break;  
                case -1:
                    SDL_RenderCopyEx(rend, Texture_Cercle  , NULL, &FormesGeo[i], 0, NULL, SDL_FLIP_NONE);
                    break;


                }
            }

////////////////////// RECTANGLE HAUT ///////////////////////          
	        SDL_SetRenderDrawColor(rend,255, 27, 25,255);
            SDL_RenderDrawLine(rend, 90, 0, 90, 107);
            SDL_RenderDrawLine(rend, 91, 0, 91, 107);
	        SDL_SetRenderDrawColor(rend,255,255,255,255);
            SDL_RenderDrawLine(rend, 89, 0, 89, 108);
            SDL_RenderDrawLine(rend, 88, 0, 88, 109);
            SDL_SetRenderDrawColor(rend,255, 27, 25,255);
            SDL_RenderDrawLine(rend, 87, 0, 87, 110);
            SDL_RenderDrawLine(rend, 86, 0, 86, 111);
  
            SDL_SetRenderDrawColor(rend,255, 27, 25,255);
            SDL_RenderDrawLine(rend, 90, 106, 710, 106);
            SDL_RenderDrawLine(rend, 90, 107, 710, 107);
	        SDL_SetRenderDrawColor(rend,255,255,255,255);
            SDL_RenderDrawLine(rend, 88, 108, 711, 108);
            SDL_RenderDrawLine(rend, 88, 109, 711, 109);
	        SDL_SetRenderDrawColor(rend,255, 27, 25,255);
            SDL_RenderDrawLine(rend, 87, 110, 714, 110);
            SDL_RenderDrawLine(rend, 86, 111, 714, 111);
   
            SDL_SetRenderDrawColor(rend,255, 27, 25,255);
            SDL_RenderDrawLine(rend, 710, 0, 710, 107);
            SDL_RenderDrawLine(rend, 709, 0, 709, 107);
	        SDL_SetRenderDrawColor(rend,255,255,255,255);
            SDL_RenderDrawLine(rend, 711, 0, 711, 108);
            SDL_RenderDrawLine(rend, 712, 0, 712, 109);
            SDL_SetRenderDrawColor(rend,255, 27, 25,255);
            SDL_RenderDrawLine(rend, 713, 0, 713, 110);
            SDL_RenderDrawLine(rend, 714, 0, 714, 111);
/////////////////////// RECTANGLE BAS ///////////////////////   
	        SDL_SetRenderDrawColor(rend,255,0,0,255);
            SDL_RenderDrawLine(rend, 90, 500, 90, 393);
            SDL_RenderDrawLine(rend, 91, 500, 91, 393);
            SDL_SetRenderDrawColor(rend,255,255,255,255);
            SDL_RenderDrawLine(rend, 89, 500, 89, 392);
            SDL_RenderDrawLine(rend, 88, 500, 88, 391);
            SDL_SetRenderDrawColor(rend,255,0,0,255);
            SDL_RenderDrawLine(rend, 87, 500, 87, 390);
            SDL_RenderDrawLine(rend, 86, 500, 86, 389);

            SDL_SetRenderDrawColor(rend,255,0,0,255);
            SDL_RenderDrawLine(rend, 91, 394, 709, 394);
            SDL_RenderDrawLine(rend, 91, 393, 710, 393);
            SDL_SetRenderDrawColor(rend,255,255,255,255);
            SDL_RenderDrawLine(rend, 88, 392, 711, 392);
            SDL_RenderDrawLine(rend, 88, 391, 711, 391);
            SDL_SetRenderDrawColor(rend,255,0,0,255);
            SDL_RenderDrawLine(rend, 88, 390, 713, 390);
            SDL_RenderDrawLine(rend, 88, 389, 714, 389);

            SDL_SetRenderDrawColor(rend,255,0,0,255);
            SDL_RenderDrawLine(rend, 709, 500, 709, 393);
            SDL_RenderDrawLine(rend, 710, 500, 710, 393);
            SDL_SetRenderDrawColor(rend,255,255,255,255);
            SDL_RenderDrawLine(rend, 711, 500, 711, 392);
            SDL_RenderDrawLine(rend, 712, 500, 712, 391);
            SDL_SetRenderDrawColor(rend,255,0,0,255);
            SDL_RenderDrawLine(rend, 713, 500, 713, 390);
            SDL_RenderDrawLine(rend, 714, 500, 714, 389);
/////////////////////////////////////////////////////////////


            SDL_SetTextureColorMod( rend, 255, 255, 255 );
            SDL_RenderCopyEx(rend, Texture, NULL, &position, rotation, NULL, SDL_FLIP_NONE);
            SDL_RenderPresent(rend);


            PositionUpdate(&position, vitesse_x, vitesse_y, w, h);
            if(Num_Carre!=-1){
                FormesGeo[Num_Carre].x = position.x;
                FormesGeo[Num_Carre].y = position.y;
            }

            SDL_Delay(1000/60);
        }
    }

    SDL_DestroyTexture(Texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

