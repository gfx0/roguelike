/********************************************************************
created:	2013/03/11
created:	11:3:2013   21:49
author:		Jani Mäkinen
purpose:	This contains basically constant values in defines.
*********************************************************************/

//Stop whining about sprintf, if I fuck it up then I fuck it up, so be it. I'm experienced enough as a programmer that shooting myself in the foot is practically fun by now.
#define _CRT_SECURE_NO_WARNINGS


/********************************************************************

	NOTE: These values below are old outdated stuff I used earlier.

*********************************************************************/


//SYSTEM
#define SETTINGS_SCREEN_WIDTH 640
#define SETTINGS_SCREEN_HEIGHT 480
#define SETTINGS_SPRITE_WIDTH 32
#define SETTINGS_SPRITE_HEIGHT 32

//Approximation offset used because both the bomb and the rocket 
//images have about 12 pixels between both sides until a color pixel appears.
//This would make gameplay annoying if player would be hit from 'afar'.
#define SETTINGS_PROJECTILE_SPRITE_OFFSET_X 12
#define SETTINGS_PROJECTILE_SPRITE_OFFSET_Y 8

//PLAYER
#define SETTINGS_PLAYER_START_X 320.0f
#define SETTINGS_PLAYER_START_Y 448.0f //480-32
#define SETTINGS_PLAYER_MOVEMENT_SPEED_LEFT 160.0f
#define SETTINGS_PLAYER_MOVEMENT_SPEED_RIGHT 160.0f
#define SETTINGS_SECONDS_TO_WAIT_FOR_ROCKET_LAUNCH 0.5f
#define SETTINGS_MISSILE_MOVEMENT_SPEED 128.0f
#define SETTINGS_PLAYER_FLICKER_EFFECT_INTERVAL 0.10f
#define SETTINGS_PLAYER_FLICKER_EFFECT_DURATION 0.5f
#define SETTINGS_PLAYER_MAX_HEALTH 3

//ALIENS
#define SETTINGS_SECONDS_BETWEEN_ALIENS_MOVING 1.0f
#define SETTINGS_SECONDS_BETWEEN_ALIENS_SHOOTING 3.5f
#define SETTINGS_ALIENS_DOWNWARD_MOVEMENT_PIXELS 16.0f
#define SETTINGS_BOMB_MOVEMENT_SPEED 64.0f
#define SETTINGS_MAX_ROWS_OF_ALIENS 6
#define SETTINGS_MIN_ROWS_OF_ALIENS 2

//GAMESTATES
#define SETTINGS_SECONDS_TO_WAIT_IN_WINGAME_STATE 5.0f
#define SETTINGS_SECONDS_TO_WAIT_IN_LOSEGAME_STATE 4.0f
#define SETTINGS_WIN_TEXT_EFFECT_INTERVAL 1.0f
#define SETTINGS_LOSE_TEXT_EFFECT_INTERVAL 1.0f