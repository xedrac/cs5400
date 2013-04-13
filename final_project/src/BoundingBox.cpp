#include "BoundingBox.hpp"

BoundingBox::BoundingBox()
{
	_bounds = glm::vec4(0.0);
	setPoints();
}

BoundingBox::BoundingBox(glm::vec4 bounds)
{
	_bounds = bounds;
	setPoints();
}

// Naive implementation of box intersection
// Checks four scenarios where boxes cannot intersect 
bool BoundingBox::intersects(const BoundingBox &other) const
{
	if (id == other.id)
		return false;

	return !((_bounds.x > other._bounds.y || _bounds.y < other._bounds.x) ||
			 (_bounds.z > other._bounds.w || _bounds.w < other._bounds.z));
}

void BoundingBox::setPoints()
{
	points = std::vector<glm::vec4>();
	points.push_back(glm::vec4(_bounds.x, _bounds.z, 0.0, 1.0));
	points.push_back(glm::vec4(_bounds.y, _bounds.z, 0.0, 1.0));
	points.push_back(glm::vec4(_bounds.x, _bounds.w, 0.0, 1.0));
	points.push_back(glm::vec4(_bounds.y, _bounds.w, 0.0, 1.0));
	_xformedPoints = std::vector<glm::vec4>();
	_xformedPoints.resize(4);
}

// Naive method to calculate xy square around object
void BoundingBox::calcBounds()
{
	// x_min, x_max, y_min, y_max
	glm::vec4 result = glm::vec4(1000, -1000, 1000, -1000);
	glm::vec4* item;
	for (size_t i=0; i<_xformedPoints.size(); i++) {
		item = &_xformedPoints[i];
		if (item->x < result.x)
			result.x = item->x;
		else if (item-> x > result.y)
			result.y = item->x;
		if (item->y < result.z)
			result.z = item->y;
		else if (item->y > result.w)
			result.w = item->y;
    }
	_bounds = result;
}

void BoundingBox::update(glm::mat4 matrix)
{
	for (size_t i = 0; i < points.size(); i++)
	{
		_xformedPoints[i] = glm::mat4(matrix) * points[i];
	}
	calcBounds();
}
