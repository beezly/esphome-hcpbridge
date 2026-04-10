from esphome.components import binary_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from .. import hcpbridge_ns, CONF_HCPBridge_ID, HCPBridge
from esphome.const import (
    DEVICE_CLASS_CONNECTIVITY,
)

DEPENDENCIES = ["hcpbridge"]

HCPBridgeIsConnected = hcpbridge_ns.class_("HCPBridgeIsConnected", binary_sensor.BinarySensor, cg.Component)
HCPBridgeRelaySensor = hcpbridge_ns.class_("HCPBridgeRelaySensor", binary_sensor.BinarySensor, cg.Component)
HCPBridgeIsReady = hcpbridge_ns.class_("HCPBridgeIsReady", binary_sensor.BinarySensor, cg.Component)

CONF_IS_CONNECTED = "is_connected"
CONF_RELAY_STATE = "relay_state"
CONF_IS_READY = "is_ready"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_HCPBridge_ID): cv.use_id(HCPBridge),
        cv.Optional(CONF_IS_CONNECTED): binary_sensor.binary_sensor_schema(
            HCPBridgeIsConnected
        ).extend({
            cv.Optional("device_class", default=DEVICE_CLASS_CONNECTIVITY): cv.string,
        }),
        cv.Optional(CONF_RELAY_STATE): binary_sensor.binary_sensor_schema(
            HCPBridgeRelaySensor
        ),
        cv.Optional(CONF_IS_READY): binary_sensor.binary_sensor_schema(
            HCPBridgeIsReady
        ).extend({
            cv.Optional("device_class", default=DEVICE_CLASS_CONNECTIVITY): cv.string,
        }),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_HCPBridge_ID])
    if conf := config.get(CONF_IS_CONNECTED):
        con_sens = await binary_sensor.new_binary_sensor(config[CONF_IS_CONNECTED])
        await cg.register_component(con_sens, config[CONF_IS_CONNECTED])
        cg.add(con_sens.set_hcpbridge_parent(parent))
    if conf := config.get(CONF_RELAY_STATE):
        relay_sens = await binary_sensor.new_binary_sensor(config[CONF_RELAY_STATE])
        await cg.register_component(relay_sens, config[CONF_RELAY_STATE])
        cg.add(relay_sens.set_hcpbridge_parent(parent))
    if conf := config.get(CONF_IS_READY):
        ready_sens = await binary_sensor.new_binary_sensor(config[CONF_IS_READY])
        await cg.register_component(ready_sens, config[CONF_IS_READY])
        cg.add(ready_sens.set_hcpbridge_parent(parent))