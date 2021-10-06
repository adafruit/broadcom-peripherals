
#include <stdint.h>
#include <string.h>

#include "broadcom/gen/bcm2711_lpa.h"
#include "broadcom/gen/vcmailbox.h"


void vcmailbox_request(vcmailbox_buffer_t* buffer) {
    buffer->code = VCMAILBOX_CODE_PROCESS_REQUEST;
    while (VCMAILBOX->STATUS0_b.FULL) {}
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    uint32_t buffer_address = (uint32_t) buffer | 8;
    #pragma GCC diagnostic pop
    VCMAILBOX->WRITE = buffer_address;
    while (VCMAILBOX->STATUS0_b.EMPTY || VCMAILBOX->READ != buffer_address) {}
    while (buffer->code == VCMAILBOX_CODE_PROCESSING_ERROR) {
        __asm__("nop");
    }
}

uint32_t compute_size(uint32_t tag_size) {
    uint32_t size = VCMAILBOX_HEADER_SIZE + tag_size + sizeof(uint32_t);
    if (size % 16 != 0) {
        size += 16 - (size % 16);
    }
    return size;
}

bool vcmailbox_get_power_state(vcmailbox_device_t device) {
    int size = compute_size(sizeof(vcmailbox_get_power_state_t));
    vcmailbox_buffer_t* buf = (vcmailbox_buffer_t*) __builtin_alloca_with_align(size, 16 * 8);
    memset(buf, size, 0);
    buf->buffer_size = size;
    vcmailbox_get_power_state_t* tag = (vcmailbox_get_power_state_t*) &buf->data;
    tag->tag = VCMAILBOX_TAG_GET_POWER_STATE;
    tag->is_response = false;
    tag->value_size = sizeof(vcmailbox_get_power_state_request_t);
    tag->request.device_id = (uint32_t) device;
    vcmailbox_request(buf);
    return tag->response.state == 1;
}

bool vcmailbox_set_power_state(vcmailbox_device_t device, bool on) {
    int size = compute_size(sizeof(vcmailbox_set_power_state_t));
    vcmailbox_buffer_t* buf = (vcmailbox_buffer_t*) __builtin_alloca_with_align(size, 16 * 8);
    memset(buf, size, 0);
    buf->buffer_size = size;
    vcmailbox_set_power_state_t* tag = (vcmailbox_set_power_state_t*) &buf->data;
    tag->tag = VCMAILBOX_TAG_SET_POWER_STATE;
    tag->is_response = false;
    tag->request.device_id = (uint32_t) device;
    tag->value_size = sizeof(vcmailbox_set_power_state_request_t);
    // Always wait for power stability.
    tag->request.state = on ? 0x3 : 0x2;
    vcmailbox_request(buf);
    return tag->response.state == 1;
}