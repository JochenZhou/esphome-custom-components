# esphome-custom-components
esphome的一些新增组件支持
## 如何使用
请参考[esphome文档](https://www.esphome.io/components/external_components.html)。
```yaml
#引用组件库示例
external_components:
  - source: github://JochenZhou/esphome-custom-components
    components: [ easear_tds ]
```

## 组件列表

* easear_tds
> 一款基于urat协议的2路带水温tds检测模块。可输出双路tds及双路水温，目前只可用一路。
```yaml
#配置示例
uart:
  id: uart_tds
  baud_rate: 9600
  rx_pin: RX #你的RX引脚
  tx_pin: TX #你的TX引脚
  
sensor:
  - platform: easear_tds
    source_tds:
      name: "Source TDS" #自来水TDS
    temperature:
      name: "Water Temperature" #水温
    update_interval: 10s # 可选，更新间隔默认10s
```
## Reference

- [ryanh7/esphome-custom-components](https://github.com/ryanh7/esphome-custom-components)