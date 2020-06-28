#include "pch.h"
#include "TextComponent.h"

TextComponent::TextComponent()
{
	m_scale = Vector2(1.0f, 1.0f);
	m_colour = Colors::Black;
}

void TextComponent::Draw(DrawData2D* _DD, Vector2 _pos, Color _colour, float _rotation, Vector2 _origin, Vector2 _scale)
{

	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	_DD->m_Font->DrawString(_DD->m_Sprites.get(), Helper::charToWChar(m_text.c_str()), _pos, _colour, _rotation, _origin, _scale);

}
