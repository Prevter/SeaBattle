#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Ship.h"

enum HitResult {
	RETRY,
	MISS,
	HIT,
	SUNK,
};

struct HitData {
	int x, y;
	HitResult result;
};

class PlayerGrid {
public:

	// it contains:
	// 1 big ship
	// 2 medium ships
	// 3 small ships
	// 4 tiny ships

	// methods:
	// hitTest - returns true if hit any ship
	// draw - draws all ships

	PlayerGrid();
	~PlayerGrid();

	bool AddShip(BaseShip& ship); // returns true if valid position
	void FillRandomly(); // fills grid with random ships
	bool ValidateGrid(); // returns true if grid is valid
	void Reset();
	
	void Draw(sf::RenderTarget& render);
	HitResult Hit(int x, int y, bool miss = false);
	HitResult WasHit(int x, int y);

	std::vector<BaseShip*> ships;

	bool isHidden = false;
	std::vector<HitData> hits;
};