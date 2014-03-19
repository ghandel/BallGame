#include "Player.h"
#include <assert.h>


Player::Player()
{
	fov_ = 84;
	speed_ = 0;
	multiplier_ = 0;
}

void Player::Initialize(const BallController & ballcontroller) {
	srand(0);
	vec2 position_tmp = ballcontroller.getRandomPosition();
	while(ballcontroller.CollideWithOtherBall(position_tmp)) {
		position_tmp = ballcontroller.getRandomPosition();
	}
	position_ = vec3(position_tmp.x, 50, position_tmp.y);
	rotation_angle_ = (float)radians(45.0);
	moving_direction_ = vec3(cos(rotation_angle_), 0, sin(rotation_angle_));
}

void Player::addFov()
{
	fov_ = glm::min(120, fov_+1);
}

void Player::mimusFov()
{
	fov_ = glm::max(10, fov_-1);
}

void Player::SpeedUp(){
	speed_ += 100;
	if (speed_ > 1000)
	{
		speed_ = 2500;
	}
		//glm::min(MAXIMUM_PLAYER_SPPED,speed_+1);
}

void Player::SpeedDown(){
	if (speed_ == 0) {
		rotation_angle_ = (float)(rotation_angle_ + radians(180.0));
		speed_ = 100;
	} else {
		speed_ = speed_ - 100;
		//speed_ = glm::max(0,speed_-1);
	}
}

vec3 Player::getViewPosition()const{
	return position_;
}
GLint Player::getFov()const {
	return fov_;
}

vec3 Player::getMovingDirection()const {
	return moving_direction_;
}

void Player::Update(GLfloat new_elapsed_time_, BallController & ballcontroller, ivec2 window_size, ivec2 mouse_position)
{
	GLfloat delta_time = new_elapsed_time_ - last_elapsed_time_;
	
// added support for mouse position base speed ups
	int h_2 = window_size.y / 2;
	int h_5 = h_2 / 5;
	
	if (mouse_position.y > h_2)
	{
		multiplier_ = 0;
	}
	if (mouse_position.y < h_2 && mouse_position.y > (h_2 - h_5))
	{
		multiplier_ = 5;
	}
	if (mouse_position.y < (h_2 - h_5) && mouse_position.y > (h_2 - (2 * h_5)))
	{
		multiplier_ = 10;
	}
	if (mouse_position.y < (h_2 - (2 * h_5)) && mouse_position.y > (h_2 - (3 * h_5)))
	{
		multiplier_ = 15;
	}
	if (mouse_position.y < (h_2 - (3 * h_5)) && mouse_position.y > (h_2 - (4 * h_5)))
	{
		multiplier_ = 20;
	}
	if (mouse_position.y < (h_2 - (4 * h_5)) && mouse_position.y > (h_2 - (5 * h_5)))
	{
		multiplier_ = 25;
	}

	speed_ = multiplier_ * 100;

	if (speed_ > 2500)
	{
		speed_ = 2500;
	}

	GLfloat delta_x = (float)mouse_position.x - window_size.x/2;
	GLfloat delta_angle = (float)(delta_x * 0.0013 * delta_time);

	rotation_angle_ = (float)(rotation_angle_ + delta_angle - ((int)((rotation_angle_ + delta_angle)/radians(360.0)))*radians(360.0));
	moving_direction_ = vec3(cos(rotation_angle_),0,sin(rotation_angle_));

	// Need to determine what happened during this time period
	GLfloat delta_time_to_process = delta_time;

	std::vector<int> oldBallStatus;
	for (unsigned int i = 0; i < ballcontroller.balls.size(); i ++) {
		oldBallStatus.push_back(ballcontroller.balls[i].status);
	}

	// Here should be a while loop to use all delta_time_to_process
	// Get the time for the nearest ball hit
	while (delta_time_to_process > 0){
		int hitball_index = -1;
		GLfloat next_time_to_hitball = getNearestBallToHit(hitball_index, ballcontroller);
		int wall_index = -1;
		// Get the time for the nearest wall hit
		GLfloat next_time_to_hitwall = getNearestWallToHit(wall_index,5280);
		assert(next_time_to_hitwall > 0);

		// Just go straight forward at current direction
		if (delta_time_to_process < glm::min((float)next_time_to_hitball, (float)next_time_to_hitwall)) {
			break;
		}

		if (next_time_to_hitwall < next_time_to_hitball) {
			delta_time_to_process -= next_time_to_hitwall;
			// Do the wall hitting
			// Update the player position and moving direction
			UpdateAfterWallHit(wall_index,next_time_to_hitwall);
			ballcontroller.updateTimer(next_time_to_hitwall);
		} else {
			delta_time_to_process -= next_time_to_hitball;
			// Do the ball hitting
			// Update the status for the hitted ball
			assert(hitball_index != -1);
			// add the timer update here
			// Update the player position and moving direction
			UpdateAfterBallHit(ballcontroller,hitball_index,next_time_to_hitball);
			ballcontroller.updateTimer(next_time_to_hitball);
			ballcontroller.balls[hitball_index].status = 1;
			ballcontroller.balls[hitball_index].time = (float)30 + 2* ballcontroller.balls.size();
		}
	}

	if (delta_time_to_process > 0) {
		position_.x += moving_direction_.x*speed_*delta_time_to_process;
		position_.z += moving_direction_.z*speed_*delta_time_to_process;
		ballcontroller.updateTimer(delta_time_to_process);
	}

	
	ballcontroller.needGodViewUpdate = false;
	for (unsigned int i = 0; i < ballcontroller.balls.size(); i ++)
	{
		if (ballcontroller.balls[i].status != oldBallStatus[i]) {
			ballcontroller.needGodViewUpdate = true;
			break;
		}
	}

	last_elapsed_time_ = new_elapsed_time_;
	
}

