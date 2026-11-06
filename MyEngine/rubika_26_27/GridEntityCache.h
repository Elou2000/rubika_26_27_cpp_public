#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics/Rect.hpp>
#include "Entity.h"


class GridEntityCache
{
public:

	void SetBounds(const sf::FloatRect& bounds);
	void SetCellSize(const sf::Vector2f& cellSize);
	void SetDebugColor(const sf::Color color);
	void DrawDebug(sf::RenderWindow& window) const;
	void Build();

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void GetEntities(const sf::Vector2f& position, std::vector<Entity*>& out) const;
	void GetEntities(const sf::FloatRect& rect, std::vector<Entity*>& out) const;
	void GetAllEntities(std::vector<Entity*>& out) const;
	void UpdateEntity(Entity* entity);

	struct CellRange 
	{
		int minX = 0, minY = 0, maxX = 0, maxY = 0; 
		bool operator==(const CellRange& o) const
		{
			return minX == o.minX && minY == o.minY && maxX == o.maxX && maxY == o.maxY;
		}
	};
private:
	

	int CellIndexX(float worldX) const;
	int CellIndexY(float worldY) const;
	CellRange RangeOf(const sf::FloatRect& rect) const;

	void InsertInCells(Entity* entity, const CellRange& r);
	void EraseFromCells(Entity* entity, const CellRange& r);
	std::unordered_map<Entity*, CellRange> ranges;

	std::vector<Entity*> entityList;
	sf::Color debugColor;
	int m_Cols = 0;
	int m_Rows = 0;
	std::vector<std::vector<Entity*>> cells;
	sf::FloatRect m_bounds;
	sf::Vector2f m_CellSize;
};

