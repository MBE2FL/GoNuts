#include "UICanvas.h"

UICanvas::UICanvas(const string & name)
{
	_name = name;
}

UICanvas::~UICanvas()
{
	for (auto const& image : _images)
	{
		delete image.second;
	}
}

void UICanvas::update(float deltaTime)
{
	for (auto const& imageKeyVal : _images)
	{
		imageKeyVal.second->getTransform()->UIUpdate(deltaTime, imageKeyVal.second->getAnimator());
		imageKeyVal.second->getAnimator()->update(deltaTime);
	}
}

void UICanvas::draw(mat4 & camView, mat4 & camProj)
{
	// Draw all of this canvases images.
	for (auto const& imageKeyVal : _images)
	{
		UIImage* image = imageKeyVal.second;

		// Retrieve the mesh, shader program, and the texture.
		Mesh* mesh = image->getMesh();
		ShaderProgram* shaderProgram = image->getShaderProgram();
		Texture* texture = image->getTexture();
		TransformComponent* transform = image->getTransform();


		// Bind and send all the uniforms to the shader program.
		shaderProgram->bind();
		shaderProgram->sendUniformMat4("uModel", transform->getLocalToWorldMatrix().data, false);
		shaderProgram->sendUniformMat4("uView", camView.data, false);
		shaderProgram->sendUniformMat4("uProj", camProj.data, false);
		shaderProgram->sendUniform("uAlpha", image->getAlpha());


		// Bind the texture.
		texture->bind(0);

		// Bind the mesh.
		mesh->vao.draw();

		// Unbind the texture.
		texture->unBind(0);


		// Unbind the shader program.
		shaderProgram->unBind();
	}


}

void UICanvas::addImage(UIImage * image)
{
	string name = image->getName();

	if (_images.find(name) != _images.end())
	{
		cout << "Image with name: " << name << "already exists!" << endl;
		system("pause");
	}

	_images[name] = image;
}

void UICanvas::addButton(UIButton* button)
{
	string name = button->getName();

	if (_images.find(name) != _images.end())
	{
		cout << "Image with name: " << name << "already exists!" << endl;
		system("pause");
	}

	_images[name] = button;
}

void UICanvas::deleteImage(const string & name)
{
	unordered_map<string, UIImage*>::iterator it = _images.find(name);
	if (it != _images.end())
	{
		delete it->second;
		_images.erase(name);
	}
}

string UICanvas::getName() const
{
	return _name;
}

unordered_map<string, UIImage*> UICanvas::getImages() const
{
	return _images;
}

unordered_map<string, UIButton*> UICanvas::getButtons() const
{
	return _buttons;
}

void UICanvas::applyCanvasAnim(const string & animName)
{
	UIAnimation* anim = UIAnimation::getAnimation(animName);

	if (anim)
	{
		UIAnimator* animator = nullptr;
		for (auto const& imageKV : _images)
		{
			animator = imageKV.second->getAnimator();
			animator->addAnimation(anim);
			animator->stopAll();
			animator->play(animName);
			animator->setActive(true);
		}
	}
}

void UICanvas::checkClick(int x, int y)
{
	UIImage* image = nullptr;
	vec3 centre;
	vec3 scale;
	float minX = 0.0f;
	float minY = 0.0f;
	float maxX = 0.0f;
	float maxY = 0.0f;
	Bounds meshBounds;
	y = 1000-y;

	for (auto const& imageKV : _images)
	{
		image = imageKV.second;

		centre = image->getTransform()->getWorldPosition();
		scale = image->getTransform()->getLocalScale();
		meshBounds = image->getMesh()->getMeshBounds();

		minX = centre.x - (scale.x * meshBounds.extends.x);
		maxX = centre.x + (scale.x * meshBounds.extends.x);

		minY = centre.y - (scale.y * meshBounds.extends.y);
		maxY = centre.y + (scale.y * meshBounds.extends.y);


		if ((x >= minX) && (x <= maxX) && (y >= minY) && (y <= maxY))
			image->setClicked(true);
		else
			image->setClicked(false);
	}
}

UIImage * UICanvas::getImage(const string & name) const
{
	if (_images.find(name) != _images.end())
		return _images.at(name);

	return nullptr;
}
