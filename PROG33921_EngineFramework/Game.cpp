#include <chrono>
#include <iostream>
#include "Game.h"
#define FPS 30
using namespace std;
using namespace chrono;

Game* Game::Instance()
{
	static Game* instance = new Game();
	return instance;
}

bool Game::Init(const char* title, const int xpos, const int ypos, 
			    const int width, const int height, const int flags)
{
	m_iWidth = width;
	m_iHeight = height;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // 0 is error code meaning success.
	{
		cout << "SDL init success!" << endl;
		// Initialize the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success. 
		{
			cout << "Window creation successful!" << endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success. 
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
				cout << "Renderer creation success!" << endl;
				if (IMG_Init(IMG_INIT_PNG) != 0)
				{
					m_pSurface = IMG_Load("Img/hood.png");
		
					m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pSurface);
					SDL_FreeSurface(m_pSurface); // It's okay to do this now.

					//ksmod
					m_pSurface = IMG_Load("Img/gridBackground.png");
					m_pTextureBackdrop = SDL_CreateTextureFromSurface(m_pRenderer, m_pSurface);
					SDL_FreeSurface(m_pSurface); // It's okay to do this now.

					m_pSurface = IMG_Load("Img/fog.png");
					m_pTextureFg = SDL_CreateTextureFromSurface(m_pRenderer, m_pSurface);
					SDL_FreeSurface(m_pSurface); // It's okay to do this now.

					m_pSurface = IMG_Load("Img/gameStart.png");
					m_pTextSplash= SDL_CreateTextureFromSurface(m_pRenderer, m_pSurface);
					SDL_FreeSurface(m_pSurface); // It's okay to do this now.

					//ksmod

					//psmod

					m_pSurface = IMG_Load("Img/block2.png");// "100x100.png");
					m_pTextureBlock = SDL_CreateTextureFromSurface(m_pRenderer, m_pSurface);
					SDL_FreeSurface(m_pSurface); // It's okay to do this now.

					//psmod

					cout << "Image creation success!" << endl;
					if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
					{
						Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
						Mix_AllocateChannels(16);
						m_mFire = Mix_LoadWAV("Fire.wav");
						m_mEngines = Mix_LoadWAV("Engines.wav");
						m_mMusic = Mix_LoadWAV("music.mp3");
						//Mix_VolumeChunk(m_mFire, 50); These give errors
						//Mix_VolumeChunk(m_mEngines, 50);
						//Mix_VolumeChunk(m_mMusic, 50);

						cout << "Mixer creation success!" << endl;
					}
					else
					{
						cout << "Mixer init fail!" << endl;
						return false; // Mixer init fail.
					}
				}
				else
				{
					cout << "Image init fail!" << endl;
					return false;// Image init fail.
				}
			}
			else
			{
				cout << "Renderer init fail!" << endl;
				return false; // Renderer init fail. 
			}
		}
		else
		{
			cout << "Window init fail!" << endl;
			return false; // Window init fail. 
		}
	}
	else
	{
		cout << "SDL init fail!" << endl;
		return false; // SDL init fail. 
	}

	if (TTF_Init() == 0) // Font init success.
	{
		cout << "Font init success!" << endl;
		m_Font = TTF_OpenFont("Img/LTYPE.TTF", 20);
	}
	else
	{
		cout << "Font init fail!" << endl;
		printf("TTF_OpenFont: %s\n", TTF_GetError());

		return false; // Font init fail.
	}
	srand((unsigned)time(NULL));
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	

	//ksmod
	m_backGround = new VScrollBack(m_pTextureBackdrop, width, height, 150, 150);
	this->m_pSplash= new MapSprite(
		0, 0, 0, 0, 0, 0,
		{ 0,0,150, 150 },
		{ 0, 0, 320, 320 },
		this->m_pTextSplash);
	
	m_pFog = new MapSprite(
		0, 0, 0, 0, 0, 0,
		{ 0,0,150, 150 },
		{ 0, 0, 320, 320 },
		this->m_pTextureFg);
	Mix_PlayChannel(14, m_mMusic, -1);

	
	m_pClockBack = new MapSprite(
		0, 0, 0, 0, 0, 0,
		{ 0,120,40,40 },
		{ 0,0,140,75 },
		this->m_pTextureBlock
	);
	m_pPlayer = new Player(width / 2, height / 2);
	//AddMapObjects();

	m_bRunning = true;
	return true;
}

