#include "hcpbridge_binarySensor.h"

namespace esphome{
namespace hcpbridge{

static const char *const TAG = "hcpbridge.binary_sensor";
static const char *const TAG2 = "hcpbridge.binary_sensor2";

void HCPBridgeRelaySensor::setup() {
    this->parent_->add_on_state_callback([this]() { this->on_event_triggered(); }, TAG);
    this->publish_state(false);
}

void HCPBridgeRelaySensor::on_event_triggered() {
    if (this->parent_->engine->state->relayOn != this->state){
        this->publish_state(this->parent_->engine->state->relayOn);
    }
}
void HCPBridgeRelaySensor::dump_config(){
    ESP_LOGCONFIG(TAG, "HCPBridgeRelaySensor");
}

void HCPBridgeIsConnected::setup() {
    this->parent_->add_on_state_callback([this]() { this->on_event_triggered(); }, TAG2);
    this->publish_state(false);
}

void HCPBridgeIsConnected::on_event_triggered() {
    if (this->parent_->engine->state->valid != this->state){
        this->publish_state(this->parent_->engine->state->valid);
    }
}
void HCPBridgeIsConnected::dump_config(){
    ESP_LOGCONFIG(TAG2, "HCPBridgeIsConnected");
}

static const char *const TAG3 = "hcpbridge.binary_sensor3";

void HCPBridgeIsReady::setup() {
    this->parent_->add_on_state_callback([this]() { this->on_event_triggered(); }, TAG3);
    this->publish_state(false);
}

void HCPBridgeIsReady::on_event_triggered() {
    if (this->parent_->engine->state->ready != this->state){
        this->publish_state(this->parent_->engine->state->ready);
    }
}
void HCPBridgeIsReady::dump_config(){
    ESP_LOGCONFIG(TAG3, "HCPBridgeIsReady");
}

}
}