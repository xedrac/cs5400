
#include "BoundingBox.hpp"
#include <iostream>

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
    return ((_xbox.x0 < other.getBounds().x1 && _xbox.x1 > other.getBounds().x0) &&
            (_xbox.y0 < other.getBounds().y1 && _xbox.y1 > other.getBounds().y0) &&
            (_xbox.z0 < other.getBounds().z1 && _xbox.z1 > other.getBounds().z0));
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

