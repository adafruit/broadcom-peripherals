#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "broadcom/defines.h"

// This catches non-interrupt exceptions that are similar to Cortex-M hard faults.
__attribute__((weak)) void HardFault_IRQHandler(void) {
    while (true) {}
}

// TODO: Route I2C, SPI and UART based on AUX_IRQ and PACTL_CS

__attribute__((weak)) void UART1_IRQHandler(void) {
    while(true) {}
}
__attribute__((weak)) void SPI1_IRQHandler(void) {
    while(true) {}
}
__attribute__((weak)) void SPI2_IRQHandler(void) {
    while(true) {}
}

// 64: ARMC Timer
__attribute__((weak)) void TIMER_IRQHandler(void) {
    while(true) {}
}

// 65: Mailbox
__attribute__((weak)) void MAILBOX_IRQHandler(void) {
    while(true) {}
}

// 66: Doorbell 0
__attribute__((weak)) void DOORBELL0_IRQHandler(void) {
    while(true) {}
}

// 67: Doorbell 1
__attribute__((weak)) void DOORBELL1_IRQHandler(void) {
    while(true) {}
}

// 68: VPU0 halted
__attribute__((weak)) void VPU0_HALTED_IRQHandler(void) {
    while(true) {}
}

// 69: VPU1 halted
__attribute__((weak)) void VPU1_HALTED_IRQHandler(void) {
    while(true) {}
}

// 70: ARM address error
__attribute__((weak)) void ARM_ADDRESS_ERROR_IRQHandler(void) {
    while(true) {}
}

// 71: ARM AXI error
__attribute__((weak)) void ARM_AXI_ERROR_IRQHandler(void) {
    while(true) {}
}

// 72: Software interrupt 0
__attribute__((weak)) void SWI0_IRQHandler(void) {
    while(true) {}
}

// 73: Software interrupt 1
__attribute__((weak)) void SWI1_IRQHandler(void) {
    while(true) {}
}

// 74: Software interrupt 2
__attribute__((weak)) void SWI2_IRQHandler(void) {
    while(true) {}
}

// 75: Software interrupt 3
__attribute__((weak)) void SWI3_IRQHandler(void) {
    while(true) {}
}

// 76: Software interrupt 4
__attribute__((weak)) void SWI4_IRQHandler(void) {
    while(true) {}
}

// 77: Software interrupt 5
__attribute__((weak)) void SWI5_IRQHandler(void) {
    while(true) {}
}

// 78: Software interrupt 6
__attribute__((weak)) void SWI6_IRQHandler(void) {
    while(true) {}
}

// 79: Software interrupt 7
__attribute__((weak)) void SWI7_IRQHandler(void) {
    while(true) {}
}

// 96: Timer 0
__attribute__((weak)) void TIMER_0_IRQHandler(void) {
    while(true) {}
}

// 97: Timer 1
__attribute__((weak)) void TIMER_1_IRQHandler(void) {
    while(true) {}
}

// 98: Timer 2
__attribute__((weak)) void TIMER_2_IRQHandler(void) {
    while(true) {}
}

// 99: Timer 3
__attribute__((weak)) void TIMER_3_IRQHandler(void) {
    while(true) {}
}

// 100: H264 0
__attribute__((weak)) void H264_0_IRQHandler(void) {
    while(true) {}
}

// 101: H264 1
__attribute__((weak)) void H264_1_IRQHandler(void) {
    while(true) {}
}

// 102: H264 2
__attribute__((weak)) void H264_2_IRQHandler(void) {
    while(true) {}
}

// 103: JPEG
__attribute__((weak)) void JPEG_IRQHandler(void) {
    while(true) {}
}

// 104: ISP
__attribute__((weak)) void ISP_IRQHandler(void) {
    while(true) {}
}

// 105: USB
__attribute__((weak)) void USB_IRQHandler(void) {
    while(true) {}
}

// 106: V3D
__attribute__((weak)) void V3D_IRQHandler(void) {
    while(true) {}
}

// 107: Transposer
__attribute__((weak)) void TRANSPOSER_IRQHandler(void) {
    while(true) {}
}

// 108: Multicore Sync 0
__attribute__((weak)) void MULTICORE_SYNC_0_IRQHandler(void) {
    while(true) {}
}

