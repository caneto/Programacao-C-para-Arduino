from machine import Pin
from time import sleep

# ESP8266 GPIO 5
relay = Pin(5, Pin.OUT)

while True:
  # RELAY ON
  relay.value(0)
  sleep(10)
  # RELAY OFF
  relay.value(1)
  sleep(10)