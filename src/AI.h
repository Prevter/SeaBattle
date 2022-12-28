#pragma once
#include "PlayerGrid.h"
#define THINKING_TIME 1.0f

class AI {
public:
	AI(PlayerGrid* grid);
	~AI();

	void Update(float dt);
	void TryTurn();
	
	bool foundShip = false;
	HitData lastSuccessHit;
	int lastDirection = 0;
	PlayerGrid* grid;

	float timeSinceLastHit = 0;
	bool finishedTurn = true;
};