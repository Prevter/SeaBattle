#pragma once
#include <SFML/Graphics.hpp>
#define TILE_SIZE 48

enum ShipType {
	TINY = 1,
	SMALL = 2,
	MEDIUM = 3,
	LARGE = 4,
};

class BaseShip {
public:
	virtual ShipType GetType() = 0;

	virtual void Draw(sf::RenderTarget& render) = 0;
	virtual bool TestFire(int x, int y) = 0;
	virtual sf::Vector2u GetTile(int index) = 0;
	virtual bool IsDestroyed() = 0;
};

class TinyShip : public BaseShip {
public:
	TinyShip(int x, int y);
	~TinyShip();
	void Draw(sf::RenderTarget& render) override;
	bool TestFire(int x, int y) override;

	ShipType GetType() override { return ShipType::TINY; }
	sf::Vector2u GetTile(int index) override { return sf::Vector2u(x, y); }
	bool IsDestroyed() override { return !isAlive; }
	
	int x, y;
	bool isAlive;

	sf::Texture texture;
	sf::Texture textureDestroyed;
};

class SmallShip : public BaseShip {
public:
	SmallShip(int x, int y, int side);
	~SmallShip();
	void Draw(sf::RenderTarget& render) override;
	bool TestFire(int x, int y) override;

	ShipType GetType() override { return ShipType::SMALL; }

	sf::Vector2u GetTile(int index) override;
	bool IsDestroyed() override { return !isAlive1 && !isAlive2; }
	
	int x, y;
	int side;
	bool isAlive1, isAlive2;

	sf::Texture texture_1, texture_2;
	sf::Texture textureDestroyed_1, textureDestroyed_2;
};

class MediumShip : public BaseShip {
public:
	MediumShip(int x, int y, int side);
	~MediumShip();
	void Draw(sf::RenderTarget& render) override;
	bool TestFire(int x, int y) override;

	ShipType GetType() override { return ShipType::MEDIUM; }

	sf::Vector2u GetTile(int index) override;
	bool IsDestroyed() override { return !isAlive1 && !isAlive2 && !isAlive3; }

	int x, y;
	int side;
	bool isAlive1, isAlive2, isAlive3;

	sf::Texture texture_1, texture_2, texture_3;
	sf::Texture textureDestroyed_1, textureDestroyed_2, textureDestroyed_3;
};

class LargeShip : public BaseShip {
public:
	LargeShip(int x, int y, int side);
	~LargeShip();
	void Draw(sf::RenderTarget& render) override;
	bool TestFire(int x, int y) override;

	ShipType GetType() override { return ShipType::LARGE; }

	sf::Vector2u GetTile(int index) override;
	bool IsDestroyed() override { return !isAlive1 && !isAlive2 && !isAlive3 && !isAlive4; }

	int x, y;
	int side;
	bool isAlive1, isAlive2, isAlive3, isAlive4;

	sf::Texture texture_1, texture_2, texture_3, texture_4;
	sf::Texture textureDestroyed_1, textureDestroyed_2, textureDestroyed_3, textureDestroyed_4;
};