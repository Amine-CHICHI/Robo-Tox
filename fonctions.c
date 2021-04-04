#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window * createWindow(){
    //Creation de la fenetre de taille ( 900 x 500)
    SDL_Window * window = SDL_CreateWindow("Bienvenue à Candy Land",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,900,500,0);

    if(window == NULL){
        SDL_Log("Erreur d'initialisation de la fenetre %s", SDL_GetError());   //une fonction "printf" de type string 
        SDL_Quit();   //faut appeler cette fonction meme si on appele SDL_QuitSubSystem() avant , qui de base quitte tt le systeme. 
        return NULL;
    }

    return window;
}


SDL_Renderer * createRenderer(SDL_Window * window){     // renderer: A software process that generates a visual image from a model.
    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;   //SDL RENDERER ACCELERATED :Utilise l'acc�l�ration mat�rielle, c'est � dire la carte graphique.
	// SDL_RENDERER_PRESENTVSYNC : synchronise le rendu avec la fr�quence de raffraichissement .

    SDL_Renderer * rend = SDL_CreateRenderer(window, -1 , renderer_flags);   // comme arguments on a la fenetre qu'on vient de cr�er , the index of the rendering driver to initialize , les flags
    if(!rend){
        printf("Erreur creation du rendu\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
}

SDL_Surface* createSurface(SDL_Window * window, SDL_Renderer * rend){    //les surfaces peuvent etre modifi�es pixel par pixel plus facilement que les textures .
    SDL_Surface* surface = IMG_Load("robot.png");  
    if(!surface){
        printf("Erreur creation de la surface \n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    return surface;
}

SDL_Texture * createTexture(SDL_Window * window, SDL_Renderer * rend, SDL_Surface * surface){
    SDL_Texture * Texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if(!Texture){
        printf("Erreur creation de la texture \n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    return Texture;
}


int handleEvents(SDL_Window * window, int *w, int *h, int *haut, int *droite, int *bas, int *gauche, SDL_Rect *position, SDL_Rect FormesGeo[], int randoms[], int* Num_Carre, int tab[], int * menu, int Zone_2[]){
    // SDL_RECT c'est la definition d'un rectangle , on commencant par le point gauche en haut , le pointeur rand est pour l'al�atoire des FormesGeo .
	SDL_Event ev;//SDL EVENT: une union qui contient diff�rentes structures de diff�rents types (concernant le kyboard , le window..)
    if(SDL_PollEvent(&ev)){// le fait de POLL un event c'est une technique d'interrogation continuelle des event pour v�rifier s'ils ont des donn�es � transf�rer
    
        if(ev.type == SDL_QUIT){
            return 1;
        }
       
        if(ev.type == SDL_KEYDOWN){// la sdl_keyevent r�agit quand on tape sur le clavier tout simplement . l'information sur quelle cl� a �t� press�e est dans le keysym.
            switch (ev.key.keysym.scancode)
            {
            
            case SDL_SCANCODE_1:
                if(*menu==0 || *menu==1){
                    clearGame(position, FormesGeo, randoms, Num_Carre, *w, *h,Zone_2);
                    *menu = 3;
                }
                break;
            case SDL_SCANCODE_R:
                if(*menu==1) *menu = 3;
                break;
            
            /*case SDL_SCANCODE_2:
                if(*menu==0 || *menu==1) *menu = 2;
                break;*/
            
            case SDL_SCANCODE_2:
                if(*menu==0 || *menu==1) return 1;
                break;
            
            case SDL_SCANCODE_P:
                if(*menu!=0) *menu = 1;
                break;
            
            case SDL_SCANCODE_UP: // fleche haut
                *haut = 1;
                break;
            
            case SDL_SCANCODE_LEFT: // fleche gauche
                *gauche = 1;
                break;
            
            case SDL_SCANCODE_DOWN: // fleche bas
                *bas = 1;
                break;
            
            case SDL_SCANCODE_RIGHT: // fleche droite
                *droite = 1;
                break;
            
            case SDL_SCANCODE_SPACE: // espace
                    checkCircle(position, FormesGeo, randoms, Num_Carre);
                    putCircle(position, FormesGeo, randoms, Num_Carre, tab);
                break;
            }
        }
       
        if(ev.type == SDL_KEYUP){
            switch (ev.key.keysym.scancode)   // quand on lache une touche tout elle revient a 0 pour que le robot n'avance pas  
            {
            
            case SDL_SCANCODE_UP:   
                *haut = 0;
                break;
            
            case SDL_SCANCODE_LEFT:
                *gauche = 0;
                break;
            
            case SDL_SCANCODE_DOWN:
                *bas = 0;
                break;
            
            case SDL_SCANCODE_RIGHT:
                *droite = 0;
                break;
            }
        }
    }
    return 0;
}


void controlRobot(int haut, int bas, int droite, int gauche, int speed, int *vitesse_x, int *vitesse_y){
    if (haut && !bas) *vitesse_y = -speed;
    if (bas && !haut) *vitesse_y = speed;
    if (gauche && !droite) *vitesse_x = -speed;
    if (droite && !gauche) *vitesse_x = speed;

}

void rotationRobot(int haut, int bas, int droite, int gauche, int *rot){
    if(bas==1 && droite==1){
        *rot = 135;       // rotation a 90 + 45 = 135 degres
    }else if(gauche==1 && bas==1){
        *rot = -135;       
    }else if(gauche==1 && haut==1){
        *rot = -45;       
    }else if(haut==1 && droite==1){
        *rot = 45;
    }else if(droite==1){
        *rot = 90;
    }else if(gauche==1){
        *rot = -90;
    }else if(haut==1){
        *rot = 0;
    }else if(bas==1){
        *rot = 180;
    }
}// une fonction qui r�sume la rotation du robot selon les sens donn�es avec la possibilit� de se d�placer inclin� .

void PositionUpdate(SDL_Rect *position, int vitesse_x, int vitesse_y, int w, int h){
    position->x+=vitesse_x;
    position->y+=vitesse_y;
    if(position->x>w-100) position->x=w-100;// cette instruction pour que le robot n'arrive pas � l'extr�mit� droite et ne d�passe pas celle gauche .
    if(position->x<0) position->x=0;
    if(position->y<0) position->y=0;
    if(position->y>h-100) position->y=h-100;//meme principe avec les deux extr�mit�s en haut et en bas 

    if((position->x<=700 && position->x>0 && position->y<=100)|| // pour ne pas depasser les bord du rectangle contenant les formesGeo en Zone 1 et Zone 2
        (position->x<=700 && position->x>0 && position->y>=300)){
            position->x-=vitesse_x;
            position->y-=vitesse_y;
    }
}

void checkCircle(SDL_Rect *position, SDL_Rect FormesGeo[], int randoms[], int* Num_Carre){
    int m = position->x + 50;//
    if(*Num_Carre==-1){
        if(position->y>=100 && position->y<130 && m>100 && m<=700){
            if(randoms[m/100-1]==0) *Num_Carre =  m/100-1;
        }else{
            *Num_Carre = -1;
        }
    }
}

void putCircle(SDL_Rect *position, SDL_Rect FormesGeo[],int randoms[], int* Num_Carre, int tab[]){
    int m = position->x + 50;
    if(*Num_Carre!=-1){
        if(position->y>=270 && position->y<=300 && m>100 && m<=700 && tab[(m/100)-1]==0){
            
            randoms[*Num_Carre]=-1; 
            FormesGeo[*Num_Carre].x= (m/100)*100;
            FormesGeo[*Num_Carre].y= 400;
            *Num_Carre =  -1;
            tab[(m/100)-1] = 1;
        }
    }
} // un blem des chiffres utilis�s 
void clearGame(SDL_Rect *position, SDL_Rect FormesGeo[],int randoms[], int* Num_Carre, int w, int h, int Zone_2[]){
    position->x = w -150;// initialiser la position du robot pour que quand on relance le jeu , on retourne a la position initiale(celle dont les parametres sont position.x , position.Y ..
    position->y = h/2 -50;
    position->w = 100;
    position->h = 100;
    for(int i = 1; i<=6; i++){
        FormesGeo[i-1].x = i*100;
        FormesGeo[i-1].y = 0;
        FormesGeo[i-1].w = 100;
        FormesGeo[i-1].h = 100;
    }

    int k;
    for (int i = 0; i<6; i++){
        k = rand()%3;// c'est cens� mettre dans k des valeurs entre 0 et trois
        randoms[i] = k;
    }

    *Num_Carre = -1;

    for(int i = 0; i<=5; i++){
        Zone_2[i] = 0;
    }
}
