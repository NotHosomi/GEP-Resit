#pragma once
#include "Projectile.h"
class AirStrikeFlare :
	public Projectile
{
public:
	AirStrikeFlare(ID3D11Device* _GD, Vector2 position, Vector2 velocity);

	void Tick(GameData* _GD) override;
private:
	static constexpr float FLARE_DIMS = 5;
	static constexpr float FLARE_WEIGHT = 300;
	static constexpr float FLARE_ELASTICITY = 0.3;
	static constexpr Color FLARE_COLOUR = Color(0, 1, 1);
	static constexpr float STRIKE_GAP = 0.3; // delay between projectiles spawned
	// static constexpr float STRIKE_GAP_DIST = 10; // distance between strikes
	static constexpr float STRIKE_DEVIANCE = 20; // Standard Deviation of the strike
	static constexpr int STRIKE_NUM = 5;
	static constexpr float STRIKE_DMG = 23;
	static constexpr float STRIKE_EXP_RADIUS = 30;
	static constexpr float STRIKE_SPEED = 700;

	void strikeTick(GameData* _GD);
	void strike(GameData* _GD);

	float flare_fuse = 0;
	Vector2 target_pos;
	bool strike_started = false;
	float strike_timer = 0;
	int strikes_occured = 0;
};

