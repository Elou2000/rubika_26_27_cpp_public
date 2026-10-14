#include "RenderComponent.h"
#include "Logger.h"


template <typename C>
inline C* RenderComponent::AddDrawable()
{
	C* drawable = new C();
	Logger::Info("Added Drawable");
	drawables.emplace_back(drawable);
	drawable->Start();
	return drawable;
}