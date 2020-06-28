#include "pch.h"
#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(ButtonTypes _type)
{
	m_use = _type;
}

void ButtonComponent::init(ButtonTypes _use)
{
	m_use = _use;
}

void ButtonComponent::addTextComponent()
{
	if (m_textComp)
	{
		delete(m_textComp);
	}
	m_textComp = new TextComponent();
}
