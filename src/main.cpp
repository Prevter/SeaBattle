#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "Ship.h"
#include "PlayerGrid.h"
#include "AI.h"

#define TILE_SIZE 48
#define GRID_SIZE 10
#define IMG_SIZE TILE_SIZE * GRID_SIZE

void RenderGrid(sf::RenderTarget& render, int rows, int cols, int tileSize) {
	sf::RectangleShape line(sf::Vector2f(tileSize * cols, 2));
	line.setFillColor(sf::Color(255, 255, 255, 160));
	for (int i = 1; i < rows; i++) {
		line.setPosition(sf::Vector2f(0, i * tileSize));
		render.draw(line);
	}

	line.setSize(sf::Vector2f(2, tileSize * rows));
	for (int i = 1; i < cols; i++) {
		line.setPosition(sf::Vector2f(i * tileSize, 0));
		render.draw(line);
	}
}

namespace ImGui {
	void ShipEditor(TinyShip* ship) {
		ImGui::PushID(ship);
		ImGui::DragInt2("Position", &ship->x);
		ImGui::Checkbox("IsAlive", &ship->isAlive);
		ImGui::PopID();
	}
	void ShipEditor(SmallShip* ship) {
		ImGui::PushID(ship);
		ImGui::DragInt2("Position", &ship->x);
		
		const char* items[] = { "Right", "Top", "Left", "Bottom" };
		ImGui::Combo("Side", &ship->side, items, IM_ARRAYSIZE(items));
		
		ImGui::Checkbox("IsAlive1", &ship->isAlive1);
		ImGui::Checkbox("IsAlive2", &ship->isAlive2);
		ImGui::PopID();
	}
	void ShipEditor(MediumShip* ship) {
		ImGui::PushID(ship);
		ImGui::DragInt2("Position", &ship->x);

		const char* items[] = { "Right", "Top", "Left", "Bottom" };
		ImGui::Combo("Side", &ship->side, items, IM_ARRAYSIZE(items));

		ImGui::Checkbox("IsAlive1", &ship->isAlive1);
		ImGui::Checkbox("IsAlive2", &ship->isAlive2);
		ImGui::Checkbox("IsAlive3", &ship->isAlive3);
		ImGui::PopID();
	}
	void ShipEditor(LargeShip* ship) {
		ImGui::PushID(ship);
		ImGui::DragInt2("Position", &ship->x);

		const char* items[] = { "Right", "Top", "Left", "Bottom" };
		ImGui::Combo("Side", &ship->side, items, IM_ARRAYSIZE(items));

		ImGui::Checkbox("IsAlive1", &ship->isAlive1);
		ImGui::Checkbox("IsAlive2", &ship->isAlive2);
		ImGui::Checkbox("IsAlive3", &ship->isAlive3);
		ImGui::Checkbox("IsAlive4", &ship->isAlive4);
		ImGui::PopID();
	}
	

	void ShipEditor(BaseShip* ship) {
		ImGui::PushID(ship);
		const char* items[] = { "Tiny", "Small", "Medium", "Large" };
		ShipType type = ship->GetType();
		
		if (ImGui::TreeNode(items[type - 1])) {
			switch (type) {
			case ShipType::TINY:
				ShipEditor((TinyShip*)ship);
					break;
			case ShipType::SMALL:
				ShipEditor((SmallShip*)ship);
					break;
			case ShipType::MEDIUM:
				ShipEditor((MediumShip*)ship);
					break;
			case ShipType::LARGE:
				ShipEditor((LargeShip*)ship);
				break;
			}
			ImGui::TreePop();
		}
		
		ImGui::PopID();
	}

	ImVec2& CalculateImageSize(sf::Sprite& image) {
		sf::Vector2u size = image.getTexture()->getSize();
		float ratio = (float)size.x / (float)size.y;
		ImVec2 avail_size = ImGui::GetContentRegionAvail();
		ImVec2 size2;
		if (avail_size.x / avail_size.y > ratio) {
			size2.x = avail_size.y * ratio;
			size2.y = avail_size.y;
		}
		else {
			size2.x = avail_size.x;
			size2.y = avail_size.x / ratio;
		}
		return size2;
	}
}


