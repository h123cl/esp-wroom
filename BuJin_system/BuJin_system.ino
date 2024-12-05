#include <SPI.h>
#include <AccelStepper.h>
// 定义步进电机控制引脚
#define stepPin 13
#define dirPin 12

// 创建 AccelStepper 对象，DRIVER 模式
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup() {
  // 设置最大速度和加速度
  stepper.setMaxSpeed(1000); // 最大速度为 1000 步/秒
  stepper.setAcceleration(1000); // 加速度为 500 步/秒²

  // 初始位置
  stepper.moveTo(50000); // 目标位置为 1000 步
}

void loop() {
  // 运行电机6
  if (stepper.distanceToGo() == 0) {
    // 如果到达目标位置，反向移动
    stepper.moveTo(-stepper.currentPosition());
  }
  stepper.run(); // 必须调用此方法来实现非阻塞运动
}
