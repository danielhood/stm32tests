/*
 * Tests.h
 *
 *  Created on: Mar 12, 2024
 *      Author: conflictedoperator
 */

#ifndef SRC_TESTS_H_
#define SRC_TESTS_H_

class Tests {
public:
	Tests(int id);
	virtual ~Tests();


	int ExecuteTests();
private:
	int _id;
};

#endif /* SRC_TESTS_H_ */