void Game::Restart(int height,int width) {
	
	DestroyMapObejcts();
	m_iState = 0;
	m_iTimer = 200;
	m_iDeath = 0;
	m_iScore = 0;

}

bool Game::Running()
{
	return m_bRunning;
}

void Game::AddMapObjects() {


	int const BLOCK_SIZE = 40;//100
	int block_x = 20;

	int block_y = 300;

	int rnd_block_size = 8 * (2 + (rand() % (8))) + 1; //min 17 - max 65

	int rnd_clock =9+(rand() % (rnd_block_size-8)); //min 9 - max block size - 8


	if (rnd_block_size == rnd_clock)
		rnd_clock--;

	for (int i = 0; i < rnd_block_size; i++)//49
	{
		if (i == rnd_clock) {
			m_vBlocks.push_back(new Clock(
				0, 0, 0, 0, { 0,80,BLOCK_SIZE, BLOCK_SIZE },
				{ block_x, block_y, BLOCK_SIZE, BLOCK_SIZE },
				this->m_pTextureBlock));
			//continue;
		
		}else{
		m_vBlocks.push_back(new Block(
			0, 0, 0, 0, { 0,0,BLOCK_SIZE, BLOCK_SIZE },
			{ block_x, block_y, BLOCK_SIZE, BLOCK_SIZE },
			this->m_pTextureBlock
		));
		}
		if( (i % 8 == 0 )&&(i != 0))    //8
		{
			block_y += BLOCK_SIZE;
			block_x = 20;
		}
		else
			block_x += BLOCK_SIZE;// -1;//will this cause any collisions?

	}





}

void Game::DestroyMapObejcts() {

	for (int i = 0; i < m_vBlocks.size(); i++)
	{
	//	if (m_vBlocks[i] != nullptr) {
			delete  m_vBlocks[i];
			m_vBlocks[i] = nullptr;
			m_vBlocks.erase(m_vBlocks.begin() + i);
			m_vBlocks.clear();
			m_vBlocks.shrink_to_fit();
		//}

	}
	
}

bool Game::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

bool Game::Tick()
{
	auto duration = steady_clock::now().time_since_epoch();
	auto count = duration_cast<microseconds>(duration).count();
	int tick = 1000000 / FPS;
	if (count % tick < 100) // Margin of error for modulus.
	{
		if (m_bGotTick == false) // Drops potential duplicate frames.
			m_bGotTick = true;
	}
	else m_bGotTick = false;
	return m_bGotTick;
}

bool Game::CircleCollisionTest(double x1, double y1, double x2, double y2, double r1, double r2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) < (r1 + r2));
}

