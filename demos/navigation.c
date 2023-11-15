#include <stdio.h>
#include <stdlib.h>


#include "arm_2d_helper.h"
#include "arm_2d_scenes.h"
#include "arm_2d_disp_adapters.h"

#include "arm_extra_controls.h"



#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif

/*============================ MACROS ========================================*/

#if __GLCD_CFG_COLOUR_DEPTH__ == 8

#define arm_2d_pixel_from_brga8888  arm_2d_pixel_brga8888_to_gray8

#elif __GLCD_CFG_COLOUR_DEPTH__ == 16

#define arm_2d_pixel_from_brga8888  arm_2d_pixel_brga8888_to_rgb565

#elif __GLCD_CFG_COLOUR_DEPTH__ == 32

#define arm_2d_pixel_from_brga8888

#else
#   error Unsupported colour depth!
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
extern
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ARIAL32_A4;

extern
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ARIAL20_A4;

extern
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ARIAL24_A4;

extern
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ARIAL24_A2;

extern
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ARIAL24_A1;

extern
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ARIAL24_A8;

extern
struct {
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
} ARM_2D_FONT_ARIAL16_A4;

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/



__OVERRIDE_WEAK
IMPL_PFB_ON_DRAW(__disp_adapter0_draw_navigation)
{
    ARM_2D_UNUSED(pTarget);
    ARM_2D_UNUSED(bIsNewFrame);

    arm_2d_size_t tScreenSize = ptTile->tRegion.tSize;

    arm_2d_canvas(ptTile, __navigator_canvas) {
    
        arm_2d_align_bottom_centre( __navigator_canvas, 
                                    tScreenSize.iWidth, 
                                    tScreenSize.iHeight >> 1) {

            arm_2d_align_centre(__bottom_centre_region, tScreenSize.iWidth - 40, 60) {
                
                draw_round_corner_box(  ptTile, 
                                        &__centre_region, 
                                        __RGB(0x56, 0xB7, 0xF3), 
                                        64,
                                        bIsNewFrame);

                ARM_2D_OP_WAIT_ASYNC();

                do {
                    arm_lcd_text_set_target_framebuffer(NULL);
                    arm_lcd_text_set_colour(GLCD_COLOR_LIGHT_GREY, GLCD_COLOR_BLACK);
                    arm_lcd_text_set_font((const arm_2d_font_t *)&ARM_2D_FONT_6x8);

                    arm_2d_size_t tStringSize = (*(arm_2d_font_t *)&ARM_2D_FONT_6x8).tCharSize;
                    tStringSize.iWidth = __centre_region.tSize.iWidth - 20;


                    arm_2d_align_centre(__bottom_centre_region, tStringSize) {

                        __centre_region.tLocation.iY += 8;

                        arm_lcd_text_set_draw_region(&__centre_region);

                        arm_lcd_text_location(0,0);
                        arm_lcd_puts("FPS:");

                        arm_lcd_text_location(0, 22);
                        arm_lcd_puts("CPU Usage:       %");

                        arm_lcd_text_location(0, 50);
                        arm_lcd_puts("LCD Latency:    ms");
                    }
                } while(0);

            }

            do {
                arm_2d_size_t tStringSize = (*(arm_2d_font_t *)&ARM_2D_FONT_ARIAL32_A4).tCharSize;
                tStringSize.iWidth *= 2;

                arm_2d_align_centre(__bottom_centre_region, tStringSize) {

                    __centre_region.tLocation.iX += 10;

                    arm_lcd_text_set_target_framebuffer(NULL);
                    COLOUR_INT tColour = arm_2d_pixel_from_brga8888( 
                                                __arm_2d_helper_colour_slider(
                                                    __RGB32(0, 0xFF, 0),
                                                    __RGB32(0xFF, 0, 0), 
                                                    10000,
                                                    (int32_t)(DISP0_ADAPTER.Benchmark.fCPUUsage * 100.0f) ));

                    arm_lcd_text_set_colour(tColour, GLCD_COLOR_BLACK);

                    arm_lcd_text_set_font((const arm_2d_font_t *)&ARM_2D_FONT_ARIAL32_A4);
                    arm_lcd_text_set_draw_region(&__centre_region);
                    arm_lcd_text_location(0,0);
                    arm_lcd_printf("%2.0f", DISP0_ADAPTER.Benchmark.fCPUUsage);

                }
            } while(0);

            do {
                arm_2d_size_t tStringSize = (*(arm_2d_font_t *)&ARM_2D_FONT_ARIAL32_A4).tCharSize;
                tStringSize.iWidth *= 2;

                int32_t nFPS =  MIN(arm_2d_helper_get_reference_clock_frequency() / DISP0_ADAPTER.Benchmark.wAverage, 999);

                arm_2d_align_mid_left(__bottom_centre_region, tStringSize) {


                    __mid_left_region.tLocation.iX += 60;

                    arm_lcd_text_set_target_framebuffer(NULL);
                    COLOUR_INT tColour = arm_2d_pixel_from_brga8888( 
                                                __arm_2d_helper_colour_slider(
                                                    __RGB32(0xFF, 0, 0), 
                                                    __RGB32(0, 0xFF, 0),
                                                    30,
                                                    nFPS ));

                    arm_lcd_text_set_colour(tColour, GLCD_COLOR_BLACK);
                    arm_lcd_text_set_font((const arm_2d_font_t *)&ARM_2D_FONT_ARIAL32_A4);
                    arm_lcd_text_set_draw_region(&__mid_left_region);
                    arm_lcd_text_location(0,0);
                    arm_lcd_printf("%d", nFPS);

                }
            } while(0);

            do {
                arm_2d_size_t tStringSize = (*(arm_2d_font_t *)&ARM_2D_FONT_ARIAL20_A4).tCharSize;
                tStringSize.iWidth *= 2;

                arm_2d_align_mid_right(__bottom_centre_region, tStringSize) {

                    int32_t nLCDLatency 
                        = (int32_t)arm_2d_helper_convert_ticks_to_ms(
                            DISP0_ADAPTER.Benchmark.wLCDLatency);

                    __mid_right_region.tLocation.iX -= 45;
                    __mid_right_region.tLocation.iY += 5;

                    arm_lcd_text_set_target_framebuffer(NULL);
                    COLOUR_INT tColour = arm_2d_pixel_from_brga8888(
                                                __arm_2d_helper_colour_slider(
                                                    __RGB32(0, 0xFF, 0),
                                                    __RGB32(0xFF, 0, 0), 
                                                    50,
                                                    nLCDLatency ));

                    arm_lcd_text_set_colour(tColour, GLCD_COLOR_BLACK);
                    arm_lcd_text_set_font((const arm_2d_font_t *)&ARM_2D_FONT_ARIAL20_A4);
                    arm_lcd_text_set_draw_region(&__mid_right_region);
                    arm_lcd_text_location(0,0);
                    arm_lcd_printf("%d", nLCDLatency);

                }
            } while(0);
        }

    }

    ARM_2D_OP_WAIT_ASYNC();

    return arm_fsm_rt_cpl;
}

