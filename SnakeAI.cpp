#include "SnakeAI.h"

SnakeAI::SnakeAI()
{
	for (int i = 0; i < 4; i++)
	{
		state_obstacle_[i] = 0;
	}
	epsilon_ = 10;
	reward_ = 0;
	learning_rate_ = 0.2f;
	distance_to_food_ = 0.0f;
	action_chosen_ = RIGHT;
	discount_factor_ = 0.2f;
}

SnakeAI::~SnakeAI()
{
}

bool SnakeAI::Update()
{
	// Updates snake and returns false if snake has bitten its tail	or gone out of bounds

	// Update previous tail position
	previous_tail_position_ = snake_body_.front()->sprite_.getPosition();

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
		snake_body_.back()->sprite_.getPosition().x > sprite_size_.x * grid_size_.x - sprite_size_.x || snake_body_.back()->sprite_.getPosition().y >(sprite_size_.y * grid_size_.y) - sprite_size_.y)
	{
		return false;
	}
	return true;
}

bool SnakeAI::CoordinateIsOnSnakeBody(sf::Vector2f position)
{
	// Returns true if position is equal to the position of snake body except the body part before the head
	for (int i = 0; i < snake_body_.size() - 2; i++)
	{
		if (position == snake_body_[i]->sprite_.getPosition())
			return true;
	}
	return false;
}

void SnakeAI::DetermineState(sf::Vector2f food_position)
{
	// Determine obstacle states
	// Obstacle up
	if (GetPosition().y - sprite_size_.y < 0 || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x, GetPosition().y - sprite_size_.y)))
		state_obstacle_[OBSTACLE_UP] = 1;
	else
		state_obstacle_[OBSTACLE_UP] = 0;
	// Obstacle down
	if (GetPosition().y + sprite_size_.y > sprite_size_.y * grid_size_.y - sprite_size_.y || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x, GetPosition().y + sprite_size_.y)))
		state_obstacle_[OBSTACLE_DOWN] = 1;
	else
		state_obstacle_[OBSTACLE_DOWN] = 0;
	// Obstacle left
	if (GetPosition().x - sprite_size_.x < 0 || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x - sprite_size_.x, GetPosition().y)))
		state_obstacle_[OBSTACLE_LEFT] = 1;
	else
		state_obstacle_[OBSTACLE_LEFT] = 0;
	// Obstacle Right
	if (GetPosition().x + sprite_size_.x > sprite_size_.x * grid_size_.x - sprite_size_.x || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x + sprite_size_.x, GetPosition().y)))
		state_obstacle_[OBSTACLE_RIGHT] = 1;
	else
		state_obstacle_[OBSTACLE_RIGHT] = 0;

	// Determine food direction
	if (GetPosition().x > food_position.x)
		state_food_direction_ = FOOD_LEFT;
	if (GetPosition().x < food_position.x)
		state_food_direction_ = FOOD_RIGHT;
	if (GetPosition().y > food_position.y)
	{
		state_food_direction_ = FOOD_UP;
		if (GetPosition().x > food_position.x)
			state_food_direction_ = FOOD_UP_LEFT;
		if (GetPosition().x < food_position.x)
			state_food_direction_ = FOOD_UP_RIGHT;
	}
	if (GetPosition().y < food_position.y)
	{
		state_food_direction_ = FOOD_DOWN;
		if (GetPosition().x > food_position.x)
			state_food_direction_ = FOOD_DOWN_LEFT;
		if (GetPosition().x < food_position.x)
			state_food_direction_ = FOOD_DOWN_RIGHT;
	}
}

