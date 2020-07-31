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
	static constexpr float FLARE_ELASTICITY = 0.6;
	static constexpr Color FLARE_COLOUR = Color(0.878, 1, 0);
	static constexpr float STRIKE_GAP = 0.3; // delay between projectiles spawned
	static constexpr float STRIKE_GAP_DIST = 10; // distance between strikes
	static constexpr int STRIKE_NUM = 5;
	static constexpr float STRIKE_DMG = 17;
	static constexpr float STRIKE_EXP_RADIUS = 35;

	void strikeTick(GameData* _GD);
	void strike(GameData* _GD);

	Vector2 target_pos;
	bool strike_started = false;
	bool strike_timer = 0;
	bool strikes_occured = 0;
};

