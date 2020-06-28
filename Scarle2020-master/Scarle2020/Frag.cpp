#include "pch.h"
#include "Frag.h"

//Frag::Frag(ID3D11Device* _GD, Vector2 _pos, Vector2 initial_vel)
//{
//	Setup(_GD, _pos, initial_vel, "grenade");
//}

void Frag::Explode()
{
	Projectile::Explode();
	//spawn clusters
}