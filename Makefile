SRC_DIR = src
OBJ_DIR = obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
CXXFLAGS = -Wall -O3 -MMD -std=c++11
CXX=g++

measure: ${OBJ_FILES}
	${CXX} ${CXXFLAGS} -o $@ ${OBJ_FILES}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	   $(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -rf obj/* measure

-include $(OBJ_FILES:.o=.d)
