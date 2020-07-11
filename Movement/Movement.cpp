#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <list>
#include <iostream>

class sSpaceObj
{
public:
	sSpaceObj()
	{
		x = 8;// x and y are pos on screen
		y = 8;
		dx = 1;// dx and dy are speed in direction
		dy = 5;
		asteroidRadius = 3;// sice
		angle = 0;// the frount diretion 
	};
public:
	float x; 
	float y; 
	float dx;
	float dy;
	float angle;
	int asteroidRadius;

	std::vector<std::pair<float, float>> asteriodModel;
};

class sPlayer
{
public:
	sPlayer()
	{
		x = 10;
		y = 10;
		dx = 0;
		dy = 0;
		angle = 0;
	};
public:
	float x;
	float y;
	float dx;
	float dy;
	float angle;

	std::vector<std::pair<float, float>> ShipModel;// creates a vector list to plot the points of the ship
};

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{

#pragma region Vars

	sSpaceObj asteriod;
	sPlayer player;

	float posX;
	float posY;

#pragma endregion
public:
	Example()
	{
		// Name you application
		sAppName = "Example";
	}
	
public:
	bool OnUserCreate() override
	{		
		//player ship inlz plots a triangel
		player.ShipModel =
		{
			{ 0.0f, -5.0f},
			{-2.0f, 4.0f},
			{2.0f, 4.0f}
		};
		//player inlz sets the pos to center screen and sets speed and angel to 0
		player.x = ScreenWidth() / 2;
		player.y = ScreenHeight() / 2;
		player.dx = 0;
		player.dy = 0;
		player.angle = 0;

		//asteriod iniz creates a 20 sided shape and sets the radius
		int verts = 20;
		for (int i = 0; i < verts; i++)
		{
			asteriod.asteroidRadius = 5.0f;
			float a = ((float)i / (float)verts) * 6.28318f;
			asteriod.asteriodModel.push_back(std::make_pair(asteriod.asteroidRadius * sinf(a), asteriod.asteroidRadius * cosf(a)));

		}
		

		// Called once at the start, so create things here
		return true;
	}



	bool OnUserUpdate(float fElapsedTime) override
	{		
		Clear(olc::BLACK);// clear screen to black

		int sWidth = ScreenWidth(); int sHight = ScreenHeight();//grabs the screen hight and width		

		posX = asteriod.x += asteriod.dx * fElapsedTime; // sets posx to be the pos * speed * time
		posY = asteriod.y += asteriod.dy * fElapsedTime;// sets posx to be the pos * speed * time
		WrapCoordinates(asteriod.x, asteriod.y, asteriod.x, asteriod.y);//wraps the asteriod around screen

		if (GetKey(olc::LEFT).bHeld)//turn left
			player.angle -= 5.0f * fElapsedTime;
		if (GetKey(olc::RIGHT).bHeld)//turn right
			player.angle += 5.0f * fElapsedTime;

		if (GetKey(olc::UP).bHeld)// move forword
		{
			player.dx += sin(player.angle) * 20.0f * fElapsedTime;
			player.dy += -cos(player.angle) * 20.0f * fElapsedTime;
		}

		player.x += player.dx * fElapsedTime;
		player.y += player.dy * fElapsedTime;

		WrapCoordinates(player.x, player.y, player.x, player.y);//wraps player around screen

		DrawWireFram(player.ShipModel, player.x, player.y, player.angle);//player

		DrawWireFram(asteriod.asteriodModel, asteriod.x, asteriod.y, asteriod.angle, asteriod.asteroidRadius);//asteriod

		
		return true;
	}

	void DrawWireFram(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, olc::Pixel p = olc::WHITE)
	{

		//pair.first = x coodinates
		//pair.second = y coodinates

		//create translated modle vector of coodinates pairs
		std::vector<std::pair<float, float>> vecTransCoordinates;//new vector
		int verts = vecModelCoordinates.size();// sets verts to the size of vecModelCoordinates
		vecTransCoordinates.resize(verts);//resizes vecTransCoordinates to fit all the verts

		//rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);// newX = oldX * Cos(r) - oldY * sin(r) ex. -1.92 = 2 * cos(20)=(0.408) - 3 * sin(20)=(0.912)
			vecTransCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);//basicly same as above
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransCoordinates[i].first = vecTransCoordinates[i].first * s;
			vecTransCoordinates[i].second = vecTransCoordinates[i].second * s;
		}

		//translate
		for (int i = 0; i < verts; i++)
		{
			vecTransCoordinates[i].first = vecTransCoordinates[i].first + x;
			vecTransCoordinates[i].second = vecTransCoordinates[i].second + y;
		}
		// Draw Ship Lines
		for (int i = 0; i < verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine(vecTransCoordinates[i % verts].first, vecTransCoordinates[i % verts].second, 
					 vecTransCoordinates[j % verts].first, vecTransCoordinates[j % verts].second, p);
		}

	}

	void WrapCoordinates(float ix, float iy, float& ox, float& oy)
	{
		ox = ix;
		oy = iy;
		if (ix < 0.0f) ox = ix + (float)ScreenWidth();
		if (ix > (float)ScreenWidth()) ox = ix - (float)ScreenWidth();
		if (iy < 0.0f) oy = iy + (float)ScreenHeight();
		if (iy > (float)ScreenHeight()) oy = iy - (float)ScreenHeight();

	}

	virtual bool Draw(int32_t x, int32_t y, olc::Pixel p = olc::WHITE) override
	{
		float fx, fy;
		WrapCoordinates((float)x, (float)y, fx, fy);
		return olc::PixelGameEngine::Draw(fx, fy, p);
	}
	
};

int main()
{
	Example demo;
	if (demo.Construct(233, 120, 8, 8))
		demo.Start();
	return 0;
}