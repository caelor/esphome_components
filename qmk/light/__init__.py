import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_ID, CONF_OUTPUT_ID
from .. import qmk_ns, CONF_QMK_ID, QMKComponent

CONF_LED_ID = 'led_id'

DEPENDENCIES = ['qmk']

QMKLED = qmk_ns.class_('QMKLED', light.LightOutput)

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(QMKLED),
    cv.GenerateID(CONF_QMK_ID): cv.use_id(QMKComponent),

    cv.Required(CONF_LED_ID): cv.uint8_t,
})


def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    yield light.register_light(var, config)

    cg.add(var.set_led_id(config[CONF_LED_ID]))

    hub = yield cg.get_variable(config[CONF_QMK_ID])
    cg.add(var.set_parent_hub(hub))