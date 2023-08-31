#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() :
	m_Center(glm::vec3(0)), m_Radius(5.0f)
{
}

BoundingSphere::BoundingSphere(glm::vec3 center, float radius) :
	m_Center(center), m_Radius(radius)
{
}

BoundingSphere::BoundingSphere(const BoundingSphere& pBoundingSphere):
	m_Center(pBoundingSphere.m_Center), m_Radius(pBoundingSphere.m_Radius)
{
}

BoundingSphere& BoundingSphere::operator=(const BoundingSphere& pBoundingSphere)
{
	if (this != &pBoundingSphere)
	{
		m_Center = pBoundingSphere.m_Center;
		m_Radius = pBoundingSphere.m_Radius;

	}
	return *this;
}

BoundingSphere::~BoundingSphere()
{
}
