#include "hcpbridge.h"

namespace esphome {
namespace hcpbridge {

static const char *TAG = "hcpbridge";
void HCPBridge::setup() {
  int8_t rx = this->rx_pin_ == nullptr ? PIN_RXD : this->rx_pin_->get_pin();
  int8_t tx = this->tx_pin_ == nullptr ? PIN_TXD : this->tx_pin_->get_pin();
  int8_t rts = this->rts_pin_ == nullptr ? -1 : this->rts_pin_->get_pin();

  this->engine = &HoermannGarageEngine::getInstance();
  this->engine->setup(rx, tx, rts);
}
void HCPBridge::add_on_state_callback(std::function<void()> &&callback, const char *tag) {
  auto wrapped_callback = [callback, tag]() {
    auto start = millis();
    callback();
    auto end = millis();
    ESP_LOGD(TAG, "Callback executed in %u ms [Tag: %s]", end - start, tag);
  };
  this->state_callback_.add(std::move(wrapped_callback));
}

void HCPBridge::on_safe_shutdown() {
  ESP_LOGI(TAG, "Safe shutdown: waiting for next Modbus response before stopping");
  unsigned long lastResponse = this->engine->state->lastModbusRespone;
  unsigned long start = millis();

  // Wait up to 5 seconds for the master to poll us one more time,
  // giving us maximum time before the next poll to reboot cleanly.
  // The Modbus task continues running on core 1 while we wait here on core 0.
  while ((millis() - start) < 5000) {
    if (this->engine->state->lastModbusRespone > lastResponse) {
      ESP_LOGI(TAG, "Master polled us, shutting down Modbus now");
      break;
    }
    delay(1);
  }

  this->engine->shutdownModbus();
}

void HCPBridge::update() {
  static bool logged_valid = false;
  static bool logged_ready = false;

  if (!logged_valid && this->engine->state->valid) {
    ESP_LOGI(TAG, "Bus connected (motor is polling)");
    logged_valid = true;
  }
  if (!logged_ready && this->engine->state->ready) {
    ESP_LOGI(TAG, "Bus ready (full command polls active, commands accepted)");
    logged_ready = true;
  }

  if (this->engine->state->changed) {
    this->engine->state->clearChanged();
    this->state_callback_.call();
  }
}
}  // namespace hcpbridge
}  // namespace esphome