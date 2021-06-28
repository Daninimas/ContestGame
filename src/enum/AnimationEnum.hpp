#pragma once

enum class Animation { // These are the base animations, without specifying in with GameObject
	NONE = 0,
	IDLE,
	RUNNING,
	DODGE,
	SHOOT_UP,
	SHOOT_DOWN,
	MELEE_ATTACK,
	TAKING_DAMAGE
};
