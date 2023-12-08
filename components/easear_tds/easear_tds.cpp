#include "easear_tds.h"
#include "esphome/core/log.h"
#include <cstring>

namespace esphome {
namespace easear_tds {

static const char *const TAG = "easear_tds";

void EASEARTDSComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "easear_tds:");
  this->check_uart_settings(9600);
  LOG_SENSOR("  ", "Source TDS", this->source_tds_sensor_);
  LOG_SENSOR("  ", "Temperature TDS", this->temperature_sensor_);
}

void EASEARTDSComponent::update() {
  this->write_array(URAT_QUERY_COMMAND, sizeof(URAT_QUERY_COMMAND));
  this->flush();
}

void EASEARTDSComponent::loop() {
        uint8_t checkcode=0;
        int shuizhi_val=0;
        int wendu_val=0;
        static int last_read_time=0;
        // 如果UART缓冲区中有数据，则取出第一个字节
        while (available() > 0)  
        {
            bytes.push_back(read());
            last_read_time = millis();
            // ESP_LOGI("custom", "收到数据：%x",bytes.end()[-1]); 
            if (bytes[0]!=0x55)//接收到0x55才开始缓存
            {
                bytes.clear();
                continue;
            }            
            // 如果UART缓冲区中的数据小于11个字节，则继续循环
            if (bytes.size() < 11)    
            {
                continue;
            }
                         
            // 遍历bytes数组，将每个元素加到校验码变量中
            for (int i = 0; i < 10; i++)
            {
                checkcode+=bytes[i];
            }
            // 如果校验码变量不等于bytes数组的第11个元素，则清空bytes数组，并继续循环
            if (checkcode != bytes[10])
            {          
                // ESP_LOGI("custom", "计算校验码：%x",checkcode); 
                // ESP_LOGI("custom", "接收校验码：%x",bytes[10]);   
                // ESP_LOGI("custom", "校验错误");
                bytes.clear();
                continue;
            }
            //直接得出的是10倍的参数，在esphome中将其转换正常参数的浮点值
            shuizhi_val = bytes[5] + bytes[4] * 256;
            wendu_val = bytes[7] + bytes[6] * 256;
            // ESP_LOGI("custom", "水质原始值=%d,温度原始值=%d",shuizhi_val,wendu_val);
            if (shuizhi_val>=0 && shuizhi_val<=20000 && wendu_val>=0 && wendu_val<=999) //电导率(0-2000),温度(0-99) 判断范围需要扩大10倍
            {
                // 发布shuizhi和wendu的值
                shuizhi->publish_state(source_tds);
                wendu->publish_state(temperature); 
            }
            // 清空bytes数组
            bytes.clear();
        }
        //如果bytes的长度大于0并且超过1秒没有数据传输，则清空bytes数组
        if (bytes.size() > 0 && (millis() - last_read_time > 1000))
        {
            bytes.clear();
        }
}


}  // namespace easear_tds
}  // namespace esphome
