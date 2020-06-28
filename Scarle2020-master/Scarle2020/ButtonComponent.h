#pragma once
#include "TextComponent.h"
#include "UIStates.h"

class ButtonComponent
{
public:
	ButtonComponent(ButtonTypes _type);
	~ButtonComponent() = default;
	void init(ButtonTypes _use);

	void addTextComponent();
	TextComponent* textComponent() { return m_textComp; }
	ButtonTypes getButtonType() { return m_use; }

private:
	TextComponent* m_textComp = nullptr;
	ButtonTypes m_use;
};

