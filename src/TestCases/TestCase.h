#pragma once
#include "../Vector/Vector.h"

class TestCase
{
private:
	/********* Static Private Variables *********/
	static TestCase* curr_test_case;

	/********* Private Variables *********/
	bool is_spacialHash = false;
	float restitution = .9;
	float gravity = 5;
	float radius = 20;
	int num_balls = 100;
	Vec2D spawn_point = Vec2D(radius, radius);
	Vec2D ball_velocity = Vec2D(0, 0);
public:
	/********* Public Getters *********/
	inline static TestCase* GetCurrTestCase() { return curr_test_case; }
	inline static float GetRestitution() {
		if (curr_test_case == nullptr) {
			return 0;
		}
		return curr_test_case->restitution;
	}
	inline static Vec2D GetBallVelocity() {
		if (curr_test_case == nullptr) {
			return Vec2D(0, 0);
		}
		return curr_test_case->ball_velocity;
	}
	inline static float GetGravity() { 
		if (curr_test_case == nullptr) {
			return 0;
		}
		return curr_test_case->gravity;
	}
	inline static float GetRadius() {
		if (curr_test_case == nullptr) {
			return 0;
		}
		return curr_test_case->radius;
	}
	inline static int GetNumBalls() {
		if (curr_test_case == nullptr) {
			return 0;
		}
		return curr_test_case->num_balls;
	}
	inline static Vec2D GetSpawnPoint() {
		if (curr_test_case == nullptr) {
			return Vec2D(0, 0);
		}
		return curr_test_case->spawn_point;
	}
	inline static bool IsSpacialHash() {
		if (curr_test_case == nullptr) {
			return false;
		}
		return curr_test_case->is_spacialHash;
	}

	/********* Builder Functions *********/
	inline TestCase* SetRestitution(float restitution) {
		this->restitution = restitution;
		return this;
	}
	inline TestCase* SetGravity(float gravity) {
		this->gravity = gravity;
		return this;
	}
	inline TestCase* SetNumBalls(int num_balls) {
		this->num_balls = num_balls;
		return this;
	}
	inline TestCase* SetSpawnPoint(Vec2D spawn_point) {
		this->spawn_point = spawn_point;
		return this;
	}
	inline TestCase* SetBallVelocity(Vec2D ball_velocity) {
		this->ball_velocity = ball_velocity;
		return this;
	}
	inline TestCase* SetRadius(float radius) {
		this->radius = radius;
		return this;
	}
	inline TestCase* SetIsSpacialHash(bool spacialHash) {
		this->is_spacialHash = spacialHash;
		return this;
	}


	/********* Static Functions *********/
	static void SetCurrTestCase(TestCase* test_case);
};

