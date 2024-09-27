#!/bin/sh
username=$(whoami)
echo"当前用户：$username"
# step4 : 运行
gnome-terminal -t "roscore" -- bash -c "roscore; exec bash"
sleep 1

cd Linktrack_UWB_ws
source devel/setup.sh
# catkin_make run_tests

mkdir rosbag

timestamp=$(date +"%Y_%m%d_%H%M%S")

gnome-terminal --window \
--tab --title="nlink_parser" -e "bash -c 'source devel/setup.sh; sleep 1; rosrun nlink_parser linktrack; exec bash'" \
--tab --title="uwb_rosbag" -e "bash -c 'source devel/setup.sh; sleep 2; rosbag record -O rosbag/uwb_${timestamp}.bag /nlink_linktrack_tagframe0; exec bash'" \
--tab --title="uwb_rostopic echo" -e "bash -c 'source devel/setup.sh; sleep 3; rostopic echo /nlink_linktrack_tagframe0; exec bash'"

read -p "Press any key to continue.."
