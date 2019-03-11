#pragma once
#include "UIImage.h"

class Button: public UIImage
{
public:
	Button();
	~Button();

private:

	bool isClicked = false;
};