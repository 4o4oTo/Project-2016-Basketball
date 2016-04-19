#include "Texture.hpp"
#include "main.hpp"
#include "MusicButton.hpp"
#include "MenuButton.hpp"
#include "BlackPlayer.hpp"
#include "BasketballPole.hpp"
#include "Ball.hpp"

const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN) - 200;
const int AVG_FPS = 60;
const int TIME_PER_FRAME = 1000 / AVG_FPS;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

//Music
Mix_Music* gMusic = NULL;
MusicButton gMusicButton;

//Menu Background Texture
Texture gBackgroundTexture;
SDL_Rect gBackgroundClip;

//Menu Buttons
MenuButton gMenuButtons[TOTAL_MENU_BUTTONS];

//Font
TTF_Font* gFont = NULL;

//Court texture
Texture gCourt;
SDL_Rect gCourtClip;

//Player
BlackPlayer troy("Troy");

//Entities
BasketballPole gBasketballPole;
Ball gBall(5);

bool init();

bool loadMedia();

void shutdown();

int main(int argc, char* argv[]) {
    if(init()) {
        if(loadMedia()) {
            bool quit = false;
            SDL_Event e;
            Mix_PlayMusic(gMusic, -1);
            Uint8 prevTime = 0;
            Uint8 currTime = 0;
            Uint8 frameTime = 0;
            Uint8 totalFrameTime = 0;
            while(!quit) {

                prevTime = currTime;
                currTime = SDL_GetTicks();
                frameTime = currTime - prevTime;
                totalFrameTime += frameTime;

                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;
                    }
                    for(int i = 0; i < 3; i++) {
                        gMenuButtons[i].handleEvents(&e);
                    }

                    gMusicButton.handleEvents(&e);

                    if(gMenuButtons[PLAY].isClicked()) {
                        troy.handleEvents(&e);
                        gBall.handleEvents(&e);
                    }
                }

                SDL_RenderClear(gRenderer);

                if(gMenuButtons[PLAY].isClicked()) {
                    while(totalFrameTime >= TIME_PER_FRAME) {
                        troy.update();
                        gBall.update();
                        troy.checkBallCollision();
                        troy.checkBasketballPoleCollision(&gBasketballPole);
                        totalFrameTime -= TIME_PER_FRAME;
                    }
                    if(!gBall.isPossessed()) {
                        gBall.checkCollisionWithPole();
                    }
                    gCourt.render(0, 0, &gCourtClip);
                    gBall.render();
                    gBasketballPole.render();
                    troy.render();
                }
                else if(gMenuButtons[OPTIONS].isClicked()) {

                }
                else if(gMenuButtons[EXIT].isClicked()) {
                    quit = true;

                    gBackgroundTexture.render(0, 0, &gBackgroundClip);

                    for(int j = 0; j < 3; j++) {
                        gMenuButtons[j].render();
                    }
                }
                else {

                    gBackgroundTexture.render(0, 0, &gBackgroundClip);

                    for(int j = 0; j < 3; j++) {
                        gMenuButtons[j].render();
                    }
                }

                gMusicButton.render();

                SDL_RenderPresent(gRenderer);
            }
        }
    }
    shutdown();
    return 0;
}

