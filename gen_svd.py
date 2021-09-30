from jinja2 import Environment, FileSystemLoader
import pathlib

env = Environment(
    loader=FileSystemLoader(searchpath=["svd"])
)

bcm2711 = env.get_template("chips/bcm2711.svd.jinja")

p = pathlib.Path("svd/chips")

bcm2711_altfunc = [[None] * 6 for i in range(58)]

def i2c(start_pin, altfunc, instance_number):
    bcm2711_altfunc[start_pin][altfunc] = f"SDA{instance_number}"
    bcm2711_altfunc[start_pin + 1][altfunc] = f"SCL{instance_number}"

i2c(0, 0, 0)
i2c(2, 0, 1)
i2c(0, 5, 6)
i2c(2, 5, 3)
i2c(4, 5, 3)
i2c(6, 5, 4)
i2c(8, 5, 4)
i2c(10, 5, 5)
i2c(12, 5, 5)
i2c(22, 5, 6)
i2c(28, 0, 0)
i2c(44, 1, 0)
i2c(44, 2, 1)

def spi(start_pin, altfunc, instance_number):
    bcm2711_altfunc[start_pin][altfunc] = f"SPI{instance_number}_CE0_N"
    bcm2711_altfunc[start_pin + 1][altfunc] = f"SPI{instance_number}_MISO"
    bcm2711_altfunc[start_pin + 2][altfunc] = f"SPI{instance_number}_MOSI"
    bcm2711_altfunc[start_pin + 3][altfunc] = f"SPI{instance_number}_SCLK"

spi(0, 3, 3)
spi(4, 3, 4)
bcm2711_altfunc[7][0] = "SPI0_CE1_N"
spi(8, 0, 0)
spi(12, 3, 5)
spi(18, 3, 6)
bcm2711_altfunc[16][4] = "SPI1_CE2_N"
bcm2711_altfunc[17][4] = "SPI1_CE1_N"
spi(18, 4, 1)
for i, instance in enumerate(range(3, 7)):
    bcm2711_altfunc[24 + i][5] = f"SPI{instance}_CE1_N"
bcm2711_altfunc[35][0] = "SPI0_CE1_N"
spi(36, 0, 0)

def in_order(start_pin, altfunc, prefix, suffixes):
    for i, suffix in enumerate(suffixes):
        if suffix is None:
            continue
        bcm2711_altfunc[start_pin + i][altfunc] = prefix + str(suffix)

in_order(4, 2, "DPI_D", range(0, 24))
in_order(8, 1, "SD", range(18))
in_order(0, 1, "SA", range(5, -1 , -1))
in_order(6, 1, "", ("SOE_N", "SWE_N"))
in_order(0, 2, "", ("PCLK", "DE", "LCD_VSYNC", "LCD_HSYNC"))
in_order(4, 0, "GPCLK", range(3))
in_order(8, 3, "BSCSL_", ("CE_N", "MISO", "MOSI", "SCLK"))
in_order(12, 0, "PWM0_", range(2))
in_order(18, 5, "PWM0_", range(2))
in_order(18, 0, "PCM_", ("CLK", "FS", "DIN", "DOUT"))
in_order(20, 5, "GPCLK", range(2))
in_order(22, 0, "SD0_", ("CLK", "CMD", "DAT0", "DAT1", "DAT2", "DAT3"))
in_order(22, 3, "SD1_", ("CLK", "CMD", "DAT0", "DAT1", "DAT2", "DAT3"))
in_order(22, 4, "ARM_", ("TRST", "RTCK", "TDO", "TCK", "TDI", "TMS"))
in_order(28, 1, "SA", range(5, -1 , -1))
in_order(28, 4, "MII_A_", ("RX_ERR", "TX_ERR", "CRS", "COL"))
in_order(28, 5, "RGMII_", ("MDIO", "MDC"))
in_order(32, 4, "SD_CARD_", ("PRES", "WRPROT", "LED"))
in_order(34, 3, "SD1_", ("CLK", "CMD"))
in_order(36, 3, "SD1_DAT", range(8))
in_order(36, 1, "SD", range(8))
in_order(34, 1, "", ("SOE_N", "SWE_N"))
in_order(28, 2, "PCM_", ("CLK", "FS", "DIN", "DOUT"))
bcm2711_altfunc[32][0] = "GPCLK0"
bcm2711_altfunc[34][0] = "GPCLK0"
in_order(40, 4, "SPI0_", ("MISO", "MOSI", "SCLK", "CE0_N", "CE1_N", "CE2_N"))
bcm2711_altfunc[34][5] = "RGMII_IRQ"
in_order(35, 4, "RGMII_", ("START_STOP", "RX_OK", "MDIO", "MDC", "IRQ"))
in_order(36, 5, "MII_A_", ("RX_ERR", "TX_ERR", "CRS", "COL"))
in_order(40, 0, "PWM1_", range(2))
in_order(42, 0, "GPCLK", range(1, 3))
bcm2711_altfunc[44][0] = "GPCLK1"
bcm2711_altfunc[45][0] = "PWM0_1"
in_order(44, 5, "SD_CARD_", ("VOLT", "PWR0"))

def uart(start_pin, altfunc, instance_number):
    bcm2711_altfunc[start_pin][altfunc] = f"TXD{instance_number}"
    bcm2711_altfunc[start_pin + 1][altfunc] = f"RXD{instance_number}"
    bcm2711_altfunc[start_pin + 2][altfunc] = f"CTS{instance_number}"
    bcm2711_altfunc[start_pin + 3][altfunc] = f"RTS{instance_number}"

uart(0, 4, 2)
uart(4, 4, 3)
uart(8, 4, 4)
uart(12, 4, 5)
bcm2711_altfunc[14][0] = "TXD0"
bcm2711_altfunc[15][0] = "RXD0"
bcm2711_altfunc[16][3] = "CTS0"
bcm2711_altfunc[17][3] = "RTS0"
uart(14, 5, 1)
for i, name in enumerate(("CTS", "RTS", "TXD", "RXD")):
    bcm2711_altfunc[30 + i][3] = name + "0"
    bcm2711_altfunc[30 + i][5] = name + "1"

uart(36, 2, 0)
uart(40, 5, 1)

for i, row in enumerate(bcm2711_altfunc):
    print(i, repr(row))

(p / "bcm2711_lpa.svd").write_text(bcm2711.render(peripheral_base=0xFE000000, altfunc=bcm2711_altfunc, name="bcm2711_lpa"))
