/*
 * Copyright (c) 2023 Rumbledethumps
 *
 * SPDX-License-Identifier: MIT-0
 * SPDX-License-Identifier: Unlicense
 */

#include <rp6502.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void task()
{
    static int x = 0;
    static int y = 100;

    xram0_struct_set(0xFF10, vga_mode3_config_t, x_pos_px, x);
    if (--x < 0)
        x = 143;

    RIA.addr0 = 0xFFE0;
    if (!(RIA.rw0 & 1))
    {
        if (--y < 0)
            y = 0;
    }
    else
    {
        if (++y > 200)
            y = 200;
    }
    xram0_struct_set(0xFF00, vga_mode4_sprite_t, y_pos_px, y);
}

void main()
{
    xreg_vga_canvas(1);

    xram0_struct_set(0xFF00, vga_mode4_sprite_t, x_pos_px, 10);
    xram0_struct_set(0xFF00, vga_mode4_sprite_t, y_pos_px, 100);
    xram0_struct_set(0xFF00, vga_mode4_sprite_t, xram_sprite_ptr, 0x0000);
    xram0_struct_set(0xFF00, vga_mode4_sprite_t, log_size, 4);
    xram0_struct_set(0xFF00, vga_mode4_sprite_t, has_opacity_metadata, false);
    xreg_vga_mode(4, 0, 0xFF00, 1, 0);

    xram0_struct_set(0xFF10, vga_mode3_config_t, x_wrap, true);
    xram0_struct_set(0xFF10, vga_mode3_config_t, y_wrap, true);
    xram0_struct_set(0xFF10, vga_mode3_config_t, x_pos_px, 0);
    xram0_struct_set(0xFF10, vga_mode3_config_t, y_pos_px, 0);
    xram0_struct_set(0xFF10, vga_mode3_config_t, width_px, 144);
    xram0_struct_set(0xFF10, vga_mode3_config_t, height_px, 256);
    xram0_struct_set(0xFF10, vga_mode3_config_t, xram_data_ptr, 0x2000);
    xram0_struct_set(0xFF10, vga_mode3_config_t, xram_palette_ptr, 0x1000);
    xreg_vga_mode(3, 3, 0xFF10, 0);

    xreg_ria_keyboard(0xFFE0);

    {
        uint8_t v = RIA.vsync;
        while (1)
        {
            if (RIA.vsync == v)
                continue;
            v = RIA.vsync;
            task();
        }
    }
}
