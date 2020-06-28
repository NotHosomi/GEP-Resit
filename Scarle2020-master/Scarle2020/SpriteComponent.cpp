#include "pch.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(std::string _fileName, ID3D11Device* _GD) :m_pTextureRV(nullptr)
{
	std::string fullfilename = "../Images/" + _fileName + ".dds";
	HRESULT hr = CreateDDSTextureFromFile(_GD, Helper::charToWChar(fullfilename.c_str()), nullptr, &m_pTextureRV);
	directory = _fileName;
	//this nasty thing is required to find out the size of this image!
	ID3D11Resource* pResource;
	D3D11_TEXTURE2D_DESC Desc;
	m_pTextureRV->GetResource(&pResource);
	((ID3D11Texture2D*)pResource)->GetDesc(&Desc);

	//m_origin = 0.5f * Vector2((float)Desc.Width, (float)Desc.Height);//around which rotation and scaing is done
}

bool SpriteComponent::loadSprite()
{
	return false;
}

void SpriteComponent::setVisibility(const bool& vis)
{
	visibility = vis;
}

bool SpriteComponent::getVisibility() const
{
	return visibility;
}


ID3D11ShaderResourceView* SpriteComponent::getTexture()
{
	return m_pTextureRV;
}

void SpriteComponent::AddAnimationComponent(int _width, int _height, int _numPoints)
{
	if (m_animationComponent)
	{
		delete(m_animationComponent);
	}
	m_animationComponent = new AnimationComponent();
	m_animationComponent->init(_width, _height, _numPoints);
}
