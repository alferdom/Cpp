#pragma once

#include "glm/glm.hpp"

class CatmullRow
{
public:
	CatmullRow();
	~CatmullRow();

	const glm::vec3 getResultPosition();
	const glm::vec3 getResultDirection();

   float paramT;

private:
	glm::vec3 resultPosition;
	glm::vec3 resultDirection;

	const float pointsCount = 6.0;

	void calcCurve(const glm::vec3 points[], const size_t pointsCount, const float paramT);
	void calcCurve_1Deriv(const glm::vec3 points[], const size_t pointsCount, const float paramT);
	float findSegment() const;

	glm::vec3 calcSegment(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, const float paramT);
	glm::vec3 calcSegment_1Deriv(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, const float paramT);
};
