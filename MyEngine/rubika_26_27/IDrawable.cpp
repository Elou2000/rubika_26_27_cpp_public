#include "IDrawable.h"
#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif
#include "Logger.h"

IDrawable::IDrawable() : Visible(true), WorldTransform(sf::Transform::Identity), Drawable(nullptr)
{}

IDrawable::~IDrawable()
{}

bool IDrawable::IsVisible() const
{
	return Visible;
}

void IDrawable::SetVisibility(bool visible)
{
	Visible = visible;
}

const sf::Transform& IDrawable::GetWorldTransform() const
{
	return WorldTransform;
}

void IDrawable::SetWorldTransform(const sf::Transform& transform)
{
	WorldTransform = transform;
}

void IDrawable::Draw(sf::RenderWindow& window) const
{
	if (!Drawable)
	{
		return;
	}

	sf::RenderStates states = sf::RenderStates::Default;
	states.transform = WorldTransform;

	window.draw(*Drawable, states);
}

void IDrawable::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::Checkbox("Visible", &Visible);

	const float* m = WorldTransform.getMatrix();
	ImGui::Text("World position : %.1f, %.1f", m[12], m[13]);
	float pos[2] = { localOffset.x, localOffset.y };
	if (ImGui::DragFloat2("Position", pos, 1.f))
		SetLocalOffset(sf::Vector2(pos[0], pos[1]));
	ImGui::Text("Is drawable : %s", Drawable ? "yes" : "no");
#endif
}

void IDrawable::SetLocalOffset(sf::Vector2f newPosition)
{
	
	localOffset = newPosition;
	localMatrix = sf::Transform::Identity;
	localMatrix.translate(localOffset);
}

sf::Vector2f IDrawable::GetLocalOffset() const
{
	return localOffset;
}
