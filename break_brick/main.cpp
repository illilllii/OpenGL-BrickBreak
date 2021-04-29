#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

#define PI 3.1415926
#define NGon 100
#define Ball_radius 0.25
#define Left -10
#define Right 10
#define Top 10
#define Bottom -10

float Cx, Cy, Dx, Dy;
float life_Cx, life_Cy;
float delta_theta;
int move_state;
int bar_move_state;//0일 때 stop, 1일 때 move
float bar_width, bar_height;
float bar_x, bar_y;
float brick_width, brick_height;
float move_x;
int score;
int life;
float brick_space;

float r_origin;
float g_origin;
float b_origin;

float r_1;
float g_1;
float b_1;

float r_2;
float g_2;
float b_2;

float r_i;
float g_i;
float b_i;

int sum;
int sum2;


struct Point2d {
	float x;
	float y;
};
struct Point3d {
	float x;
	float y;
	float z;
};
struct Color {
	float r;
	float g;
	float b;
};
struct Item {
	float x1, x2, y1, y2;
	int state;
};
Item *item;
int item_count;
struct Brick
{
	Color color;
	int state;
	int collision;
	float x1, x2, y1, y2;
};
Brick *brick;
int total;

int brick1_total;
int brick2_total;
int bricktotal;

void init();
void DrawAxis();
void DrawCircle();
void DrawLife();
void DrawArc();
void Display();
void Reshape(int w, int h);
void Key(unsigned char key, int x, int y);
void Specialkey(int key, int x, int y);
void DrawBrick();
void CheckBrick();
void GameOver();



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(700, 700);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutSpecialFunc(Specialkey);
	glutIdleFunc(Display);
	glutMainLoop();
}
void init() {
	brick_space = 0.15;

	r_origin = 1.00;
	g_origin = 1.00;
	b_origin = 0.47;

	r_1 = 1.00;
	g_1 = 0.65;
	b_1 = 0.33;

	r_2 = 1.00;
	g_2 = 0.33;
	b_2 = 0.33;

	r_i = 0.5;
	g_i = 0.5;
	b_i = 0.5;

	bar_x = -1.5;
	bar_y = -7.7;
	bar_width = 3;
	bar_height = -0.3;
	brick_width = 1;
	brick_height = -1;
	Dx = 0.00;
	Dy = 0.12;
	Cx = ((bar_x + bar_width) + bar_x) / 2;
	Cy = bar_y + (Ball_radius)-Dy + 0.05;
	life_Cx = Right - Ball_radius * 9 - 0.8;
	life_Cy = Bottom + 0.2 + Ball_radius;

	score = 0;
	life = 3;
	move_x = 0.0;
	delta_theta = 2 * PI / NGon;
	move_state = 0;
	bar_move_state = 0;
	total = 96;
	brick1_total = 10;
	brick2_total = 5;
	bricktotal = 12;
	sum = 37;
	sum2 = 53;
	brick = new Brick[total];
	srand((unsigned)time(NULL));
	int brick_1[10];
	int brick_2[5];
	int brick_brick[12];

	for (int j = 0; j < brick1_total; j++)
	{
		brick_1[j] = rand() % total;
	}
	for (int j = 0; j < brick2_total; j++)
	{
		brick_2[j] = rand() % total;
	}

	for (int j = 0; j < 6; j++)
	{

		brick_brick[j] = sum;
		sum++;

	}
	for (int j = 6; j < 12; j++) {
		brick_brick[j] = sum2;
		sum2++;
	}

	for (int i = 0; i < total; i++) {
		brick[i].state = 0;
		brick[i].color.r = r_origin;
		brick[i].color.g = g_origin;
		brick[i].color.b = b_origin;
		for (int j = 0; j < brick1_total; j++) {

			brick[brick_1[j]].color.r = r_1;
			brick[brick_1[j]].color.g = g_1;
			brick[brick_1[j]].color.b = b_1;
		}
		for (int j = 0; j < brick2_total; j++) {

			brick[brick_2[j]].color.r = r_2;
			brick[brick_2[j]].color.g = g_2;
			brick[brick_2[j]].color.b = b_2;
		}
		for (int j = 0; j < bricktotal; j++) {

			brick[brick_brick[j]].color.r = r_i;
			brick[brick_brick[j]].color.g = g_i;
			brick[brick_brick[j]].color.b = b_i;
		}

		if (brick[i].color.r == r_origin && brick[i].color.g == g_origin && brick[i].color.b == b_origin) {
			brick[i].collision = 1;
		}
		if (brick[i].color.r == r_1 && brick[i].color.g == g_1 && brick[i].color.b == b_1) {
			brick[i].collision = 2;
		}
		if (brick[i].color.r == r_2 && brick[i].color.g == g_2 && brick[i].color.b == b_2) {
			brick[i].collision = 3;
		}
		if (brick[i].color.r == r_i && brick[i].color.g == g_i && brick[i].color.b == b_i) {
			brick[i].collision = 4;
		}

	}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}
