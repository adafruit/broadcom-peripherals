videocore_interrupts = [
    "Timer 0",
    "Timer 1",
    "Timer 2",
    "Timer 3",
    "H264 0",
    "H264 1",
    "H264 2",
    "JPEG",
    "ISP",
    "USB",
    "V3D",
    "Transposer",
    "Multicore Sync 0",
    "Multicore Sync 1",
    "Multicore Sync 2",
    "Multicore Sync 3",
    "DMA 0",
    "DMA 1",
    "DMA 2",
    "DMA 3",
    "DMA 4",
    "DMA 5",
    "DMA 6",
    ("DMA_7_8", "OR of DMA 7 and 8"),
    ("DMA_9_10", "OR of DMA 9 and 10"),
    "DMA 11",
    "DMA 12",
    "DMA 13",
    "DMA 14",
    ("AUX", "OR of UART1, SPI1 and SPI2"),
    "ARM",
    "DMA 15",
    "HDMI CEC",
    "HVS",
    "RPIVID",
    "SDC",
    "DSI 0",
    "Pixel Valve 2",
    "Camera 0",
    "Camera 1",
    "HDMI 0",
    "HDMI 1",
    "Pixel Valve 3",
    "SPI/BSC Slave",
    "DSI 1",
    "Pixel Valve 0",
    ("PIXEL_VALVE_1_2", "OR of Pixel Valve 1 and 2"),
    "CPR",
    "SMI",
    "GPIO 0",
    "GPIO 1",
    "GPIO 2",
    "GPIO 3",
    ("I2C", "OR of all I2C"),
    ("SPI", "OR of all SPI"),
    "PCM/I2S",
    "SDHOST",
    ("UART", "OR of all PL011 UARTs"),
    ("ETH_PCIE", "OR of all ETH_PCIe L2"),
    "VEC",
    "CPG",
    "RNG",
    ("EMMC", "OR of EMMC and EMMC2"),
    "ETH_PCIe secure"
]

armc_interrupts = [
    ("TIMER", "ARMC Timer"),
    ("MAILBOX", "Mailbox"),
    ("DOORBELL0", "Doorbell 0"),
    ("DOORBELL1", "Doorbell 1"),
    ("VPU0_HALTED", "VPU0 halted"),
    ("VPU1_HALTED", "VPU1 halted"),
    ("ARM_ADDRESS_ERROR", "ARM address error"),
    ("ARM_AXI_ERROR", "ARM AXI error"),
]

for i in range(8):
    armc_interrupts.append((f"SWI{i}", f"Software interrupt {i}"))

interrupt_names = {}
for i, irq in enumerate(armc_interrupts):
    interrupt_names[64 + i] = irq
for i, irq in enumerate(videocore_interrupts):
    if isinstance(irq, str):
        name = irq.replace(" ", "_").replace("/", "_").upper()
        irq = (name, irq)
    interrupt_names[96 + i] = irq

interrupt_count = max(interrupt_names.keys()) + 1

if __name__ == "__main__":
    from jinja2 import Environment, FileSystemLoader
    import pathlib

    env = Environment(loader=FileSystemLoader(searchpath=["."]), trim_blocks=True)

    interrupt_handlers = env.get_template("broadcom/gen/interrupt_handlers.c.jinja")

    pathlib.Path("broadcom/gen/interrupt_handlers.c").write_text(
        interrupt_handlers.render(
            interrupt_count=interrupt_count,
            interrupt_names=interrupt_names
        )
    )

    interrupt_handlers = env.get_template("broadcom/gen/interrupt_handlers.h.jinja")

    pathlib.Path("broadcom/gen/interrupt_handlers.h").write_text(
        interrupt_handlers.render(
            interrupt_count=interrupt_count,
        )
    )
