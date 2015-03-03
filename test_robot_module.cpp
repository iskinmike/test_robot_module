#include <stdio.h>
#include <map>
#include <functional>

#include <windows.h>

#include "../module_headers/module.h"
#include "../module_headers/robot_module.h"

#include "test_robot_module.h"

/* GLOBALS CONFIG */
const int COUNT_ROBOTS = 99;
const int COUNT_FUNCTIONS = 4;
const int COUNT_AXIS = 3;

#define DEFINE_ALL_FUNCTIONS \
	ADD_ROBOT_FUNCTION("none", 0, false)\
	ADD_ROBOT_FUNCTION("do_something", 1, false)\
	ADD_ROBOT_FUNCTION("get_some_value", 1, false)\
	ADD_ROBOT_FUNCTION("throw_exception", 0, true)

#define DEFINE_ALL_AXIS \
	ADD_ROBOT_AXIS("X", 100, -100)\
	ADD_ROBOT_AXIS("Y", 1, 0)\
	ADD_ROBOT_AXIS("Z", 100, 0)

FunctionResult* TestRobot::executeFunction(regval command_index, regval *args) {
	switch (command_index) {
		case 1: {
			break;
		}
		case 2: {
			Sleep(args[0]);
			break;
		}
		case 3: {
			if (args[0]) {
				Sleep(args[0]);
			}
			FunctionResult *fr = new FunctionResult(1, rand());
			return fr;
		}
		case 4: {
			FunctionResult *fr = new FunctionResult(0);
			return fr;
		}
		default: break;
	}
	return NULL;
}

void TestRobot::axisControl(regval axis_index, regval value) {
	const char *name;
	switch (axis_index) {
		case 1: { name = "X"; break; }
		case 2: { name = "Y"; break; }
		case 3: { name = "Z"; break; }
		default: { name = "O_o"; break; };
	}
	parent->colorPrintf(ConsoleColor(ConsoleColor::green), "change axis value: %s = %d\n", name, value);
}

TestRobotModule::TestRobotModule() {
	{
		robot_functions = new FunctionData*[COUNT_FUNCTIONS];
		regval function_id = 0;
		DEFINE_ALL_FUNCTIONS
	}
	{
		robot_axis = new AxisData*[COUNT_AXIS];
		regval axis_id = 0;
		DEFINE_ALL_AXIS
	}
}

const char *TestRobotModule::getUID() {
	return "Test robot module v1.00";
}

void TestRobotModule::colorPrintf(ConsoleColor colors, const char *mask, ...) {
	va_list args;
    va_start(args, mask);
    (*colorPrintf_p)(this, colors, mask, args);
    va_end(args);
}

void TestRobotModule::prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p) {
	this->colorPrintf_p = colorPrintfVA_p;
}

int TestRobotModule::init() {
	for (int i = 0; i < COUNT_ROBOTS; ++i) {
		TestRobot *test_robot = new TestRobot(this);
		aviable_connections[i] = test_robot;
	}
	return 0;
}

FunctionData** TestRobotModule::getFunctions(int *count_functions) {
	(*count_functions) = COUNT_FUNCTIONS;
	return robot_functions;
}

AxisData** TestRobotModule::getAxis(int *count_axis) {
	(*count_axis) = COUNT_AXIS;
	return robot_axis;
}

Robot* TestRobotModule::robotRequire() {
	colorPrintf(ConsoleColor(), "new robot require\n");

	for(m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second->isAviable) {
			colorPrintf(ConsoleColor(ConsoleColor::green), "finded free robot: %p\n", i->second);
			
			TestRobot *tr = i->second;
			tr->isAviable = false;

			return tr;
		}
	}
	return NULL;
}

void TestRobotModule::robotFree(Robot *robot) {
	TestRobot *test_robot = reinterpret_cast<TestRobot*>(robot);

	for(m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second == test_robot) {
			colorPrintf(ConsoleColor(), "free robot: %p\n", test_robot);
			test_robot->isAviable = true;
			return;
		}
	}
}

void TestRobotModule::final() {
	for(m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		delete i->second;
	}
	aviable_connections.clear();
}

void TestRobotModule::destroy() {
	for (int j = 0; j < COUNT_FUNCTIONS; ++j) {
		delete robot_functions[j];
	}
	for (int j = 0; j < COUNT_AXIS; ++j) {
		delete robot_axis[j];
	}
	delete[] robot_functions;
	delete[] robot_axis;
	delete this;
}

__declspec(dllexport) RobotModule* getRobotModuleObject() {
	return new TestRobotModule();
}