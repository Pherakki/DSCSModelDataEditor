#pragma once

#include "../Utils/Matrix.hpp"
#include <math.h>


static const float pi = 3.1415f;

// Can do so much SIMD here if bothered
class Camera
{
public:
	float cam_ang_speed = pi / 400;
	float cam_cart_speed = 0.0005f;
	float fov = pi * 45.f / 180.f;
	float zNear = 0.1f;
	float zFar = 1000.f;

	std::array<float, 3> getPosition() { return this->position; }
	std::array<float, 3> getTarget()   { return this->target; }
	std::array<float, 3> getUp()       { return this->up; }

	void setPosition(const std::array<float, 3>& pos) { this->position = pos;  this->calcPosToTarget(); this->calcPolarPosition(); };
	void setTarget(const std::array<float, 3>& pos)   { this->target = pos;    this->calcPosToTarget(); this->calcPolarPosition(); };
	void incPosition(const std::array<float, 3>& inc) { for (uint8_t i = 0; i < 3; ++i) this->position[i] += inc[i]; this->calcPosToTarget(); this->calcPolarPosition(); };
	void incTarget(const std::array<float, 3>& inc)   { for (uint8_t i = 0; i < 3; ++i) this->target[i]   += inc[i]; this->calcPosToTarget(); this->calcPolarPosition(); };
	void incPosAndTarget(const std::array<float, 3>& inc) { for (uint8_t i = 0; i < 3; ++i) { this->position[i] += inc[i]; this->target[i] += inc[i]; }; this->calcPosToTarget(); this->calcPolarPosition(); };

	void translate(float x, float y)
	{
		auto rotation    = makeRotationMatrix(this->position, this->target, this->up);
		auto translation = genericMatrixProduct<3, 3, 1>(rotation, { -this->radius*cam_cart_speed*x, this->radius*cam_cart_speed*y, -this->radius*cam_cart_speed });

		this->incPosAndTarget(translation);
	}

	void mulRadius(float fac) 
	{  
		this->radius += (fac * this->cam_cart_speed) * this->radius;
		this->calcCartPosition();
	};
	void incAltAzi(float aziInc, float altInc) 
	{ 
		this->azimuth += aziInc*this->cam_ang_speed;
		this->enforceAzimuthBounds();
		this->altitude += altInc * this->cam_ang_speed * this->up[1];
		this->enforceAltitudeBounds();
		this->calcCartPosition(); 
	}
	void flipUpDirection() { this->up[1] = -this->up[1]; }

private:
	float radius = 0.f;
	std::array<float, 3> position{ 0.f, 0.f, 0.f };
	std::array<float, 3> target{ 0.f, 0.f, 0.f };
	std::array<float, 3> pos_to_target{ 0.f, 0.f, 0.f };
	std::array<float, 3> up{ 0.f, 1.f, 0.f };

	float azimuth = 0.f;
	float altitude = 0.f;

	void calcPosToTarget()
	{
		this->pos_to_target[0] = this->position[0] - this->target[0];
		this->pos_to_target[1] = this->position[1] - this->target[1];
		this->pos_to_target[2] = this->position[2] - this->target[2];
	}
	void calcRadius() { this->radius = std::sqrt(dot(this->pos_to_target, this->pos_to_target)); };
	void calcPolarPosition()
	{
		this->calcRadius();
		this->azimuth = atan2(this->pos_to_target[2], this->pos_to_target[0]);
		this->altitude = acos(this->pos_to_target[1] / this->radius);
	};
	void calcCartPosition()
	{
		this->pos_to_target[0] = this->radius * cos(this->azimuth) * sin(this->altitude);
		this->pos_to_target[2] = this->radius * sin(this->azimuth) * sin(this->altitude);
		this->pos_to_target[1] = this->radius * cos(this->altitude);

		this->position[0] = this->pos_to_target[0] + this->target[0];
		this->position[1] = this->pos_to_target[1] + this->target[1];
		this->position[2] = this->pos_to_target[2] + this->target[2];
	};

	void enforceAzimuthBounds()
	{
		if (this->azimuth < -pi || this->azimuth > pi)
		{
			this->azimuth += pi;
			this->azimuth -= floorf(this->azimuth / (2 * pi)) * 2 * pi;
			this->azimuth -= pi;
		}
	}

	void enforceAltitudeBounds()
	{
		if (this->altitude > pi)
		{
			this->altitude = ceilf(altitude / pi) * pi - this->altitude;
			this->azimuth += pi;
			this->flipUpDirection();
		}
		else if (this->altitude < 0)
		{
			this->altitude -= this->altitude;
			this->azimuth += pi;
			this->flipUpDirection();
		}
		else if (this->altitude == 0) // Disgusting hack to stop the model disappearing for some reason (doesn't work though, RIP)
			this->altitude = 0.0001f;
	}
};