//<ksmod>
bool Game::BlockCollisionCheck(Player* player, MapSprite* sprite) {

	//cout << "Colliding" << endl;
	if (CircleCollisionTest(//if there is a collision
		player->GetX() + player->GetDX(), player->GetY() + player->GetDY(),
		sprite->GetX(), sprite->GetY(),
		player->GetRadius(), sprite->GetRadius()

	)) {
		if (dynamic_cast<Clock*>(sprite) != nullptr)
		{
			m_iTimer += 90;//80
			m_iScore++;
			Mix_PlayChannel(-1, m_mFire, 0);
		
			return true;
		}
		else {
			if ((player->GetY() + (player->GetRadius())) < sprite->GetY())//if the player is on top of block
			{
				//player->SetDY(0);//stop player from moving vertically
				//cout << "1" << endl;
				if (!(player->GetGrouded())) {
					player->SetFoot();
				}


				if (m_intDir == 1) {
					m_intDir = 0;
					player->SetIdle();
					return true;
				}
				else { player->SetDY(0); }



			}
			else if ((player->GetX() - sprite->GetX()) > 0 && player->GetDX() < 0)//if player is on the right side of the block and is moving left
			{
				//player->SetDX(0); //stop player from going through left block
				//cout << "2" << endl;
				if (m_intDir == 2) {
					m_intDir = 0;
					return true;
				}
				else {
					player->SetDX(0);

				}



			}
			else if ((player->GetX() - sprite->GetX()) < 0 && player->GetDX() > 0)//if player is on the left side of the block and is moving right
			{
				//layer->SetDX(0);//stop player from going through right block
				//cout << "3" << endl;
				if (m_intDir == 3) {
					m_intDir = 0;
					return true;
				}
				else {
					player->SetDX(0);
				}



			}
		}
		
	}
	return false;
}


//</ksmod>

	

void Game::CheckCollision() {}


void Game::Update()
{
	if (m_iState == 0) {
	
	}
	else if(m_iState==2){
	//	if (m_pPlayer) delete m_pPlayer;

	}
	else if (m_iState == 1) {
		if (this->m_iTimer <= 0) {
				m_pPlayer->SetDestroy(true);
				if (m_iDeath > 7) {
					m_iState = 2;
				    Mix_Pause(15);
				}

			else {
				Mix_PlayChannel(15, m_mEngines, -1);
					m_pPlayer->Animate();
					m_pPlayer->UpdateAni();
				
				m_iDeath++;
				
			}
			return;
		}
		else {

			int l_intPDY; //This will hold the players queued Y movement
			int l_intPDX;//This will hold the players queued X movement

			if (m_pPlayer) m_pPlayer->Update();
			m_pPlayer->SetDY(-4);


			for (int i = 0; i < (int)m_vBlocks.size(); i++) //check collisions with blocks
			{
				if (!(m_vBlocks[i]->getDestroy())) {
					if (BlockCollisionCheck(m_pPlayer, m_vBlocks[i])) {
						
						m_vBlocks[i]->setDestroy(true);

					}
				}
			}


			l_intPDY = m_pPlayer->GetDY(); //get the change in coordinates

			if (m_pPlayer->GetX() + m_pPlayer->GetDX() > m_iWidth - 10)
				m_pPlayer->SetDX(0);
			if (m_pPlayer->GetX() + m_pPlayer->GetDX() < 10)
				m_pPlayer->SetDX(0);

			l_intPDX = m_pPlayer->GetDX();



			if (l_intPDY != 0) {//if the player can move vertically
				this->m_backGround->ScrollUp(l_intPDY / 2); //scroll the background by half the player movement
				for (int i = 0; i < (int)m_vBlocks.size(); i++) {//move the blocks up
					m_vBlocks[i]->IncrY(l_intPDY * 2);
					m_vBlocks[i]->Update();
				}
				for (int i = 0; i < (int)m_vClocks.size(); i++) {//move the clocks up
					m_vClocks[i]->IncrY(l_intPDY * 2);
					m_vClocks[i]->Update();
				}


			}

			m_pPlayer->SetX(m_pPlayer->GetX() + m_pPlayer->GetDX());  //set the players new m_x value. This doesnt have to be done with m_y because the player sprite doesnt actually move vertically.
			m_pPlayer->UpdateDst();


			for (int i = 0; i < (int)m_vBlocks.size(); i++) {//animate destroyed block
				if (m_vBlocks[i]->getDestroy() && (!(m_vBlocks[i]->IFrameMaxed()))) {
					if (dynamic_cast<Block*>(m_vBlocks[i]) != nullptr)
						((Block*)m_vBlocks[i])->Animate();//jump
					if (dynamic_cast<Clock*>(m_vBlocks[i]) != nullptr)
						((Clock*)m_vBlocks[i])->Animate();//jump
				}
			}
			//for (int i = 0; i < (int)m_vClocks.size(); i++) {//animated destroyed block
			//	if (m_vClocks[i]->getDestroy() && (!(m_vClocks[i]->IFrameMaxed())))
			//		m_vClocks[i]->Animate();
			//}

			m_intDir = 0;

			this->m_iTimer--;


			if (m_vBlocks.back()->GetY() < 0) {
				DestroyMapObejcts();
				AddMapObjects();

			}



		}

	}
}

