#pragma once
#include "SimpleMath.h"
#include <d3d11_1.h>
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "GameData.h"
#include "helper.h"
using namespace DirectX::SimpleMath;
class AnimationComponent
{
public :
	AnimationComponent() = default;
	~AnimationComponent() = default;
	void init(int _widht, int _height, int _numSprites);
	
	void nextFrame(float _deltaTime);

	ID3D11ShaderResourceView* currentAnimationFrame() { return m_currentTexture; }

	void updateRect();
	RECT currentRect() { return m_currentRect; }

	int width() { return m_singleWidth; }
	int height() { return m_singleHeight; }

	void setFlipX(bool _value);

private:
	ID3D11ShaderResourceView* m_currentTexture = nullptr;
	RECT m_currentRect = { 0,0,0,0 };
	int m_currentPoint = 0;//first animation is indexed at zero
	int m_maxPoint = 0;
	int m_singleWidth = 0;
	int m_singleHeight = 0;

	float m_waitSoFar = 0.0f;
	float m_delayWait = 0.2f;
	bool m_flipX = false;
};

