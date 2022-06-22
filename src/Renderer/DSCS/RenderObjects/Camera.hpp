#pragma once

#include "../../../Utils/Matrix.hpp"


static const float pi = 3.1415f;

// Can do so much SIMD here if bothered
class Camera
{
public:
	float cam_ang_speed = pi / 400;
	float cam_cart_speed = 0.0005f; // Need a factor in here for the screen size
	float fov = 45.f * (pi  / 180.f); // 45 degrees converted to radians
	float zNear = 0.1f;
	float zFar = 1000.f;

	const std::array<float, 3>& getPosition() { return this->position; }
	const std::array<float, 3>& getTarget()   { return this->target; }
	const std::array<float, 3>& getUp()       { return this->up; }

	void setPosition(const std::array<float, 3>& pos);
	void setTarget(const std::array<float, 3>& pos);
	void incPosition(const std::array<float, 3>& inc);
	void incTarget(const std::array<float, 3>& inc);
	void incPosAndTarget(const std::array<float, 3>& inc);

	void translate(float x, float y);
	void mulRadius(float fac);
	void incAltAzi(float aziInc, float altInc);
	void flipUpDirection() { this->up[1] = -this->up[1]; }

private:
	float azimuth = 0.f;
	float altitude = 0.f;
	float radius = 0.f;
	std::array<float, 3> position{ 0.f, 0.f, 0.f };
	std::array<float, 3> target{ 0.f, 0.f, 0.f };
	std::array<float, 3> pos_to_target{ 0.f, 0.f, 0.f };
	std::array<float, 3> up{ 0.f, 1.f, 0.f };


	void calcPosToTarget();
	void calcRadius();
	void calcPolarPosition();
	void calcCartPosition();
	void enforceAzimuthBounds();
	void enforceAltitudeBounds();
};