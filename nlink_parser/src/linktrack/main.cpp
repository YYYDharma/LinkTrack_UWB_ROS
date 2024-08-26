#include <ros/ros.h>

#include "init.h"
#include "init_serial.h"
#include "protocol_extracter/nprotocol_extracter.h"

#include <iomanip>
#include <iostream>
#include <chrono>

void printHexData(const std::string &data) {
  if (!data.empty()) {
    std::cout << "data received: ";
    for (int i = 0; i < data.size(); ++i) {
      std::cout << std::hex << std::setfill('0') << std::setw(2)
                << int(uint8_t(data.at(i))) << " ";
    }
    std::cout << std::endl;
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "linktrack_parser");
  ros::NodeHandle nh;
  serial::Serial serial;
  initSerial(&serial);
  NProtocolExtracter protocol_extraction;
  linktrack::Init init(&protocol_extraction, &serial);
  ros::Rate loop_rate(1000);
  while (ros::ok()) {
    if (!serial.isOpen()) {
      if (!initSerial(&serial)) {
        ros::Duration(1.0).sleep();
        continue;
      }
    }

    try {
      auto available_bytes = serial.available();
      std::string str_received;
      if (available_bytes) {
        serial.read(str_received, available_bytes);
        // printHexData(str_received);
        // auto now = std::chrono::system_clock::now(); // 获取当前时间点
        // auto duration = now.time_since_epoch(); // 获取时间点与 UNIX 时间原点之间的时间间隔
        // long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(); // 转换为毫秒级的时间戳
        // std::cout << "当前时间戳（毫秒）：" << timestamp << std::endl;
        protocol_extraction.AddNewData(str_received);
      } else {
        if (!serial.isOpen()) {
          ROS_ERROR("serial port unconnected,retry to connect...");
        }
      }
    } catch (serial::IOException& e) {
      ROS_ERROR("read serial port data error, msg : %s", e.what());
      serial.close();
      ros::Duration(1.0).sleep();
    }
    
    ros::spinOnce();
    loop_rate.sleep();
  }
  return EXIT_SUCCESS;
}