void print_bitmap_string(const char* s)
{
	while (*s) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
		s++;
	}

}
void DrawStuff()
{
	char buff[256];            // 변환될 int형 변수를 저장할 공간
	sprintf_s(buff, "%d", score);        // 변환시킬 변수가 저장될 공간, 서식, 저장될 값

	const char* score_number = { buff };
	const char* score_ = { "Score:" };
	const char* life_ = { "Life:" };
	const char* game_over = { "GAME OVER" };

	GLfloat x, y, ystep, yild, stroke_scale;


	yild = 0.2;
	if (life > 0) {
		glColor3f(0.7, 0.0, 0.0);
		glRasterPos2f(-8, Bottom + yild);
		print_bitmap_string(score_number);

		glColor3f(0.0, 0.0, 0.0);
		glRasterPos2f(Left, Bottom + yild);
		print_bitmap_string(score_);
		glRasterPos2f(Right - 5, Bottom + yild);
		print_bitmap_string(life_);
	}
	else {
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos2f(-1, 0);
		print_bitmap_string(game_over);

		glRasterPos2f(-1, -2);
		print_bitmap_string(score_);

		glColor3f(0.7, 0.0, 0.0);
		glRasterPos2f(1.5, -2);
		print_bitmap_string(score_number);
	}


}
void DrawAxis() {
	glColor3f(0.3f, 0.3f, 0.3f);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex2f(Left, 0.0);
	glVertex2f(Right, 0.0);
	glVertex2f(0.0, Bottom);
	glVertex2f(0.0, Top);
	glEnd();
}
void DrawBrick() {


	for (int i = 0; i < total; i++) {

		brick[0].x1 = -9;
		brick[0].x2 = brick[0].x1 + brick_width;
		brick[0].y1 = 9;
		brick[0].y2 = brick[0].y1 + brick_height;

		if (i < total - 1) {

			brick[i + 1].x1 = brick[i].x2;
			brick[i + 1].x2 = brick[i].x2 + brick_width;
			brick[i + 1].y1 = brick[i].y1;
			brick[i + 1].y2 = brick[i].y2;

			glColor3f(brick[i].color.r, brick[i].color.g, brick[i].color.b);

			if ((brick[i].x2) > 9 - i * brick_space)
			{
				brick[i + 1].x1 = -9 - (brick_space * (i + 1));
				brick[i + 1].x2 = brick[i + 1].x1 + brick_width;
				brick[i + 1].y1 = brick[i].y2 - brick_space;
				brick[i + 1].y2 = brick[i].y2 + brick_height - brick_space;
			}


		}
		if (brick[i].state == 0) {

			glBegin(GL_POLYGON);
			glVertex2f(brick_space * i + brick[i].x1, brick[i].y1);
			glVertex2f(brick_space * i + brick[i].x1, brick[i].y2);
			glVertex2f(brick_space * i + brick[i].x2, brick[i].y2);
			glVertex2f(brick_space * i + brick[i].x2, brick[i].y1);
			glEnd();

		}


	}


}

void DrawCircle() {
	float speed;
	float distance_x, distance_y;
	float Nx, Ny;
	float NDx, NDy;
	float A;
	float tri_width;
	float tri_height;
	float slope;
	glColor3f(0.62, 0.80, 0.64);

	delta_theta = 2 * PI / NGon;
	glPointSize(5.0);

	if (move_state == 1) {
		if ((Left >= Cx - Ball_radius) ||
			(Cx + Ball_radius >= Right - 0.01))
			Dx *= -1.0;
		if (Cy + Ball_radius >= Top)
			Dy *= -1.0;
		if (Bottom + 0.5 >= Cy - Ball_radius) {
			Cy = bar_y + Ball_radius - 0.2;
			Cx = ((bar_x + bar_width) + bar_x) / 2;
			if (life > 0) life--;
			move_x = 0;
			move_state = 0;
			bar_move_state = 0;
		}

		if (Cy - Ball_radius <= bar_y && Cx > bar_x + move_x
			&& Cx < bar_x + bar_width + move_x) {
			distance_x = (bar_x + bar_width / 2 + move_x) - Cx;
			distance_y = Cy - bar_y;

			Dx = 0.04;
			Dy = 0.12;
			speed = sqrt((distance_x*distance_x) + (distance_y*distance_y));


			if (Cx < bar_x + bar_width / 2 + move_x) {
				Dy = Dy;
				Dx *= -speed;
			}
			if (Cx > bar_x + bar_width / 2 + move_x) {
				Dx *= speed;
				Dy = Dy;
			}
			if (Cx == bar_x + bar_width / 2 + move_x) {
				Dx = 0.0;
				Dy = Dy;
			}

		}

		CheckBrick();
		Cx += Dx;
		Cy += Dy;



	}

	glBegin(GL_POLYGON);
	for (int i = 0; i < NGon; i++) {
		glVertex2f(Cx + Ball_radius * cos(delta_theta*i), Cy + Ball_radius * sin(delta_theta*i));
	}
	glEnd();

}
void DrawLife()
{
	delta_theta = 2 * PI / NGon;
	glPointSize(5.0);
	switch (life) {
	case 0:
		break;
	case 1:
		glBegin(GL_POLYGON);
		for (int i = 0; i < NGon; i++) {
			glVertex2f(life_Cx + Ball_radius * cos(delta_theta*i), life_Cy + Ball_radius * sin(delta_theta*i));
		}
		glEnd();
		break;
	case 2:
		for (int j = 0; j < 2; j++) {
			glBegin(GL_POLYGON);
			for (int i = 0; i < NGon; i++) {
				glVertex2f(life_Cx + Ball_radius * cos(delta_theta*i) + j * 1, life_Cy + Ball_radius * sin(delta_theta*i));
			}
			glEnd();
		}
		break;
	case 3:
		for (int j = 0; j < 3; j++) {
			glBegin(GL_POLYGON);
			for (int i = 0; i < NGon; i++) {
				glVertex2f(life_Cx + Ball_radius * cos(delta_theta*i) + j * 1, life_Cy + Ball_radius * sin(delta_theta*i));
			}
			glEnd();
		}
		break;
	}

}
void Drawitem() {

	glColor3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < total; i++) {

	}
}




