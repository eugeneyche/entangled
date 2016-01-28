SRC_DIR		:= src
INC_DIR		:= include
BUILD_DIR	:= .build
BIN_DIR		:= bin

CC			:= g++
CFLAGS		:= -O2 -msse2 -std=c++14 -Wall -Wno-unused-result
INCLUDES	:= -I$(INC_DIR)
LIBRARIES	:= -lGL -lGLEW -lSDL2 -lSDL2_image
TARGET		:= $(BIN_DIR)/entangled
SOURCES		:= $(shell find $(SRC_DIR) -name '*.cpp' -type f)
HEADERS		:= $(shell find $(INC_DIR) -name '*.hpp' -type f)
OBJECTS		:= $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBRARIES)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

valgrind:
	valgrind --tool=memcheck $(TARGET)

new: clean $(TARGET)

.PHONY:
	clean
	new
