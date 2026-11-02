#pragma once

#include <IComponent.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>
#include <string>

class IDrawable;
class TransformComponent;

class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& entity);
	~RenderComponent();

	virtual void Start() override;

	virtual void Update(float fDeltaTime) override;

	virtual void Destroy() override;

	template <class D>
	D* AddNewDrawable(const std::string& drawableFriendlyName);

	template <class D>
	D* AddNewDrawable(const std::string& drawableFriendlyName, sf::Vector2f relativePos, float relativeAngle, sf::Vector2f relativeScale);

	void SetDrawableRelativePosition(const IDrawable* drawable, const sf::Vector2f& position);
	void SetDrawableRelativeRotation(const IDrawable* drawable, float rotation);
	void SetDrawableRelativeScale(const IDrawable* drawable, const sf::Vector2f& scale);

	void Draw(sf::RenderWindow& window) const;

	const char* GetName() const override { return "RenderComponent"; }
	void DrawDebug() override;

protected:

	struct DrawableInfo
	{
		DrawableInfo();

		std::string FriendlyName;
		IDrawable* Drawable;
		sf::Vector2f RelativePosition;
		float RelativeRotation;
		sf::Vector2f RelativeScale;
		sf::Transform RelativeTransform;
		bool HasRelativeTransform;

		void ComputeTransform();
	};

	std::vector<DrawableInfo> Drawables;
	TransformComponent* Transform;
};

#include "RendererComponent.hxx"