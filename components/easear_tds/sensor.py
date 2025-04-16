import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import (
    CONF_ID, CONF_TEMPERATURE,
    UNIT_PARTS_PER_MILLION, ICON_WATER_PERCENT,
    ICON_THERMOMETER, DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT, UNIT_CELSIUS
)

CONF_SOURCE_TDS_1 = "source_tds_1"
CONF_SOURCE_TDS_2 = "source_tds_2"
CONF_TEMPERATURE_1 = "temperature_1"
CONF_TEMPERATURE_2 = "temperature_2"

easear_tds_ns = cg.esphome_ns.namespace("easear_tds")
EASEARTDSComponent = easear_tds_ns.class_(
    "EASEARTDSComponent", cg.PollingComponent, uart.UARTDevice
)

def validate_config(config):
    if CONF_SOURCE_TDS_1 not in config and CONF_SOURCE_TDS_2 not in config:
        raise cv.Invalid("At least one TDS sensor must be configured")
    return config

CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(EASEARTDSComponent),
        # Channel 1
        cv.Optional(CONF_SOURCE_TDS_1): sensor.sensor_schema(
            unit_of_measurement=UNIT_PARTS_PER_MILLION,
            icon=ICON_WATER_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT
        ),
        cv.Optional(CONF_TEMPERATURE_1): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT
        ),
        # Channel 2
        cv.Optional(CONF_SOURCE_TDS_2): sensor.sensor_schema(
            unit_of_measurement=UNIT_PARTS_PER_MILLION,
            icon=ICON_WATER_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT
        ),
        cv.Optional(CONF_TEMPERATURE_2): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT
        ),
    })
    .extend(cv.polling_component_schema("10s"))
    .extend(uart.UART_DEVICE_SCHEMA),
    validate_config
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    # Channel 1
    if CONF_SOURCE_TDS_1 in config:
        sens = await sensor.new_sensor(config[CONF_SOURCE_TDS_1])
        cg.add(var.set_source_tds_sensor_1(sens))
    if CONF_TEMPERATURE_1 in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE_1])
        cg.add(var.set_temperature_sensor_1(sens))
    
    # Channel 2
    if CONF_SOURCE_TDS_2 in config:
        sens = await sensor.new_sensor(config[CONF_SOURCE_TDS_2])
        cg.add(var.set_source_tds_sensor_2(sens))
    if CONF_TEMPERATURE_2 in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE_2])
        cg.add(var.set_temperature_sensor_2(sens))