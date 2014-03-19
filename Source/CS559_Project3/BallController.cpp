#include "BallController.h"

BallController::BallController(GLint num, GLint width) 
	:num_(num), width_(width)
{

}

void BallController::Initialize(GLint num)
{
	//srand(time(0));
	num_ = num;
	for (GLint i = 0; i < num_; i ++) {
		vec2 position = getRandomPosition();
		while (CollideWithOtherBall(position,40000)) {
			position = getRandomPosition();
		}
		balls.push_back(BallStatus(0,position,0));
	}
	needGodViewUpdate = false;
}

void BallController::updateTimer(GLfloat elapsed_time) {
	for (unsigned int i = 0; i < balls.size(); i ++) {
		if (balls[i].status == 1) {
			if (balls[i].time < elapsed_time) {
				balls[i].status = 0;
				balls[i].time = 0;
			} else {
				balls[i].time -= elapsed_time;
			}
		}
	}
}

void BallController::Draw(
	TestBall & ball_to_draw,
	Shader & shader, 
	const glm::mat4& projection, 
	glm::mat4 modelview, 
	const glm::ivec2 & size, 
	const Light & light, 
	GLint with_texture, 
	GLint with_attenuation,
	Shader & noise_shader,
	const float time /* = 0 */ 
) {
	for (unsigned int i = 0; i < balls.size(); i ++)
	{
		glm::mat4 translated_modelview = glm::translate(modelview, vec3(balls[i].position.x, 0, balls[i].position.y));
		if (balls[i].status == 0) {
			ball_to_draw.textid_ = 11;
			ball_to_draw.Draw(
				noise_shader,
				projection,
				translated_modelview,
				size,
				light,
				with_texture,
				with_attenuation,
				time
				);
		} else {
			ball_to_draw.textid_ = 2;
			ball_to_draw.Draw(
				shader,
				projection,
				translated_modelview,
				size,
				light,
				with_texture,
				with_attenuation,
				time
			);
		}

		
	}
}

bool BallController::CollideWithOtherBall(vec2 position, int threshold)const {
	bool collides = false;
	for (unsigned int i = 0; i < balls.size(); i ++) {
		GLfloat dx = balls[i].position.x - position.x;
		GLfloat dy = balls[i].position.y - position.y;
		if (dx*dx + dy * dy <= threshold) {
			collides = true;
			break;
		}
	}
	return collides;
}


vec2 BallController::getRandomPosition()const{
	

	GLint interval = width_ - 100+1;

	GLint x = rand() % interval - width_/2 + 50;
	GLint y = rand() % interval - width_/2 + 50;

	return vec2(x,y);

}