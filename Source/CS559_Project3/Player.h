#pragma once 

#include "BallController.h"

#define MAXIMUM_PLAYER_SPPED 20

class Player {
public:
	Player();

	// The player will be born at a random place that doesn't collide with any ball
	void Initialize(const BallController & ballcontroller);

	void addFov();
	void mimusFov();

	void SpeedUp();
	void SpeedDown();

	vec3 getViewPosition()const;
	vec3 getMovingDirection()const;
	GLint getFov()const;

	GLfloat last_elapsed_time_;

	void Update(GLfloat new_elapsed_time_, BallController & ballcontroller, ivec2 window_size, ivec2 mouse_position);

	float getNearestBallToHit(int & ball_index, BallController & ballcontroller);
	float getNearestWallToHit(int & wall_index, int wall_width);

	void UpdateAfterBallHit(BallController & ballcontroller, int ball_index, GLfloat travel_time);
	void UpdateAfterWallHit(int wall_index, GLfloat travel_time);
	void UpdateMovingDirectionAfterBumping(vec3 normal);
	void UpdateRotationAngleBasedonPosition();
	GLfloat rotation_angle_;
	vec3 position_;
private:
	GLint fov_;
	
	vec3 moving_direction_;
	GLint speed_;
	
	GLint multiplier_;
	
};