int main() {
	srand(time(0));
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280, 720)), "Sea Battle");
    ImGui::SFML::Init(window);

    ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

    sf::Clock deltaClock;
	bool show_debug_window = true;

	// load background image
	sf::Texture bg_texture;
	if (!bg_texture.loadFromFile("assets/bg.png")) {
		std::cout << "Failed to load background image" << std::endl;
		return -1;
	}
	bg_texture.setRepeated(true);
	
	PlayerGrid playerGrid;
	PlayerGrid enemyGrid;
	enemyGrid.isHidden = true;

	AI ai(&playerGrid);
	bool gameOver = false;
	bool playerTurn = true;

	while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(event.size.width, event.size.height));
				window.setView(sf::View(visibleArea));
			}
        }

        sf::Time deltaTime = deltaClock.restart();
		
		// Check for winner
		// Check players ships
		bool playerHasShips = false;
		for (BaseShip* ship : playerGrid.ships) {
			if (!ship->IsDestroyed()) {
				playerHasShips = true;
				break;
			}
		}

		// Check enemy ships
		bool enemyHasShips = false;
		for (BaseShip* ship : enemyGrid.ships) {
			if (!ship->IsDestroyed()) {
				enemyHasShips = true;
				break;
			}
		}

		// If someone has no ships, end the game
		if (!playerHasShips || !enemyHasShips) {
			gameOver = true;
			playerTurn = false;
		}
	
		if (!playerTurn && !gameOver) {
			// AI's turn
			ai.finishedTurn = playerTurn;
			ai.Update(deltaTime.asSeconds());
			playerTurn = ai.finishedTurn;
		}


        ImGui::SFML::Update(window, deltaTime);
		
		// enable docking
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(255, 255, 255, 0));
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// show menu
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Game")) {
				if (ImGui::MenuItem("New Game")) {
					playerGrid.Reset();
					enemyGrid.Reset();
					playerTurn = true;
					gameOver = false;
				}
				if (ImGui::MenuItem("Exit")) {
					window.close();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				ImGui::MenuItem("Debug Window", NULL, &show_debug_window);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		
		sf::RenderTexture renderTexturePlayer;
		sf::RenderTexture renderTextureEnemy;
		
		ImGui::Begin("Your grid");
		{
			renderTexturePlayer.create(sf::Vector2u(IMG_SIZE, IMG_SIZE));
			sf::Sprite sprite;
			sprite.setTexture(renderTexturePlayer.getTexture());
			playerGrid.Draw(renderTexturePlayer);
			RenderGrid(renderTexturePlayer, GRID_SIZE, GRID_SIZE, TILE_SIZE);
			ImVec2 size = ImGui::CalculateImageSize(sprite);
			ImGuiStyle& style = ImGui::GetStyle();
			float size_a = (size.x) + style.FramePadding.x * 2.0f;
			float avail = ImGui::GetContentRegionAvail().x;
			float off = (avail - size_a) * 0.5f;
			if (off > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
			ImGui::Image(sprite, size);
		}
		ImGui::End();

		
		ImGui::Begin("Enemy grid");
		{
			renderTextureEnemy.create(sf::Vector2u(IMG_SIZE, IMG_SIZE));
			sf::Sprite sprite;
			sprite.setTexture(renderTextureEnemy.getTexture());
			enemyGrid.Draw(renderTextureEnemy);
			RenderGrid(renderTextureEnemy, GRID_SIZE, GRID_SIZE, TILE_SIZE);
			ImVec2 size = ImGui::CalculateImageSize(sprite);

			// draw sprite centered horizontally
			ImGuiStyle& style = ImGui::GetStyle();
			float size_a = (size.x) + style.FramePadding.x * 2.0f;
			float avail = ImGui::GetContentRegionAvail().x;
			float off = (avail - size_a) * 0.5f;
			if (off > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
			
			

			if (playerTurn) {
				// get mouse position
				sf::Vector2f mousePos = ImGui::GetMousePos();
				mousePos.x -= ImGui::GetWindowPos().x + ImGui::GetCursorPosX();
				mousePos.y -= ImGui::GetWindowPos().y + ImGui::GetCursorPosY();
				mousePos.y = size.x - mousePos.y;
				mousePos.x /= size.x;
				mousePos.y /= size.y;
				int x = mousePos.x * GRID_SIZE;
				int y = mousePos.y * GRID_SIZE;
				
				// draw opaque tile if mouse is over grid
				if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
					sf::RectangleShape rect(sf::Vector2f(TILE_SIZE + 2, TILE_SIZE + 2));
					rect.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
					rect.setFillColor(sf::Color(0, 0, 0, 100));
					renderTextureEnemy.draw(rect);
				}

				// if clicked on grid, shoot
				if (ImGui::IsMouseClicked(0) && x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
					HitResult hit = enemyGrid.Hit(x, y);
					if (hit == HitResult::MISS) {
						playerTurn = false;
					}
				}
			}
			
			ImGui::Image(sprite, size);
		}
		ImGui::End();
		ImGui::PopStyleColor();
		
		if (gameOver) {
			ImGui::Begin("Game Over");
			ImGui::Text("You %s!", playerHasShips ? "win" : "lose");
			if (ImGui::Button("New Game")) {
				playerGrid.Reset();
				enemyGrid.Reset();
				playerTurn = true;
				gameOver = false;
			}
			ImGui::End();
		}
		
		sf::Vector2u bg_size = window.getSize();
		
		if (show_debug_window) {
			ImGui::Begin("Debug");
			ImGui::Checkbox("Hide enemy ships", &enemyGrid.isHidden);

			if (ImGui::TreeNode("Player Ships")) {
				for (BaseShip* ship : playerGrid.ships) {
					ImGui::ShipEditor(ship);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Enemy Ships")) {
				for (BaseShip* ship : enemyGrid.ships) {
					ImGui::ShipEditor(ship);
				}
				ImGui::TreePop();
			}
			ImGui::End();
		}
		
        window.clear();
		// draw background
		sf::Sprite bg_sprite;
		bg_sprite.setTexture(bg_texture);
		bg_sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(bg_size)));
		window.draw(bg_sprite);
		
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}