// 109: Multicore Sync 1
__attribute__((weak)) void MULTICORE_SYNC_1_IRQHandler(void) {
    while(true) {}
}

// 110: Multicore Sync 2
__attribute__((weak)) void MULTICORE_SYNC_2_IRQHandler(void) {
    while(true) {}
}

// 111: Multicore Sync 3
__attribute__((weak)) void MULTICORE_SYNC_3_IRQHandler(void) {
    while(true) {}
}

// 112: DMA 0
__attribute__((weak)) void DMA_0_IRQHandler(void) {
    while(true) {}
}

// 113: DMA 1
__attribute__((weak)) void DMA_1_IRQHandler(void) {
    while(true) {}
}

// 114: DMA 2
__attribute__((weak)) void DMA_2_IRQHandler(void) {
    while(true) {}
}

// 115: DMA 3
__attribute__((weak)) void DMA_3_IRQHandler(void) {
    while(true) {}
}

// 116: DMA 4
__attribute__((weak)) void DMA_4_IRQHandler(void) {
    while(true) {}
}

// 117: DMA 5
__attribute__((weak)) void DMA_5_IRQHandler(void) {
    while(true) {}
}

// 118: DMA 6
__attribute__((weak)) void DMA_6_IRQHandler(void) {
    while(true) {}
}

// 119: OR of DMA 7 and 8
__attribute__((weak)) void DMA_7_8_IRQHandler(void) {
    while(true) {}
}

// 120: OR of DMA 9 and 10
__attribute__((weak)) void DMA_9_10_IRQHandler(void) {
    while(true) {}
}

// 121: DMA 11
__attribute__((weak)) void DMA_11_IRQHandler(void) {
    while(true) {}
}

// 122: DMA 12
__attribute__((weak)) void DMA_12_IRQHandler(void) {
    while(true) {}
}

// 123: DMA 13
__attribute__((weak)) void DMA_13_IRQHandler(void) {
    while(true) {}
}

// 124: DMA 14
__attribute__((weak)) void DMA_14_IRQHandler(void) {
    while(true) {}
}

// 125: OR of UART1, SPI1 and SPI2
__attribute__((weak)) void AUX_IRQHandler(void) {
    while (AUX->IRQ != 0) {
        if (AUX->IRQ_b.UART_1) {
            UART1_IRQHandler();
        } else if (AUX->IRQ_b.SPI_1) {
            SPI1_IRQHandler();
        } else if (AUX->IRQ_b.SPI_2) {
            SPI2_IRQHandler();
        }
    }
}

// 126: ARM
__attribute__((weak)) void ARM_IRQHandler(void) {
    while(true) {}
}

// 127: DMA 15
__attribute__((weak)) void DMA_15_IRQHandler(void) {
    while(true) {}
}

// 128: HDMI CEC
__attribute__((weak)) void HDMI_CEC_IRQHandler(void) {
    while(true) {}
}

// 129: HVS
__attribute__((weak)) void HVS_IRQHandler(void) {
    while(true) {}
}

// 130: RPIVID
__attribute__((weak)) void RPIVID_IRQHandler(void) {
    while(true) {}
}

// 131: SDC
__attribute__((weak)) void SDC_IRQHandler(void) {
    while(true) {}
}

// 132: DSI 0
__attribute__((weak)) void DSI_0_IRQHandler(void) {
    while(true) {}
}

// 133: Pixel Valve 2
__attribute__((weak)) void PIXEL_VALVE_2_IRQHandler(void) {
    while(true) {}
}

// 134: Camera 0
__attribute__((weak)) void CAMERA_0_IRQHandler(void) {
    while(true) {}
}

// 135: Camera 1
__attribute__((weak)) void CAMERA_1_IRQHandler(void) {
    while(true) {}
}

// 136: HDMI 0
__attribute__((weak)) void HDMI_0_IRQHandler(void) {
    while(true) {}
}

// 137: HDMI 1
__attribute__((weak)) void HDMI_1_IRQHandler(void) {
    while(true) {}
}

// 138: Pixel Valve 3
__attribute__((weak)) void PIXEL_VALVE_3_IRQHandler(void) {
    while(true) {}
}

