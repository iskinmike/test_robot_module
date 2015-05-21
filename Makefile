# Environment
CC=g++
CCC=g++
CXX=g++

# Constants
FLAGS_MUTUAL = -Wall -m32 -std=c++11 -MMD -MP -MF "${OBJECTDIR}/$@.d"
SOURCE = test_robot_module
TARGET = test_module


.PHONY: debug


debug: OBJECTDIR = build/Debug
debug: FLAGS = -g ${FLAGS_MUTUAL}
debug: FILENAME_FIX = debug
debug: clean all


release: OBJECTDIR = build/Release
release: FLAGS = -O3 ${FLAGS_MUTUAL}
release: FILENAME_FIX = release
release: clean all


all: test_module_$(FILENAME_FIX).so
	@echo "-------------------------" 

clean:
	@echo "-------------------------" 
	rm -rf ${OBJECTDIR}


$(SOURCE).o:  $(SOURCE).cpp
	@echo "-------------------------" 
	mkdir -p ${OBJECTDIR}
	rm -f "${OBJECTDIR}/$@.d"
	$(COMPILE.cc) -c $(FLAGS) -o ${OBJECTDIR}/$(SOURCE).o $(SOURCE).cpp


test_module_$(FILENAME_FIX).so: $(SOURCE).o
	@echo "-------------------------" 
	mkdir -p ${OBJECTDIR}
	${LINK.cc} -shared -o ${OBJECTDIR}/test_module_$(FILENAME_FIX).so ${OBJECTDIR}/$(SOURCE).o


