#pragma once

#include "SnakeCharacter.h"

#include <array>
#include <random>

#define STATE_FOOD_DIRECTION_SIZE 8
#define ACTION_NUM 4
#define STATE_OBSTACLE_SIZE 4

enum StateObstacle {OBSTACLE_UP = 0, OBSTACLE_DOWN, OBSTACLE_LEFT, OBSTACLE_RIGHT};
enum StateFoodDirection { FOOD_UP = 0, FOOD_DOWN, FOOD_LEFT, FOOD_RIGHT, FOOD_UP_LEFT, FOOD_UP_RIGHT, FOOD_DOWN_LEFT, FOOD_DOWN_RIGHT };

class SnakeAI :
    public SnakeCharacter
{
private:
    // State variables
    std::array<bool, STATE_OBSTACLE_SIZE> state_obstacle_;
    StateFoodDirection state_food_direction_;
    
    // Next state variables
    std::array<bool, STATE_OBSTACLE_SIZE> next_state_obstacle_;
    StateFoodDirection next_state_food_direction_;

    // Matrix
    float Q[STATE_FOOD_DIRECTION_SIZE][2][2][2][2][ACTION_NUM];

    // Variables
    SnakeMovement action_chosen_;
    float next_action_reward;
    float reward_;
    float distance_to_food_;
    float previous_distance_to_food_;
    // Algorithm constants
    float epsilon_;
    float learning_rate_;
    float discount_factor_;
public:
    SnakeAI();
    ~SnakeAI();

    bool Update();
    bool CoordinateIsOnSnakeBody(sf::Vector2f position);
    void DetermineState(sf::Vector2f food_position);
    void DetermineNextState(sf::Vector2f food_position);
    void DetermineAction();
    void DetermineNextActionReward();
    void CalculateReward(sf::Vector2f food_position, bool has_eaten, bool has_died);

    void SetEpsilon(float epsilon);
    void SetDiscountFactor(float discount);

    float GetEpsilon();
    float GetLearningRate();
    float GetDiscountFactor();
};

