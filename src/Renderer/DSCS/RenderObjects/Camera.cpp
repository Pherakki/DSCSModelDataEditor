#include <math.h>

#include "Camera.hpp"

// ######## //
// Commands //
// ######## //


void Camera::setPosition(const std::array<float, 3>& pos) 
{ 
	this->position = pos;  
	this->calcPosToTarget(); 
	this->calcPolarPosition();
}

void Camera::setTarget(const std::array<float, 3>& pos)
{ 
	this->target = pos; 
	this->calcPosToTarget();
	this->calcPolarPosition(); 
}

void Camera::incPosition(const std::array<float, 3>& inc) 
{ 
	for (uint8_t i = 0; i < 3; ++i)
		this->position[i] += inc[i]; 
	this->calcPosToTarget(); 
	this->calcPolarPosition(); 
}

void Camera::incTarget(const std::array<float, 3>& inc) 
{ 
	for (uint8_t i = 0; i < 3; ++i) 
		this->target[i] += inc[i]; 
	this->calcPosToTarget(); 
	this->calcPolarPosition();
}

void Camera::incPosAndTarget(const std::array<float, 3>& inc) 
{
	for (uint8_t i = 0; i < 3; ++i) 
	{
		this->position[i] += inc[i]; 
		this->target[i] += inc[i];
	};
	this->calcPosToTarget();
	this->calcPolarPosition();
};

void Camera::translate(float x, float y)
{
	auto rotation = makeRotationMatrix(this->position, this->target, this->up);
	auto translation = genericMatrixProduct<3, 3, 1>(rotation, { -this->radius * cam_cart_speed * x, this->radius * cam_cart_speed * y, -this->radius * cam_cart_speed });

	this->incPosAndTarget(translation);
}


void Camera::mulRadius(float fac)
{
	this->radius += (fac * this->cam_cart_speed) * this->radius;
	this->calcCartPosition();
};

void Camera::incAltAzi(float aziInc, float altInc)
{
	this->azimuth += aziInc * this->cam_ang_speed;
	this->enforceAzimuthBounds();
	this->altitude += altInc * this->cam_ang_speed * this->up[1];
	this->enforceAltitudeBounds();
	this->calcCartPosition();
}

// ##### //
// Maths //
// ##### //

void Camera::calcPosToTarget()
{
	this->pos_to_target[0] = this->position[0] - this->target[0];
	this->pos_to_target[1] = this->position[1] - this->target[1];
	this->pos_to_target[2] = this->position[2] - this->target[2];
}

void Camera::calcRadius()
{ 
	this->radius = std::sqrt(dot(this->pos_to_target, this->pos_to_target)); 
};

void Camera::calcPolarPosition()
{
	this->calcRadius();
	this->azimuth = atan2(this->pos_to_target[2], this->pos_to_target[0]);
	this->altitude = acos(this->pos_to_target[1] / this->radius);
};

void Camera::calcCartPosition()
{
	this->pos_to_target[0] = this->radius * cos(this->azimuth) * sin(this->altitude);
	this->pos_to_target[2] = this->radius * sin(this->azimuth) * sin(this->altitude);
	this->pos_to_target[1] = this->radius * cos(this->altitude);

	this->position[0] = this->pos_to_target[0] + this->target[0];
	this->position[1] = this->pos_to_target[1] + this->target[1];
	this->position[2] = this->pos_to_target[2] + this->target[2];
};

void Camera::enforceAzimuthBounds()
{
	if (this->azimuth < -pi || this->azimuth > pi)
	{
		this->azimuth += pi;
		this->azimuth -= floorf(this->azimuth / (2 * pi)) * 2 * pi;
		this->azimuth -= pi;
	}
}

void Camera::enforceAltitudeBounds()
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