void Game::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				if (m_pPlayer && !Mix_Playing(15))
					//
				break;
			case SDLK_s:
				m_intDir = 1;
				break;
			case SDLK_a:
				m_intDir = 2;
				break;
			case SDLK_d:
				m_intDir = 3;
				break;
			case SDLK_x:
				if (m_iState == 0) {
					Clean();
					exit(0);
				}
				break;
			case SDLK_RETURN:
				if (m_iState == 0) {
					m_iState = 1;
					AddMapObjects();
				
				}
				if (m_iState == 2) {
					Restart(m_iWidth, m_iHeight);
					m_iState = 0;
					
				}
				    
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				if (m_pPlayer)
				{
					//Mix_FadeOutChannel(15, 250);
					m_pPlayer->SetIdle();
				}
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT && m_pPlayer)
			{ 
				
				//m_pPlayer->SpawnBullet();
			}
			break;
		}
	}
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen to the draw color.
	
	if (m_iState == 0) {
		m_pSplash->setSrcY(0);
		m_pSplash->Render();
		


	}
	else if (m_iState == 2) {
		//
		m_pSplash->setSrcY(150);
		m_pSplash->Render();
		m_sScore = "Score: " + to_string(m_iScore);
		RenderFont((1), m_sScore.c_str(), (m_iWidth/2)-40, (m_iHeight/2)-40);
		
	

	}
	else if (m_iState == 1) {
		
		//<ksmod>
		m_backGround->Render();
			//</ksmod>

			for (int i = 0; i < (int)m_vBlocks.size(); i++)
				m_vBlocks[i]->Render();

			//psmod

			m_pFog->Render();
			//time
			m_sTime = "TIME: " + to_string(m_iTimer / 30);
			m_pClockBack->Render();
			//	RenderFont((1), m_sTime.c_str(), 32, 50);
			RenderFont((1), m_sTime.c_str(), 12, 8);
		
		if (m_pPlayer) m_pPlayer->Render();
	
	}
	SDL_RenderPresent(m_pRenderer); // Draw anew.
}



void Game::RenderFont(bool r, const char* c, int x, int y)
{
	if (r)
	{
		SDL_Color textColor = { 255, 255, 255, 0 }; // White text.
		SDL_Surface* fontSurf = TTF_RenderText_Solid(m_Font, c, textColor);
		SDL_DestroyTexture(m_pFontText); // Need to de-allocate previous font texture.
		m_pFontText = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), fontSurf);
		m_rFontRect = { x, y, (fontSurf->w), (fontSurf->h) };
		SDL_FreeSurface(fontSurf);
	}
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pFontText, 0, &m_rFontRect);
}





void Game::Clean()
{
	cout << "Cleaning game. Bye!" << endl;
	if (m_pPlayer) delete m_pPlayer;

	m_vBlocks.clear();
	m_vBlocks.shrink_to_fit();
	SDL_DestroyTexture(m_pTexture);
	SDL_DestroyTexture(m_pTextureBackdrop);
	SDL_DestroyTexture(m_pTextSplash);
	SDL_DestroyTexture(m_pTextureFg);
	SDL_DestroyTexture(m_pTextureBlock);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	Mix_FreeChunk(m_mFire);
	Mix_FreeChunk(m_mEngines);
	Mix_FreeChunk(m_mMusic);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}