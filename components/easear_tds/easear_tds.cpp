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
        float source_tds_val=0;
        float source_temperature_val=0;
        static int last_read_time=0;
        // 如果UART缓冲区中有数据，则取出第一个字节
        while (available() > 0)  
        {
            bytes.push_back(read());
            last_read_time = millis();
            // ESP_LOGI("TAG", "收到数据：%x",bytes.end()[-1]); 
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
                // ESP_LOGI("TAG", "计算校验码：%x",checkcode); 
                // ESP_LOGI("TAG", "接收校验码：%x",bytes[10]);   
                ESP_LOGE("TAG", "校验错误");
                bytes.clear();
                continue;
            }
            //直接得出的是10倍的参数，在esphome中将其转换正常参数的浮点值
            source_tds_val = (bytes[5] + bytes[4] * 256)*0.1*0.47; // TDS=电导率*0.47
            source_temperature_val = (bytes[7] + bytes[6] * 256)*0.1;
            if (source_tds_val>=0 && source_tds_val<=2000 && source_temperature_val>=0 && source_temperature_val<=99) //电导率(0-2000),温度(0-99)
            {
                // 发布shuizhi和wendu的值
                source_tds_sensor_->publish_state(source_tds_val);
                temperature_sensor_->publish_state(source_temperature_val); 
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
