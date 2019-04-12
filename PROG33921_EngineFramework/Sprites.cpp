#include <algorithm> // For min/max.
#include <cmath>	 // For cos/sin.
#include "Game.h"
#include "Sprites.h"
using namespace std;

Sprite::Sprite(double x, double y, double a, int fm, int sm, double spd):
	m_X(x), m_Y(y), m_dAngle(a), m_iFrameMax(fm), m_iSpriteMax(sm), m_dSpeed(spd) {}

void Sprite::Animate()
{
	m_iFrame++;
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
			m_iSprite = 0;
	}
}

void Sprite::SetDX(int p_intDX) {
	this->m_DX = p_intDX;

}
void Sprite::SetDY(int p_intDY) {
	this->m_DY = p_intDY;
}

void Sprite::Render()
{
	SDL_RenderCopyEx(Game::Instance()->GetRenderer(), Game::Instance()->GetTexture(),
		&m_rSrc, &m_rDst, m_dAngle, nullptr, SDL_FLIP_NONE);
}

void Sprite::UpdateDst()
{
	m_rDst.x = (int)(m_X- m_rDst.w / 2);
	m_rDst.y = (int)(m_Y - m_rDst.h / 2);
}


Player::Player(double x, double y):Sprite(x-50, y-50, 0, 3, 5, 1.0f)
{
	m_rSrc = { 0, 0, 60, 60 };
	m_rDst = { (int)(m_X - 150), (int)(m_Y - 150), 50, 50 };
	m_dRadius = 25;
	
}

Player::~Player()
{
	

}

void Player::Update()
{
	m_DX = 0;
	m_DY = 0;
	if (Game::Instance()->KeyDown(SDL_SCANCODE_A)) {
	//	m_iFrameMax = 5;
		if(m_blnGrouded)
			m_rSrc.y = 60;
		m_DX = -4;
		flipped = true;
	}
		
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_D)) {
		//m_iFrameMax = 5;
		if (m_blnGrouded)
			m_rSrc.y = 60;
		m_DX = 4;
		flipped = false;
	}


	
	else if(Game::Instance()->KeyDown(SDL_SCANCODE_S))
	{ 
		m_rSrc.y = 120;
		
	}

	else {
		this->SetIdle();
	}//hjgh
	
	this->Animate();
	this->UpdateAni(); // Update animation.
		
	
}


void Player::SetDestroy(bool bln) {
	this->m_blnDestroy = bln;
	//SetFoot();
	this->m_rSrc.y = 180;
	//this->m_rSrc.x = 0;
	//this->m_iFrame = 5;
}
void Player::SetIdle()
{
	
	m_rSrc.y = 120;
	this->SetGrouded(false);

}
void Player::SetFoot()
{
	m_rSrc.y = 0;
	this->SetGrouded(true);

	
}


void Player::Render()
{
	/*for (int i = 0; i < (int)m_vBullets.size(); i++)
		m_vBullets[i]->Render();
		*/
	//this->Sprite::Render(); // Invoking base class Render().
	if (this->flipped) {
		SDL_RenderCopyEx(Game::Instance()->GetRenderer(), Game::Instance()->GetTexture(),
			&m_rSrc, &m_rDst, m_dAngle, nullptr, SDL_FLIP_HORIZONTAL);
	}
	else {
		SDL_RenderCopyEx(Game::Instance()->GetRenderer(), Game::Instance()->GetTexture(),
			&m_rSrc, &m_rDst, m_dAngle, nullptr, SDL_FLIP_NONE);
	
	}
}



/*=======================================================================================
									OUR METHOD ADDITIONS
=========================================================================================*/
//start mod ks001
SpriteEx::SpriteEx(double x, double y, double a, int fm, int sm, double spd,
	vector<int> p_vSrc, vector<int> p_vDst, SDL_Texture* p_pTexture)
	:Sprite(x, y, a, fm, sm, spd) {
	this->m_pTexture = p_pTexture;

	m_rSrc = { p_vSrc.at(0),p_vSrc.at(1),p_vSrc.at(2),p_vSrc.at(3) };
	m_rDst = { p_vDst.at(0),p_vDst.at(1),p_vDst.at(2),p_vDst.at(3) };
	m_X = p_vDst.at(0);
	m_Y = p_vDst.at(1);



}
void SpriteEx::Render()
{
	SDL_RenderCopyEx(Game::Instance()->GetRenderer(), this->m_pTexture,
		&m_rSrc, &m_rDst, m_dAngle, nullptr, SDL_FLIP_NONE);
}

