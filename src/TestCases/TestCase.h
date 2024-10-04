#pragma once

class TestCase
{
private:
	/********* Static Private Variables *********/
	static TestCase* curr_test_case;

	/********* Private Variables *********/
	float restitution = .9;
	float gravity = .5;
	float radius = 10;
	int num_balls = 100;
public:
	/********* Public Getters *********/
	inline static TestCase* GetCurrTestCase() { return curr_test_case; }
	inline static float GetRestitution() {
		if (curr_test_case == nullptr) {
			return 0;
		}
		return curr_test_case->restitution;
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


	/********* Static Functions *********/
	static void SetCurrTestCase(TestCase* test_case);
};

