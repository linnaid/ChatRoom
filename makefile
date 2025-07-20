# === 项目配置 ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iutils
LDFLAGS =

# === 目录结构 ===
SRC_DIR = src
INCLUDE_DIR = include
UTILS_DIR = utils
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN = $(BUILD_DIR)/chatroom

# === 查找源文件 ===
SRCS = $(wildcard $(SRC_DIR)/**/*.cpp) \
       $(wildcard $(UTILS_DIR)/*.cpp) \
       main.cpp

OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# === 默认目标 ===
all: $(BIN)

# === 编译目标 ===
$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# === 生成 .o 文件规则 ===
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === 清理目标 ===
clean:
	rm -rf $(BUILD_DIR)

# === 运行目标 ===
run: all
	./$(BIN)
