#pragma once

#include "UIImage.h"
#include "UIButton.h"
#include <unordered_map>

using std::unordered_map;

class UICanvas
{
public:
	UICanvas(const string& name);
	~UICanvas();

	void update(float deltaTime);
	void draw(mat4& camView, mat4& camProj);
	void addImage(UIImage* image);
	void addButton(UIButton* button);
	void deleteImage(const string& name);
	string getName() const;
	unordered_map<string, UIImage*> getImages() const;
	unordered_map<string, UIButton*> getButtons() const;
	void applyCanvasAnim(const string& animName);
	void checkClick(int x, int y);

private:
	string _name;
	unordered_map<string, UIImage*> _images;
	unordered_map<string, UIButton*> _buttons;
};