#ifndef TEST_ROBOT_MODULE_H
#define	TEST_ROBOT_MODULE_H

class TestRobotModule;

class TestRobot : public Robot {
	TestRobotModule *parent;

    public: 
		bool isAviable;
		TestRobot(TestRobotModule *parent) : isAviable(true), parent(parent) {}
		FunctionResult* executeFunction(system_value command_index, void **args);
		void axisControl(system_value axis_index, variable_value value);
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
		FunctionData** getFunctions(unsigned int *count_functions);
		AxisData** getAxis(unsigned int *count_axis);
		Robot* robotRequire();
		void robotFree(Robot *robot);
		void final();
		void destroy();
		~TestRobotModule() {};

		void colorPrintf(ConsoleColor colors, const char *mask, ...);
};

#define ADD_ROBOT_AXIS(AXIS_NAME, UPPER_VALUE, LOWER_VALUE) \
	robot_axis[axis_id] = new AxisData; \
	robot_axis[axis_id]->axis_index = axis_id + 1; \
	robot_axis[axis_id]->upper_value = UPPER_VALUE; \
	robot_axis[axis_id]->lower_value = LOWER_VALUE; \
	robot_axis[axis_id]->name = AXIS_NAME; \
	++axis_id;

#endif	/* TEST_ROBOT_MODULE_H */