void SnakeAI::DetermineNextState(sf::Vector2f food_position)
{
	// Determine next obstacle states
	// Obstacle up
	if (GetPosition().y - sprite_size_.y < 0 || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x, GetPosition().y - sprite_size_.y)))
		next_state_obstacle_[OBSTACLE_UP] = 1;
	else
		next_state_obstacle_[OBSTACLE_UP] = 0;
	// Obstacle down
	if (GetPosition().y + sprite_size_.y > sprite_size_.y * grid_size_.y - sprite_size_.y || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x, GetPosition().y + sprite_size_.y)))
		next_state_obstacle_[OBSTACLE_DOWN] = 1;
	else
		next_state_obstacle_[OBSTACLE_DOWN] = 0;
	// Obstacle left
	if (GetPosition().x - sprite_size_.x < 0 || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x - sprite_size_.x, GetPosition().y)))
		next_state_obstacle_[OBSTACLE_LEFT] = 1;
	else
		next_state_obstacle_[OBSTACLE_LEFT] = 0;
	// Obstacle Right
	if (GetPosition().x + sprite_size_.x > sprite_size_.x * grid_size_.x - sprite_size_.x || CoordinateIsOnSnakeBody(sf::Vector2f(GetPosition().x + sprite_size_.x, GetPosition().y)))
		next_state_obstacle_[OBSTACLE_RIGHT] = 1;
	else
		next_state_obstacle_[OBSTACLE_RIGHT] = 0;

	// Determine next state food direction
	if (GetPosition().x > food_position.x)
		next_state_food_direction_ = FOOD_LEFT;
	if (GetPosition().x < food_position.x)
		next_state_food_direction_ = FOOD_RIGHT;
	if (GetPosition().y > food_position.y)
	{
		next_state_food_direction_ = FOOD_UP;
		if (GetPosition().x > food_position.x)
			next_state_food_direction_ = FOOD_UP_LEFT;
		if (GetPosition().x < food_position.x)
			next_state_food_direction_ = FOOD_UP_RIGHT;
	}
	if (GetPosition().y < food_position.y)
	{
		next_state_food_direction_ = FOOD_DOWN;
		if (GetPosition().x > food_position.x)
			next_state_food_direction_ = FOOD_DOWN_LEFT;
		if (GetPosition().x < food_position.x)
			next_state_food_direction_ = FOOD_DOWN_RIGHT;
	}
}

void SnakeAI::DetermineAction()
{
	int random_number = rand() % 100 + 1;
	if (random_number > epsilon_)
	{
		int index;
		if (previous_move_ != DOWN)
			index = 0;
		else
			index = 1;
		int best_value = Q[state_food_direction_][state_obstacle_[OBSTACLE_UP]][state_obstacle_[OBSTACLE_DOWN]][state_obstacle_[OBSTACLE_LEFT]][state_obstacle_[OBSTACLE_RIGHT]][index];
		// Loop through all actions
		for (int i = 0; i < ACTION_NUM; i++)
		{
			int current_value = Q[state_food_direction_][state_obstacle_[OBSTACLE_UP]][state_obstacle_[OBSTACLE_DOWN]][state_obstacle_[OBSTACLE_LEFT]][state_obstacle_[OBSTACLE_RIGHT]][i];
			if (current_value > best_value)
			{
				// Make sure AI can't select a move that breaks the snake
				switch (i)
				{
				case 0:
					if (previous_move_ != DOWN)
					{
						best_value = current_value;
						index = i;
					}
					break;
				case 1:
					if (previous_move_ != UP)
					{
						best_value = current_value;
						index = i;
					}
					break;
				case 2:
					if (previous_move_ != RIGHT)
					{
						best_value = current_value;
						index = i;
					}
					break;
				case 3:
					if (previous_move_ != LEFT)
					{
						best_value = current_value;
						index = i;
					}
					break;
				}
			
			}
		}
		SelectMove(SnakeMovement(index));
		action_chosen_ = SnakeMovement(index);
	}
	else
	{
		bool has_seleceted_move = 0;
		random_number = rand() % 4;
		// Make sure random number is a valid move
		while (!has_seleceted_move)
		{
			switch (random_number)
			{
			case 0:
				if (previous_move_ != DOWN)
				{
					SelectMove(SnakeMovement(random_number));
					action_chosen_ = SnakeMovement(random_number);
				}
				has_seleceted_move = 1;
				break;
			case 1:
				if (previous_move_ != UP)
				{
					SelectMove(SnakeMovement(random_number));
					action_chosen_ = SnakeMovement(random_number);
				}
				has_seleceted_move = 1;
				break;
			case 2:
				if (previous_move_ != RIGHT)
				{
					SelectMove(SnakeMovement(random_number));
					action_chosen_ = SnakeMovement(random_number);
				}
				has_seleceted_move = 1;
				break;
			case 3:
				if (previous_move_ != LEFT)
				{
					SelectMove(SnakeMovement(random_number));
					action_chosen_ = SnakeMovement(random_number);
				}
				has_seleceted_move = 1;
				break;
			}
		}
	}
}

