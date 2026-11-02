#include "RenderComponent.h"
#include <IDrawable.h>
#include <TransformComponent.h>
#include <Entity.h>

RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{}

RenderComponent::~RenderComponent()
{
	for (const DrawableInfo& info : Drawables)
	{
		delete info.Drawable;
	}

	Drawables.clear();
}

void RenderComponent::Start()
{
	Transform = GetEntity().GetComponent<TransformComponent>();

	for (const DrawableInfo& info : Drawables)
	{
		info.Drawable->Start();
	}
}

void RenderComponent::Update(float fDeltaTime)
{
	for (const DrawableInfo& info : Drawables)
	{
		if (info.Drawable->IsVisible())
		{
			if (info.HasRelativeTransform)
			{
				info.Drawable->SetWorldTransform(Transform->GetMatrix() * info.RelativeTransform);
			}
			else
			{
				info.Drawable->SetWorldTransform(Transform->GetMatrix());
			}

			info.Drawable->Update(fDeltaTime);
		}
	}
}

void RenderComponent::Destroy()
{}

void RenderComponent::SetDrawableRelativePosition(const IDrawable* drawable, const sf::Vector2f& position)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable == drawable)
		{
			info.RelativePosition = position;
			info.ComputeTransform();
			return;
		}
	}
}

void RenderComponent::SetDrawableRelativeRotation(const IDrawable* drawable, float rotation)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable == drawable)
		{
			info.RelativeRotation = rotation;
			info.ComputeTransform();
			return;
		}
	}
}

void RenderComponent::SetDrawableRelativeScale(const IDrawable* drawable, const sf::Vector2f& scale)
{
	for (DrawableInfo& info : Drawables)
	{
		if (info.Drawable == drawable)
		{
			info.RelativeScale = scale;
			info.ComputeTransform();
			return;
		}
	}
}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	for (const DrawableInfo& info : Drawables)
	{
		if (info.Drawable->IsVisible())
		{
			info.Drawable->Draw(window);
		}
	}
}

void RenderComponent::DrawDebug()
{
}

RenderComponent::DrawableInfo::DrawableInfo()
{
	FriendlyName = "";
	Drawable = nullptr;
	RelativePosition = sf::Vector2f();
	RelativeRotation = 0.f;
	RelativeScale = sf::Vector2f(1.f, 1.f);
	RelativeTransform = sf::Transform::Identity;
	HasRelativeTransform = false;
}

void RenderComponent::DrawableInfo::ComputeTransform()
{
	RelativeTransform = sf::Transform::Identity;
	RelativeTransform.translate(RelativePosition);
	RelativeTransform.rotate(sf::degrees(RelativeRotation));
	RelativeTransform.scale(RelativeScale);

	HasRelativeTransform = RelativeTransform != sf::Transform::Identity;
}
