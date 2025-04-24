import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID, CONF_TX_PIN

DEPENDENCIES = ["remote_transmitter"]

voltas_ns = cg.esphome_ns.namespace("voltas")
VoltasClimate = voltas_ns.class_("VoltasClimate", climate.Climate, cg.Component)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(VoltasClimate),
        cv.Required(CONF_TX_PIN): cv.templatable(cv.uint16_t),
    }
).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_TX_PIN])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
