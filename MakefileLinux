# Environment
CC=g++
CCC=g++
CXX=g++

# Constants
FLAGS_MUTUAL = -Wall -m32 -std=c++11 -MMD -MP -MF "${OBJECTDIR}/$@.d"

.PHONY: debug

debug: OBJECTDIR = build/Debug
debug: FLAGS = -g ${FLAGS_MUTUAL}
debug: FILENAME_FIX = debug
debug: clean module

release: OBJECTDIR = build/Release
release: FLAGS = -O3 ${FLAGS_MUTUAL}
release: FILENAME_FIX = release
release: clean module


clean:
	@echo "-------------------------" 
	rm -rf ${OBJECTDIR}


test_robot_module.o:  test_robot_module.cpp
	@echo "-------------------------" 
	mkdir -p ${OBJECTDIR}
	rm -f "${OBJECTDIR}/$@.d"
	$(COMPILE.cc) -c $(FLAGS) -o ${OBJECTDIR}/test_robot_module.o test_robot_module.cpp


module: test_robot_module.o
	@echo "-------------------------" 
	mkdir -p ${OBJECTDIR}
	${LINK.cc} -shared -o ${OBJECTDIR}/test_module_$(FILENAME_FIX).so ${OBJECTDIR}/test_robot_module.o


