/** Project name: PongDS
  * Author: SammygoodTunes
  * Version: 1.0 **/

#include <nds.h>
#include <stdio.h>
#include <math.h>
#include <gl2d.h>
#include <stdlib.h>
#include <maxmod9.h>
#include <time.h>
#include <string.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define DEFAULT_PADDLE_WIDTH 10
#define DEFAULT_PADDLE_HEIGHT 30
#define DEFAULT_BALL_DIAMETER 5
#define MIN_BALL_ANGLE 45
#define MAX_BALL_ANGLE 50

#define MAX_CONSOLE_COLUMNS 32
#define MAX_CONSOLE_LINES 24

#define PI 3.14159265359

typedef struct Paddle
{

	float x;
	float y;
	float x2;
	float y2;
	int width;
	int height;
	int score;
	float speed;

} Paddle;

typedef struct Ball
{

	float x;
	float y;
	float x2;
	float y2;
	int width;
	int height;
	float speed;
	float angle;
	float maxAngle;
	float velocity[2];

} Ball;


void author(int usedLines) {
	const char* authorMessage = "Author: SammygoodTunes";
	for(int i=0; i < MAX_CONSOLE_LINES - usedLines - 1; ++i)
		iprintf("\n");
	for(int i=0; i < MAX_CONSOLE_COLUMNS - strlen(authorMessage); ++i)
		iprintf(" ");
	iprintf(authorMessage);
}


