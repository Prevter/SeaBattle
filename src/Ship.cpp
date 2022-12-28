#include "Ship.h"

TinyShip::TinyShip(int x, int y) {
	this->x = x;
	this->y = y;
	isAlive = true;
	
	texture.loadFromFile("assets/ship_1.png");
	textureDestroyed.loadFromFile("assets/ship_1_destroyed.png");
}

TinyShip::~TinyShip() {
}

void TinyShip::Draw(sf::RenderTarget& render) {
	sf::Sprite sprite;
	sprite.setTexture(isAlive ? texture : textureDestroyed);
	sprite.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
	render.draw(sprite);
}

bool TinyShip::TestFire(int x, int y) {
	if (this->x == x && this->y == y) {
		isAlive = false;
		return true;
	}
	return false;
}


SmallShip::SmallShip(int x, int y, int side) {
	this->x = x;
	this->y = y;
	this->side = side;
	isAlive1 = true;
	isAlive2 = true;

	texture_1.loadFromFile("assets/ship_2_0.png");
	texture_2.loadFromFile("assets/ship_2_1.png");
	textureDestroyed_1.loadFromFile("assets/ship_2_0_destroyed.png");
	textureDestroyed_2.loadFromFile("assets/ship_2_1_destroyed.png");
}

SmallShip::~SmallShip() {
}

void SmallShip::Draw(sf::RenderTarget& render) {
	sf::Sprite sprite1;
	sprite1.setTexture(isAlive1 ? texture_1 : textureDestroyed_1);
	sprite1.setPosition(sf::Vector2f(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2));
	sprite1.setOrigin(sf::Vector2f(24, 24));
	sprite1.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite1);

	sf::Sprite sprite2;
	sprite2.setTexture(isAlive2 ? texture_2 : textureDestroyed_2);
	sf::Vector2u tile = GetTile(1);
	sprite2.setPosition(sf::Vector2f(tile.x * TILE_SIZE + TILE_SIZE / 2, tile.y * TILE_SIZE + TILE_SIZE / 2));
	sprite2.setOrigin(sf::Vector2f(24, 24));
	sprite2.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite2);
}

bool SmallShip::TestFire(int x, int y) {
	for (int i = 0; i < 2; i++) {
		sf::Vector2u tile = GetTile(i);
		if (tile.x == x && tile.y == y) {
			switch (i)
			{
			case 0:
				isAlive1 = false;
				break;
			case 1:
				isAlive2 = false;
				break;
			}
			return true;
		}
	}
	return false;
}

sf::Vector2u SmallShip::GetTile(int index)
{
	if (index == 0) {
		return sf::Vector2u(x, y);
	}
	else if (index == 1) {
		if (side == 0) {
			return sf::Vector2u(x + 1, y);
		}
		else if (side == 1) {
			return sf::Vector2u(x, y + 1);
		}
		else if (side == 2) {
			return sf::Vector2u(x - 1, y);
		}
		else {
			return sf::Vector2u(x, y - 1);
		}
	}
}

MediumShip::MediumShip(int x, int y, int side) {
	this->x = x;
	this->y = y;
	this->side = side;
	isAlive1 = true;
	isAlive2 = true;
	isAlive3 = true;
	
	texture_1.loadFromFile("assets/ship_3_0.png");
	texture_2.loadFromFile("assets/ship_3_1.png");
	texture_3.loadFromFile("assets/ship_3_2.png");
	textureDestroyed_1.loadFromFile("assets/ship_3_0_destroyed.png");
	textureDestroyed_2.loadFromFile("assets/ship_3_1_destroyed.png");
	textureDestroyed_3.loadFromFile("assets/ship_3_2_destroyed.png");
}

MediumShip::~MediumShip() {
}

void MediumShip::Draw(sf::RenderTarget& render) {
	sf::Sprite sprite1;
	sprite1.setTexture(isAlive1 ? texture_1 : textureDestroyed_1);
	sprite1.setPosition(sf::Vector2f(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2));
	sprite1.setOrigin(sf::Vector2f(24, 24));
	sprite1.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite1);

	sf::Sprite sprite2;
	sprite2.setTexture(isAlive2 ? texture_2 : textureDestroyed_2);
	sf::Vector2u tile = GetTile(1);
	sprite2.setPosition(sf::Vector2f(tile.x * TILE_SIZE + TILE_SIZE / 2, tile.y * TILE_SIZE + TILE_SIZE / 2));
	sprite2.setOrigin(sf::Vector2f(24, 24));
	sprite2.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite2);

	sf::Sprite sprite3;
	sprite3.setTexture(isAlive3 ? texture_3 : textureDestroyed_3);
	tile = GetTile(2);
	sprite3.setPosition(sf::Vector2f(tile.x * TILE_SIZE + TILE_SIZE / 2, tile.y * TILE_SIZE + TILE_SIZE / 2));
	sprite3.setOrigin(sf::Vector2f(24, 24));
	sprite3.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite3);
}

bool MediumShip::TestFire(int x, int y) {
	for (int i = 0; i < 3; i++) {
		sf::Vector2u tile = GetTile(i);
		if (tile.x == x && tile.y == y) {
			switch (i)
			{
			case 0:
				isAlive1 = false;
				break;
			case 1:
				isAlive2 = false;
				break;
			case 2:
				isAlive3 = false;
				break;
			}
			return true;
		}
	}
	return false;
}

