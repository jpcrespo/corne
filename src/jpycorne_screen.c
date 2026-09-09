/* SPDX-License-Identifier: MIT
 * Original pixel Bitcoin mark. No external font or image download required.
 */
#include <lvgl.h>
#include <stdint.h>
#include <stddef.h>

/* A double-stem Bitcoin B, 16 x 20 monochrome pixels. */
static const uint16_t bitcoin_rows[] = {
    0x0A00, 0x0A00, 0x3FE0, 0x3FF0, 0x0C38,
    0x0C18, 0x0C18, 0x0C38, 0x0FF0, 0x0FE0,
    0x0FF0, 0x0C38, 0x0C18, 0x0C18, 0x0C38,
    0x3FF0, 0x3FE0, 0x0A00, 0x0A00, 0x0000
};

static void draw_mark(lv_obj_t *parent, int x, int y) {
    /* Merge adjacent pixels into runs to keep the LVGL object count small. */
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 16;) {
            if (!(bitcoin_rows[row] & (0x8000u >> col))) { col++; continue; }
            int start = col;
            while (col < 16 && (bitcoin_rows[row] & (0x8000u >> col))) col++;
            lv_obj_t *run = lv_obj_create(parent);
            lv_obj_remove_style_all(run);
            lv_obj_set_pos(run, x + start, y + row);
            lv_obj_set_size(run, col - start, 1);
            lv_obj_set_style_bg_color(run, lv_color_white(), 0);
            lv_obj_set_style_bg_opa(run, LV_OPA_COVER, 0);
        }
    }
}

lv_obj_t *zmk_display_status_screen(void) {
#if LVGL_VERSION_MAJOR >= 9
    int width = lv_display_get_horizontal_resolution(lv_display_get_default());
    int height = lv_display_get_vertical_resolution(lv_display_get_default());
#else
    int width = lv_disp_get_hor_res(lv_disp_get_default());
    int height = lv_disp_get_ver_res(lv_disp_get_default());
#endif
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_size(screen, width, height);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *name = lv_label_create(screen);
    lv_obj_set_style_text_font(name, &lv_font_unscii_8, 0);
    lv_obj_set_style_text_color(name, lv_color_white(), 0);
    lv_obj_set_style_text_line_space(name, 0, 0);
    if (width >= 96) {
        int left = (width - 88) / 2;
        draw_mark(screen, left, (height - 20) / 2);
        lv_label_set_text(name, "Jpycorne");
        lv_obj_set_pos(name, left + 24, (height - 8) / 2);
    } else {
        /* Accommodate a display configured in portrait orientation. */
        draw_mark(screen, (width - 16) / 2, 4);
        lv_label_set_text(name, "J\np\ny\nc\no\nr\nn\ne");
        lv_obj_set_pos(name, (width - 8) / 2, 28);
    }
    return screen;
}
