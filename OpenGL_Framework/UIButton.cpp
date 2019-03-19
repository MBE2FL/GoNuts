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

bool UIButton::getIsClicked() const
{
	return _isClicked;
}

void UIButton::setIsClicked(const bool isClicked)
{
	_isClicked = isClicked;
}
