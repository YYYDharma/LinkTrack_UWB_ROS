#include "init_serial.h"
#include <ros/ros.h>
#include <string>
#include <vector>
#include <memory>
/*
void enumerate_ports() {
  auto devices_found = serial::list_ports();
  auto iter = devices_found.begin();
  while (iter != devices_found.end()) {
    serial::PortInfo device = *iter++;

    printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
           device.hardware_id.c_str());
  }
  std::string test;
  test.clear();
}
*/
bool isLinkTrackUWBDevice(const serial::PortInfo &device) {
  // 基于设备的hardware_id是否为Lintrack UWB设备
  return device.hardware_id.find("VID:PID=10c4:ea60") != std::string::npos;
}

std::vector<std::shared_ptr<serial::Serial>> initSerial() {
  std::vector<std::shared_ptr<serial::Serial>> uwb_serials;

  // 持续扫描直到找到设备
  auto devices_found = serial::list_ports();
  bool uwb_found = false;

  for (const auto &device : devices_found) {
    if (device.port.find("/dev/ttyUSB") != std::string::npos) {
      ROS_INFO("Checking port: %s", device.port.c_str());

      if (isLinkTrackUWBDevice(device)) {
        auto serial_ptr = std::make_shared<serial::Serial>();
        serial_ptr->setPort(device.port);
        serial_ptr->setBaudrate(921600); // refer to all device config
        auto timeout = serial::Timeout::simpleTimeout(10);
        serial_ptr->setTimeout(timeout);
        
        try {
          serial_ptr->open();
        } catch (serial::IOException& e) {
          ROS_ERROR("Failed to open port: %s", e.what());
          continue;
        }

        if (serial_ptr->isOpen()) {
          ROS_INFO("UWB device found and opened at port: %s", device.port.c_str());
          uwb_serials.push_back(serial_ptr);  // 将设备添加到列表
          uwb_found = true;
        }
      }
    }
  }

  return uwb_serials;  // 返回已找到的UWB设备列表
}
