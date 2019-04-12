#pragma once
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Sprites.h"
#include "SDL_ttf.h"
#include <string>
using namespace std;

class Game
{
private:

	bool m_bRunning;
	bool m_bGotTick;
	int m_iState=0;//0 start, 1 game, 2 end
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
	SDL_Texture* m_pTextSplash;
	SDL_Surface* m_pSurface;
	const Uint8* m_iKeystates;
	Mix_Chunk* m_mFire;
	Mix_Chunk* m_mEngines;
	Mix_Chunk* m_mMusic;//background music
	int m_intDir = 0;
	int m_iWidth = 0;
	int m_iHeight = 0;

	//<ksmod>
	
	SDL_Texture* m_pTextureBackdrop;
	SDL_Texture* m_pTextureFg;
	MapSprite* m_pFog;
	MapSprite* m_pSplash;
	VScrollBack* m_backGround;

	TTF_Font *m_Font;
	SDL_Texture* m_pFontText;
	SDL_Rect m_rFontRect;
	int m_iTimer=200;
	int m_iScore = 0;
	int m_iDeath=0;
	string m_sTime;
	string m_sScore;


	//</ksmod>

	//psmod

	SDL_Texture* m_pTextureBlock;
	//MapSprite* m_pBlock;
	MapSprite* m_pClockBack;

	vector<Clock*> m_vClocks;
	//vector<MapSprite*> m_vBlocks;
	vector<MapSprite*> m_vBlocks;

	//psmod

	Player* m_pPlayer;
	

	Game() :m_bRunning(false), m_bGotTick(false) {}

public:
	void Restart(int,int);
	static Game* Instance();
	SDL_Window* GetWindow() { return m_pWindow; }
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	SDL_Texture* GetTexture() { return m_pTexture; }
	bool Init(const char* title, const int xpos, const int ypos, 
		      const int width, const int height, const int flags);

	void RenderFont(bool r, const char* c, int x, int y);
	
	bool Running();
	bool KeyDown(SDL_Scancode c);
	bool Tick();
	bool CircleCollisionTest(double, double, double, double, double, double);
	bool BlockCollisionCheck(Player*,MapSprite*);
	//int BlockCollisionCheck(Player* player, SpriteEx* sprite, int sdl_keycode);


	void CheckCollision();
	void Update();
	void HandleEvents();
	void Render();
	void Clean();

	void AddMapObjects();
	void DestroyMapObejcts();
};