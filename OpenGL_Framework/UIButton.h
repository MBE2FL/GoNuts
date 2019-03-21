#pragma once
#include "UIImage.h"

class UIButton: public UIImage
{
public:
	UIButton(const string& name);
	~UIButton();

	bool getHover() const;
	void setHover(const bool isHover);

	bool getIsClicked() const;
	void setIsClicked(const bool isClicked);

private:

	bool _isClicked = false;
	bool _isHover = false;
};