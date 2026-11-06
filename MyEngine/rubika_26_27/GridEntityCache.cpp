#include "GridEntityCache.h"
#include "DynamicColliderComponent.h"
#include "TransformComponent.h"
#include <SFML/Graphics.hpp>

#include "Globals.h"
#include "SceneMgr.h"
#include "Scene.h"

void GridEntityCache::SetBounds(const sf::FloatRect& bounds)
{
	m_bounds = bounds;
}

void GridEntityCache::SetCellSize(const sf::Vector2f& cellSize)
{
	if (cellSize.x > 0.f && cellSize.y > 0.f) m_CellSize = cellSize;
}

void GridEntityCache::SetDebugColor(const sf::Color color)
{
	debugColor = color;
}


void GridEntityCache::Build()
{
	m_Cols = std::max(1, (int)std::ceil(m_bounds.size.x / m_CellSize.x));
	m_Rows = std::max(1, (int)std::ceil(m_bounds.size.y / m_CellSize.y));
	cells.assign((size_t)m_Cols * m_Rows, {});
}


void GridEntityCache::GetEntities(const sf::Vector2f& position, std::vector<Entity*>& out) const
{
	out = cells[CellIndexY(position.y) * m_Cols + CellIndexX(position.x)];
}

void GridEntityCache::GetEntities(const sf::FloatRect& rect, std::vector<Entity*>& out) const
{
	out.clear();

	for (int y = CellIndexY(rect.position.y); y <= CellIndexY(rect.position.y + rect.size.y); ++y)
	{
		for (int x = CellIndexX(rect.position.x); x <= CellIndexX(rect.position.x + rect.size.x); ++x)
		{
			auto& cell = cells[y * m_Cols + x];
			out.insert(out.end(), cell.begin(), cell.end());
		}
	}

	std::sort(out.begin(), out.end());
	out.erase(std::unique(out.begin(), out.end()), out.end());


}

void GridEntityCache::GetAllEntities(std::vector<Entity*>& out) const
{
	out.clear();

	out.insert(out.end(), entityList.begin(), entityList.end());

	std::sort(out.begin(), out.end());
	out.erase(std::unique(out.begin(), out.end()), out.end());
}



int GridEntityCache::CellIndexX(float worldX) const
{
	int x = (int)std::floor((worldX - m_bounds.position.x) / m_CellSize.x);
	return std::clamp(x, 0, m_Cols - 1);
}

int GridEntityCache::CellIndexY(float worldY) const
{
	int y = (int)std::floor((worldY - m_bounds.position.y) / m_CellSize.y);
	return std::clamp(y, 0, m_Rows - 1);
}

GridEntityCache::CellRange GridEntityCache::RangeOf(const sf::FloatRect& rect) const
{
	CellRange r;
	r.minX = CellIndexX(rect.position.x);
	r.minY = CellIndexY(rect.position.y);
	r.maxX = CellIndexX(rect.position.x + rect.size.x);
	r.maxY = CellIndexY(rect.position.y + rect.size.y);
	return r;
}

void GridEntityCache::DrawDebug(sf::RenderWindow& window) const
{
	sf::RectangleShape cellShape(m_CellSize);
	cellShape.setFillColor(debugColor);
	//sf::Color(255, 200, 0, 90)

	for (int y = 0; y < m_Rows; ++y)
	{
		for (int x = 0; x < m_Cols; ++x)
		{
			
			if (cells[y * m_Cols + x].empty()) continue;
			
			cellShape.setPosition({ m_bounds.position.x + x * m_CellSize.x, m_bounds.position.y + y * m_CellSize.y });
			window.draw(cellShape);
			
		}
	}

	const sf::Color lineColor(120, 120, 120, 150);
	const float right = m_bounds.position.x + m_Cols * m_CellSize.x;
	const float bottom = m_bounds.position.y + m_Rows * m_CellSize.y;

	sf::VertexArray lines(sf::PrimitiveType::Lines);

	for (int x = 0; x <= m_Cols; ++x)
	{
		float wx = m_bounds.position.x + x * m_CellSize.x;
		lines.append(sf::Vertex{ {wx, m_bounds.position.y}, lineColor });
		lines.append(sf::Vertex{ {wx, bottom}, lineColor });
	}
	for (int y = 0; y <= m_Rows; ++y)
	{
		float wy = m_bounds.position.y + y * m_CellSize.y;
		lines.append(sf::Vertex{ {m_bounds.position.x, wy}, lineColor });
		lines.append(sf::Vertex{ {right, wy}, lineColor });
	}

	window.draw(lines);
}


void GridEntityCache::InsertInCells(Entity* entity, const CellRange& r)
{
	for (int y = r.minY; y <= r.maxY; ++y)
		for (int x = r.minX; x <= r.maxX; ++x)
			cells[y * m_Cols + x].push_back(entity);
}

void GridEntityCache::EraseFromCells(Entity* entity, const CellRange& r)
{
	for (int y = r.minY; y <= r.maxY; ++y)
	{
		for (int x = r.minX; x <= r.maxX; ++x)
		{
			auto& cell = cells[y * m_Cols + x];
			cell.erase(std::remove(cell.begin(), cell.end(), entity), cell.end());
		}
	}
		
}

void GridEntityCache::AddEntity(Entity* entity)
{
	auto* collider = entity->GetComponent<DynamicColliderComponent>();
	if (!collider) return;

	if (ranges.count(entity))   
	{
		UpdateEntity(entity);
		return;
	}

	CellRange r = RangeOf(collider->GetWorldHitbox());
	InsertInCells(entity, r);
	ranges[entity] = r;
	entityList.push_back(entity);
}

void GridEntityCache::RemoveEntity(Entity* entity)
{
	auto it = ranges.find(entity);
	if (it == ranges.end()) return;

	EraseFromCells(entity, it->second);  
	ranges.erase(it);
	entityList.erase(std::remove(entityList.begin(), entityList.end(), entity), entityList.end());
}

void GridEntityCache::UpdateEntity(Entity* entity)
{
	auto it = ranges.find(entity);
	if (it == ranges.end()) return;

	auto* collider = entity->GetComponent<DynamicColliderComponent>();
	if (!collider) return;

	CellRange newRange = RangeOf(collider->GetWorldHitbox());
	if (newRange == it->second) return;  

	EraseFromCells(entity, it->second);
	InsertInCells(entity, newRange);
	it->second = newRange;
}