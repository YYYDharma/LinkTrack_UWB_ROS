# LinkTrack_UWB_ROS
ROS读取LinkTrack UWB数据

参考：
https://www.yuque.com/nooploop/spseqo/hp6ib2?#NGnRE
https://github.com/nooploop-dev/nlink_parser/tree/master

修改：
1.增加UTC时间戳，基于tagframe0协议，可以获取到数据对应的utc时间戳
2.增加热插拔，监听串口

运行方法：

git clone <URL> --recursive

1.执行UWB_config.sh 完成配置

2.执行UWB_read_data.sh 读取数据