void CheckBrick()
{
	for (int i = 0; i < total; i++)
	{
		if (brick[i].state == 0) {

			if (brick[i].y2 < Cy + Ball_radius && brick[i].y1>Cy - Ball_radius
				&& Cx + Ball_radius >= brick[i].x1 + (i * brick_space) && Cx - Ball_radius <= brick[i].x2 + (i * brick_space)) {
				if (brick[i].collision > 0 && brick[i].collision != 4) brick[i].collision--;
				if (brick[i].collision == 0) brick[i].state = 1;
				Dy *= -1.0;
				if (brick[i].collision != 4) score += 100;
				//Drawitem();
				break;
			}


			switch (brick[i].collision)
			{
			case 1:
				brick[i].color.r = r_origin;
				brick[i].color.g = g_origin;
				brick[i].color.b = b_origin;
				break;
			case 2:
				brick[i].color.r = r_1;
				brick[i].color.g = g_1;
				brick[i].color.b = b_1;
				break;
			case 3:
				brick[i].color.r = r_2;
				brick[i].color.g = g_2;
				brick[i].color.b = b_2;
				break;
			default:
				break;
			}

		}

	}

}
void GameOver()
{
	if (life == 0) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0, 1.0, 1.0, 1.0);

		float delta = 2.0*PI / 5;
		float radius = 3;
		float radius2 = 1.5;
		glPointSize(3.0);
		float cx = 0;
		float cy = 6;

		float cx1 = 3;
		float cy2 = 4;

		glBegin(GL_LINE_LOOP);
		glVertex2f(cx + radius * cos(delta * 0), cy + radius * sin(delta * 0));
		glVertex2f(cx + radius * cos(delta * 2), cy + radius * sin(delta * 2));
		glVertex2f(cx + radius * cos(delta * 4), cy + radius * sin(delta * 4));
		glVertex2f(cx + radius * cos(delta * 6), cy + radius * sin(delta * 6));
		glVertex2f(cx + radius * cos(delta * 8), cy + radius * sin(delta * 8));
		glEnd();

		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(cx1 + radius2 * cos(delta * 0), cy2 + radius2 * sin(delta * 0));
		glVertex2f(cx1 + radius2 * cos(delta * 2), cy2 + radius2 * sin(delta * 2));
		glVertex2f(cx1 + radius2 * cos(delta * 4), cy2 + radius2 * sin(delta * 4));
		glVertex2f(cx1 + radius2 * cos(delta * 6), cy2 + radius2 * sin(delta * 6));
		glVertex2f(cx1 + radius2 * cos(delta * 8), cy2 + radius2 * sin(delta * 8));
		glEnd();

		DrawStuff();
		move_state = 0;

	}
}
void DrawArc() {
	glPointSize(5.0);
	glColor3f(0.0, 0.8, 0.0);

	glBegin(GL_POLYGON);

	glVertex2f(move_x + bar_x, bar_y);
	glVertex2f(move_x + bar_x + bar_width, bar_y);
	glVertex2f(move_x + bar_x + bar_width, bar_y + bar_height);
	glVertex2f(move_x + bar_x, bar_y + bar_height);
	glEnd();
}

void Key(unsigned char key, int x, int y) {
	switch (key) {
	case 27: exit(0); break;
	case 32: if (life > 0) { move_state = 1; bar_move_state = 1; } break;
	}
}
void Specialkey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT: if (bar_move_state == 1)move_x -= 0.4; break;
	case GLUT_KEY_RIGHT: if (bar_move_state == 1) move_x += 0.4; break;
	}
	while (move_x < -8.4) move_x = -8.4;
	while (move_x > 8.4) move_x = 8.4;
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.53, 0.6, 0.53, 0.0);
	DrawBrick();
	DrawArc();
	DrawCircle();
	DrawLife();
	DrawStuff();
	GameOver();
	glFlush();
	glutSwapBuffers();
}
void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(Left, Right, Bottom, Top);
	glMatrixMode(GL_MODELVIEW);
}