float Player::getNearestBallToHit(int & ball_index, BallController & ballcontroller) {
	ball_index = -1;
	GLfloat a = moving_direction_.x;
	GLfloat b = moving_direction_.z;
	GLfloat na = -moving_direction_.z;
	GLfloat nb = -moving_direction_.x;
	GLfloat xp = position_.x;
	GLfloat yp = position_.z;

	GLfloat cmid = na*position_.x + nb*position_.z;
	GLfloat c1 = 100 * sqrt(na*na+nb*nb) + cmid;
	GLfloat c2 = cmid - 100 * sqrt(na*na+nb*nb);

	if (c1 < c2) {
		swap(c1, c2);
	}

	float minTime = FLT_MAX;
	for (unsigned int i = 0; i < ballcontroller.balls.size(); i ++) {
		GLfloat xb = ballcontroller.balls[i].position.x;
		GLfloat yb = ballcontroller.balls[i].position.y;
		GLfloat  c = na*xb + nb*yb;
		if (c < c1 && c > c2) {
			GLfloat direction = (xb-xp)*a + (yb-yp)*b;
			// Abandon negative direction
			if (direction <= 0)
				continue;
			GLfloat timetohit = 0;
			// Below is basically the math to compute the time to hit this i-th ball
			GLfloat d = abs(c-cmid)/sqrt(na*na+nb*nb);
			GLfloat l1 = sqrt(10000-d*d);
			GLfloat d0 = (xp-xb)*(xp-xb) + (yp-yb)*(yp-yb);
			GLfloat l = sqrt(d0-d*d);
			timetohit = (l-l1)/speed_;
			if (timetohit < minTime) {
				ball_index = i;
				minTime = timetohit;
			}
		}
	}
	return minTime;
}

float Player::getNearestWallToHit(int & wall_index, int wall_width) {
	GLfloat a = moving_direction_.x;
	GLfloat b = moving_direction_.z;

	GLfloat na = -moving_direction_.z;
	GLfloat nb = moving_direction_.x;

	GLfloat cmid = na*position_.x + nb*position_.z;

	// The actual width should take the size of ball into consideration
	wall_width = wall_width - 100;

	

	// Check for four special corners
	if (abs(a-b) <= 0.001) {
		if (a>=0) {
			wall_index = 1;
			return (wall_width/2-position_.x)/speed_/a;
		} else {
			wall_index = 5;
			return (-wall_width/2-position_.x)/speed_/a;
		}
	}

	if (abs(a+b) <= 0.001) {
		if (a>=0) {
			wall_index = 7;
			return (wall_width/2-position_.x)/speed_/a;
		} else {
			wall_index = 3;
			return (-wall_width/2-position_.x)/speed_/a;
		}
	}

	// Now take care of these more general cases, that hit the ball on the wall
	GLfloat intersect_with_0 = (cmid - na*wall_width/2)/nb;
	GLfloat intersect_with_2 = (cmid - nb*wall_width/2)/na;
	GLfloat intersect_with_4 = (cmid + na*wall_width/2)/nb;
	GLfloat intersect_with_6 = (cmid + nb*wall_width/2)/na;

	if (intersect_with_0 >= -wall_width/2 && intersect_with_0 <= wall_width/2 && a > 0) {
		wall_index = 0;
		return (wall_width/2-position_.x)/speed_/a;
	}

	if (intersect_with_2 >= -wall_width/2 && intersect_with_2 <= wall_width/2 && b > 0) {
		wall_index = 2;
		return (wall_width/2-position_.z)/speed_/b;
	}

	if (intersect_with_4 >= -wall_width/2 && intersect_with_4 <= wall_width/2 && a < 0) {
		wall_index = 4;
		return (-wall_width/2-position_.x)/speed_/a;
	}

	if (intersect_with_6 >= -wall_width/2 && intersect_with_6 <= wall_width/2 && b < 0) {
		wall_index = 6;
		return (-wall_width/2-position_.z)/speed_/b;
	}



	return -1;
}

