#include "Food.h"

Food::Food()
{

}

Food::~Food()
{
}

void Food::Initialise(sf::RenderWindow* window, sf::Vector2i grid_size)
{
	grid_size_ = grid_size;
	// Initialise food
	sprite_size_ = sf::Vector2f(window->getSize().x / grid_size_.x, window->getSize().y / grid_size_.y);
	sprite_.setSize(sprite_size_);
	sprite_.setFillColor(sf::Color::Blue);
}

void Food::Draw(sf::RenderWindow* window)
{
	window->draw(sprite_);
}


void Food::ResetPosition()
{
	// position food on coordinate x y on the grid
	int position_x, position_y;
	position_x = rand() % grid_size_.x;
	position_y = rand() % grid_size_.y;
	sprite_.setPosition(sprite_size_.x * position_x, sprite_size_.y * position_y);
}

sf::Vector2f Food::GetPosition()
{
	return sprite_.getPosition();
}
