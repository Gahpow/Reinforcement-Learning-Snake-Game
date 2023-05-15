#include "Game.h"

Game::Game(sf::RenderWindow* window, Input* input)
{
	window_ = window;
	input_ = input;

	player_snake_active_ = 0;
	if (player_snake_active_)
	{
		player_snake_.Initialise(window_, sf::Vector2f(0, 0), sf::Vector2i(GRID_SIZE, GRID_SIZE));
		player_snake_.SetTickSpeed(0.5f);
	}
	else
	{
		agent_snake_.Initialise(window_, sf::Vector2f(0, 0), sf::Vector2i(GRID_SIZE, GRID_SIZE));
		agent_snake_.SetEpsilon(1);
		agent_snake_.SetDiscountFactor(0.6f);
		agent_snake_.SetTickSpeed(0.1f);

		// Initialise text
		if (!font_.loadFromFile("Font/arial.ttf"))
		{
			std::cout << "Font failed to load" << std::endl;
		}
		else
		{
			text_.setFont(font_);
			text_.setCharacterSize(15);
			text_.setFillColor(sf::Color(157, 24, 11));
		}
	}

	food_.Initialise(window_, sf::Vector2i(GRID_SIZE, GRID_SIZE));
	// Make sure food doesn't reset on the snake
	do
	{
		food_.ResetPosition();
	} while (agent_snake_.CoordinateIsOnSnake(food_.GetPosition()));
}

void Game::Update(float dt)
{
	// Close app
	if (input_->IsKeyDown(sf::Keyboard::Escape))
	{
		window_->close();
	}

	if (player_snake_active_)
		ControlledSnake(dt);
	else
		AgentSnake(dt);
	UpdateText();
}

void Game::Render()
{
	// Render snake and food
	food_.Draw(window_);
	if (player_snake_active_)
	{
		player_snake_.Draw(window_);
	}
	else
	{
		agent_snake_.Draw(window_);
		window_->draw(text_);
	}
}

void Game::ControlledSnake(float dt)
{
	// Select snake move
	if (input_->IsKeyDown(sf::Keyboard::D))
	{
		if (player_snake_.GetPreviousMove() != SnakeMovement::LEFT)
			player_snake_.SelectMove(SnakeMovement::RIGHT);
		input_->SetKeyUp(sf::Keyboard::D);
	}
	if (input_->IsKeyDown(sf::Keyboard::A))
	{
		if (player_snake_.GetPreviousMove() != SnakeMovement::RIGHT)
			player_snake_.SelectMove(SnakeMovement::LEFT);
		input_->SetKeyUp(sf::Keyboard::A);
	}
	if (input_->IsKeyDown(sf::Keyboard::W))
	{
		if (player_snake_.GetPreviousMove() != SnakeMovement::DOWN)
			player_snake_.SelectMove(SnakeMovement::UP);
		input_->SetKeyUp(sf::Keyboard::W);
	}
	if (input_->IsKeyDown(sf::Keyboard::S))
	{
		if (player_snake_.GetPreviousMove() != SnakeMovement::UP)
			player_snake_.SelectMove(SnakeMovement::DOWN);
		input_->SetKeyUp(sf::Keyboard::S);
	}

	// Update snake
	if (!player_snake_.Update(dt))
	{
		// Snake has bitten its tail, reset game
		player_snake_.Reset();			
		// Make sure food doesn't reset on the snake
		do
		{
			food_.ResetPosition();
		} while (player_snake_.CoordinateIsOnSnake(food_.GetPosition()));
	}

	// Reset food position when snake head goes over it
	if (player_snake_.GetPosition() == food_.GetPosition())
	{
		// Expand snake
		player_snake_.FoodEaten();
		if (player_snake_.CheckForWin())
			player_snake_.Reset();
		// Make sure food doesn't reset on the snake
		do
		{
			food_.ResetPosition();
		} while (player_snake_.CoordinateIsOnSnake(food_.GetPosition()));

	}
}

void Game::AgentSnake(float dt)
{
	// Input
	// Train ai / exploration
	if (input_->IsKeyDown(sf::Keyboard::Q))
	{
		agent_snake_.SetEpsilon(80);
		agent_snake_.SetDiscountFactor(0.2f);
		agent_snake_.SetTickSpeed(0.00001f);
		input_->SetKeyDown(sf::Keyboard::Q);
	}
	// Exploitation
	if (input_->IsKeyDown(sf::Keyboard::W))
	{
		agent_snake_.SetEpsilon(0);
		agent_snake_.SetDiscountFactor(0.2f);
		agent_snake_.SetTickSpeed(0.1f);
		input_->SetKeyDown(sf::Keyboard::W);
	}
	agent_snake_.UpdateTickTimer(dt);
	if (agent_snake_.GetTickTimer() >= agent_snake_.GetTickSpeed())
	{
		bool has_eaten = 0;
		bool has_died = 0;

		// Run AI
		agent_snake_.DetermineState(food_.GetPosition());
		agent_snake_.DetermineAction();
		// Update snake
		if (!agent_snake_.Update())
		{
			has_died = 1;
			// Snake has bitten its tail, reset game
			agent_snake_.Reset();
			// Make sure food doesn't reset on the snake
			do
			{
				food_.ResetPosition();
			} while (agent_snake_.CoordinateIsOnSnake(food_.GetPosition()));
		}

		// Reset food position when snake head goes over it
		if (agent_snake_.GetPosition() == food_.GetPosition())
		{
			has_eaten = 1;
			// Expand snake and check for win / reset game
			agent_snake_.FoodEaten();
			if (agent_snake_.CheckForWin())
				agent_snake_.Reset();
			// Make sure food doesn't reset on the snake
			do
			{
				food_.ResetPosition();
			} while (agent_snake_.CoordinateIsOnSnake(food_.GetPosition()));
		}
		agent_snake_.CalculateReward(food_.GetPosition(), has_eaten, has_died);
	}
}

void Game::UpdateText()
{
	std::string epsilon = sf::String("Epsilon: ") + std::to_string(agent_snake_.GetEpsilon()) + sf::String(" ");
	std::string learning_rate = sf::String("Learning Rate: ") + std::to_string(agent_snake_.GetLearningRate()) + sf::String(" ");
	std::string discount_factor = sf::String("Discount Factor: ") + std::to_string(agent_snake_.GetDiscountFactor()) + sf::String(" ");

	text_.setString(epsilon + learning_rate + discount_factor);
}
