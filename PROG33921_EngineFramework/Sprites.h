#pragma once
#include <vector>
#include "SDL.h"
using namespace std;

class Sprite
{
protected:
	int m_iSprite = 0,	// Which sprite to display for animation.
		m_iSpriteMax,	// How many sprites total.
		m_iFrame = 0,	// Frame counter.
		m_iFrameMax;	// Number of frames per sprite.
//	SDL_Point m_pos;    // Could use a point for x and y.  
	double m_X,		// X coordinate. 
		m_Y,		// Y coordinate.
		m_DX,		// Change in X coordinate.
		m_DY,		// Change in Y coordinate.
		m_dAngle,   // Angle to display sprite in degrees.
		m_dSpeed,	// Speed in pixels to move per change.
		m_dRadius;	// Radius of sprite's circle bound.
	SDL_Rect m_rSrc;	// Rectangle for source pixels.
	SDL_Rect m_rDst;	// Rectangle for destination window.
//	SDL_Texture* m_pTexture;

public:
	Sprite(double, double, double, int, int, double); // Constructor.
	SDL_Rect GetSrc() { return m_rSrc; }
	SDL_Rect GetDst() { return m_rDst; }
	virtual void Update() = 0;
	void Animate();
	virtual void Render();
	void UpdateDst();
	double GetRadius() { return m_dRadius; }
	double GetX() { return m_X; }
	double GetY() { return m_Y; }
	double GetDX() { return m_DX; }
	double GetDY() { return m_DY; }
	void SetDX(int);
	void  SetDY(int);
	void SetX(int);//set m_x
	void SetY(int);//set m_ys
};






class Player : public Sprite
{
private:
	double m_dVelX = 0,
		m_dVelY = 0, 
		m_dVelMax = 10;
	    bool flipped = false;
		bool m_blnGrouded;
		bool m_blnDestroy = false;

public:
	Player(double, double);
	~Player();
	void Update();
	void Render();
	void SetIdle();
	void SetFoot();

	bool GetGrouded() {
		return m_blnGrouded;
	}
	void SetGrouded(bool bln) {
		this->m_blnGrouded=bln;
	}
	void SetDestroy(bool bln);
	bool GetDestroy() { return m_blnDestroy; }
	void UpdateAni() { m_rSrc.x = m_rSrc.w * m_iSprite; }// Update animation.

};



/*=======================================================================================
								OUR CLASS ADDITIONS   
=========================================================================================

*/
//<ksmod>

/* SpriteEx class allows you to set m_rSrc, m_rDst, and a reference to a SDL_texture
   on creation*/
class SpriteEx :public Sprite {

protected:
	//vector<int> m_vSrc;//used to load m_rSrc
	//vector<int> m_vDst;//used to load m_rDst
	SDL_Texture * m_pTexture;
public:
	SpriteEx(double, double, double, int, int, double,
		vector<int>,//used to load m_rSrc
		vector<int>,//used to load m_rDst
		SDL_Texture*);//used to set render texture more dynamically
	void  Render();
	void IncrY(int);//Increment m_y

};


/*A generic sprite blueprint. Use this to create sprites you want to place on the screen
such as trees, rocks, clouds, or a background image.
*/
class MapSprite :public SpriteEx {
private:
	bool m_blnDestroy = false;


public:
	MapSprite(double, double, double, int, int, double,
		vector<int>,
		vector<int>,
		SDL_Texture*);
	void Update();
	bool getDestroy() {
		return m_blnDestroy;
	}
	bool IFrameMaxed() {
		if (m_iFrame < m_iFrameMax)return false;
		else return true;
	}
	void setDestroy(bool bln) {
		m_blnDestroy = bln;
		//this->m_rSrc.y = 40;
		this->m_iFrameMax = 3;
	}
	void setSrcY(int y) { this->m_rSrc.y = y; }

};

class VScrollBack {//vertical scroll class. Loops two mapsprites
private:
	MapSprite* m_pBack1;
	MapSprite* m_pBack2;
	int m_intScrnHt;//Screen height
	int m_intScrnWt;//Screen width

public:
	VScrollBack(
		SDL_Texture*,
		int,int,int,int);
	void ScrollUp(int);//scrolls map up
	void Render();

};

//</ksmod>

//psmod

class Block : public MapSprite
{
private:
	//double m_dRotSpeed; // Rotation speed.
public:
	Block(double, double, double, double, vector<int>, vector<int>, SDL_Texture*);
	void Update();
	
	void Animate();

	

};


class Clock : public MapSprite
{
private:
	bool m_blnDestroy = false;
public:
	Clock(double, double, double, double, vector<int>, vector<int>, SDL_Texture*);
	void Update();
	void Animate();


};

//psmod