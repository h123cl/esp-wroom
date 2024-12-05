#include <SPI.h>
#include <TFT_eSPI.h>
#include "Hcl_20.h"
#include <WiFi.h>
#include <TJpg_Decoder.h>
#include <SD.h>

const char * ssid="HCL8086";
const char * password="19916378086";

#define PSX  35
#define PSZ  32

TFT_eSPI tft = TFT_eSPI();

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  tft.pushImage(x, y, w, h, bitmap); // 将像素数据绘制到屏幕
  return true; // 返回 true 表示绘制成功
}


int AnNiu_val = 1;    // 当前选项值
int last_AnNiu_val = 1; // 上次选项值

void setup() {
  
  Serial.begin(115200);
  pinMode(PSX,INPUT);
  pinMode(PSZ, INPUT_PULLUP); 
  tft_init();         // 初始化屏幕
  draw_menu();        // 绘制菜单
  update_selection(AnNiu_val);  // 绘制初始选中标志

  if (!SD.begin(21)) { // CS 引脚根据实际硬件设置
    Serial.println("SD 卡初始化失败!");
    //return;
  }
  Serial.println("SD 卡初始化成功!");
  //显示espflash空间
  if(1){
    // Serial.println("=== Flash 存储信息 ===");
    // Serial.print("Flash 芯片大小: ");
    // Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
    // Serial.println(" MB");

    // Serial.print("当前程序占用大小: ");
    // Serial.print(ESP.getSketchSize() / 1024);
    // Serial.println(" KB");

    // Serial.print("可用程序空间: ");
    // Serial.print(ESP.getFreeSketchSpace() / 1024);
    // Serial.println(" KB");

    // Serial.print("Flash 芯片实际大小: ");
    // Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
    // Serial.println(" MB");
   }

  
  // 初始化 TJpg_Decoder
  TJpgDec.setCallback(tft_output); // 设置回调函数
  TJpgDec.setJpgScale(1);          // 设置缩放比例（1 表示原始大小）

}

void loop() {
 // Serial.printf("x:%dz:%d\n",analogRead(PSX),digitalRead(PSZ));
  if (analogRead(PSX) == 0) {  // 检测“下键”按下
    delay(200);  // 按键防抖
    AnNiu_val++;
    if (AnNiu_val > 3) AnNiu_val = 1;  // 循环切换

  }

  else if (analogRead(PSX) == 4095) {  // 检测“上键”按下
    delay(200);  // 按键防抖
    AnNiu_val--;
    if (AnNiu_val < 1) AnNiu_val = 3;  // 循环切换
  }
  if (digitalRead(PSZ)==0) {  // 检测“确定”按下
    delay(200);  // 按键防抖
    Serial.printf("YES");
    
      if(AnNiu_val==1){
        tft.fillScreen(TFT_BLACK);
    TJpgDec.drawSdJpg(0, 0, "/JGSystem你/outputHB.jpg");
    WiFi.softAP(ssid,password);
          while (1) {
        
      }
      }
    
   


  }


  if (AnNiu_val != last_AnNiu_val) {
    update_selection(AnNiu_val);
    last_AnNiu_val = AnNiu_val;  // 更新上次选项值
  }
  //tft.unloadFont();释放字体库
}

// 屏幕初始化函数
void tft_init() {
  tft.init();               // 初始化TFT屏幕
  tft.setRotation(2);        // 设置屏幕方向
  tft.fillScreen(TFT_BLACK); // 清屏并设置背景颜色为黑色
  tft.loadFont(Hcl_20);      // 加载自定义字体
}

// 绘制菜单文字
void draw_menu() {
  tft.setTextColor(TFT_YELLOW);  // 设置文字颜色为黄色
  tft.setCursor(8, 0);           // 第一行菜单
  tft.println("连接WiFi");
  tft.setCursor(8, 21);          // 第二行菜单
  tft.println("切换模式");
  tft.setCursor(8, 42);          // 第三行菜单
  tft.println("文件");
}

// 更新选中标志
void update_selection(int AnNiu_val) {
  // 清除之前的选中标志
  tft.setTextColor(TFT_BLACK);  // 用背景色覆盖旧的标志
  if (last_AnNiu_val == 1) {
    tft.setCursor(0, 0);
    tft.drawGlyph(0x00B7);
    tft.drawGlyph(0x00B7);
  } else if (last_AnNiu_val == 2) {
    tft.setCursor(0, 21);
    tft.drawGlyph(0x00B7);
    tft.drawGlyph(0x00B7);
  } else if (last_AnNiu_val == 3) {
    tft.setCursor(0, 42);
    tft.drawGlyph(0x00B7);
    tft.drawGlyph(0x00B7);
  }
  // 绘制新的选中标志
  tft.setTextColor(TFT_YELLOW);  // 设置标志颜色为黄色
  if (AnNiu_val == 1) {
    tft.setCursor(0, 0);
    tft.drawGlyph(0x00B7);
    tft.drawGlyph(0x00B7);
  } else if (AnNiu_val == 2) {
    tft.setCursor(0, 21);
    tft.drawGlyph(0x00B7);
    tft.drawGlyph(0x00B7);
  } else if (AnNiu_val == 3) {
    tft.setCursor(0, 42);
    tft.drawGlyph(0x00B7);
    tft.drawGlyph(0x00B7);
  }
}
