#pragma once
#include <d3d11_1.h>
#include "SimpleMath.h"
#include <string>
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "GameData.h"
#include "helper.h"

#include "AnimationComponent.h"

//SIMON:: You have a set of things called _SOMETHING_Component which you would expect to have a shared base class.
//GameObject2D could then contain a data structure of them or references/pointers to them.
//Potnetially makes things more customisable.
// Alos this one just repicates code from ImageGO2D, so could have inherited from that.
class SpriteComponent
{
public:
	SpriteComponent(std::string _fileName, ID3D11Device* _GD);
	~SpriteComponent() = default;

	bool loadSprite();
	std::string getDirectory() { return directory; }
	void setVisibility(const bool& vis);
	bool getVisibility() const;

	ID3D11ShaderResourceView* getTexture();

	void AddAnimationComponent(int _width, int height_, int _numPoints);
	AnimationComponent* animationComponent() { return m_animationComponent; }
private:
	ID3D11ShaderResourceView* m_pTextureRV;
	bool visibility = true;

	std::string directory = "";

	AnimationComponent* m_animationComponent = nullptr;
};


