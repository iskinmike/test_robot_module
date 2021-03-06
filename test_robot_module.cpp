#include <stdio.h>
#include <map>
#include <functional>

#include <windows.h>

#include "../module_headers/module.h"
#include "../module_headers/robot_module.h"

#include "test_robot_module.h"

/* GLOBALS CONFIG */
const unsigned int COUNT_ROBOTS = 99;
const unsigned int COUNT_FUNCTIONS = 5;
const unsigned int COUNT_AXIS = 3;

#define DEFINE_ALL_AXIS \
	ADD_ROBOT_AXIS("X", 100, -100)\
	ADD_ROBOT_AXIS("Y", 1, 0)\
	ADD_ROBOT_AXIS("Z", 100, 0)

TestRobotModule::TestRobotModule() {
	{
		robot_functions = new FunctionData*[COUNT_FUNCTIONS];
		system_value function_id = 0;
		FunctionData::ParamTypes *pt;

		robot_functions[function_id] = new FunctionData(function_id + 1, 0, NULL, "none");
		function_id++;
		
		pt = new FunctionData::ParamTypes[1];
		pt[0] = FunctionData::ParamTypes::FLOAT;
		robot_functions[function_id] = new FunctionData(function_id + 1, 1, pt, "do_something");
		function_id++;

		pt = new FunctionData::ParamTypes[1];
		pt[0] = FunctionData::ParamTypes::FLOAT;
		robot_functions[function_id] = new FunctionData(function_id + 1, 1, pt, "get_some_value");
		function_id++;

		robot_functions[function_id] = new FunctionData(function_id + 1, 0, NULL, "throw_exception");
		function_id++;
		
		pt = new FunctionData::ParamTypes[1];
		pt[0] = FunctionData::ParamTypes::STRING;
		robot_functions[function_id] = new FunctionData(function_id + 1, 1, pt, "print");
		function_id++;
	}
	{
		robot_axis = new AxisData*[COUNT_AXIS];
		system_value axis_id = 0;
		DEFINE_ALL_AXIS
	}
}

const char *TestRobotModule::getUID() {
	return "Test robot module v1.01";
}

void TestRobotModule::prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p) {
	this->colorPrintf_p = colorPrintfVA_p;
}

FunctionData** TestRobotModule::getFunctions(unsigned int *count_functions) {
	(*count_functions) = COUNT_FUNCTIONS;
	return robot_functions;
}

AxisData** TestRobotModule::getAxis(unsigned int *count_axis) {
	(*count_axis) = COUNT_AXIS;
	return robot_axis;
}

void *TestRobotModule::writePC(unsigned int *buffer_length) {
	(*buffer_length) = 0;
	return NULL;
}

int TestRobotModule::init() {
	for (unsigned int i = 0; i < COUNT_ROBOTS; ++i) {
		TestRobot *test_robot = new TestRobot(this);
		aviable_connections[i] = test_robot;
	}
	return 0;
}

Robot* TestRobotModule::robotRequire() {
	colorPrintf(ConsoleColor(), "new robot require\n");

	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
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

	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second == test_robot) {
			colorPrintf(ConsoleColor(), "free robot: %p\n", test_robot);
			test_robot->isAviable = true;
			return;
		}
	}
}

void TestRobotModule::final() {
	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		delete i->second;
	}
	aviable_connections.clear();
}

int TestRobotModule::startProgram(int uniq_index, void *buffer, unsigned int buffer_length) {
	return 0;
}

int TestRobotModule::endProgram(int uniq_index) {
	return 0;
}

void TestRobotModule::destroy() {
	for (unsigned int j = 0; j < COUNT_FUNCTIONS; ++j) {
		if (robot_functions[j]->count_params) {
			delete[] robot_functions[j]->params;
		}
		delete robot_functions[j];
	}
	for (unsigned int j = 0; j < COUNT_AXIS; ++j) {
		delete robot_axis[j];
	}
	delete[] robot_functions;
	delete[] robot_axis;
	delete this;
}

void TestRobotModule::colorPrintf(ConsoleColor colors, const char *mask, ...) {
	va_list args;
	va_start(args, mask);
	(*colorPrintf_p)(this, colors, mask, args);
	va_end(args);
}

FunctionResult* TestRobot::executeFunction(system_value command_index, void **args) {
	FunctionResult *fr = NULL;

	switch (command_index) {
		case 1: {
			break;
		}
		case 2: {
			variable_value *vv = (variable_value*) args[0];
			Sleep((DWORD) *vv);
			break;
		}
		case 3: {
			variable_value *vv = (variable_value*) args[0];
			if (*vv) {
				Sleep((DWORD) *vv);
			}
			fr = new FunctionResult(1, rand());
			break;
		}
		case 4: {
			fr = new FunctionResult(0);
			break;
		}
		case 5: {
			puts((const char *) args[0]);
			break;
		}
		default: break;
	}

	return fr;
}

void TestRobot::axisControl(system_value axis_index, variable_value value) {
	const char *name;
	switch (axis_index) {
		case 1: { name = "X"; break; }
		case 2: { name = "Y"; break; }
		case 3: { name = "Z"; break; }
		default: { name = "O_o"; break; };
	}
	parent->colorPrintf(ConsoleColor(ConsoleColor::green), "change axis value: %s = %d\n", name, value);
}

__declspec(dllexport) RobotModule* getRobotModuleObject() {
	return new TestRobotModule();
}