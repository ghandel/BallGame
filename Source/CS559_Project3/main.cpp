#include <iostream>

/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:	2/26/13 - removed support for multiple windows.
*/

#include <iostream>
#include <assert.h>
#include <vector>
#include <map>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Shader.h"
#include "Pile.h"
#include "TestBall.h"
#include "util.h"
#include "Skybox.h"
#include "ArenaWall.h"
#include "BallController.h"
#include "Player.h"
#include "globals.h"
#include "fbo.h"
#include "Jumbotron.h"
#include "JumbotronScreen.h"
#include "GodView.h"
#include "TextManager.h"
#include "Status.h"
#include "Timer.h"
GLuint nDisplayMode = 5;

struct Window
{
	int window_handle;
	glm::ivec2 size;
	glm::ivec2 mouse_position;
	float window_aspect;
	bool wireframe;
	GLuint shader_mode;
	GLuint display_mode;
	GLint with_texture;
	GLint with_attenuation;
	bool isPaused;
	bool inited;
	int difficulty;
	int upside_down;
	int hard_mode;
} window;

Camera camera;
Pile pile;
std::map<std::string, Shader> shaders;
//Stool stool;
Light light;
TestBall ball;
Skybox skybox;
//LoadedObject monkey("suzanne.obj");
ArenaWall wall;
TestBall ball_to_draw;
Player player;
GodView godview;
TextManager textmanager;
Status status;
Timer timer;

BallController ballcontroller(10,2640*2);
FrameBufferObject fbo;
Jumbotron jumbotron;
JumbotronScreen screen;

void CloseFunc()
{
	window.window_handle = -1;
}

void ReshapeFunc(int w, int h)
{
	if (window.window_handle != -1 &&  h > 0)
	{
		window.size = glm::ivec2(w, h);
		window.window_aspect = float(w) / float(h);
	}
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	if (window.window_handle == -1)
		return;

	switch (c)
	{
	case 'a':
	case 'A':
		window.with_attenuation = (window.with_attenuation+1)%2;
		break;
	case 's':
	case 'S':
		window.shader_mode = (window.shader_mode + 1)%3;
		break;
	case 't':
	case 'T':
		window.with_texture = (window.with_texture + 1)%2;
		break;
	case 'w':
	case 'W':
		window.wireframe = !window.wireframe;
		break;
	case '+':
	case '=':
		//camera.minusFov();
		player.mimusFov();
		break;
	case '_':
	case '-':
		//camera.addFov();
		player.addFov();
		break;
	case 'P':
	case 'p':
		window.isPaused = !window.isPaused;
		break;
	case 'D':
	case 'd':
		window.difficulty++;
		if (window.difficulty > 3)
		{
			window.difficulty = 0;
		}
		switch (window.difficulty)
		{
		case 0:
			window.upside_down = 1;
			window.hard_mode = 0;
			break;
		case 1:
			window.upside_down = -1;
			window.hard_mode = 0;
			break;
		case 2:
			window.upside_down = 1;
			window.hard_mode = -1;
			break;
		case 3:
			window.upside_down = -1;
			window.hard_mode = -1;
			break;
		}
		break;
	case 'x':
	case 27:
		std::cout << "You lose the game!" << std::endl;
		glutLeaveMainLoop();
		return;
	}
}

// Callback function for up/down/left/right key
void processSpecialKeys(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		camera.addYaw();
		break;
	case GLUT_KEY_RIGHT:
		camera.minusYaw();
		break;
	case GLUT_KEY_UP:
		//camera.addPitch();
		player.SpeedUp();
		break;
	case GLUT_KEY_DOWN:
		//camera.minusPitch();
		player.SpeedDown();
		break;
	case GLUT_KEY_F1:
		window.display_mode = (window.display_mode+1)%nDisplayMode;
		break;
	default:
		break;
	}
}

void mouse_moved(int x, int y) {
	window.mouse_position = ivec2(x,y);
}


