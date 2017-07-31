# !/bin/bash
###########################################
# File  : pinger.sh
# Date  : 2017-07-23
# Author: Zhang Tianjiu
# Email : zhangtianjiu@vip.qq.com
###########################################

bin_name="pinger"
cmd="./bin/${bin_name} -flagfile=./conf/${bin_name}.conf"

process_alive() {
    process_str=`ps -ef | grep -v "grep" | grep "${cmd}"`
    if [[ -z "${process_str}" ]]; then
        echo "no"
    else
        echo "yes"
    fi
}

process_id() {
    echo `ps -ef | grep "${cmd}" | grep -v "grep" | awk '{print $2}'`
}

start() {
    alive=$(process_alive)
    if [[ ${alive} == "yes" ]]; then
        echo "Process ${bin_name} is running..."
    else
        eval ${cmd}
        echo "Start ${bin_name}..."
    fi
}

stop() {
    pid=$(process_id)
    if [[ -z ${pid} ]]; then
        echo "Process ${bin_name} is not running..."
    else
        kill ${pid}
        echo "Stop process ${bin_name}..."
    fi
}

restart() {
    stop
    start
}

status() {
    alive=$(process_alive)
    if [[ ${alive} -eq 0 ]]; then
        echo "Process ${bin_name} is running..."
    else
        echo "Process ${bin_name} is stopped..."
    fi
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        stop
        start
        ;;
    status)
        # code to check status of app comes here 
        # example: status program_name
        ;;
    *)
        echo "Usage: $0 {start|stop|status|restart}"
esac
