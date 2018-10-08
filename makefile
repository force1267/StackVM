BUILD_DIR=./build
CC=g++
ASSM=./build/assm

_dir:
	mkdir -p $(BUILD_DIR)
_vm: _dir
	$(CC) ./vm/main.cpp -o $(BUILD_DIR)/vm
_assm: _dir
	$(CC) ./assm/assm.cpp -o $(BUILD_DIR)/assm

_minimal: _dir
	$(CC) ./minimal/main.cpp -o $(BUILD_DIR)/minimal

_script_test: _dir
	$(ASSM) ./scripts/test.assm $(BUILD_DIR)/test.bin
_script_fibo: _dir
	$(ASSM) ./scripts/fibo.assm $(BUILD_DIR)/fibo.bin
_scripts: _script_test _script_fibo

run: _script_test
	$(BUILD_DIR)/vm $(BUILD_DIR)/test.bin

all: _dir _assm _vm _scripts _minimal
	
clean:
	rm -rf $(BUILD_DIR)/*