sf::Vector2u MediumShip::GetTile(int index)
{
	if (index == 0) {
		return sf::Vector2u(x, y);
	}
	else if (index == 1) {
		if (side == 0) {
			return sf::Vector2u(x + 1, y);
		}
		else if (side == 1) {
			return sf::Vector2u(x, y + 1);
		}
		else if (side == 2) {
			return sf::Vector2u(x - 1, y);
		}
		else {
			return sf::Vector2u(x, y - 1);
		}
	}
	else if (index == 2) {
		if (side == 0) {
			return sf::Vector2u(x + 2, y);
		}
		else if (side == 1) {
			return sf::Vector2u(x, y + 2);
		}
		else if (side == 2) {
			return sf::Vector2u(x - 2, y);
		}
		else {
			return sf::Vector2u(x, y - 2);
		}
	}
}

LargeShip::LargeShip(int x, int y, int side) {
	this->x = x;
	this->y = y;
	this->side = side;
	isAlive1 = true;
	isAlive2 = true;
	isAlive3 = true;
	isAlive4 = true;

	texture_1.loadFromFile("assets/ship_4_0.png");
	texture_2.loadFromFile("assets/ship_4_1.png");
	texture_3.loadFromFile("assets/ship_4_2.png");
	texture_4.loadFromFile("assets/ship_4_3.png");
	textureDestroyed_1.loadFromFile("assets/ship_4_0_destroyed.png");
	textureDestroyed_2.loadFromFile("assets/ship_4_1_destroyed.png");
	textureDestroyed_3.loadFromFile("assets/ship_4_2_destroyed.png");
	textureDestroyed_4.loadFromFile("assets/ship_4_3_destroyed.png");
}

LargeShip::~LargeShip() {
}

void LargeShip::Draw(sf::RenderTarget& render) {
	sf::Sprite sprite1;
	sprite1.setTexture(isAlive1 ? texture_1 : textureDestroyed_1);
	sprite1.setPosition(sf::Vector2f(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2));
	sprite1.setOrigin(sf::Vector2f(24, 24));
	sprite1.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite1);

	sf::Sprite sprite2;
	sprite2.setTexture(isAlive2 ? texture_2 : textureDestroyed_2);
	sf::Vector2u tile = GetTile(1);
	sprite2.setPosition(sf::Vector2f(tile.x * TILE_SIZE + TILE_SIZE / 2, tile.y * TILE_SIZE + TILE_SIZE / 2));
	sprite2.setOrigin(sf::Vector2f(24, 24));
	sprite2.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite2);

	sf::Sprite sprite3;
	sprite3.setTexture(isAlive3 ? texture_3 : textureDestroyed_3);
	tile = GetTile(2);
	sprite3.setPosition(sf::Vector2f(tile.x * TILE_SIZE + TILE_SIZE / 2, tile.y * TILE_SIZE + TILE_SIZE / 2));
	sprite3.setOrigin(sf::Vector2f(24, 24));
	sprite3.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite3);

	sf::Sprite sprite4;
	sprite4.setTexture(isAlive4 ? texture_4 : textureDestroyed_4);
	tile = GetTile(3);
	sprite4.setPosition(sf::Vector2f(tile.x * TILE_SIZE + TILE_SIZE / 2, tile.y * TILE_SIZE + TILE_SIZE / 2));
	sprite4.setOrigin(sf::Vector2f(24, 24));
	sprite4.setRotation(sf::degrees(side * 90.f));
	render.draw(sprite4);
}

bool LargeShip::TestFire(int x, int y) {
	for (int i = 0; i < 4; i++) {
		sf::Vector2u tile = GetTile(i);
		if (tile.x == x && tile.y == y) {
			switch (i)
			{
			case 0:
				isAlive1 = false;
				break;
			case 1:
				isAlive2 = false;
				break;
			case 2:
				isAlive3 = false;
				break;
			case 3:
				isAlive4 = false;
				break;
			}
			return true;
		}
	}
	return false;
}

sf::Vector2u LargeShip::GetTile(int index)
{
	if (index == 0) {
		return sf::Vector2u(x, y);
	}
	else if (index == 1) {
		if (side == 0) {
			return sf::Vector2u(x + 1, y);
		}
		else if (side == 1) {
			return sf::Vector2u(x, y + 1);
		}
		else if (side == 2) {
			return sf::Vector2u(x - 1, y);
		}
		else {
			return sf::Vector2u(x, y - 1);
		}
	}
	else if (index == 2) {
		if (side == 0) {
			return sf::Vector2u(x + 2, y);
		}
		else if (side == 1) {
			return sf::Vector2u(x, y + 2);
		}
		else if (side == 2) {
			return sf::Vector2u(x - 2, y);
		}
		else {
			return sf::Vector2u(x, y - 2);
		}
	}
	else if (index == 3) {
		if (side == 0) {
			return sf::Vector2u(x + 3, y);
		}
		else if (side == 1) {
			return sf::Vector2u(x, y + 3);
		}
		else if (side == 2) {
			return sf::Vector2u(x - 3, y);
		}
		else {
			return sf::Vector2u(x, y - 3);
		}
	}
}

