#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <vector>
#include <memory>
#include <limits>
#include "glm/glm.hpp"

struct Box3D
{
    Box3D()
    {
        x0 = y0 = z0 = std::numeric_limits<float>::max();
        x1 = y1 = z1 = std::numeric_limits<float>::lowest();  // don't use min() here
    }

    Box3D(const Box3D &b)
        : x0(b.x0), x1(b.x1),
          y0(b.y0), y1(b.y1),
          z0(b.z0), z1(b.z1)
    {
    }

    float x0, x1;
    float y0, y1;
    float z0, z1;
};



// primitive xy bounding box
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const Box3D &box);

	bool intersects(const BoundingBox &other) const;
	void update(glm::mat4 matrix);
    const Box3D getBounds() const  { return _xbox; }
    glm::vec3 getCenter() const    { return glm::vec3((_xbox.x1-_xbox.x0)/2.0f,
                                                      (_xbox.y1-_xbox.y0)/2.0f,
                                                      (_xbox.z1-_xbox.z0)/2.0f); }
private:
	void setPoints();
	void calcBounds();

private:
    Box3D _box;
    Box3D _xbox;
	std::vector<glm::vec4> _points;
	std::vector<glm::vec4> _xformedpoints;
};




#endif
