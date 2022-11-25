default_target: release

.PHONY : default_target


build_debug:
	@mkdir -p build_debug
	@cd build_debug && \
	cmake -DCMAKE_BUILD_TYPE=Debug .. -B .

build_release:
	@mkdir -p build_release
	@cd build_release && \
	cmake -DCMAKE_BUILD_TYPE=Release .. -B .


release:
	@make build_release
	@cd build_release && \
	make all


debug:
	@make build_debug
	@cd build_debug && \
	make all

.PHONY: dist-clean
dist-clean:
	@rm -rf build_*
	@rm -rf bin.*

all: release debug
