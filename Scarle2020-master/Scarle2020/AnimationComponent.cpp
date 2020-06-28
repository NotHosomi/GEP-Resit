#include "pch.h"
#include "AnimationComponent.h"

void AnimationComponent::init(int _width, int _height, int _numSprites)
{
	if (_numSprites == 0)
		_numSprites = 1;


	m_singleWidth = _width;// / _numSprites;
	m_singleHeight = _height / _numSprites;

	m_maxPoint = _numSprites;
	m_currentPoint = _numSprites / 2;

}

void AnimationComponent::nextFrame(float _deltaTime)
{
	if (m_waitSoFar < m_delayWait)
	{
		m_waitSoFar += _deltaTime;
		return;
	}
	if (m_currentPoint != m_maxPoint - 1 && m_currentPoint > 0)
	{
		if (m_flipX)
			m_currentPoint--;
		else
			m_currentPoint++;
	}
	else
	{
		if (m_flipX)
			m_currentPoint = m_maxPoint / 2 - 1;
		else
			m_currentPoint = m_maxPoint / 2;
	}
	updateRect();
	m_waitSoFar = 0.0f;
}


void AnimationComponent::updateRect()
{
	m_currentRect.left = 0;
	m_currentRect.right = m_singleWidth;
	m_currentRect.top = 0 + m_singleHeight * m_currentPoint;
	m_currentRect.bottom = m_singleHeight + m_singleHeight * m_currentPoint;
}

void AnimationComponent::setFlipX(bool _value)
{
	if(_value && _value != m_flipX)
		m_currentPoint = m_maxPoint  / 2 - 1;
	else if(!_value && _value != m_flipX)
		m_currentPoint = m_maxPoint / 2;
	m_flipX = _value;
}
