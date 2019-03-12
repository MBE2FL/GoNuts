#include "UIButton.h"

UIButton::UIButton()
{
}

UIButton::~UIButton()
{
}

bool UIButton::getHover() const
{
	return _isHover;
}

void UIButton::setHover(const bool isHover)
{
	_isHover = isHover;
}
