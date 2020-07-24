#include "pch.h"
#include "CollisionComponent.h"

void CollisionComponent::Init(RECT _colRect)
{
	updateRect(_colRect);
}
void CollisionComponent::Init(float _radius, Vector2 _pos)
{
	m_raduis = _radius;
	m_isCircluar = true;

	m_rect.left = _pos.x;
	m_rect.top = _pos.y;
}
/*
retrieve rect of collider object
test axis for overlapping
*/
bool CollisionComponent::getCollison(CollisionComponent* collider)
{
	RECT colRect;
	collider->getRect(colRect);
	if (((m_rect.left >= colRect.left && m_rect.left <= colRect.right) || (m_rect.right < colRect.right && m_rect.right > colRect.left)) &&
		((m_rect.bottom >= colRect.bottom && m_rect.bottom <= colRect.top) || (m_rect.top < colRect.top && m_rect.top > colRect.bottom)))
	{
		return true;
	}

	return false;
}

bool CollisionComponent::getCollison(Vector2 _pos)
{
	if ((_pos.x >= m_rect.left && _pos.x <= m_rect.right) &&
		(_pos.y >= m_rect.top && _pos.y <= m_rect.bottom))
	{
		return true;
	}
	return false;
}

bool CollisionComponent::getCircularCollision(CollisionComponent* collider)
{
	RECT colRect;
	collider->getRect(colRect);
	Vector2 centre(m_rect.left + m_raduis / 2, m_rect.top + m_raduis / 2);
	
	if (((colRect.left > centre.x - m_raduis && colRect.left < centre.x + m_raduis) || (colRect.right > centre.x - m_raduis && colRect.right < centre.x + m_raduis))&&
		((colRect.top > centre.y - m_raduis && colRect.top < centre.y + m_raduis) || (colRect.bottom > centre.y - m_raduis && colRect.bottom < centre.y + m_raduis)))
	{
	return true;
	}
	
	return false;
}

void CollisionComponent::updateRect(int _x, int _y, int _width, int _height)
{
	m_rect.left = _x;
	m_rect.right = _x + _width;
	m_rect.top = _y;
	m_rect.bottom = _y + _height;
}

void CollisionComponent::updateRect(RECT _rect)
{
	m_rect.left = _rect.left;
	m_rect.right = _rect.right;
	m_rect.top = _rect.top;
	m_rect.bottom = _rect.bottom;
}

void CollisionComponent::getRect(RECT& OUT_rect)
{
	OUT_rect.left = m_rect.left;
	OUT_rect.right = m_rect.right;
	OUT_rect.top = m_rect.top;
	OUT_rect.bottom = m_rect.bottom;
}

bool CollisionComponent::isDetectable()
{
	return m_isDetectable;
}

void CollisionComponent::isDetectable(bool _value)
{
	m_isDetectable = _value;
}

bool CollisionComponent::isTrigger()
{
	return m_isTrigger;
}

void CollisionComponent::isTrigger(bool _value)
{
	m_isTrigger = _value;
}

bool CollisionComponent::isCircular()
{
	return m_isCircluar;
}

void CollisionComponent::isCircular(bool _value)
{
	m_isCircluar = _value;
}

void CollisionComponent::updatePixelPos(int _offset)
{
	m_pixelsPosBottom.clear();
	m_pixelsPosTop.clear();
	m_pixelsPosLeft.clear();
	m_pixelsPosRight.clear();
	m_pixelsPosRightStep.clear();
	m_pixelsPosLeftStep.clear();

	int x1 = m_rect.left + _offset ;
	int y1 = m_rect.top + _offset;

	//top side/ bottom side
	for (int i = x1; i < m_rect.right - _offset / 2; i++)
	{
		m_pixelsPosTop.push_back(Vector2(i, y1));
		m_pixelsPosBottom.push_back(Vector2(i, m_rect.bottom - _offset));
	}
	// right/left torso
	for (int i = y1; i < m_rect.bottom - (_offset/50) + 1; i++)
	{
		m_pixelsPosLeft.push_back(Vector2(x1, i));
		m_pixelsPosRight.push_back(Vector2(m_rect.right - _offset, i));
	}
	// right/left step space
	for (int i = y1; i < m_rect.bottom - (_offset/2) + 1; i++)
	{
		m_pixelsPosLeftStep.push_back(Vector2(x1, i));
		m_pixelsPosRightStep.push_back(Vector2(m_rect.right - _offset, i));
	}
}

/*
only map and unmap when terrain is exploded, not every frame
*/
void CollisionComponent::terrainCollision(RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, GameData* _GD , bool(&OUT_sides)[6])
{
	_renderTarget->Map(_d3dContext);
	bool sides[6] = { false, false, false, false, false, false };
	//collision happens here, get which side collisions happens on
	if (hasPixelSideCollided(m_pixelsPosTop, _renderTarget, _d3dContext, _GD))
		sides[0] = true;
	if (hasPixelSideCollided(m_pixelsPosRight, _renderTarget, _d3dContext, _GD))
		sides[1] = true;
	if (hasPixelSideCollided(m_pixelsPosBottom, _renderTarget, _d3dContext, _GD))
		sides[2] = true;
	if (hasPixelSideCollided(m_pixelsPosLeft, _renderTarget, _d3dContext, _GD))
		sides[3] = true;
	if (hasPixelSideCollided(m_pixelsPosRightStep, _renderTarget, _d3dContext, _GD))
		sides[4] = true;
	if (hasPixelSideCollided(m_pixelsPosRightStep, _renderTarget, _d3dContext, _GD))
		sides[5] = true;

	for (size_t i = 0; i < 6; i++)
	{
		OUT_sides[i] = sides[i];
	}
	
	_renderTarget->Unmap(_d3dContext);
}

bool CollisionComponent::hasPixelSideCollided(std::list<Vector2> _pixels, RenderTarget* _renderTarget, ID3D11DeviceContext* _d3dContext, 
											  GameData* _GD)
{
	for (auto& pos : _pixels)
	{
		Color* col = _renderTarget->GetPixel(pos.x, pos.y, _GD);
		if (col->A() > 0.0f)
		{
			return true;
		}
	}
	return false;
}
