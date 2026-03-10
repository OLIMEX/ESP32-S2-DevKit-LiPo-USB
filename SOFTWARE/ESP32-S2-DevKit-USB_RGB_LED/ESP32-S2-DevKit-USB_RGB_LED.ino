#include <Arduino.h>
#include "driver/rmt_tx.h"
#include "driver/gpio.h"

#define LED_PIN 18
#define BUTTON_PIN 0
#define RMT_RESOLUTION_HZ 10000000

rmt_channel_handle_t tx_chan = NULL;
rmt_encoder_handle_t encoder = NULL;

bool lastButton = HIGH;
uint8_t mode = 0;

uint32_t lastAnim = 0;
uint8_t rainbow = 0;

const char* modeDescription[] = {
  "Mode 0: LED OFF",
  "Mode 1: Red LED always ON",
  "Mode 2: Green LED always ON",
  "Mode 3: Blue LED always ON",
  "Mode 4: Rainbow animation"
};

void ws2812_init()
{
  rmt_tx_channel_config_t tx_config = {
      .gpio_num = (gpio_num_t)LED_PIN,
      .clk_src = RMT_CLK_SRC_DEFAULT,
      .resolution_hz = RMT_RESOLUTION_HZ,
      .mem_block_symbols = 64,
      .trans_queue_depth = 4
  };

  rmt_new_tx_channel(&tx_config, &tx_chan);

  rmt_bytes_encoder_config_t bytes_config;

  bytes_config.bit0.duration0 = 4;
  bytes_config.bit0.level0 = 1;
  bytes_config.bit0.duration1 = 8;
  bytes_config.bit0.level1 = 0;

  bytes_config.bit1.duration0 = 8;
  bytes_config.bit1.level0 = 1;
  bytes_config.bit1.duration1 = 4;
  bytes_config.bit1.level1 = 0;

  bytes_config.flags.msb_first = 1;

  rmt_new_bytes_encoder(&bytes_config, &encoder);

  rmt_enable(tx_chan);
}

void ws2812_write(uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t data[3] = {g,r,b};

  rmt_transmit_config_t tx_config = {0};

  rmt_transmit(tx_chan, encoder, data, sizeof(data), &tx_config);
  rmt_tx_wait_all_done(tx_chan, portMAX_DELAY);
}

void rainbowStep()
{
  uint8_t r,g,b;
  uint8_t pos = rainbow;

  if(pos < 85) {
    r = pos * 3;
    g = 255 - pos * 3;
    b = 0;
  }
  else if(pos < 170) {
    pos -= 85;
    r = 255 - pos * 3;
    g = 0;
    b = pos * 3;
  }
  else {
    pos -= 170;
    r = 0;
    g = pos * 3;
    b = 255 - pos * 3;
  }

  ws2812_write(r,g,b);
  rainbow++;
}

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  ws2812_init();

  Serial.println();
  Serial.println("ESP32-S2 RGB LED Demo");
  Serial.println("Press USER button to change LED mode");
  Serial.println(modeDescription[mode]);
}

void loop()
{
  bool button = digitalRead(BUTTON_PIN);

  if(lastButton == HIGH && button == LOW)
  {
    mode++;
    if(mode > 4) mode = 0;

    Serial.println();
    Serial.print("Button pressed -> ");
    Serial.println(modeDescription[mode]);
  }

  lastButton = button;

  if(mode == 0) ws2812_write(0,0,0);
  if(mode == 1) ws2812_write(40,0,0);
  if(mode == 2) ws2812_write(0,40,0);
  if(mode == 3) ws2812_write(0,0,40);

  if(mode == 4)
  {
    if(millis() - lastAnim > 30)
    {
      rainbowStep();
      lastAnim = millis();
    }
  }
}
