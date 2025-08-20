# ========================
# 项目配置
# ========================
PROJECT_NAME := SHMS
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin
CONF_DIR := conf
LOG_DIR := log
DATA_DIR := data

# ========================
# 工具链配置
# ========================
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -I$(INC_DIR) -I$(SRC_DIR)
LDFLAGS := -llog4cpp -lcrypt -lpthread

# 解决宏冲突问题
CXXFLAGS += -DLOG_DEBUG=SHMS_LOG_DEBUG -DLOG_INFO=SHMS_LOG_INFO

# ========================
# 文件自动发现
# ========================
# 查找所有源文件
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# 生成对应的目标文件
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
# 主目标文件
TARGET := $(BIN_DIR)/$(PROJECT_NAME).exe

# ========================
# 构建规则
# ========================
all: create_dirs $(TARGET)

# 创建必要目录
create_dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LOG_DIR)
	@mkdir -p $(DATA_DIR)

# 链接可执行文件
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# 编译源文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ========================
# 实用规则
# ========================
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)/*

install_conf:
	cp $(CONF_DIR)/server.conf $(BIN_DIR)/server.conf

run: all install_conf
	@echo "启动服务器..."
	@$(TARGET) -c $(BIN_DIR)/server.conf

debug: CXXFLAGS += -g -DDEBUG
debug: all

# ========================
# 特殊目标声明
# ========================
.PHONY: all clean create_dirs install_conf run debug
