#pragma once
#include "Vector2.h"
#include <map>
#include <memory>
#include <thread>
#include <mutex>


class RigidBody
{
private:
	std::mutex rigidMutex;
	bool kinematic; 
	std::unique_ptr<std::map<int, Vector2>> bounds;
	std::unique_ptr<Vector2> accel;
	double drag;
	

	void UpdateBound(int index,  Vector2 data);

public:
	RigidBody(bool kin, std::map<int, Vector2> points, Vector2 a = { 0,0 }, double d = 0);
	bool IsKinemtaic();
	bool IsHit();
	void UpdateBounds(std::map<int, Vector2> points);
	Vector2 GetAcceleration();
	void ApplyConstantForce(Vector2 acceleration);
	//add q rotation later on. that is a lot harder
};