void drawScene(glm::mat4 modelview_matrix, glm::mat4 projection_matrix, float time, std::string shader_type){
	glm::mat4 ballModelView;
	glm::mat4 monkeyModelView;

	pile.textid_ = 1;

	ball.textid_ = 9;
	ballModelView = translate(modelview_matrix, vec3(-200,200,-200));
	ballModelView = scale(modelview_matrix, vec3(5,5,5));
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000;
	
	glm::mat4 skybox_projection_matrix = glm::perspective(80.0f, window.window_aspect, 10.0f,5000.0f); 
	glm::mat4 View       = glm::mat4(1.0f);
	glm::mat4 skybox_modelview_matrix      = glm::scale(glm::mat4(1.0f),glm::vec3(2800,2800,2800));
	GLfloat skybox_rotate_angle = (float)(player.rotation_angle_*(180/radians(180.0)));
	skybox_modelview_matrix = skybox_modelview_matrix*rotate(glm::mat4(1.0f),skybox_rotate_angle,glm::vec3(0,1,0));

	
		glDisable(GL_DEPTH_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		// Draw 1 into the stencil buffer.
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		// Draw the pile to set the stencil buffer
		pile.Draw(shaders["phong"],projection_matrix,modelview_matrix,window.size,light,window.with_texture,window.with_attenuation,0);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		// Now, only render where stencil is set to 1. 
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);  // draw if stencil ==1 
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		// Draw all the reflection part only in the pile part
		glm::mat4 reflect_model_view_matrix = glm::scale(modelview_matrix,vec3(1,-1,1));
		wall.Draw(shaders["phong_reflect"], projection_matrix, reflect_model_view_matrix , window.size, light, window.with_texture,window.with_attenuation, 0);
		GLfloat angle = 0;
		glm::mat4 jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
		jumbotron_model_view = translate(jumbotron_model_view, vec3(2660,0,0));
		jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
		jumbotron.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

		
		angle = 90;
		//jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
		jumbotron_model_view = translate(modelview_matrix, vec3(0,0,-2660));
		jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
		jumbotron_model_view = rotate(jumbotron_model_view,angle, vec3(0,1,0));
		jumbotron.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

		angle = -180;
		//jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
		jumbotron_model_view = translate(modelview_matrix, vec3(-2660,0,0));
		jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
		jumbotron_model_view = rotate(jumbotron_model_view,angle, vec3(0,1,0));
		jumbotron.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

		angle = 270;
		//jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
		jumbotron_model_view = translate(modelview_matrix, vec3(0,0,2660));
		jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
		jumbotron_model_view = rotate(jumbotron_model_view,angle, vec3(0,1,0));
		jumbotron.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);
		
		
		ballcontroller.Draw(ball_to_draw,shaders["phong_reflect"], projection_matrix, reflect_model_view_matrix, window.size, light, window.with_texture,window.with_attenuation,shaders["phong_perlin_reflect"], current_time);
		skybox.Draw(shaders["skybox_reflect"], skybox_projection_matrix, scale(skybox_modelview_matrix,vec3(1,-1,1)), window.size, light, window.with_texture,window.with_attenuation,0);


		glDisable(GL_STENCIL_TEST);
	
		wall.Draw(shaders["phong"], projection_matrix, modelview_matrix , window.size, light, window.with_texture,window.with_attenuation, 0);
		angle = 0;
		jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
		jumbotron_model_view = translate(jumbotron_model_view, vec3(2660,0,0));
		jumbotron.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);
		
		angle = 90;
		jumbotron_model_view = translate(modelview_matrix, vec3(0,0,-2660));
		jumbotron_model_view = rotate(jumbotron_model_view, angle, vec3(0,1,0));		
		jumbotron.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

		angle = 180;
		jumbotron_model_view = translate(modelview_matrix, vec3(-2660,0,0));
		jumbotron_model_view = rotate(jumbotron_model_view, angle, vec3(0,1,0));		
		jumbotron.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

		angle = 270;
		jumbotron_model_view = translate(modelview_matrix, vec3(0,0,2660));
		jumbotron_model_view = rotate(jumbotron_model_view, angle, vec3(0,1,0));		
		jumbotron.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);


		ballcontroller.Draw(ball_to_draw,shaders["phong"], projection_matrix, modelview_matrix, window.size, light, window.with_texture,window.with_attenuation,shaders["phong_perlin"], current_time);
		skybox.Draw(shaders["skybox"], skybox_projection_matrix, skybox_modelview_matrix, window.size, light, window.with_texture,window.with_attenuation,0);

		
}


