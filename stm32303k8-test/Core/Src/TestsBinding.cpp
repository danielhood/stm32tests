/*
 * TestsBinding.cpp
 *
 *  Created on: Mar 12, 2024
 *      Author: conflictedoperator
 */

#include "Tests.h"

struct TestParams {        // has no base classes or virtual functions
	TestParams(int id) : id(id) { }
    int id;
    int result;
};

extern "C" void ExecTest1(TestParams *testParams) {
// Definition of the C++ function to be called from C.
// The function is written in C++, only the linkage is C.

	Tests *tests = new Tests(testParams->id);
    testParams->result = tests->ExecuteTests();
}