// 139: SPI/BSC Slave
__attribute__((weak)) void SPI_BSC_SLAVE_IRQHandler(void) {
    while(true) {}
}

// 140: DSI 1
__attribute__((weak)) void DSI_1_IRQHandler(void) {
    while(true) {}
}

// 141: Pixel Valve 0
__attribute__((weak)) void PIXEL_VALVE_0_IRQHandler(void) {
    while(true) {}
}

// 142: OR of Pixel Valve 1 and 2
__attribute__((weak)) void PIXEL_VALVE_1_2_IRQHandler(void) {
    while(true) {}
}

// 143: CPR
__attribute__((weak)) void CPR_IRQHandler(void) {
    while(true) {}
}

// 144: SMI
__attribute__((weak)) void SMI_IRQHandler(void) {
    while(true) {}
}

// 145: GPIO 0
__attribute__((weak)) void GPIO_0_IRQHandler(void) {
    while(true) {}
}

// 146: GPIO 1
__attribute__((weak)) void GPIO_1_IRQHandler(void) {
    while(true) {}
}

// 147: GPIO 2
__attribute__((weak)) void GPIO_2_IRQHandler(void) {
    while(true) {}
}

// 148: GPIO 3
__attribute__((weak)) void GPIO_3_IRQHandler(void) {
    while(true) {}
}

// 149: OR of all I2C
__attribute__((weak)) void I2C_IRQHandler(void) {
    while(true) {}
}

// 150: OR of all SPI
__attribute__((weak)) void SPI_IRQHandler(void) {
    while(true) {}
}

// 151: PCM/I2S
__attribute__((weak)) void PCM_I2S_IRQHandler(void) {
    while(true) {}
}

// 152: SDHOST
__attribute__((weak)) void SDHOST_IRQHandler(void) {
    while(true) {}
}

// 153: OR of all PL011 UARTs
__attribute__((weak)) void UART_IRQHandler(void) {
    while(true) {}
}

// 154: OR of all ETH_PCIe L2
__attribute__((weak)) void ETH_PCIE_IRQHandler(void) {
    while(true) {}
}

// 155: VEC
__attribute__((weak)) void VEC_IRQHandler(void) {
    while(true) {}
}

// 156: CPG
__attribute__((weak)) void CPG_IRQHandler(void) {
    while(true) {}
}

// 157: RNG
__attribute__((weak)) void RNG_IRQHandler(void) {
    while(true) {}
}

// 158: OR of EMMC and EMMC2
__attribute__((weak)) void EMMC_IRQHandler(void) {
    while(true) {}
}

// 159: ETH_PCIe secure
__attribute__((weak)) void ETH_PCIE_SECURE_IRQHandler(void) {
    while(true) {}
}



