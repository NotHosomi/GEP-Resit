#pragma once
#include <list>
#include "SimpleMath.h"
#include "RenderTarget.h"
using namespace DirectX::SimpleMath;
class CollisionComponent
{
public:
	CollisionComponent() = default;
	~CollisionComponent() = default;
	void Init(RECT _colRect);
	void Init(float _radius, Vector2 _pos);

	bool getCollison(CollisionComponent* collider);
	bool getCollison(Vector2 _pos);
	bool getCircularCollision(CollisionComponent* collider);

	void updateRect(int _x, int _y, int _width, int _height);
	void updateRect(RECT _rect);
	void getRect(RECT &OUT_rect);

	bool isDetectable();
	void isDetectable(bool _value);
	bool isTrigger();
	void isTrigger(bool _value);
	bool isCircular();
	void isCircular(bool _value);

	void updatePixelPos(int _offset);
	void terrainCollision(RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, GameData* _GD, bool (&OUT_sides)[6]);

	static constexpr int MV_STEPHEIGHT = 3;
private:

	bool hasPixelSideCollided(std::list<Vector2> _pixels, RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, GameData* _GD);

	bool m_isDetectable = true;
	bool m_isTrigger = false;
	bool m_isCircluar = false;
	int m_raduis = 0; //width of image * scale

	RECT m_rect;
	std::list<Vector2> m_pixelsPositions;

	std::list<Vector2> m_pixelsPosRight;
	std::list<Vector2> m_pixelsPosLeft;
	std::list<Vector2> m_pixelsPosTop;
	std::list<Vector2> m_pixelsPosBottom;
	std::list<Vector2> m_pixelsPosRightStep;
	std::list<Vector2> m_pixelsPosLeftStep;
};

