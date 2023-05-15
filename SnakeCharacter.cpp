#include "SnakeCharacter.h"

SnakeCharacter::SnakeCharacter(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2i grid_size, int body_length)
{
	grid_size_ = grid_size;
	tick_speed_ = 1;
	// Set sprite_ size to fit grid
	sprite_size_ = sf::Vector2f(window->getSize().x / grid_size.x, window->getSize().y / grid_size.y);
	previous_move_ = SnakeMovement::RIGHT;
	selected_move_ = SnakeMovement::RIGHT;
	// Initialise body
	for (int i = 0; i < body_length; i++)
	{
		SnakePart* part = new SnakePart;
		part->sprite_.setSize(sprite_size_);
		part->sprite_.setFillColor(sf::Color::Green);
		part->sprite_.setOutlineColor(sf::Color(119, 136, 153, 255));
		part->sprite_.setOutlineThickness(-1);
		part->sprite_.setPosition(sf::Vector2f(position.x + (sprite_size_.x * i), position.y * sprite_size_.y));
		snake_body_.push_back(part);
	}
	// Set last part color to red
	snake_body_.back()->sprite_.setFillColor(sf::Color::Red);
}

SnakeCharacter::~SnakeCharacter()
{
	while (snake_body_.size()> 0)
	{
		delete snake_body_.front();
		snake_body_.pop_front();
	}

}

void SnakeCharacter::Initialise(sf::RenderWindow* window, sf::Vector2f position, sf::Vector2i grid_size, int body_length)
{
	grid_size_ = grid_size;
	tick_speed_ = 1;
	// Set sprite_ size to fit grid
	sprite_size_ = sf::Vector2f(window->getSize().x / grid_size.x, window->getSize().y / grid_size.y);
	previous_move_ = SnakeMovement::RIGHT;

	// Initialise body
	for (int i = 0; i < body_length; i++)
	{
		SnakePart* part = new SnakePart;
		part->sprite_.setSize(sprite_size_);
		part->sprite_.setFillColor(sf::Color::Green);
		part->sprite_.setOutlineColor(sf::Color(119, 136, 153, 255));
		part->sprite_.setOutlineThickness(-1);
		part->sprite_.setPosition(sf::Vector2f(position.x + (sprite_size_.x * i), position.y * sprite_size_.y));
		snake_body_.push_back(part);
	}
	// Set last part color to red
	snake_body_.back()->sprite_.setFillColor(sf::Color::Red);
}

void SnakeCharacter::MoveRight()
{
	if (previous_move_ != SnakeMovement::LEFT)
	{
		previous_move_ = SnakeMovement::RIGHT;

		SnakePart* tail = snake_body_.front();
		SnakePart* head = snake_body_.back();

		// Move tail to the right of head and change color to red
		tail->sprite_.setPosition(head->sprite_.getPosition().x + sprite_size_.x, head->sprite_.getPosition().y);
		tail->sprite_.setFillColor(sf::Color::Red);

		// Restructure deque
		snake_body_.pop_front();
		snake_body_.push_back(tail);

		// Recolor head
		head->sprite_.setFillColor(sf::Color::Green);
	}
}

void SnakeCharacter::MoveLeft()
{
	if (previous_move_ != SnakeMovement::RIGHT)
	{
		previous_move_ = SnakeMovement::LEFT;

		SnakePart* tail = snake_body_.front();
		SnakePart* head = snake_body_.back();

		// Move tail to the right of head and change color to red
		tail->sprite_.setPosition(head->sprite_.getPosition().x - sprite_size_.x, head->sprite_.getPosition().y);
		tail->sprite_.setFillColor(sf::Color::Red);

		// Restructure deque
		snake_body_.pop_front();
		snake_body_.push_back(tail);

		// Recolor head
		head->sprite_.setFillColor(sf::Color::Green);
	}
}

void SnakeCharacter::MoveUp()
{
	if (previous_move_ != SnakeMovement::DOWN)
	{
		previous_move_ = SnakeMovement::UP;

		SnakePart* tail = snake_body_.front();
		SnakePart* head = snake_body_.back();

		// Move tail to the right of head and change color to red
		tail->sprite_.setPosition(head->sprite_.getPosition().x, head->sprite_.getPosition().y - sprite_size_.y);
		tail->sprite_.setFillColor(sf::Color::Red);

		// Restructure deque
		snake_body_.pop_front();
		snake_body_.push_back(tail);

		// Recolor head
		head->sprite_.setFillColor(sf::Color::Green);
	}
}

