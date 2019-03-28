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
	//glEnable(GL_BLEND);
	
	glDisable(GL_BLEND);

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
	_images.erase(name);
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
