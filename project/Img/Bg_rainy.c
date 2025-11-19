#include "lvgl.h"

// small helper to create one raindrop
static void create_raindrop(lv_obj_t *parent, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *drop = lv_obj_create(parent);
    lv_obj_remove_style_all(drop);                 // no default style

    lv_obj_set_size(drop, 10, 18);
    lv_obj_set_style_radius(drop, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(drop, lv_color_hex(0x008CFF), 0); // blue
    lv_obj_set_style_border_width(drop, 0, 0);

    lv_obj_align(drop, LV_ALIGN_TOP_LEFT, x, y);
}

void draw_rainy_ui(lv_obj_t *tile)
{
    // background gradient (dark top -> light bottom)
    lv_obj_set_style_bg_color(tile, lv_color_hex(0x3F4E5A), 0);
    lv_obj_set_style_bg_grad_color(tile, lv_color_hex(0xD8F2FF), 0);
    lv_obj_set_style_bg_grad_dir(tile, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_border_width(tile, 0, 0);

    // big cloud at the top
    lv_obj_t *cloud = lv_obj_create(tile);
    lv_obj_remove_style_all(cloud);
    lv_obj_set_size(cloud, 480, 110);                 // a bit wider than screen
    lv_obj_set_style_bg_color(cloud, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(cloud, 60, 0);
    lv_obj_set_style_border_width(cloud, 0, 0);
    lv_obj_align(cloud, LV_ALIGN_TOP_MID, 0, -40);    // push it up so the top is hidden

    // raindrops in a staggered grid
    int start_y = 120;      // start under the cloud
    int rows    = 7;
    int cols    = 5;
    int dx      = 70;
    int dy      = 45;
    int start_x = 30;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int x = start_x + c * dx + ((r & 1) ? 30 : 0);  // offset every other row
            int y = start_y + r * dy;
            create_raindrop(tile, x, y);
        }
    }
}
