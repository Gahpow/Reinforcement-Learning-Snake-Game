#pragma once

#include <deque>
#include <SFML/Graphics.hpp>

enum SnakeMovement{UP = 0, DOWN, LEFT, RIGHT};

class SnakeCharacter
{
protected:
	// I thought I would need more variables for each snake part
	// and forgot to change it back to just the sprite
	struct SnakePart
	{
		sf::RectangleShape sprite_;
	};

	std::deque<SnakePart*> snake_body_;
	SnakeMovement selected_move_;
	SnakeMovement previous_move_;
	sf::Vector2f sprite_size_;
	sf::Vector2i grid_size_;
	sf::Vector2f previous_tail_position_;

	float tick_timer_;
	float tick_speed_;
public:
	SnakeCharacter() {};
	~SnakeCharacter();

	SnakeCharacter(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2i grid_size, int body_length = 3);
	void Initialise(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2i grid_size, int body_length = 3);

	// Game functions
	bool Update(float dt);
	void Draw(sf::RenderWindow* window);
	bool CheckForWin();

	// Snake functionality
	sf::Vector2f GetPosition();
	sf::Vector2f GetSpriteSize();
	SnakeMovement GetPreviousMove();
	SnakeMovement GetSelectedMove();
	float GetTickTimer();
	float GetTickSpeed();
	void UpdateTickTimer(float dt);
	bool CoordinateIsOnSnake(sf::Vector2f position);
	void FoodEaten();
	void Reset();
	void SelectMove(SnakeMovement move);
	void SetTickSpeed(float tick_speed);
protected:
	// Movement functions
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
};

