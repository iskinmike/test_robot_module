#ifndef TEST_ROBOT_MODULE_H
#define	TEST_ROBOT_MODULE_H

class TestRobot : public Robot {
    public: 
		bool isAviable;
		TestRobot() : isAviable(true) {}
		FunctionResult* executeFunction(regval command_index, regval *args);
		void axisControl(regval axis_index, regval value);
        ~TestRobot() {}
};
typedef std::map<int, TestRobot*> m_connections;

class TestRobotModule : public RobotModule {
	m_connections aviable_connections;
	FunctionData *robot_functions;
	AxisData *robot_axis;

	public:
		TestRobotModule();
		int init();
		FunctionData* getFunctions(int *count_functions);
		AxisData* getAxis(int *count_axis);
		Robot* robotRequire();
		void robotFree(Robot *robot);
		void final();
		void destroy();
		~TestRobotModule() {};
};

extern "C" {
	__declspec(dllexport) RobotModule* getRobotModuleObject();
}

#define ADD_ROBOT_FUNCTION(FUNCTION_NAME, COUNT_PARAMS, GIVE_EXCEPTION) \
	robot_functions[function_id].command_index = function_id + 1; \
	robot_functions[function_id].count_params = COUNT_PARAMS; \
	robot_functions[function_id].give_exception = GIVE_EXCEPTION; \
	robot_functions[function_id].name = FUNCTION_NAME; \
	function_id++;

#define ADD_ROBOT_AXIS(AXIS_NAME, UPPER_VALUE, LOWER_VALUE) \
	robot_axis[axis_id].axis_index = axis_id + 1; \
	robot_axis[axis_id].upper_value = UPPER_VALUE; \
	robot_axis[axis_id].lower_value = LOWER_VALUE; \
	robot_axis[axis_id].name = AXIS_NAME; \
	axis_id++;

#endif	/* TEST_ROBOT_MODULE_H */