void SnakeCharacter::MoveDown()
{
	if (previous_move_ != SnakeMovement::UP)
	{
		previous_move_ = SnakeMovement::DOWN;

		SnakePart* tail = snake_body_.front();
		SnakePart* head = snake_body_.back();

		// Move tail to the right of head and change color to red
		tail->sprite_.setPosition(head->sprite_.getPosition().x, head->sprite_.getPosition().y + sprite_size_.y);
		tail->sprite_.setFillColor(sf::Color::Red);

		// Restructure deque
		snake_body_.pop_front();
		snake_body_.push_back(tail);

		// Recolor head
		head->sprite_.setFillColor(sf::Color::Green);
	}
}

sf::Vector2f SnakeCharacter::GetPosition()
{
	// Returns position of snake head
	return snake_body_.back()->sprite_.getPosition();
}

sf::Vector2f SnakeCharacter::GetSpriteSize()
{
	return sprite_size_;
}

SnakeMovement SnakeCharacter::GetPreviousMove()
{
	return previous_move_;
}

SnakeMovement SnakeCharacter::GetSelectedMove()
{
	return selected_move_;
}

float SnakeCharacter::GetTickTimer()
{
	return tick_timer_;
}

float SnakeCharacter::GetTickSpeed()
{
	return tick_speed_;
}

void SnakeCharacter::UpdateTickTimer(float dt)
{
	tick_timer_ += dt;
}

bool SnakeCharacter::CoordinateIsOnSnake(sf::Vector2f position)
{
	// Returns true if position is equal to the position of any snake part including head
	for (int i = 0; i < snake_body_.size(); i++)
	{
		if (position == snake_body_[i]->sprite_.getPosition())
			return true;
	}
	return false;
}

void SnakeCharacter::FoodEaten()
{
	// Construct new part to be the tail on the position of the last tail part 
	SnakePart* part = new SnakePart;
	part->sprite_.setSize(sprite_size_);
	part->sprite_.setFillColor(sf::Color::Green);
	part->sprite_.setOutlineColor(sf::Color(119, 136, 153, 255));
	part->sprite_.setOutlineThickness(-1);
	part->sprite_.setPosition(previous_tail_position_);
	snake_body_.push_front(part);
}

void SnakeCharacter::Reset()
{
	// Delete all the parts except the last 3
	while (snake_body_.size() != 3)
	{
		delete snake_body_.front();
		snake_body_.pop_front();
	}
	
	// Reset position to 0, 0
	for (int i = 0; i < snake_body_.size(); i++)
	{
		snake_body_[i]->sprite_.setPosition(sf::Vector2f(sprite_size_.x * i, 0));
	}

	// Reset previous move
	previous_move_ = SnakeMovement::RIGHT;
	selected_move_ = SnakeMovement::RIGHT;
}

void SnakeCharacter::SelectMove(SnakeMovement move)
{
	selected_move_ = move;
}

bool SnakeCharacter::Update(float dt)
{
	// Updates snake and returns false if snake has bitten its tail	or gone out of bounds

	// Update previous tail position
	previous_tail_position_ = snake_body_.front()->sprite_.getPosition();

	// Update timer
	tick_timer_ += dt;

	// Check if timer has reached tick speed
	if (tick_timer_ >= tick_speed_)
	{
		// Reset timer
		tick_timer_ = 0;

		// Move snake
		switch (selected_move_)
		{
		case SnakeMovement::LEFT:
			MoveLeft();
			break;

		case SnakeMovement::RIGHT:
			MoveRight();
			break;

		case SnakeMovement::UP:
			MoveUp();
			break;

		case SnakeMovement::DOWN:
			MoveDown();
			break;

		default:
			break;
		}
	}

	// Check if snake has bitten its tail
	for (int i = 0; i < snake_body_.size() - 2; i++)
	{
		if (snake_body_.back()->sprite_.getPosition() == snake_body_[i]->sprite_.getPosition())
			return false;
	}

	// Check if snake has gone out of bounds
	if (snake_body_.back()->sprite_.getPosition().x < 0 || snake_body_.back()->sprite_.getPosition().y < 0 ||
		snake_body_.back()->sprite_.getPosition().x > sprite_size_.x * grid_size_.x - sprite_size_.x || snake_body_.back()->sprite_.getPosition().y > (sprite_size_.y * grid_size_.y) - sprite_size_.y)
	{
		return false;
	}
	return true;
}

void SnakeCharacter::Draw(sf::RenderWindow* window)
{
	for (int i = 0; i < snake_body_.size(); i++)
	{
		window->draw(snake_body_[i]->sprite_);
	}
}

bool SnakeCharacter::CheckForWin()
{
	if (snake_body_.size() == grid_size_.x * grid_size_.y)
		return true;
	else
		return false;
}

void SnakeCharacter::SetTickSpeed(float tick_speed)
{
	tick_speed_ = tick_speed;
}