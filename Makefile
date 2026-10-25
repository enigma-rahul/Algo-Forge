CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include -I./third_party -pthread

BUILD_DIR = build
BIN_SERVER = $(BUILD_DIR)/algo_forge_server
BIN_TESTS = $(BUILD_DIR)/algo_tests

ENGINE_SRCS = src/StepRecorder.cpp \
              src/algorithms/sorting.cpp \
              src/algorithms/searching.cpp \
              src/algorithms/graph.cpp \
              src/algorithms/tree.cpp

SERVER_SRCS = src/main.cpp src/server.cpp $(ENGINE_SRCS)
TEST_SRCS = tests/test_main.cpp $(ENGINE_SRCS)

.PHONY: all clean test run cmake-build

all: $(BIN_SERVER) $(BIN_TESTS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_SERVER): $(SERVER_SRCS) | $(BUILD_DIR)
	@echo "==> Building Algo Forge Server ($@)..."
	$(CXX) $(CXXFLAGS) -o $@ $(SERVER_SRCS)
	@echo "[+] Server build complete."

$(BIN_TESTS): $(TEST_SRCS) | $(BUILD_DIR)
	@echo "==> Building Algo Forge Tests ($@)..."
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRCS)
	@echo "[+] Tests build complete."

test: $(BIN_TESTS)
	@echo "==> Running Automated Engine Tests..."
	@./$(BIN_TESTS)

run: $(BIN_SERVER)
	@echo "==> Starting Algo Forge Server on http://localhost:8080 ..."
	@./$(BIN_SERVER) --port 8080 --web ./web

cmake-build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make -j4

clean:
	rm -rf $(BUILD_DIR)
