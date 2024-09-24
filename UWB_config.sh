#!/bin/sh
username=$(whoami)
echo"当前用户：$username"

# step1 : 安装驱动
cd ch343ser_linux/driver/
make
sudo make load
sudo make install

# ( your cp210x driver )
# cd ../../Linux_3.x.x_4.x.x_VCP_Driver_Source
# make 
# cp cp210x.ko to /lib/modules/<kernel-version>/kernel/drivers/usb/serial
# sudo insmod /lib/modules/<kernel-version/kernel/drivers/usb/serial/usbserial.ko
# sudo insmod cp210x.ko

cd ../../serial
make test
sudo make install


# for permission of serials
ls -l /dev/ttyUSB*

id -Gn $username
sudo usermod -a -G dialout $username
source ~/.bashrc

# cat /dev/ttyUSB0

# step2 : 创建工作空间
cd ..
rm -rf Linktrack_UWB_ws
mkdir -p Linktrack_UWB_ws/src
cd  Linktrack_UWB_ws/src
catkin_init_workspace
cd ..
catkin_make
catkin_make install
echo $ROS_PACKAGE_PATH
cd ..

# step3 : 编译nlink_parser
cp -r nlink_parser Linktrack_UWB_ws/src/
cd Linktrack_UWB_ws
rm -rf build/ devel/
catkin_make

read -p "Press any key to continue.."
