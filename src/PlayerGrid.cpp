#include "PlayerGrid.h"
#include <math.h>

PlayerGrid::PlayerGrid() {
	ships.push_back(new LargeShip(-10,0,0));
	ships.push_back(new MediumShip(-10,0,0));
	ships.push_back(new MediumShip(-100,0,0));
	ships.push_back(new SmallShip(-10,0,0));
	ships.push_back(new SmallShip(-10,0,0));
	ships.push_back(new SmallShip(-10,0,0));
	ships.push_back(new TinyShip(-10,0));
	ships.push_back(new TinyShip(-10,0));
	ships.push_back(new TinyShip(-10,0));
	ships.push_back(new TinyShip(-10,0));
	FillRandomly();
}

PlayerGrid::~PlayerGrid() {
	for (auto ship : ships) {
		delete ship;
	}
}

bool PlayerGrid::AddShip(BaseShip& ship) {
	if (std::find(ships.begin(), ships.end(), &ship) != ships.end()) {
		return false;
	}	
	
	ships.push_back(&ship);
	return true;
}

void PlayerGrid::FillRandomly() {
	std::vector<BaseShip*> tempShips = ships;
	ships.clear();
	
	for (auto& ship : tempShips) {
		AddShip(*ship);

		do {
			switch (ship->GetType())
			{
			case ShipType::TINY:
			{
				TinyShip* tiny = (TinyShip*)ship;
				tiny->x = rand() % 10;
				tiny->y = rand() % 10;
				break;
			}
			case ShipType::SMALL:
			{
				SmallShip* small = (SmallShip*)ship;
				small->x = rand() % 10;
				small->y = rand() % 10;
				small->side = rand() % 4;
				break;
			}
			case ShipType::MEDIUM:
			{
				MediumShip* medium = (MediumShip*)ship;
				medium->x = rand() % 10;
				medium->y = rand() % 10;
				medium->side = rand() % 4;
				break;
			}
			case ShipType::LARGE:
			{
				LargeShip* large = (LargeShip*)ship;
				large->x = rand() % 10;
				large->y = rand() % 10;
				large->side = rand() % 4;
				break;
			}
			}
		} while (!ValidateGrid());
	}
}

bool PlayerGrid::ValidateGrid() {
	for (auto& ship : ships) {
		// check if the ship is placed outside the grid
		int tiles = ship->GetType();
		// check every tile of the ship
		for (int i = 0; i < tiles; i++) {
			sf::Vector2u tile = ship->GetTile(i);

			// check if the tile is outside the grid
			if (tile.x < 0 || tile.x >= 10 || tile.y < 0 || tile.y >= 10) {
				return false;
			}

			// check if the tile is occupied by another ship
			for (auto& otherShip : ships) {
				if (otherShip == ship) {
					continue;
				}

				for (int j = 0; j < otherShip->GetType(); j++) {
					sf::Vector2u otherTile = otherShip->GetTile(j);
					if (tile.x == otherTile.x && tile.y == otherTile.y) {
						return false;
					}
					if (abs(((int)tile.x) - ((int)otherTile.x)) <= 1 && abs(((int)tile.y) - ((int)otherTile.y)) <= 1) {
						return false;
					}
				}
			}
		}
	}

	return true;
}

void PlayerGrid::Draw(sf::RenderTarget& render) {
	for (auto& ship : ships) {
		if (!isHidden || ship->IsDestroyed()) {
			ship->Draw(render);
		}
	}

	for (auto& hit : hits) {
		if (hit.result == HitResult::MISS) {
			// if miss, draw small circle
			int radius = 4;
			sf::CircleShape circle(radius);
			circle.setFillColor(sf::Color::Black);
			sf::Vector2f pos = sf::Vector2f(hit.x * TILE_SIZE + TILE_SIZE / 2 - radius, hit.y * TILE_SIZE + TILE_SIZE / 2 - radius);
			circle.setPosition(pos);
			render.draw(circle);
		}
		else {
			// else draw X
			sf::Vector2f pos = sf::Vector2f(hit.x * TILE_SIZE + TILE_SIZE / 2, hit.y * TILE_SIZE + TILE_SIZE / 2);
			sf::Vector2f size = sf::Vector2f(TILE_SIZE / 2, 2);
			sf::RectangleShape rect(size);
			rect.setOrigin(sf::Vector2f(TILE_SIZE / 4, 1));
			rect.setFillColor(sf::Color::Green);
			rect.setPosition(pos);
			rect.rotate(sf::degrees(45));
			render.draw(rect);
			rect.rotate(sf::degrees(90));
			render.draw(rect);
		}
		
	}
}

HitResult PlayerGrid::Hit(int x, int y, bool miss) {
	// check if the tile is already hit
	for (auto& hit : hits) {
		if (hit.x == x && hit.y == y) {
			return HitResult::RETRY;
		}
	}

	// add hit to the list
	HitData hit {x, y, HitResult::MISS};
	
	// check if the tile is occupied by a ship
	for (auto& ship : ships) {
		if (ship->TestFire(x, y)) {
			// check if the ship is destroyed
			if (ship->IsDestroyed()) {
				// hit all tiles nearby
				if (!miss) {
					for (int i = 0; i < ship->GetType(); i++) {
						sf::Vector2u tile = ship->GetTile(i);
						for (int x = -1; x <= 1; x++) {
							for (int y = -1; y <= 1; y++) {
								if (x == 0 && y == 0) continue;
								Hit(tile.x + x, tile.y + y, true);
							}
						}
					}
				}
				hit.result = HitResult::SUNK;
				break;
			}
			hit.result = HitResult::HIT;
			break;
		}
	}

	hits.push_back(hit);
	return hit.result;
}

void PlayerGrid::Reset() {
	hits.clear();
	for (auto& ship : ships) {
		switch (ship->GetType())
		{
		case ShipType::TINY:
		{
			TinyShip* tiny = (TinyShip*)ship;
			tiny->isAlive = true;
			break;
		}
		case ShipType::SMALL:
		{
			SmallShip* small = (SmallShip*)ship;
			small->isAlive1 = true;
			small->isAlive2 = true;
			break;
		}
		case ShipType::MEDIUM:
		{
			MediumShip* medium = (MediumShip*)ship;
			medium->isAlive1 = true;
			medium->isAlive2 = true;
			medium->isAlive3 = true;
			break;
		}
		case ShipType::LARGE:
		{
			LargeShip* large = (LargeShip*)ship;
			large->isAlive1 = true;
			large->isAlive2 = true;
			large->isAlive3 = true;
			large->isAlive4 = true;
			break;
		}
		}
	}
	FillRandomly();
}

HitResult PlayerGrid::WasHit(int x, int y) {
	// if out of bounds, return MISS
	if (x < 0 || x >= 10 || y < 0 || y >= 10) {
		return HitResult::MISS;
	}

	for (auto& hit : hits) {
		if (hit.x == x && hit.y == y) {
			return hit.result;
		}
	}
	return HitResult::RETRY;
}