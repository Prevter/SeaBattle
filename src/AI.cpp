#include "AI.h"

AI::AI(PlayerGrid* grid) : grid(grid) {
}

AI::~AI() {
}

void AI::Update(float dt) {
	if (finishedTurn) {
		timeSinceLastHit = 0;
		return;
	}
	
	timeSinceLastHit += dt;
	if (timeSinceLastHit > THINKING_TIME) {
		timeSinceLastHit = 0;
		TryTurn();
	}
}

void AI::TryTurn() {
	while (1) {
		sf::Vector2i cell;
		if (foundShip) {
			cell.x = lastSuccessHit.x;
			cell.y = lastSuccessHit.y;

			// check if we have empty cells around
			if (lastDirection == -1) {
				if (!grid->WasHit(cell.x, cell.y + 1)) {
					cell.y += 1;
					lastDirection = 0;
				}
				else if (!grid->WasHit(cell.x + 1, cell.y)) {
					cell.x += 1;
					lastDirection = 1;
				}
				else if (!grid->WasHit(cell.x, cell.y - 1)) {
					cell.y -= 1;
					lastDirection = 2;
				}
				else if (!grid->WasHit(cell.x - 1, cell.y)) {
					cell.x -= 1;
					lastDirection = 3;
				}
				else {
					// no free space, so just pick randomly
					foundShip = false;
					lastDirection = -1;
					continue;
				}
			}
			else {
				switch (lastDirection)
				{
				case 0:
					cell.y += 1;
					break;
				case 1:
					cell.x += 1;
					break;
				case 2:
					cell.y -= 1;
					break;
				case 3:
					cell.x -= 1;
					break;
				}

				if (grid->WasHit(cell.x, cell.y)) {
					foundShip = false;
					lastDirection = -1;
					continue;
				}
			}
			
			// we are out of bounds, try again
			if (cell.x < 0 || cell.x >= 10 || cell.y < 0 || cell.y >= 10) {
				continue;
			}
		}
		else {
			cell.x = rand() % 10;
			cell.y = rand() % 10;
		}
		
		HitResult hit = grid->Hit(cell.x, cell.y);

		if (hit == HitResult::HIT) {
			foundShip = true;
			lastSuccessHit.x = cell.x;
			lastSuccessHit.y = cell.y;
		}
		else if (hit == HitResult::SUNK) {
			foundShip = false;
			lastDirection = -1;
		}
		else if (hit == HitResult::RETRY) {
			continue;
		}
		else {
			// Missed
			timeSinceLastHit = 0;
			finishedTurn = true;
			lastDirection = -1;
		}
		break;
	}
}
