
void vcmailbox_request(const uint8_t* buffer, size_t size) {
    while (VCMAILBOX->FULL) {}
    VCMAILBOX->WRITE = buffer;
    while (VCMAILBOX->EMPTY || VCMAILBOX->READ != buffer) {}
}

bool vcmailbox_get_power_state(vcmailbox_device_t device) {
    size_t size = VCMAILBOX_HEADER_SIZE + VCMAILBOX_GET_POWER_STATE_TAG_SIZE;
    uint8_t* buf = __builtin_alloca_with_align(size, 16 * 8);
    vcmailbox_get_power_state_request_t* request = buf + VCMAILBOX_HEADER_SIZE;
    request->tag = VCMAILBOX_TAG_GET_POWER_STATE;
    request->response = false;
    request->device_id = (uint32_t) device;
    vcmailbox_request(buf, size);
    vcmailbox_get_power_state_response_t* response = buf + VCMAILBOX_HEADER_SIZE;
    return response->state == 1;
}

bool vcmailbox_set_power_state(vcmailbox_device_t device, bool on) {
    size_t size = VCMAILBOX_HEADER_SIZE + VCMAILBOX_SET_POWER_STATE_TAG_SIZE;
    uint8_t* buf = __builtin_alloca_with_align(size, 16 * 8);
    vcmailbox_set_power_state_request_t* request = buf + VCMAILBOX_HEADER_SIZE;
    request->tag = VCMAILBOX_TAG_SET_POWER_STATE;
    request->response = false;
    request->device_id = (uint32_t) device;
    vcmailbox_request(buf, size);
    vcmailbox_set_power_state_response_t* response = buf + VCMAILBOX_HEADER_SIZE;
    return response->state == 1;
}