void DisplayFunc()
{
	if (window.window_handle == -1)
		return;

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);

	if (!window.inited) {
		glutWarpPointer(window.size.x/2, window.size.y/2);
		window.inited = true;
	}

	
	// Get the current mouse location and elapsed time to update the player position
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	if (window.isPaused) {
		player.last_elapsed_time_ = current_time;
	} else {
		player.Update(current_time,ballcontroller,window.size,window.mouse_position);
		if (ballcontroller.needGodViewUpdate) {
			godview.UpdateBallStatus(ballcontroller);
		}
	}
	// Projection and modern view matrix
	glm::mat4 projection_matrix = glm::perspective(
		//camera.getFov(),
		(float)player.getFov(),
		window.window_aspect, 
		40.0f, 
		8800.0f
	);

	
	glm::mat4 modelview_matrix = glm::lookAt(player.getViewPosition(), (player.getViewPosition()+player.getMovingDirection()), glm::vec3(1* window.hard_mode,1 * window.upside_down,0));
	glViewport(0, 0, window.size.x, window.size.y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 200.0f;
	
	std::string shader_type = "phong";
	
	glm::mat4 stoolModelView;
	glm::mat4 ballModelView;
	glm::mat4 monkeyModelView;

	glm::mat4 reflect_model_view_matrix = glm::scale(modelview_matrix,vec3(1,-1,1));

	glEnable(GL_DEPTH_TEST);
	//pile.Draw(shaders["phong"],projection_matrix,modelview_matrix,window.size,light,window.with_texture,window.with_attenuation,0);
	drawScene(modelview_matrix,projection_matrix,time,"a");
	glEnable(GL_TEXTURE_2D);
	fbo.Bind( );
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//pile.Draw(shaders["phong"],projection_matrix,modelview_matrix,window.size,light,window.with_texture,window.with_attenuation,0);
	drawScene(modelview_matrix,projection_matrix,time,"a");
	fbo.Unbind();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);
	screen.textid_ = fbo.texture_handles[0];
	//screen.textid_ = textmanager.texture_id;
	GLfloat angle = 0;
	glm::mat4 jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
	jumbotron_model_view = translate(jumbotron_model_view, vec3(2660,0,0));
	
	screen.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);
	
	angle = 90;
	jumbotron_model_view = translate(modelview_matrix, vec3(0,0,-2660));
	jumbotron_model_view = rotate(jumbotron_model_view, angle, vec3(0,1,0));		
	screen.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

	angle = 180;
	jumbotron_model_view = translate(modelview_matrix, vec3(-2660,0,0));
	jumbotron_model_view = rotate(jumbotron_model_view, angle, vec3(0,1,0));		
	screen.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

	angle = 270;
	jumbotron_model_view = translate(modelview_matrix, vec3(0,0,2660));
	jumbotron_model_view = rotate(jumbotron_model_view, angle, vec3(0,1,0));		
	screen.Draw(shaders["phong"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

	
	angle = 0;
	jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
	jumbotron_model_view = translate(jumbotron_model_view, vec3(2660,0,0));
	jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
	screen.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);


	angle = 90;
	//jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
	jumbotron_model_view = translate(modelview_matrix, vec3(0,0,-2660));
	jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
	jumbotron_model_view = rotate(jumbotron_model_view,angle, vec3(0,1,0));
	screen.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

	angle = -180;
	//jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
	jumbotron_model_view = translate(modelview_matrix, vec3(-2660,0,0));
	jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
	jumbotron_model_view = rotate(jumbotron_model_view,angle, vec3(0,1,0));
	screen.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

	angle = 270;
	//jumbotron_model_view = rotate(modelview_matrix, angle, vec3(0,1,0));
	jumbotron_model_view = translate(modelview_matrix, vec3(0,0,2660));
	jumbotron_model_view = scale(jumbotron_model_view,vec3(1,-1,1));
	jumbotron_model_view = rotate(jumbotron_model_view,angle, vec3(0,1,0));
	screen.Draw(shaders["phong_reflect"], projection_matrix, jumbotron_model_view , window.size, light, window.with_texture,window.with_attenuation, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	godview.Draw(shaders["godview"], projection_matrix, modelview_matrix, window.size, light, window.with_texture, window.with_attenuation,0);
	
	char tmp[50];
	_itoa_s((int)current_time,tmp,10);
	std::string status_string = "t:";
	status_string = status_string+ tmp + "s-";
	_itoa_s(ballcontroller.getHittedBallNumber(), tmp, 10);
	status_string = status_string + tmp + "/";
	_itoa_s(ballcontroller.getTotalBall(), tmp, 10);
	status_string = status_string + tmp;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindTexture(GL_TEXTURE_2D, status.textid_);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (unsigned int i = 0; i < status_string.size(); i ++)
	{
		status.UpdateStatus(&status_string[i]);
		status.Draw(shaders["font"], projection_matrix, modelview_matrix, window.size, light, window.with_texture, window.with_attenuation,0);
	
	}
	status.left = (float)-0.97;
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindTexture(GL_TEXTURE_2D, timer.textid_);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (unsigned i = 0; i < ballcontroller.balls.size(); i ++) {
		if (ballcontroller.balls[i].status == 1) {
			int time_to_show = (int)ballcontroller.balls[i].time;
			vec2 ball_position = ballcontroller.balls[i].position;
			timer.position_ = vec3(ball_position.x, 150, ball_position.y);
			vec3 d1 = vec3(ball_position.x - player.position_.x,0, ball_position.y - player.position_.z);
			timer.direction_ = normalize(cross(d1, vec3(0,1,0)));
			timer.font_size = 25;

			_itoa_s(time_to_show, tmp, 10);
			const char * p = NULL;
			for (p = tmp;*p;p++) {
				timer.UpdateStatus(p);
				timer.Draw(shaders["font_float"], projection_matrix, modelview_matrix, window.size, light, window.with_texture, window.with_attenuation,0);
			}
		}

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// Text for the status part
	if (ballcontroller.getHittedBallNumber() == ballcontroller.getTotalBall()) {
		std::cout << "You win!" << std::endl;
		std::cout << "Used time: " << current_time << std::endl;
		glutLeaveMainLoop();
	}


	glutSwapBuffers();
	glutPostRedisplay();

}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 1024);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |GLUT_DEPTH);

	window.wireframe = false;
	window.difficulty = 0;
	window.upside_down = 1;
	window.hard_mode = 0;

	window.window_handle = glutCreateWindow("Modern Hello World");
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(processSpecialKeys);
	glutPassiveMotionFunc(mouse_moved);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	window.mouse_position = vec2(window.size.x/2, window.size.y/2);
	glutWarpPointer(window.mouse_position.x, window.mouse_position.y);
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize." << std::endl;
		return 0;
	}

	// Shader initializations
	Shader s4;
	s4.Initialize("phong_shader.vert", "phong_shader.frag");
	shaders["phong"] = s4;
	window.shader_mode = 0;

	Shader s5;
	s5.Initialize("phong_shader_reflect.vert", "phong_shader_reflect.frag");
	shaders["phong_reflect"] = s5;
	window.shader_mode = 0;

	Shader s6;
	s6.Initialize("skybox_shader.vert", "skybox_shader.frag");
	shaders["skybox"] = s6;
	window.shader_mode = 0;

	Shader s7;
	s7.Initialize("skybox_shader_reflect.vert", "skybox_shader_reflect.frag");
	shaders["skybox_reflect"] = s7;

	Shader s8;
	s8.Initialize("godview.vert", "godview.frag");
	shaders["godview"] = s8;

	Shader s9;
	s9.Initialize("phong_shader_perlin.vert", "phong_shader_perlin.frag");
	shaders["phong_perlin"] = s9;

	Shader s10;
	s10.Initialize("phong_shader_perlin_reflect.vert", "phong_shader_perlin_reflect.frag");
	shaders["phong_perlin_reflect"] = s10;

	Shader s11;
	s11.Initialize("font_shader.vert", "font_shader.frag");
	shaders["font"] = s11;

	Shader s12;
	s12.Initialize("font_float_shader.vert", "font_float_shader.frag");
	shaders["font_float"] = s12;

	Shader s13;
	s13.Initialize("flat_shader.vert", "flat_shader.frag");
	shaders["flat"] = s13;
	
	Shader s14;
	s13.Initialize("basic_shader.vert", "basic_shader.frag");
	shaders["basic"] = s14;
	
	Shader s15;
	s13.Initialize("brick_shader.vert", "brick_shader.frag");
	shaders["brick"] = s15;
	
	Shader s16;
	s13.Initialize("colored_shader.vert", "colored_shader.frag");
	shaders["colored"] = s16;

	Shader s17;
	s13.Initialize("gouraud_shader.vert", "gouraud_shader.frag");
	shaders["gouraud"] = s17;

	// display mode
	window.with_texture = 1;
	window.display_mode = 4;
	window.with_attenuation = 0;
	window.isPaused = false;

	// Object initializations
	pile.Initialize();
	ball.Initialize();
	wall.Initialize();
	if (argc >=2) {
		ballcontroller.Initialize(atoi(argv[1]));
	} else {
		ballcontroller.Initialize(10);
	}
	if (argc >=3) {
		srand(atoi(argv[2]));
	} else {
		srand(0);
	}
	// God view initialzied after ballcontroller
	godview.Initialize(&ballcontroller);
	ball_to_draw.Initialize();
	player.Initialize(ballcontroller);
	wall.textid_ = 3;
	jumbotron.Initialize();
	jumbotron.textid_ = 1;
	
	// Texture initializations
	TextureManager::Inst()->LoadTexture("uvmap.tga", 0);
	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);

	TextureManager::Inst()->LoadTexture("floor.jpg", 1, GL_BGR, GL_RGB);
	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);

	TextureManager::Inst()->LoadTexture("earth.jpg", 2, GL_BGR, GL_RGB);
	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);

	TextureManager::Inst()->LoadTexture("iron.jpg", 3, GL_BGR, GL_RGB);
	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);

	std::vector<std::string> cube_files;
	
	cube_files.push_back("stormy_days\\stormydays_lf.jpg");
	cube_files.push_back("stormy_days\\stormydays_rt.jpg");
	cube_files.push_back("stormy_days\\stormydays_up.jpg");
	cube_files.push_back("stormy_days\\stormydays_dn.jpg");
	cube_files.push_back("stormy_days\\stormydays_ft.jpg");
	cube_files.push_back("stormy_days\\stormydays_bk.jpg");
	
	GLuint cube_tex_id = 4;
	TextureManager::Inst()->LoadCubeTexture(cube_files,cube_tex_id,GL_BGR, GL_RGB);
	glDisable(GL_TEXTURE_CUBE_MAP);
	
	TextureManager::Inst()->LoadTexture("silver.jpg", 11, GL_BGR, GL_RGB);
	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
	
	if (!fbo.Initialize(glm::ivec2(1000,1000), 1, true)) {
		std::cerr << "Frame buffer failed to initialize\n";
		return 0;
	}

	screen.Initialize();
	
	GLuint skybox_texture_id = 4;
	skybox.Initialize(skybox_texture_id);

	player.last_elapsed_time_ = (float)glutGet(GLUT_ELAPSED_TIME)/1000;
	window.inited = false;

	status.Initialize();
	timer.Initialize();
	glutMainLoop();

	// Release used memory
	pile.TakeDown();
	ball.TakeDown();
	skybox.TakeDown();
	wall.TakeDown();
	ball_to_draw.TakeDown();
	jumbotron.TakeDown();
	screen.TakeDown();
	fbo.TakeDown();
	godview.TakeDown();
	status.TakeDown();
	timer.TakeDown();
	
	// receive close command
	std::cout << "Press any letter and enter to close" << std::endl;
	char ch;
	std::cin >> ch;

	exit(0);
}
