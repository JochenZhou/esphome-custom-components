# esphome-custom-components
esphome的一些新增组件支持
## 如何使用
请参考[esphome文档](https://www.esphome.io/components/external_components.html)。
```yaml
#引用组件库示例
external_components:
  - source: github://JochenZhou/esphome-custom-components
    components: [ easear_tds ]

#国内git引用组件库示例
external_components:
  - source:
      type: git
      url: https://gitee.com/pickcode/esphome-custom-components
      ref: main    
    components: [ easear_tds ]
    refresh : 0s
```

## 组件列表

* easear_tds
> 一款基于urat协议的2路带水温tds检测模块。可输出双路tds及双路水温，目前只配置了一路。
```yaml
#配置示例
uart:
  id: uart_tds
  baud_rate: 9600
  rx_pin: RX #你的RX引脚
  tx_pin: TX #你的TX引脚
  
sensor:
  - platform: easear_tds
    source_tds_1:
      name: "Source TDS" #自来水TDS
      accuracy_decimals: 1   
    temperature_1:
      name: "Source Temp" #水温
      accuracy_decimals: 1
    source_tds_2:
      name: "Pour TDS" #纯净水TDS
      accuracy_decimals: 1 
    temperature_2:
      name: "Pour Temp" #水温
      accuracy_decimals: 1
    update_interval: 5s # 可选，更新间隔默认5s 
```
## Reference

- [ryanh7/esphome-custom-components](https://github.com/ryanh7/esphome-custom-components)
