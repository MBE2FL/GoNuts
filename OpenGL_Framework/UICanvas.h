#pragma once

#include "UIImage.h"
#include "UIButton.h"
#include <unordered_map>

using std::unordered_map;

class UICanvas
{
public:
	UICanvas();
	~UICanvas();

	void update(float deltaTime);
	void draw(mat4& camView, mat4& camProj);
	void addImage(const string& name, UIImage* image);
	void addButton(UIButton* button);
	void deleteImage(const string& name);

private:
	unordered_map<string, UIImage*> _images;
	vector<UIButton*> buttons;
};