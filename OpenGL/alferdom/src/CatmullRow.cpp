#include "CatmullRow.h"

glm::vec3 controlPoints[] = {
  glm::vec3(0.0,  0.0, 0.0),
  glm::vec3(0.33, 0.0,  0.35),
  glm::vec3(0.66, 0.0,  0.35),
  glm::vec3(0.80, 0.0, 0.0),
  glm::vec3(0.66, 0.0, -0.35),
  glm::vec3(0.33, 0.0, -0.35)
};


CatmullRow::CatmullRow()
{}

CatmullRow::~CatmullRow()
{}

const glm::vec3 CatmullRow::getResultPosition()
{
	calcCurve(controlPoints, pointsCount, paramT);
	return resultPosition;
}

const glm::vec3 CatmullRow::getResultDirection()
{
	calcCurve_1Deriv(controlPoints, pointsCount, paramT);
	return resultDirection;
}

void CatmullRow::calcCurve(const glm::vec3 points[], const size_t pointsCount, const float paramT)
{
	resultPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	float paramSeq = findSegment();
	size_t index = size_t(paramSeq);

	resultPosition = calcSegment(
		points[(index - 1 + pointsCount) % pointsCount],
		points[(index) % pointsCount],
		points[(index + 1) % pointsCount],
		points[(index + 2) % pointsCount],
		paramSeq - floor(paramSeq));

}

void CatmullRow::calcCurve_1Deriv(const glm::vec3 points[], const size_t pointsCount, const float paramT)
{
	resultDirection = glm::vec3(0.0f, 0.0f, 0.0f);

	float paramSeq = findSegment();
	size_t index = size_t(paramSeq);

	resultDirection = calcSegment_1Deriv(
		points[(index - 1 + pointsCount) % pointsCount],
		points[(index) % pointsCount],
		points[(index + 1) % pointsCount],
		points[(index + 2) % pointsCount],
		paramSeq - floor(paramSeq)
	);
}

float CatmullRow::findSegment() const
{
	float amp = 6.0f - 0.0f;
	float val = fmod(paramT - 0.0f, amp);

	if (val < float(0))
		val += amp;

	return val;
}

glm::vec3 CatmullRow::calcSegment(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, const float paramT)
{
	const float t2 = pow(paramT, 2);
	const float t3 = pow(paramT, 3);

	return 0.5f * 
		(P0 * (-t3 + 2.0f * t2 - paramT)
		+ P1 * (3.0f * t3 - 5.0f * t2 + 2.0f)
		+ P2 * (-3.0f * t3 + 4.0f * t2 + paramT)
		+ P3 * (t3 - t2));
}


glm::vec3 CatmullRow::calcSegment_1Deriv(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, const float paramT)
{
	const float t2 = pow(paramT, 2);

	return 0.5f * 
		(P0 * (-3.0f * t2 + 4.0f * paramT - 1.0f)
		+ P1 * (9.0f * t2 - 10.0f * paramT)
		+ P2 * (-9.0f * t2 + 8.0f * paramT + 1.0f)
		+ P3 * (3.0f * t2 - 2.0f * paramT));
}
