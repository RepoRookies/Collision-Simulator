#include "TestCase.h"
#include "../CollissionEngine/CollissionEngine.h"

TestCase* TestCase::curr_test_case = nullptr;

void TestCase::SetCurrTestCase(TestCase* test_case) {
	CollissionEngine::getCircles().clear();
	//CollissionEngine::resizeBall();
	curr_test_case = test_case;
	if (test_case->IsSpacialHash() == true) {
		CollissionEngine::InitSpacialHash();
	}
}
