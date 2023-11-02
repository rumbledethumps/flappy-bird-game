#!/bin/python3

from PIL import Image


def rp6502_rgb(r, g, b):
    if r == 0 and g == 0 and b == 0:
        return 0
    else:
        return (((b >> 3) << 11) | ((g >> 3) << 6) | (r >> 3)) | 1 << 5


def get_palette():
    with Image.new(mode="RGB", size=[10, 10]) as im:
        with open("bin/palette.bin", "wb") as o:
            im2 = im.convert("P", palette=Image.WEB, colors=256)
            pal = im2.getpalette()
            for r, g, b in zip(*[iter(pal)] * 3):
                o.write(
                    rp6502_rgb(r, g, b).to_bytes(2, byteorder="little", signed=False)
                )
    return pal


def conv_img(name_in, name_out):
    with Image.open(name_in) as im:
        # im2 = im.convert("P", palette=palette, colors=256)
        # im = im.convert("RGB")
        im2 = im.convert("P", palette=Image.ADAPTIVE, colors=256)
        # im2 = im.quantize(palette=palette)
        with open("bin/" + name_out + ".dat.bin", "wb") as o:
            for y in range(0, int(im2.height / 2)):
                for x in range(0, int(im2.width / 2)):
                    o.write(
                        im2.getpixel((x * 2, y * 2)).to_bytes(
                            1, byteorder="little", signed=False
                        )
                    )
        with open("bin/" + name_out + ".pal.bin", "wb") as o:
            for r, g, b in zip(*[iter(im2.getpalette())] * 3):
                o.write(
                    rp6502_rgb(r, g, b).to_bytes(2, byteorder="little", signed=False)
                )


def conv_spr(name_in, name_out):
    with Image.open(name_in) as im:
        with open("bin/" + name_out, "wb") as o:
            rgb_im = im.convert("RGB")
            im2 = rgb_im.resize(size=[16, 16])
            for y in range(0, im2.height):
                for x in range(0, im2.width):
                    r, g, b = im2.getpixel((x, y))
                    o.write(
                        rp6502_rgb(r, g, b).to_bytes(
                            2, byteorder="little", signed=False
                        )
                    )


# palette = get_palette()

conv_img("samuelcust/sprites/background-day.png", "bg_day_144x256")

conv_spr("samuelcust/sprites/bluebird-midflap.png", "midflap_spr16.bin")
