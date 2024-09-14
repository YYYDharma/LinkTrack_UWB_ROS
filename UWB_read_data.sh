#!/bin/sh
username=$(whoami)
echo"当前用户：$username"
# step4 : 运行
gnome-terminal -t "roscore" -- bash -c "roscore; exec bash"
sleep 1

cd Linktrack_UWB_ws
source devel/setup.sh
catkin_make run_tests

gnome-terminal --window \
--tab -e 'bash -c "source devel/setup.sh; sleep 1; rosrun nlink_parser linktrack; exec bash"' \
--tab -e 'bash -c "source devel/setup.sh; sleep 3; rostopic echo /nlink_linktrack_tagframe0; exec bash"' \

read -p "Press any key to continue.."