void Player::UpdateAfterBallHit(BallController & ballcontroller, int ball_index, GLfloat travel_time) {
	assert(ball_index != -1);
	GLfloat dist = travel_time * speed_;
	position_.x += dist*moving_direction_.x;
	position_.z += dist*moving_direction_.z;

	vec3 ball_position = vec3(ballcontroller.balls[ball_index].position.x, 50, ballcontroller.balls[ball_index].position.y);

	vec3 normal = position_ - ball_position;
	GLfloat length = sqrt(normal.x * normal.x  + normal.y * normal.y + normal.z * normal.z);
	normal = vec3(normal.x/length, normal.y/length, normal.z/length);

	UpdateMovingDirectionAfterBumping(normal);
	

}

void Player::UpdateAfterWallHit(int wall_index, GLfloat travel_time) {
	assert(wall_index != -1);
	GLfloat dist = travel_time * speed_;
	position_.x += dist*moving_direction_.x;
	position_.z += dist*moving_direction_.z;

	switch(wall_index) {
	case 1:
		rotation_angle_ = (float)radians(180.0+45);
		moving_direction_ = vec3(cos(rotation_angle_), 0, sin(rotation_angle_));
		break;
	case 3:
		rotation_angle_ = (float)radians(360.0-45);
		moving_direction_ = vec3(cos(rotation_angle_), 0, sin(rotation_angle_));
		break;
	case 5:
		rotation_angle_ = (float)radians(45.0);
		moving_direction_ = vec3(cos(rotation_angle_), 0, sin(rotation_angle_));
		break;
	case 7:
		rotation_angle_ = (float)radians(135.0);
		moving_direction_ = vec3(cos(rotation_angle_), 0, sin(rotation_angle_));
		break;
	case 0:
		UpdateMovingDirectionAfterBumping(vec3(-1,0,0));
		break;
	case 2:
		UpdateMovingDirectionAfterBumping(vec3(0,0,-1));
		break;
	case 4:
		UpdateMovingDirectionAfterBumping(vec3(1,0,0));
		break;
	case 6:
		UpdateMovingDirectionAfterBumping(vec3(0,0,1));
		break;
	default:
		std::cerr << "Invalid wall index\n";
		break;
	}
}

void Player::UpdateMovingDirectionAfterBumping(vec3 normal) {
	moving_direction_ = moving_direction_ - 2*(moving_direction_.x*normal.x + moving_direction_.z*normal.z)*normal;
	UpdateRotationAngleBasedonPosition();
	moving_direction_ = vec3(cos(rotation_angle_), 0, sin(rotation_angle_));
}

void Player::UpdateRotationAngleBasedonPosition() {
	if (moving_direction_.x == 0) {
		if (moving_direction_.z > 0) {
			rotation_angle_ = (float)radians(90.0);
		}else{
			rotation_angle_ = (float)radians(270.0);
		}
	}
	if (moving_direction_.x > 0) {
		if (moving_direction_.z>=0) {
			rotation_angle_ = (float)atan(moving_direction_.z/moving_direction_.x);
		} else {
			rotation_angle_ = (float)(atan(moving_direction_.z/moving_direction_.x) + radians(360.0));
		}
	} else {
		if (moving_direction_.z >=0) {
			rotation_angle_ = (float)(atan(moving_direction_.z/moving_direction_.x) + radians(180.0));
		} else {
			rotation_angle_ = (float)(atan(moving_direction_.z/moving_direction_.x) + radians(180.0));
		}
	}
}

