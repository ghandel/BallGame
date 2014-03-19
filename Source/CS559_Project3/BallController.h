#pragma once

#include "TestBall.h"
#include <vector>
#include <time.h>
#include <stdlib.h>

struct BallStatus{
	GLfloat time;
	vec2 position;
	GLuint status; // 0-unhit, 1-hit
	BallStatus(GLfloat t, vec2 p, GLuint s)
		:time(t), position(p), status(0) {

	}
};

class BallController
{
public:
	BallController(GLint num, GLint width);
	void Initialize(GLint num);
	void Draw(
		TestBall & ball_to_draw,
		Shader & shader, 
		const glm::mat4& projection, 
		glm::mat4 modelview, 
		const glm::ivec2 & size, 
		const Light & light,
		GLint with_texture,
		GLint with_attenuation,
		Shader & noise_shader,
		const float time = 0
	);
	bool CollideWithOtherBall(vec2 position, int threshold=10000)const;
	void updateTimer(GLfloat elapsed_time);
	vec2 getRandomPosition()const;
	std::vector<BallStatus> balls;
	int getTotalBall()const {return balls.size();}
	int getHittedBallNumber()const {int n = 0;for (unsigned int i = 0; i < balls.size(); i ++){if(balls[i].status == 1)n++;} return n;}

	bool needGodViewUpdate;
private:
	GLint num_; // The number of balls in the arena
	GLint width_; // The width of the square arena

};