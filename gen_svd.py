from jinja2 import Environment, FileSystemLoader
import pathlib

env = Environment(
    loader=FileSystemLoader(searchpath=["svd"])
)

bcm2711 = env.get_template("chips/bcm2711.svd.jinja")

p = pathlib.Path("svd/chips")

bcm2711_altfunc = [[None] * 5 for i in range(57)]
for i in range(0, 24):
    bcm2711_altfunc[i+4][2] = f"DPI_D{i}"
for i in range(18):
    bcm2711_altfunc[i+8][1] = f"SD{i}"
for row in bcm2711_altfunc:
    print(repr(row))

(p / "bcm2711_lpa.svd").write_text(bcm2711.render(peripheral_base=0xFE000000, altfunc=bcm2711_altfunc))
