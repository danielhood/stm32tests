/*
 * Tests.cpp
 *
 *  Created on: Mar 12, 2024
 *      Author: Daniel Hood
 */

#include "Tests.h"

Tests::Tests(int id) {
	_id = id;

}

Tests::~Tests() {
}


int Tests::ExecuteTests() {
	if (_id == 1) {
		return 100;
	} else {
		return 0;
	}
}
