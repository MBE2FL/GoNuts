#pragma once
#include "UIImage.h"

class UIButton: public UIImage
{
public:
	UIButton();
	~UIButton();

	bool getHover() const;
	void setHover(const bool isHover);

	bool getIsClicked() const;
	void setIsClicked(const bool isClicked);

private:

	bool _isClicked = false;
	bool _isHover = false;
};