void SnakeAI::DetermineNextActionReward()
{
	int index;
	if (previous_move_ != DOWN)
		index = 0;
	else
		index = 1;
	int best_value = Q[next_state_food_direction_][next_state_obstacle_[OBSTACLE_UP]][next_state_obstacle_[OBSTACLE_DOWN]][next_state_obstacle_[OBSTACLE_LEFT]][next_state_obstacle_[OBSTACLE_RIGHT]][index];
	// Loop through all actions
	for (int i = 0; i < ACTION_NUM; i++)
	{
		int current_value = Q[next_state_food_direction_][next_state_obstacle_[OBSTACLE_UP]][next_state_obstacle_[OBSTACLE_DOWN]][next_state_obstacle_[OBSTACLE_LEFT]][next_state_obstacle_[OBSTACLE_RIGHT]][i];
		if (current_value > best_value)
		{
			// Make sure AI can't select a move that breaks the snake
			switch (i)
			{
			case 0:
				if (previous_move_ != DOWN)
				{
					best_value = current_value;
					index = i;
				}
				break;
			case 1:
				if (previous_move_ != UP)
				{
					best_value = current_value;
					index = i;
				}
				break;
			case 2:
				if (previous_move_ != RIGHT)
				{
					best_value = current_value;
					index = i;
				}
				break;
			case 3:
				if (previous_move_ != LEFT)
				{
					best_value = current_value;
					index = i;
				}
				break;
			}

		}
	}
	next_action_reward = Q[state_food_direction_][state_obstacle_[OBSTACLE_UP]][state_obstacle_[OBSTACLE_DOWN]][state_obstacle_[OBSTACLE_LEFT]][state_obstacle_[OBSTACLE_RIGHT]][index];
}

void SnakeAI::CalculateReward(sf::Vector2f food_position, bool has_eaten, bool has_died)
{
	// Calculate distance to food
	previous_distance_to_food_ = distance_to_food_;
	distance_to_food_ = sqrt(powf(GetPosition().x - food_position.x, 2) + powf(GetPosition().y - food_position.y, 2));

	// Determine reward
	if (has_eaten)
	{
		reward_ = 10;
	}
	else
	{
		if (distance_to_food_ < previous_distance_to_food_)
		{
			reward_ = 0.5f;
		}
		else if (distance_to_food_ > previous_distance_to_food_)
		{
			reward_ = -1.0f;
		}
		else
		{
			reward_ = -0.5f;
		}
	}
	if (has_died)
		reward_ = -50.0f;

	DetermineNextState(food_position);
	DetermineNextActionReward();
	float previous_action_reward = Q[state_food_direction_][state_obstacle_[OBSTACLE_UP]][state_obstacle_[OBSTACLE_DOWN]][state_obstacle_[OBSTACLE_LEFT]][state_obstacle_[OBSTACLE_RIGHT]][action_chosen_];

	// Calculate and apply reward
	Q[state_food_direction_][state_obstacle_[OBSTACLE_UP]][state_obstacle_[OBSTACLE_DOWN]][state_obstacle_[OBSTACLE_LEFT]][state_obstacle_[OBSTACLE_RIGHT]][action_chosen_]
		+= learning_rate_ * (reward_ + (discount_factor_ * (next_action_reward - previous_action_reward)));
}

void SnakeAI::SetEpsilon(float epsilon)
{
	epsilon_ = epsilon;
}

void SnakeAI::SetDiscountFactor(float discount)
{
	discount_factor_ = discount;
}

float SnakeAI::GetEpsilon()
{
	return epsilon_;
}

float SnakeAI::GetLearningRate()
{
	return learning_rate_;
}

float SnakeAI::GetDiscountFactor()
{
	return discount_factor_;
}

