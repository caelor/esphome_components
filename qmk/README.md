# Description
ESPHome component to interface with a QMK keyboard using serial. 

The data flow is two-way, with ESPHome being able to present individual QMK keys as Binary Sensors, and LEDs on 
the QMK keyboard being configurable as Lights in Home Assistant

## Usage Example 

    # set up a UART
    uart:
      id: uart_bus
      tx_pin: D2
      rx_pin: D3
      baud_rate: 9600

    # initialise the QMK hub. 
    qmk:

    # map keycodes HA_SW1 and HA_SW5 to binary sensors
    binary_sensor:
      - platform: qmk
        key_id: 1
        name: "QMK Key 1"
      - platform: qmk
        key_id: 5
        name: "QMK Key 5"
    
    # map QMK RGBLED #3 to a light
    light:
      - platform: qmk
        led_id: 5
        name: "QMK LED 3"


## Details

This needs a compatible QMK build - see [here](https://github.com/caelor/qmk_firmware/tree/master/keyboards/40percentclub/gherkin/keymaps/caelor_macro) for a suitable implementation.

## Implementation

This is currently working and in use on a 30 key Gherkin keyboard being used as a macro board. The keyboard uses 
a Pro Micro running QMK, and ESPHome is running on a D1 Mini Lite. The D1 Mini is powered off the Pro Micro 5V
supply from the host PC USB.

The boards are linked using a serial UART. On the Pro Micro end, the hardware serial port is
used (exposed to pins 2 and 3 on the Gherkin PCB expansion header), whereas on the D1 Mini end a software UART is
used on pins D2 and D3. Data flows TX pin -> RX pin, bidirectionally (e.g. QMK Pin 2 to ESP D3 and QMK Pin 3 to ESP D2 in the above example).

Using the reference hardware and the config above:

Gherkin Pin | D1 Mini Pin
------------+------------
2 | D3
3 | D2

## Protocol

The protocol is a simple packetised protocol running at 9600 baud, as follows:

### QMK to ESPHome 
Packet length: 1 byte

Packet | Usage
-------+------
0x00 | Sync request. Sent by QMK if heartbeats have not been received for a period of time
0x01 | Key 1 pressed
0x02 | Key 2 pressed
...  |
0x1f | Key 31 pressed
0x20 | Key 32 pressed
0x21 | Key 1 released
0x22 | Key 2 released
...  | 
0x3f | Key 31 released
0x40 | Key 32 released
0x41 | Reserved
...  |
0xFF | Reserved
 

### ESPHome to QMK
Pcket length: 4 bytes

Packet | Usage
-------+------
0x00 0x00 0x00 0x00 | Sync packet type 1
0x00 0x00 0x00 0x01 | Sync packet type 2
0x00 0x00 0x00 0x02 | Heartbeat. Sent periodically by ESPHome. QMK will request a resync if a heartbeat is not received for some time.
0x01 0xRR 0xGG 0xBB | RGB values for LED #1
... |
0x10 0xRR 0xGG 0xBB | RGB values for LED #10


#### Resync process
When the QMK keyboard requests a resync from ESPHome, the reply is 2 packets - Sync packet type 1 followed by 
Sync packet type 2. When it receives a sync packet type 1 (a stream of 4 consecutive 0x00), QMK reads individual bytes until it receives the 0x01 at the end of Sync packet type 2. The next received byte is considered to be the
start of the next packet and sync is complete.
