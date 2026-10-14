#include "TransformComponent.h"
#include <Entity.h>

TransformComponent::TransformComponent(Entity& entity) : IComponent(entity)
{
	WorldPosition = sf::Vector2f();
	WorldPositionQuery = sf::Vector2f();
	Rotation = 0.f;
	Scale = sf::Vector2f(1.f, 1.f);

	UpdateMatrix();
}


TransformComponent::~TransformComponent()
{}

void TransformComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	float pos[2] = { GetWorldPosition().x, GetWorldPosition().y};
	if (ImGui::DragFloat2("Position", pos, 1.f))
		SetWorldPosition(sf::Vector2(pos[0], pos[1]));

	float rotation = GetRotation();
	if (ImGui::DragFloat("Rotation", &rotation, 0.5f, -360.f, 360.f))
		SetRotation(rotation);

	float scl[2] = { GetScale().x, GetScale().y};
	if (ImGui::DragFloat2("Scale", scl, 0.01f, 0.01f, 100.f))
		SetScale(sf::Vector2(scl[0], scl[1]));

	if (ImGui::Button("Reset"))
	{
		SetWorldPositionQuery(sf::Vector2(0.f, 0.f));
		SetRotation(0.f);
		SetScale(sf::Vector2(1.f, 1.f));
	}
#endif
}

const sf::Transform& TransformComponent::GetMatrix() const
{
	return Matrix;
}

void TransformComponent::SetWorldPosition(sf::Vector2f newPosition)
{
	WorldPosition = newPosition;
	WorldPositionQuery = newPosition;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetWorldPosition() const
{
	return WorldPosition;
}

void TransformComponent::SetWorldPositionQuery(sf::Vector2f newPosition)
{
	WorldPositionQuery = newPosition;
}

sf::Vector2f TransformComponent::GetWorldPositionQuery() const
{
	return WorldPositionQuery;
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

void TransformComponent::UpdateMatrix()
{
	Matrix = sf::Transform::Identity;
	Matrix.translate(WorldPosition);
	Matrix.rotate(sf::degrees(Rotation));
	Matrix.scale(Scale);
}