void SpriteEx::IncrY(int y)
{
	this->m_Y= (this->m_Y)+y;
}

void Sprite::SetX(int x)
{
	this->m_X =  x;
}

void Sprite::SetY(int y)
{
	this->m_Y = y;
}

MapSprite::MapSprite(double x, double y, double a, int fm, int sm, double spd,
	vector<int> p_vSrc, vector<int> p_vDst, SDL_Texture* p_pTexture) :
	SpriteEx(x, y, a, fm, sm, spd, p_vSrc, p_vDst, p_pTexture)
{
	m_pTexture = p_pTexture;
}

void MapSprite::Update()
{
	if (m_iFrameMax>1)
		this->Animate();
	m_rSrc.x = m_rSrc.w * m_iSprite;
}

//end mod ks001

//psmod

Block::Block(double x, double y, double a, double r, 
	vector<int> m_vSrc, vector<int>m_vDst, SDL_Texture* m_pTexture)
	:MapSprite(x, y, a, 0, 0, 0.0f, m_vSrc, m_vDst, m_pTexture)
{
	m_dRadius = 16;//14

}

Clock::Clock(double x, double y, double a, double r,
	vector<int> m_vSrc, vector<int>m_vDst, SDL_Texture* m_pTexture)
	:MapSprite(x, y, a, 0, 0, 0.0f, m_vSrc, m_vDst, m_pTexture)
{
	m_dRadius = 16;//14

}


void Block::Animate() {
	//if (this->m_blnDestroy == true && this->m_iFrame> this->m_iFrameMax) {
		m_iFrame++;
		this->m_rSrc.y = 40;
		if (m_iFrame == m_iFrameMax)
		{
			m_iFrame = 0;
			m_iSprite++;
			if (m_iSprite == m_iSpriteMax) {
				this->m_rSrc.y = 0;
				this->m_rSrc.x = 80;
			}
				
		}
		
		m_rSrc.x = m_rSrc.w * m_iSprite;

	


}
void Clock::Animate() {
	//if (this->m_blnDestroy == true && this->m_iFrame> this->m_iFrameMax) {
	m_iFrame++;
	this->m_rSrc.y = 80;

	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax) {
			this->m_rSrc.y = 0;
			this->m_rSrc.x = 80;
		}

	}

	m_rSrc.x = m_rSrc.w * m_iSprite;




}

void Block::Update()
{


	this->UpdateDst();
	//m_dAngle += m_dRotSpeed;
}

void Clock::Update()
{


	this->UpdateDst();
	//m_dAngle += m_dRotSpeed;
}


//psmod


//<ksmod>
VScrollBack::VScrollBack(
  SDL_Texture* p_pTexture, int p_intScrnHt, int p_intScrnWt, int p_intSrcX,int p_intSrcY) {

	this->m_pBack1 = new MapSprite(0,0,0,0,0,0,
		{0,0,p_intSrcX,p_intSrcY },
		{ 0,0,p_intScrnWt,p_intScrnHt},
		p_pTexture);

	this->m_pBack2 = new MapSprite(0, 0, 0, 0, 0, 0,
		{ 0,0,p_intSrcX,p_intSrcY },
		{ 0,(0- p_intScrnHt),p_intScrnWt,p_intScrnHt },
		p_pTexture);
	   m_pBack2->SetY((p_intScrnHt));
	  
		
	this->m_intScrnHt = p_intScrnHt;
	this->m_intScrnWt = p_intScrnWt;

	
}

void VScrollBack::ScrollUp(int p_intY) {
	m_pBack1->IncrY(p_intY);
	m_pBack1->SetX( (this->m_intScrnWt) / 2);
	m_pBack1->UpdateDst();
	
	m_pBack2->IncrY(p_intY);
	m_pBack2->UpdateDst();
	m_pBack2->SetX((this->m_intScrnWt) / 2);

	//Resets backgrounds to bottom
	if (m_pBack1->GetY() <= (0 - (m_intScrnHt/2))) {
		m_pBack1->IncrY(m_intScrnHt*2);
		m_pBack1->SetX((this->m_intScrnWt/2));
		m_pBack1->UpdateDst();

	}
	if (m_pBack2->GetY() <= (0 - (m_intScrnHt / 2))) {
		m_pBack2->IncrY(m_intScrnHt * 2);
		m_pBack2->SetX((this->m_intScrnWt / 2));
		m_pBack2->UpdateDst();

	}



}

void VScrollBack::Render() {
	this->m_pBack1->Render();
	this->m_pBack2->Render();
}

//<ksmod>