int main(void) {

	srand(time(NULL));

	int key;

	int paused = 1;
	int showPauseMessage = 1;
	int updateScore = 0;

	Paddle player;
	Paddle opponent;
	Ball ball;

	player.x = 0;
	player.y = 0;
	player.width = DEFAULT_PADDLE_WIDTH;
	player.height = DEFAULT_PADDLE_HEIGHT;
	player.x2 = player.x + player.width;
	player.y2 = player.y + player.height;
	player.score = 0;
	player.speed = 3.0;

	opponent.x = SCREEN_WIDTH - DEFAULT_PADDLE_WIDTH;
	opponent.y = 0;
	opponent.width = DEFAULT_PADDLE_WIDTH;
	opponent.height = DEFAULT_PADDLE_HEIGHT;
	opponent.x2 = opponent.x + opponent.width;
	opponent.y2 = opponent.y + opponent.height;
	opponent.score = 0;
	opponent.speed = 3.0;

	ball.x = SCREEN_WIDTH / 2;
	ball.y = SCREEN_HEIGHT / 2;
	ball.width = ball.height = DEFAULT_BALL_DIAMETER;
	ball.x2 = ball.x + ball.width;
	ball.y2 = ball.y + ball.height;
	ball.speed = 1.75;
	ball.angle = MIN_BALL_ANGLE + rand() % (MAX_BALL_ANGLE + 1 - MIN_BALL_ANGLE);
	ball.maxAngle = 2 * PI / 12;
	ball.velocity[0] = ball.speed * cos(ball.angle);
	ball.velocity[1] = ball.speed * -sin(ball.angle);

	videoSetMode(MODE_5_3D);
	consoleDemoInit();
	glScreen2D();

	mmInitDefaultMem((mm_addr)soundbank_bin);

	mmLoad(MOD_PONGDS);
	
	mmLoadEffect(SFX_HIT);
	mmLoadEffect(SFX_PADDLE);
	mmLoadEffect(SFX_WIN);
	mmLoadEffect(SFX_LOSE);

	mm_sound_effect hit = {{ SFX_HIT }, (int)(1.0f * (1<<10)), 0, 255, 127,};
	mm_sound_effect paddle = {{ SFX_PADDLE }, (int)(1.0f * (1<<10)), 0, 255, 127,};
	mm_sound_effect win = {{ SFX_WIN }, (int)(1.0f * (1<<10)), 0, 255, 127,};
	mm_sound_effect lose = {{ SFX_LOSE }, (int)(1.0f * (1<<10)), 0, 255, 127,};

	// Uncomment to enable terrible music
	// mmStart(MOD_PONGDS, MM_PLAY_LOOP);

	// mm_sfxhand sfxHander = 0;

	while(1) {

		scanKeys();
		key = keysHeld();

		/* Pause menu. */
		if(paused) {

			if(showPauseMessage) {
				consoleClear();
				iprintf("Press START to start game...\n");
				author(1);
				showPauseMessage = 0;
			}
			if(key & KEY_START) {
				paused = 0;
				updateScore = 1;
			}

		} else {

			glBegin2D();

			/* Render boxes. */
			glBoxFilled(player.x, player.y, player.x2, player.y2, RGB15(255, 0, 0));
			glBoxFilled(opponent.x, opponent.y, opponent.x2, opponent.y2, RGB15(0, 255, 0));
			glBoxFilled(ball.x, ball.y, ball.x2, ball.y2, RGB15(255, 255, 255));

			/* Top wall/Ball collision. */
			if(ball.y < 0) {
				ball.y = 0;
				ball.y2 = ball.y + ball.height;
				ball.velocity[1] = -ball.velocity[1];
				mmEffectEx(&hit);
			}

			/* Bottom wall/Ball collision. */
			if(ball.y > SCREEN_HEIGHT - ball.height) {
				ball.y = SCREEN_HEIGHT - ball.height;
				ball.y2 = ball.y + ball.height;
				ball.velocity[1] = -ball.velocity[1];
				mmEffectEx(&hit);
			}

			/* Out of bounds detection */
			if(ball.x < -5 || ball.x > SCREEN_WIDTH + 5) {
				if(ball.x < -5) {
					++opponent.score;
					mmEffectEx(&lose);
				}
				if(ball.x > SCREEN_WIDTH + 5) {
					++player.score;
					mmEffectEx(&win);
				}
				ball.x = SCREEN_WIDTH / 2;
				ball.y = SCREEN_HEIGHT / 2;
				ball.x2 = ball.x + ball.width;
				ball.y2 = ball.y + ball.height;
				ball.angle = MIN_BALL_ANGLE + rand() % (MAX_BALL_ANGLE + 1 - MIN_BALL_ANGLE);
				ball.velocity[0] = ball.speed * cos(ball.angle);
				ball.velocity[1] = ball.speed * -sin(ball.angle);
				paused = showPauseMessage = 1;
			}

			/* Player/Ball collision. */
			if(ball.y2 > player.y && ball.y < player.y2 && ball.x2 > player.x && ball.x < player.x2) {
				ball.x = player.width;
				ball.x2 = ball.x + ball.width;
				ball.angle = (player.y + player.height / 2 - ball.y) / (player.height / 2) * ball.maxAngle;
				ball.velocity[0] = ball.speed * cos(ball.angle);
				ball.velocity[1] = ball.speed * -sin(ball.angle);
				mmEffectEx(&paddle);
			}

			/* Opponent/Ball collision. */
			if(ball.y2 > opponent.y && ball.y < opponent.y2 && ball.x2 > opponent.x && ball.x < opponent.x2) {
				ball.x = SCREEN_WIDTH - opponent.width - ball.width;
				ball.x2 = ball.x + ball.width;
				ball.angle = (opponent.y + opponent.height / 2 - ball.y) / (opponent.height / 2) * ball.maxAngle;
				ball.velocity[0] = ball.speed * cos(ball.angle);
				ball.velocity[1] = ball.speed * -sin(ball.angle);
				ball.velocity[0] = -ball.velocity[0];
				mmEffectEx(&paddle);
			}

			/* Player controls. */
			if(key & KEY_UP) {
				if(player.y > 0) {
					player.y -= player.speed;
					player.y2 = player.y + player.height;
				}
			}
			if(key & KEY_DOWN) {
				if(player.y2 < SCREEN_HEIGHT) {
					player.y += player.speed;
					player.y2 = player.y + player.height;
				}
			}

			/* Opponent AI. */
			if(ball.x > SCREEN_WIDTH / 2) {
				if(ball.y + ball.height / 2 + 8 < opponent.y + opponent.height / 2) {
					opponent.y -= opponent.speed;
					opponent.y2 = opponent.y + opponent.height;
				}
				if(ball.y + ball.height / 2 - 8 > opponent.y + opponent.height / 2) {
					opponent.y += opponent.speed;
					opponent.y2 = opponent.y + opponent.height;
				}
			}

			/* Ball movement update. */
			ball.x += ball.velocity[0] * ball.speed;
			ball.y += ball.velocity[1] * ball.speed;
			ball.x2 = ball.x + ball.width;
			ball.y2 = ball.y + ball.height;

			if(updateScore) {
				consoleClear();
				iprintf("Your score:\t\t\tCPUs Score:\n-----------\t\t\t-----------\n%d\t\t\t\t\t\t%d\n", 
					player.score, opponent.score);
				author(3);
				updateScore = 0;
			}

			//printf("Pos: (%f, %f)\n", ball.x, ball.y);
			//printf("Vel: (%f, %f)\n", ball.velocity[0], ball.velocity[1]);

			glEnd2D();
			glFlush(0);
		}

		swiWaitForVBlank();
	}
}
