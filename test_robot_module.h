#ifndef TEST_ROBOT_MODULE_H
#define	TEST_ROBOT_MODULE_H

class TestRobotModule;

class TestRobot : public Robot {
	TestRobotModule *parent;

    public: 
		bool isAviable;
		TestRobot(TestRobotModule *parent) : isAviable(true), parent(parent) {}
		FunctionResult* executeFunction(regval command_index, regval *args);
		void axisControl(regval axis_index, regval value);
        ~TestRobot() {}
};
typedef std::map<int, TestRobot*> m_connections;

class TestRobotModule : public RobotModule {
	m_connections aviable_connections;
	FunctionData **robot_functions;
	AxisData **robot_axis;
	colorPrintfVA_t *colorPrintf_p;

	public:
		TestRobotModule();
		const char *getUID();
		void prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p);
		int init();
		FunctionData** getFunctions(int *count_functions);
		AxisData** getAxis(int *count_axis);
		Robot* robotRequire();
		void robotFree(Robot *robot);
		void final();
		void destroy();
		~TestRobotModule() {};

		void colorPrintf(ConsoleColor colors, const char *mask, ...);
};

#define ADD_ROBOT_FUNCTION(FUNCTION_NAME, COUNT_PARAMS, GIVE_EXCEPTION) \
	robot_functions[function_id] = new FunctionData; \
	robot_functions[function_id]->command_index = function_id + 1; \
	robot_functions[function_id]->count_params = COUNT_PARAMS; \
	robot_functions[function_id]->give_exception = GIVE_EXCEPTION; \
	robot_functions[function_id]->name = FUNCTION_NAME; \
	++function_id;

#define ADD_ROBOT_AXIS(AXIS_NAME, UPPER_VALUE, LOWER_VALUE) \
	robot_axis[axis_id] = new AxisData; \
	robot_axis[axis_id]->axis_index = axis_id + 1; \
	robot_axis[axis_id]->upper_value = UPPER_VALUE; \
	robot_axis[axis_id]->lower_value = LOWER_VALUE; \
	robot_axis[axis_id]->name = AXIS_NAME; \
	++axis_id;

#endif	/* TEST_ROBOT_MODULE_H */