void disp_adapter0_navigator_init(void)
{
    /*! define dirty regions for the navigation layer */
    IMPL_ARM_2D_REGION_LIST(s_tNavDirtyRegionList, static)

        /* a region for the status bar on the bottom of the screen */
        ADD_LAST_REGION_TO_LIST(s_tNavDirtyRegionList,
            0
        ),

    END_IMPL_ARM_2D_REGION_LIST(s_tNavDirtyRegionList)

    /* get the screen region */
    arm_2d_region_t tScreen
        = arm_2d_helper_pfb_get_display_area(
            &DISP0_ADAPTER.use_as__arm_2d_helper_pfb_t);
    
    arm_2d_align_bottom_centre(tScreen, tScreen.tSize.iWidth, tScreen.tSize.iHeight >> 1) {

        arm_2d_align_centre(__bottom_centre_region, tScreen.tSize.iWidth - 40, 60) {
            s_tNavDirtyRegionList[0].tRegion = __centre_region;
        }
    }


    /* register event handler for evtOnDrawNavigation */
    arm_2d_scene_player_register_on_draw_navigation_event_handler(
                    &DISP0_ADAPTER,
                    __disp_adapter0_draw_navigation,
                    NULL,
                    (arm_2d_region_list_item_t *)s_tNavDirtyRegionList);
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
