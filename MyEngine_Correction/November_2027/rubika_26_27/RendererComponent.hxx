#pragma once

#include "RenderComponent.h"

template <class D>
inline D* RenderComponent::AddNewDrawable(const std::string& drawableFriendlyName)
{
	for (const DrawableInfo& info : Drawables)
	{
		if (info.FriendlyName == drawableFriendlyName)
		{
			return nullptr;
		}
	}

	D* drawable = new D();
	if (!drawable)
	{
		return nullptr;
	}

	DrawableInfo& info = Drawables.emplace_back();
	info.Drawable = drawable;
	info.FriendlyName = drawableFriendlyName;
	info.HasRelativeTransform = false;
	info.RelativeTransform = sf::Transform::Identity;

	return drawable;
}

template <class D>
inline D* RenderComponent::AddNewDrawable(const std::string& drawableFriendlyName, sf::Vector2f relativePos, float relativeAngle, sf::Vector2f relativeScale)
{
	if ((relativePos.x == 0.f && relativePos.y == 0.f) && relativeAngle == 0.f && (relativeScale.x == 1.f && relativeScale.y == 1.f))
	{
		return AddNewDrawable<D>(drawableFriendlyName);
	}

	for (const DrawableInfo& info : Drawables)
	{
		if (info.FriendlyName == drawableFriendlyName)
		{
			return nullptr;
		}
	}

	D* drawable = new D();
	if (!drawable)
	{
		return nullptr;
	}

	DrawableInfo& info = Drawables.emplace_back();
	info.Drawable = drawable;
	info.FriendlyName = drawableFriendlyName;
	info.HasRelativeTransform = true;

	info.RelativeTransform = sf::Transform::Identity;
	info.RelativeTransform.translate(relativePos).rotate(sf::degrees(relativeAngle)).scale(relativeScale);

	return drawable;
}