void* interrupt_handlers[160] = {
  NULL, // 0
  NULL, // 1
  NULL, // 2
  NULL, // 3
  NULL, // 4
  NULL, // 5
  NULL, // 6
  NULL, // 7
  NULL, // 8
  NULL, // 9
  NULL, // 10
  NULL, // 11
  NULL, // 12
  NULL, // 13
  NULL, // 14
  NULL, // 15
  NULL, // 16
  NULL, // 17
  NULL, // 18
  NULL, // 19
  NULL, // 20
  NULL, // 21
  NULL, // 22
  NULL, // 23
  NULL, // 24
  NULL, // 25
  NULL, // 26
  NULL, // 27
  NULL, // 28
  NULL, // 29
  NULL, // 30
  NULL, // 31
  NULL, // 32
  NULL, // 33
  NULL, // 34
  NULL, // 35
  NULL, // 36
  NULL, // 37
  NULL, // 38
  NULL, // 39
  NULL, // 40
  NULL, // 41
  NULL, // 42
  NULL, // 43
  NULL, // 44
  NULL, // 45
  NULL, // 46
  NULL, // 47
  NULL, // 48
  NULL, // 49
  NULL, // 50
  NULL, // 51
  NULL, // 52
  NULL, // 53
  NULL, // 54
  NULL, // 55
  NULL, // 56
  NULL, // 57
  NULL, // 58
  NULL, // 59
  NULL, // 60
  NULL, // 61
  NULL, // 62
  NULL, // 63
  TIMER_IRQHandler, // 64
  MAILBOX_IRQHandler, // 65
  DOORBELL0_IRQHandler, // 66
  DOORBELL1_IRQHandler, // 67
  VPU0_HALTED_IRQHandler, // 68
  VPU1_HALTED_IRQHandler, // 69
  ARM_ADDRESS_ERROR_IRQHandler, // 70
  ARM_AXI_ERROR_IRQHandler, // 71
  SWI0_IRQHandler, // 72
  SWI1_IRQHandler, // 73
  SWI2_IRQHandler, // 74
  SWI3_IRQHandler, // 75
  SWI4_IRQHandler, // 76
  SWI5_IRQHandler, // 77
  SWI6_IRQHandler, // 78
  SWI7_IRQHandler, // 79
  NULL, // 80
  NULL, // 81
  NULL, // 82
  NULL, // 83
  NULL, // 84
  NULL, // 85
  NULL, // 86
  NULL, // 87
  NULL, // 88
  NULL, // 89
  NULL, // 90
  NULL, // 91
  NULL, // 92
  NULL, // 93
  NULL, // 94
  NULL, // 95
  TIMER_0_IRQHandler, // 96
  TIMER_1_IRQHandler, // 97
  TIMER_2_IRQHandler, // 98
  TIMER_3_IRQHandler, // 99
  H264_0_IRQHandler, // 100
  H264_1_IRQHandler, // 101
  H264_2_IRQHandler, // 102
  JPEG_IRQHandler, // 103
  ISP_IRQHandler, // 104
  USB_IRQHandler, // 105
  V3D_IRQHandler, // 106
  TRANSPOSER_IRQHandler, // 107
  MULTICORE_SYNC_0_IRQHandler, // 108
  MULTICORE_SYNC_1_IRQHandler, // 109
  MULTICORE_SYNC_2_IRQHandler, // 110
  MULTICORE_SYNC_3_IRQHandler, // 111
  DMA_0_IRQHandler, // 112
  DMA_1_IRQHandler, // 113
  DMA_2_IRQHandler, // 114
  DMA_3_IRQHandler, // 115
  DMA_4_IRQHandler, // 116
  DMA_5_IRQHandler, // 117
  DMA_6_IRQHandler, // 118
  DMA_7_8_IRQHandler, // 119
  DMA_9_10_IRQHandler, // 120
  DMA_11_IRQHandler, // 121
  DMA_12_IRQHandler, // 122
  DMA_13_IRQHandler, // 123
  DMA_14_IRQHandler, // 124
  AUX_IRQHandler, // 125
  ARM_IRQHandler, // 126
  DMA_15_IRQHandler, // 127
  HDMI_CEC_IRQHandler, // 128
  HVS_IRQHandler, // 129
  RPIVID_IRQHandler, // 130
  SDC_IRQHandler, // 131
  DSI_0_IRQHandler, // 132
  PIXEL_VALVE_2_IRQHandler, // 133
  CAMERA_0_IRQHandler, // 134
  CAMERA_1_IRQHandler, // 135
  HDMI_0_IRQHandler, // 136
  HDMI_1_IRQHandler, // 137
  PIXEL_VALVE_3_IRQHandler, // 138
  SPI_BSC_SLAVE_IRQHandler, // 139
  DSI_1_IRQHandler, // 140
  PIXEL_VALVE_0_IRQHandler, // 141
  PIXEL_VALVE_1_2_IRQHandler, // 142
  CPR_IRQHandler, // 143
  SMI_IRQHandler, // 144
  GPIO_0_IRQHandler, // 145
  GPIO_1_IRQHandler, // 146
  GPIO_2_IRQHandler, // 147
  GPIO_3_IRQHandler, // 148
  I2C_IRQHandler, // 149
  SPI_IRQHandler, // 150
  PCM_I2S_IRQHandler, // 151
  SDHOST_IRQHandler, // 152
  UART_IRQHandler, // 153
  ETH_PCIE_IRQHandler, // 154
  VEC_IRQHandler, // 155
  CPG_IRQHandler, // 156
  RNG_IRQHandler, // 157
  EMMC_IRQHandler, // 158
  ETH_PCIE_SECURE_IRQHandler, // 159
  
};