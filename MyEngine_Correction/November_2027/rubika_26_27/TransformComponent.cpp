#include "TransformComponent.h"

#include <Imgui/imgui.h>

TransformComponent::TransformComponent(Entity& entity) : IComponent(entity)
{
	WorldPosition = sf::Vector2f();
	Rotation = 0.f;
	Scale = sf::Vector2f(1.f, 1.f);

	UpdateMatrix();
}

const sf::Transform& TransformComponent::GetMatrix() const
{
	return Matrix;
}

void TransformComponent::SetWorldPosition(sf::Vector2f newPosition)
{
	WorldPosition = newPosition;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetWorldPosition() const
{
	return WorldPosition;
}

void TransformComponent::SetRotation(float newRotation)
{
	Rotation = newRotation;
	UpdateMatrix();
}

float TransformComponent::GetRotation() const
{
	return Rotation;
}

void TransformComponent::SetScale(sf::Vector2f newScale)
{
	Scale = newScale;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetScale() const
{
	return Scale;
}

void TransformComponent::Start()
{}

void TransformComponent::Update(float fDeltaTime)
{}

void TransformComponent::Destroy()
{}

void TransformComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	if (ImGui::InputFloat2("Translation", &WorldPosition.x))
		UpdateMatrix();
	if (ImGui::SliderFloat("Rotation", &Rotation, -180, 180))
		UpdateMatrix();
	if (ImGui::InputFloat2("Scale", &Scale.x))
		UpdateMatrix();
#endif
}

void TransformComponent::UpdateMatrix()
{
	Matrix = sf::Transform::Identity;
	Matrix.translate(WorldPosition);
	Matrix.rotate(sf::degrees(Rotation));
	Matrix.scale(Scale);
}
