# config for the qmk: top level component
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CONF_QMK_ID = 'qmk_id'

DEPENDENCIES = ['uart']

qmk_ns = cg.esphome_ns.namespace('qmk')
QMKComponent = qmk_ns.class_('QMKHub', cg.PollingComponent, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
  cv.GenerateID(): cv.declare_id(QMKComponent),
}).extend(cv.polling_component_schema('3s')).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

QMK_DEVICe_SCHEMA = cv.Schema({
  cv.GenerateID(CONF_QMK_ID): cv.use_id(QMKComponent)
})