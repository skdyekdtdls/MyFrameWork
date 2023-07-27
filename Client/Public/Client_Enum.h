#pragma once

namespace Client
{
	enum LEVELID { LEVEL_STATIC, LEVEL_IMGUI, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };

	enum CLINT_ANIM
	{
		CLINT_SHOOT = 5,
		CLINT_DASH = 27,
		CLINT_IDLE = 64,
		CLINT_MVP = 74,
		CLINT_RUN = 89,
		CLINT_SKILL_01 = 109,
		CLINT_SKILL_02 = 112,
		CLINT_ULTIMATE01 = 133,
		CLINT_ULTIMATE02 = 135,
		CLINT_WALK_B = 142,
		CLINT_WALK_BL,
		CLINT_WALK_BL_45,
		CLINT_WALK_BR,
		CLINT_WALK_BR_45,
		CLINT_WALK_F,
		CLINT_WALK_FL,
		CLINT_WALK_FL_45,
		CLINT_WALK_FR,
		CLINT_WALK_FR_45,
		CLINT_WEAPONCHHANGE = 152,
	};

	enum ALIEN_PRAWN_ANIM
	{
		ALIEN_PRAWN_APPEAR,
		ALIEN_PRAWN_ATTACK01,
		ALIEN_PRAWN_ATTACK01_MID,
		ALIEN_PRAWN_ATTACK02,
		ALIEN_PRAWN_CHARGE,
		ALIEN_PRAWN_CHARGE_IN,
		ALIEN_PRAWN_CHARGE_OUT,
		ALIEN_PRAWN_HIT,
		ALIEN_PRAWN_HIT_02,
		ALIEN_PRAWN_HIT_LAUNCH,
		ALIEN_PRAWN_HIT_LAUNCH_FALLING,
		ALIEN_PRAWN_HIT_LAUNCH_FALLING_FAR,
		ALIEN_PRAWN_LAUNCH_FAR,
		ALIEN_PRAWN_LAUNCH_LAND,
		ALIEN_PRAWN_LOCK,
		ALIEN_PRAWN_MID,
		ALIEN_PRAWN_SPINN,
		ALIEN_PRAWN_IDLE,
		ALIEN_PRAWN_IDLE_CODEX,
		ALIEN_PRAWN_RUN,
		ALIEN_PRAWN_STUNNED_IN,
		ALIEN_PRAWN_STUNNED,
		ALIEN_PRAWN_END,
	};

	enum BATPOTATO_RIG_ANIM
	{
		BATPOTATO_RIG_ATTACK01,
		BATPOTATO_RIG_HIT,
		BATPOTATO_RIG_HIT_LAUNCH,
		BATPOTATO_RIG_HIT_LAUNCH_FALLING,
		BATPOTATO_RIG_HIT_LAUNCH_FALLING_FAR,
		BATPOTATO_RIG_HIT_LAUNCH_FAR,
		BATPOTATO_RIG_HIT_LAUNCH_LAND,
		BATPOTATO_RIG_IDLE,
		BATPOTATO_RIG_RUN,
		BATPOTATO_RIG_STUNNED,
		BATPOTATO_RIG_STUNNED_IN,
		BATPOTATO_RIG_END,
	};

	enum CANNON_SPIDER_ANIM
	{
		CANNON_SPIDER_AIM_LEFT,
		CANNON_SPIDER_IDLE,
		CANNON_SPIDER_AIM_RIGHT,
		CANNON_SPIDER_ATTACK01,
		CANNON_SPIDER_ATTACK02,
		CANNON_SPIDER_HIT,
		CANNON_SPIDER_HIT_LAUNCH,
		CANNON_SPIDER_HIT_LAUNCH_FALLING,
		CANNON_SPIDER_HIT_LAUNCH_FALLING_FAR,
		CANNON_SPIDER_HIT_LAUNCH_FAR,
		CANNON_SPIDER_HIT_LAUNCH_LAND,
		CANNON_SPIDER_RUN_F,
		CANNON_SPIDER_STUNNED,
		CANNON_SPIDER_STUNNED_IN,
		CANNON_SPIDER_TURN180_L,
		CANNON_SPIDER_TURN180_R,
		CANNON_SPIDER_TURN90_L,
		CANNON_SPIDER_TURN90_R,
		CANNON_SPIDER_ANIM_END
	};
}