#include "RigidBody.h"


RigidBody::RigidBody(bool kin, std::map<int, Vector2> points, Vector2 a, double d) {
	kinematic = kin;
	drag = d;
	bounds = std::unique_ptr<std::map<int, Vector2>>(new std::map<int, Vector2>(points));
	accel = std::unique_ptr<Vector2>(new Vector2{a.x, a.y});
}

void RigidBody::ApplyConstantForce(Vector2 acceleration) {
	accel.get()->x = acceleration.x;
	accel.get()->y = acceleration.y;
}

Vector2 RigidBody::GetAcceleration() {
	return Vector2{ accel.get()->x, accel.get()->y };
}

bool RigidBody::IsKinemtaic() {
	return kinematic;
}

bool RigidBody::IsHit() {
	std::lock_guard<std::mutex> readLock(rigidMutex);
	return false;
}

void RigidBody::UpdateBound(int index, Vector2 data) {
	if (bounds.get()->find(index) == bounds.get()->end()) {
		return;
	}
	bounds.get()->insert_or_assign(index, data);
}

void RigidBody::UpdateBounds(std::map<int, Vector2> points) {
	std::lock_guard<std::mutex> writeLock(rigidMutex);
	for (auto key : points) {
		std::thread(&RigidBody::UpdateBound, this, key.first, key.second);
	}
}