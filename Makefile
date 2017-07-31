.PHONY: pinger
all: pinger

#BUILD_OPT="--package_path %workspace%:/home/jiu/Projects/cpp"

BIN_DIR="release/bin"
CONF_DIR="release/conf"
LOG_DIR="release/log"


pinger:
	rm -rf release
	mkdir -p ${BIN_DIR}
	mkdir -p ${CONF_DIR}
	mkdir -p ${LOG_DIR}
	bazel build ${BUILD_OPT} pinger:pinger
	cp bazel-bin/pinger/pinger ${BIN_DIR}
	cp pinger/scripts/pinger.sh ${BIN_DIR}
	cp -rf pinger/conf/* ${CONF_DIR}

clean:
	bazel clean
