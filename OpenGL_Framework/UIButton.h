#pragma once
#include "UIImage.h"

class UIButton: public UIImage
{
public:
	UIButton();
	~UIButton();

	bool getHover() const;
	void setHover(const bool isHover);

private:

	bool isClicked = false;
	bool _isHover = false;
};