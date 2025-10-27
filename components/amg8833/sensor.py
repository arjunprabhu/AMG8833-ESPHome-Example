import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
)
from . import AMG8833Component, amg8833_ns

CONF_AMG8833_ID = 'amg8833_id'
CONF_MIN_TEMPERATURE = 'min_temperature'
CONF_MAX_TEMPERATURE = 'max_temperature'
CONF_AVERAGE_TEMPERATURE = 'average_temperature'
CONF_THERMISTOR = 'thermistor'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_AMG8833_ID): cv.use_id(AMG8833Component),
    cv.Optional(CONF_MIN_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_MAX_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_AVERAGE_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_THERMISTOR): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_AMG8833_ID])
    
    if CONF_MIN_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_MIN_TEMPERATURE])
        cg.add(parent.set_min_temperature_sensor(sens))
    
    if CONF_MAX_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_MAX_TEMPERATURE])
        cg.add(parent.set_max_temperature_sensor(sens))
    
    if CONF_AVERAGE_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_AVERAGE_TEMPERATURE])
        cg.add(parent.set_average_temperature_sensor(sens))
    
    if CONF_THERMISTOR in config:
        sens = await sensor.new_sensor(config[CONF_THERMISTOR])
        cg.add(parent.set_thermistor_sensor(sens))