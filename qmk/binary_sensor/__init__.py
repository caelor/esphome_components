import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID
from .. import qmk_ns, CONF_QMK_ID, QMKComponent

CONF_KEY_ID = 'key_id'

DEPENDENCIES = ['qmk']

QMKKey = qmk_ns.class_('QMKKey', binary_sensor.BinarySensor)

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(QMKKey),
    cv.GenerateID(CONF_QMK_ID): cv.use_id(QMKComponent),

    cv.Required(CONF_KEY_ID): cv.uint8_t,
})


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield binary_sensor.register_binary_sensor(var, config)

    cg.add(var.set_key_id(config[CONF_KEY_ID]))

    hub = yield cg.get_variable(config[CONF_QMK_ID])
    cg.add(hub.register_receiver(var))