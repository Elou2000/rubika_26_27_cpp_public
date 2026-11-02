#pragma once

#include <string>
#include <vector>

class IComponent;

namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	Entity(std::string friendlyName = "");
	virtual ~Entity();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	template <typename C>
	C* AddComponent();

	template <typename C>
	C* GetComponent() const;

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
	void DrawDebug();

	const std::string& GetName() const { return FriendlyName; }

protected:
	std::string FriendlyName;
	std::vector<IComponent*> Components;
};

#include <Entity.hxx>
