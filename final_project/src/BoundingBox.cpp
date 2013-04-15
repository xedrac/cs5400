
#include "BoundingBox.hpp"

using namespace std;

BoundingBox::BoundingBox()
{
    setPoints();
}

BoundingBox::BoundingBox(const Box3D &box)
{
    _box = box;
    setPoints();
}


// Checks to see if two BoundingBoxes overlap.  Assumes axis-aligned edges
bool BoundingBox::intersects(const BoundingBox &other) const
{
    return !((_box.x0 > other.getBox().x1 || _box.x1 < other.getBox().x0) ||
             (_box.y0 > other.getBox().y1 || _box.y1 < other.getBox().y0) ||
             (_box.z0 > other.getBox().z1 || _box.z1 < other.getBox().z0));
}

void BoundingBox::setBox(const Box3D &box)
{
    _box = box;
    setPoints();
}

void BoundingBox::setPoints()
{
	_points.clear();

	_points.push_back(glm::vec4(_box.x0, _box.y0, _box.z0, 1.0));
	_points.push_back(glm::vec4(_box.x0, _box.y1, _box.z0, 1.0));
	_points.push_back(glm::vec4(_box.x1, _box.y1, _box.z0, 1.0));
	_points.push_back(glm::vec4(_box.x1, _box.y0, _box.z0, 1.0));

	_points.push_back(glm::vec4(_box.x0, _box.y0, _box.z1, 1.0));
	_points.push_back(glm::vec4(_box.x0, _box.y1, _box.z1, 1.0));
	_points.push_back(glm::vec4(_box.x1, _box.y1, _box.z1, 1.0));
	_points.push_back(glm::vec4(_box.x1, _box.y0, _box.z1, 1.0));

	_xformedpoints.resize(_points.size());
}


// Naive method to calculate xy square around object
void BoundingBox::calcBounds()
{
    // Clear the box
    _xbox = Box3D();
    
	for (size_t i=0; i<_xformedpoints.size(); i++) {
        const glm::vec4 &p = _xformedpoints[i];

		if      (p.x < _xbox.x0) _xbox.x0 = p.x;
		else if (p.x > _xbox.x1) _xbox.x1 = p.x;

		if      (p.y < _xbox.y0) _xbox.y0 = p.y;
		else if (p.y > _xbox.y1) _xbox.y1 = p.y;

		if      (p.z < _xbox.z0) _xbox.z0 = p.z;
		else if (p.z > _xbox.z1) _xbox.z1 = p.z;
    }
}


void BoundingBox::update(glm::mat4 matrix)
{
	for (size_t i=0; i<_points.size(); i++) {
		_xformedpoints[i] = matrix * _points[i];
	}

	calcBounds();
}
