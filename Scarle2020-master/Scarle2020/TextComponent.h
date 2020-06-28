#pragma once
#include "DrawData2D.h"
#include "helper.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

class TextComponent
{
public:
	TextComponent();
	~TextComponent() = default;
	void setScale(Vector2 _scale) { m_scale = _scale; }
	Vector2 getScale() { return m_scale; }
	void setColour(Color _color) { m_colour = _color; }
	Color getColor() { return m_colour; }

	int getXOffSet() { return m_xOffset; }
	int getYOffSet() { return m_yOffset; }
	void setXOffSet(int _value){m_xOffset = _value;}
	void setYOffSet(int _value) { m_yOffset = _value; }

 	void text(std::string _text) { m_text = _text; }
	void Draw(DrawData2D* _DD, Vector2 _pos, Color _colour, float _rotation, Vector2 _origin, Vector2 _scale);
private:
	std::string m_text = "";
	int m_xOffset = 0;
	int m_yOffset = 0;

	Vector2 m_pos;
	float m_rotation;
	Vector2 m_scale;
	Color m_colour;
	Vector2 m_origin;
};