bool init() {
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("1v1 Basketball game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //SDL_WINDOW_FULLSCREEN_DESKTOP);
        if(gWindow == NULL) {
            printf("%s\n",SDL_GetError());
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == NULL) {
                printf("%s\n",SDL_GetError());
                success = false;
            }

            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

            //Different image format initialization
            int imgFlags = IMG_INIT_JPG;
            if(!(IMG_Init(imgFlags) & imgFlags)) {
                printf("%s\n",IMG_GetError());
                success = false;
            }

            //Fonts init
            if(TTF_Init() == -1) {
                printf("%s\n",TTF_GetError());
                success = false;
            }

            //Music init
            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                printf("%s\n",Mix_GetError());
                success = false;
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    if(!gBackgroundTexture.loadFromFile("background/background.jpg")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gBackgroundClip.h = SCREEN_HEIGHT;
        gBackgroundClip.w = SCREEN_WIDTH;
        gBackgroundClip.x = SCREEN_WIDTH*0.2;
        gBackgroundClip.y = SCREEN_HEIGHT*0.4;
    }

    if(!gCourt.loadFromFile("background/court.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gCourtClip.x = 500;
        gCourtClip.y = 20;
        gCourtClip.w = SCREEN_WIDTH;
        gCourtClip.h = SCREEN_HEIGHT;
    }

    if(!gMusicButton.loadTextureFromFile("music/icon.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gMusicButton.setClip(UNMUTED, 0, 0, 40, 70);
        gMusicButton.setClip(MUTED, 40, 0, 40, 70);
        gMusicButton.setDimensions(50, 0, 40, 70);
    }

    gMusic = Mix_LoadMUS("music/hustle.wav");
    if(gMusic == NULL) {
        printf("%s\n", Mix_GetError());
        success = false;
    }

    gFont = TTF_OpenFont("font/Pacifico.ttf", 60);
    if(gFont == NULL) {
        printf("%s\n", TTF_GetError());
        success = false;
    }
    else {
        SDL_Color textColor = {0, 0, 0};
        //Play
        if(!gMenuButtons[PLAY].loadTextureFromText("Play", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[PLAY].setDimensions(SCREEN_WIDTH*0.45, SCREEN_HEIGHT*0.16, gMenuButtons[PLAY].getWidth(), gMenuButtons[PLAY].getHeight());
            gMenuButtons[PLAY].setCurrentButton(PLAY);
        }

        //Options
        if(!gMenuButtons[OPTIONS].loadTextureFromText("Options", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[OPTIONS].setDimensions(SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.5, gMenuButtons[OPTIONS].getWidth(), gMenuButtons[OPTIONS].getHeight());
            gMenuButtons[OPTIONS].setCurrentButton(OPTIONS);
        }

        //Exit
        if(!gMenuButtons[EXIT].loadTextureFromText("Exit", textColor)) {
            success = false;
        }
        else {
            gMenuButtons[EXIT].setDimensions(SCREEN_WIDTH*0.59, SCREEN_HEIGHT*0.5, gMenuButtons[EXIT].getWidth(), gMenuButtons[EXIT].getHeight());
            gMenuButtons[EXIT].setCurrentButton(EXIT);
        }


    }

    if(!gBasketballPole.getTexture().loadFromFile("entity/BasketballPole/BasketballPole.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gBasketballPole.setBoardDimensions(0, 0, 120,240);
        gBasketballPole.setDimensionsBelowBoard(0, gBasketballPole.getBoard().y + gBasketballPole.getBoard().h, 40,495);
        gBasketballPole.setRimDimensions(0, 220, 100, 10);
        gBasketballPole.setPosition(SCREEN_WIDTH -
                                    gBasketballPole.getBoard().w -
                                    (gBasketballPole.getTexture().getWidth() - gBasketballPole.getBoard().w)/2,
                                    SCREEN_HEIGHT - gBasketballPole.getTexture().getHeight());
        gBasketballPole.getBoard().x = gBasketballPole.getX() + (gBasketballPole.getTexture().getWidth() - gBasketballPole.getBoard().w)/2 - gBasketballPole.getBoard().w + 20;
        gBasketballPole.getBelowBoard().x = SCREEN_WIDTH - gBasketballPole.getBelowBoard().w;
        gBasketballPole.getRim().x = gBasketballPole.getBoard().x - gBasketballPole.getRim().w;
    }

    if(!gBall.getTexture().loadFromFile("entity/Basketball/Ball.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        gBall.setPosition(100, SCREEN_HEIGHT - gBall.getTexture().getHeight());
        gBall.passThePole(&gBasketballPole);
    }

    if(!troy.setNormalStance("player/NormalRunning/Run.1.png")) {
        printf("%s\n", SDL_GetError());
        success = false;
    }
    else {
        troy.setTextureRealDimensions(100,290);
        troy.setInitialPosition((SCREEN_WIDTH - troy.getTexture().getWidth())/2,(SCREEN_HEIGHT - troy.getTextureRealHeight()));
        troy.setFacingDirection(RIGHT);
        if(troy.setDefenceStance("player/DefenceStance/DefenceStance.png")) {
            troy.setDefenceScenes();
        }
        troy.setRunningScenes();
        troy.setJumpingScenes();
        troy.setDribblingScenes();
        troy.setStandDribbleScenes();
        troy.setShootingScenes();
        troy.passTheBall(&gBall);
    }

    return success;
}

void shutdown() {
    gMusicButton.free();
    gCourt.free();
    gBackgroundTexture.free();
    gBasketballPole.free();
    for(int i=0; i<3; i++) {
        gMenuButtons[i].free();
    }

    TTF_CloseFont(gFont);
    gFont = NULL;

    Mix_FreeMusic(gMusic);
    gMusic = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
