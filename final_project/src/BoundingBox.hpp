#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <vector>
#include "glm/glm.hpp"

// primitive xy bounding box
class BoundingBox {
public:
	BoundingBox();
    BoundingBox(glm::vec4 bounds);

	// true if this intersects another box
	bool intersects(const BoundingBox &other) const;

	int id;

	std::vector<glm::vec4> points;

	void update(glm::mat4 matrix);
private:
	// x_min, x_max, y_min, y_max
	glm::vec4 _bounds;

	void setPoints();
	void calcBounds();

	std::vector<glm::vec4> _xformedPoints;
};

#endif
