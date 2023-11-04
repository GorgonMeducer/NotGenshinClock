/*
 * Copyright (c) 2009-2022 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*============================ INCLUDES ======================================*/

#include "arm_2d.h"

#define __USER_SCENE_AUDIOMARK_IMPLEMENT__
#include "arm_2d_scene_audiomark.h"

#include "arm_2d_helper.h"
#include "arm_extra_controls.h"

#include <stdlib.h>
#include <string.h>

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
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wmissing-declarations"  
#elif __IS_COMPILER_ARM_COMPILER_5__
#   pragma diag_suppress 64,177
#elif __IS_COMPILER_IAR__
#   pragma diag_suppress=Pa089,Pe188,Pe177,Pe174
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#   pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#endif

/*============================ MACROS ========================================*/

#if __GLCD_CFG_COLOUR_DEPTH__ == 8

#   define c_tileCMSISLogo          c_tileCMSISLogoGRAY8

#elif __GLCD_CFG_COLOUR_DEPTH__ == 16

#   define c_tileCMSISLogo          c_tileCMSISLogoRGB565

#elif __GLCD_CFG_COLOUR_DEPTH__ == 32

#   define c_tileCMSISLogo          c_tileCMSISLogoCCCA8888
#else
#   error Unsupported colour depth!
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#undef this
#define this (*ptThis)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

extern const arm_2d_tile_t c_tileCMSISLogo;
extern const arm_2d_tile_t c_tileCMSISLogoMask;
extern const arm_2d_tile_t c_tileCMSISLogoA2Mask;
extern const arm_2d_tile_t c_tileCMSISLogoA4Mask;
extern const arm_2d_tile_t c_tileAudioPOVGRAY8;

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
} ARM_2D_FONT_ARIAL16_A4;

extern const arm_2d_tile_t c_tileInnerGearMask;
extern const arm_2d_tile_t c_tileInnerGearMidMask;
extern const arm_2d_tile_t c_tileGenshinPointerMask;

/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/

#define PROCESSOR_INFO(__NAME, __SIZE, __SCORE, __COLOUR, __SCORE_COLOUR, __FONT)        \
            {__NAME, (__SIZE), (__SCORE), (__COLOUR), (__SCORE_COLOUR), (arm_2d_font_t *)&(__FONT)}

static const struct {
    const char  *pchName;
    int16_t     iWheelSize;
    int16_t     iAudioMark;
    COLOUR_INT  tColour;
    COLOUR_INT  tScoreColour;
    arm_2d_font_t *ptScoreFont;
} c_tProcessorInfo[] = {
    [AUDIOMARK_CORTEX_M4]           
        = PROCESSOR_INFO( "Cortex-M4",           100 + 75 * 0,    73,     __RGB(0x40, 0x40, 0x40),  __RGB(0x80, 0x80, 0x80), ARM_2D_FONT_ARIAL20_A4 ),
    [AUDIOMARK_CORTEX_M33]          
        = PROCESSOR_INFO( "Cortex-M33",           65 + 75 * 1,    101,    __RGB(0x60, 0x60, 0x60),  __RGB(0xFF, 0xFF, 0xFF), ARM_2D_FONT_ARIAL20_A4 ),
    [AUDIOMARK_CORTEX_M7]           
        = PROCESSOR_INFO( "Cortex-M7",            35 + 75 * 2,    132,    __RGB(0x80, 0x80, 0x80),  __RGB(0x80, 0x80, 0x80), ARM_2D_FONT_ARIAL20_A4 ),
    [AUDIOMARK_CORTEX_M85_SCALER]   
        = PROCESSOR_INFO( "Cortex-M85 Scaler",    20 + 75 * 3,    201,    __RGB(0x00, 0x80, 0x00),  __RGB(0xFF, 0xFF, 0xFF), ARM_2D_FONT_ARIAL32_A4 ),
    [AUDIOMARK_CORTEX_M55_HELIUM]   
        = PROCESSOR_INFO( "Cortex-M55 Helium",    20 + 75 * 4,    367,    __RGB(0x00, 0xC0, 0x00),  __RGB(0x00, 0xC0, 0x00), ARM_2D_FONT_ARIAL32_A4 ),
    [AUDIOMARK_CORTEX_M85_HELIUM]   
        = PROCESSOR_INFO( "Cortex-M85 Helium",    40 + 75 * 5,    423,    GLCD_COLOR_GREEN,         __RGB(0xFF, 0x80, 0x00), ARM_2D_FONT_ARIAL32_A4),
};

/*============================ IMPLEMENTATION ================================*/


static void __on_scene_audiomark_depose(arm_2d_scene_t *ptScene)
{
    user_scene_audiomark_t *ptThis = (user_scene_audiomark_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    ptScene->ptPlayer = NULL;
    
    arm_foreach(int64_t,this.lTimestamp, ptItem) {
        *ptItem = 0;
    }

    arm_foreach(arm_2d_op_fill_cl_msk_opa_trans_t, this.tTransformOP, ptOP) {
        ARM_2D_OP_DEPOSE(*ptOP);
    }

    for (int_fast8_t n = 0; n < dimof(this.Processor); n ++) {
        progress_wheel_depose(&this.Processor[n].tWheel);
        this.Processor[n].lTimestamp = 0;
    }

    if (!this.bUserAllocated) {
        free(ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene audiomark                                                                    *
 *----------------------------------------------------------------------------*/

static void __on_scene_audiomark_background_start(arm_2d_scene_t *ptScene)
{
    user_scene_audiomark_t *ptThis = (user_scene_audiomark_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_audiomark_background_complete(arm_2d_scene_t *ptScene)
{
    user_scene_audiomark_t *ptThis = (user_scene_audiomark_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}


static void __on_scene_audiomark_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_audiomark_t *ptThis = (user_scene_audiomark_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    for (int_fast8_t n = 0; n < dimof(this.Processor); n++) {
        
        int32_t nResult;
        if (arm_2d_helper_time_liner_slider(0, 
                                            1000, 
                                            5000ul * 100ul / (int32_t)c_tProcessorInfo[n].iAudioMark , 
                                            &nResult, 
                                            &this.Processor[n].lTimestamp)) {
            this.Processor[n].lTimestamp = 0;
        }

        this.Processor[n].iProgress = (int16_t)nResult;
    }



}

static void __on_scene_audiomark_frame_complete(arm_2d_scene_t *ptScene)
{
    user_scene_audiomark_t *ptThis = (user_scene_audiomark_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    /* switch to next scene after 20s */
    if (arm_2d_helper_is_time_out(20000, &this.lTimestamp[0])) {
        arm_2d_scene_player_switch_to_next_scene(ptScene->ptPlayer);
    }
}

static void __before_scene_audiomark_switching_out(arm_2d_scene_t *ptScene)
{
    user_scene_audiomark_t *ptThis = (user_scene_audiomark_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

void __draw_processor_list_item(user_scene_audiomark_t *ptThis,
                                const arm_2d_tile_t *ptTile, 
                                const arm_2d_region_t *ptRegion, 
                                uint_fast8_t chIndex,
                                bool bIsNewFrame)
{

    arm_2d_container(ptTile, __processor_bar, ptRegion) {
        draw_round_corner_box(  &__processor_bar, 
                                &__processor_bar_canvas, 
                                GLCD_COLOR_WHITE, 
                                32,
                                bIsNewFrame);

        arm_2d_layout(__processor_bar_canvas) {

            __item_line_horizontal(160, __processor_bar_canvas.tSize.iHeight) {
                arm_lcd_text_set_target_framebuffer((arm_2d_tile_t *)&__processor_bar);

                arm_lcd_text_set_colour(__RGB(0x56, 0xB7, 0xF3), GLCD_COLOR_WHITE);
                arm_lcd_print_banner(this.Processor[chIndex].pchName, __item_region, &ARM_2D_FONT_ARIAL16_A4);
            }

            __item_line_horizontal(200, __processor_bar_canvas.tSize.iHeight) {

               progress_bar_drill_show( &__processor_bar,
                                        &__item_region,
                                        c_tProcessorInfo[chIndex].iAudioMark * 2,
                                        c_tProcessorInfo[chIndex].tColour,
                                        bIsNewFrame);
            }

            __item_line_horizontal(100, __processor_bar_canvas.tSize.iHeight, 10, 0, 0, 0) {

                arm_2d_align_centre(__item_region, __item_region.tSize.iWidth, c_tProcessorInfo[chIndex].ptScoreFont->tCharSize.iHeight) {
                    arm_lcd_text_set_target_framebuffer((arm_2d_tile_t *)&__processor_bar);
                    arm_lcd_text_set_colour(c_tProcessorInfo[chIndex].tScoreColour, GLCD_COLOR_WHITE);
                    arm_lcd_text_set_font((const arm_2d_font_t *)(c_tProcessorInfo[chIndex].ptScoreFont));
                    arm_lcd_text_set_draw_region(&__centre_region);
                    arm_lcd_text_location(0,0);
                    arm_lcd_printf("%1.2f", (float)c_tProcessorInfo[chIndex].iAudioMark / 100.0f);
                }

            }

        }


    }

}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene_audiomark_handler)
{
    user_scene_audiomark_t *ptThis = (user_scene_audiomark_t *)pTarget;
    arm_2d_size_t tScreenSize = ptTile->tRegion.tSize;

    int32_t iResult;
    float fAngle;

    ARM_2D_UNUSED(ptTile);
    ARM_2D_UNUSED(bIsNewFrame);
    ARM_2D_UNUSED(tScreenSize);
    arm_2d_canvas(ptTile, __top_canvas) {
    /*-----------------------draw the foreground begin-----------------------*/
        
        /* following code is just a demo, you can remove them */
        
        arm_2d_fill_colour(ptTile, NULL, GLCD_COLOR_BLACK);

        do {
            static const arm_2d_location_t tSourceCentre = {110, 110};
            arm_2d_location_t tTargetCentre = { (tScreenSize.iWidth >> 1) + 150, 
                                                tScreenSize.iHeight >> 2};

            if (arm_2d_helper_time_liner_slider(0, 3600, 50 * 1000, &iResult, &this.lTimestamp[1])) {
                this.lTimestamp[1] = 0;
            }
            fAngle = ARM_2D_ANGLE((float)iResult / 10.0f);

            arm_2dp_fill_colour_with_mask_opacity_and_transform(&this.tTransformOP[0],
                                                                &c_tileInnerGearMask,
                                                                ptTile,
                                                                &__top_canvas,
                                                                tSourceCentre,
                                                                fAngle,
                                                                1.5f,
                                                                GLCD_COLOR_OLIVE,
                                                                180,
                                                                &tTargetCentre);
            arm_2d_op_wait_async((arm_2d_op_core_t *)&this.tTransformOP[0]);
        } while(0);

        do {
            static const arm_2d_location_t tSourceCentre = {90, 90};
            arm_2d_location_t tTargetCentre = { (tScreenSize.iWidth >> 1) + 100, 
                                                (tScreenSize.iHeight >> 2) + 60};

            if (arm_2d_helper_time_liner_slider(0, 3600,13 * 1000, &iResult, &this.lTimestamp[2])) {
                this.lTimestamp[2] = 0;
            }
            fAngle = ARM_2D_ANGLE((float)iResult / 10.0f);

            arm_2dp_fill_colour_with_mask_opacity_and_transform(&this.tTransformOP[1],
                                                                &c_tileInnerGearMidMask,
                                                                ptTile,
                                                                &__top_canvas,
                                                                tSourceCentre,
                                                                fAngle,
                                                                0.5f,
                                                                GLCD_COLOR_OLIVE,
                                                                200,
                                                                &tTargetCentre);

            arm_2d_op_wait_async((arm_2d_op_core_t *)&this.tTransformOP[1]);
        } while(0);


        arm_2d_align_top_centre(__top_canvas, tScreenSize.iWidth, tScreenSize.iHeight >> 1) {
        
            arm_2d_layout(__top_canvas) {
                __item_line_vertical(c_tileAudioPOVGRAY8.tRegion.tSize) {
                    
                    arm_2d_fill_colour_with_mask(   
                                                ptTile, 
                                                &__item_region, 
                                                &c_tileAudioPOVGRAY8, 
                                                (__arm_2d_color_t){GLCD_COLOR_WHITE});



                    arm_lcd_text_set_colour(GLCD_COLOR_WHITE, GLCD_COLOR_WHITE);
                    arm_lcd_print_banner("AudioMark", __item_region, &ARM_2D_FONT_ARIAL32_A4);

                    arm_2d_align_bottom_right(__item_region, 300, 70) {
                        arm_lcd_text_set_target_framebuffer(NULL);
                        arm_lcd_text_set_colour(GLCD_COLOR_GREEN, GLCD_COLOR_WHITE);
                        arm_lcd_text_set_font((const arm_2d_font_t *)(&ARM_2D_FONT_6x8));

                        
                        arm_lcd_text_set_draw_region(&__bottom_right_region);
                        arm_lcd_text_location(0,0);
                        arm_lcd_puts("https://www.eembc.org/audiomark/scores.php");
                    }

                }

                __item_line_vertical(450, 35, 15, 15, 0, 5) {
                    __draw_processor_list_item(ptThis, ptTile, &__item_region, AUDIOMARK_CORTEX_M4, bIsNewFrame);
                    
                    
                }

                __item_line_vertical(450, 35, 15, 15, 5, 5) {
                    __draw_processor_list_item(ptThis, ptTile, &__item_region, AUDIOMARK_CORTEX_M33, bIsNewFrame);
                }

                __item_line_vertical(450, 35, 15, 15, 5, 5) {
                    __draw_processor_list_item(ptThis, ptTile, &__item_region, AUDIOMARK_CORTEX_M7, bIsNewFrame);
                }

                __item_line_vertical(450, 35, 15, 15, 5, 5) {
                    __draw_processor_list_item(ptThis, ptTile, &__item_region, AUDIOMARK_CORTEX_M85_SCALER, bIsNewFrame);
                }

                __item_line_vertical(450, 35, 15, 15, 5, 5) {
                    __draw_processor_list_item(ptThis, ptTile, &__item_region, AUDIOMARK_CORTEX_M55_HELIUM, bIsNewFrame);
                }

                __item_line_vertical(450, 35, 15, 15, 5, 5) {
                    __draw_processor_list_item(ptThis, ptTile, &__item_region, AUDIOMARK_CORTEX_M85_HELIUM, bIsNewFrame);
                }

            }
        }


        arm_2d_align_bottom_centre(__top_canvas, tScreenSize.iWidth, tScreenSize.iHeight >> 1) {
            
            for (int_fast8_t n = 0; n < dimof(this.Processor); n++) {

                progress_wheel_show(&this.Processor[n].tWheel,
                    ptTile, 
                    &__bottom_centre_region,       
                    this.Processor[n].iProgress,
                    255,
                    bIsNewFrame);
            }

        #if __FITNESS_CFG_NEBULA_ENABLE__
            /* show nebula */
            dynamic_nebula_show(&this.tNebula, 
                                ptTile, 
                                &__bottom_centre_region, 
                                GLCD_COLOR_WHITE, 
                                255,
                                bIsNewFrame);
        #endif
        }

    /*-----------------------draw the foreground end  -----------------------*/
    }
    arm_2d_op_wait_async(NULL);

    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
user_scene_audiomark_t *__arm_2d_scene_audiomark_init(   arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_audiomark_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);

#if 0
    /*! define dirty regions */
    IMPL_ARM_2D_REGION_LIST(s_tDirtyRegions, static)

        /* a dirty region to be specified at runtime*/
        ADD_REGION_TO_LIST(s_tDirtyRegions,
            0  /* initialize at runtime later */
        ),
        
        /* add the last region:
         * it is the top left corner for text display 
         */
        ADD_LAST_REGION_TO_LIST(s_tDirtyRegions,
            .tLocation = {
                .iX = 0,
                .iY = 0,
            },
            .tSize = {
                .iWidth = 0,
                .iHeight = 8,
            },
        ),

    END_IMPL_ARM_2D_REGION_LIST(s_tDirtyRegions)

    s_tDirtyRegions[dimof(s_tDirtyRegions)-1].ptNext = NULL;

    /* get the screen region */
    arm_2d_region_t tScreen
        = arm_2d_helper_pfb_get_display_area(
            &ptDispAdapter->use_as__arm_2d_helper_pfb_t);
    
    /* initialise dirty region 0 at runtime
     * this demo shows that we create a region in the centre of a screen(320*240)
     * for a image stored in the tile c_tileCMSISLogoMask
     */
    arm_2d_align_centre(tScreen, c_tileCMSISLogoMask.tRegion.tSize) {
        s_tDirtyRegions[0].tRegion = __centre_region;
    }

    s_tDirtyRegions[dimof(s_tDirtyRegions)-1].tRegion.tSize.iWidth 
                                                        = tScreen.tSize.iWidth;
#endif

    if (NULL == ptThis) {
        ptThis = (user_scene_audiomark_t *)malloc(sizeof(user_scene_audiomark_t));
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }

    memset(ptThis, 0, sizeof(user_scene_audiomark_t));

    *ptThis = (user_scene_audiomark_t){
        .use_as__arm_2d_scene_t = {
            /* Please uncommon the callbacks if you need them
             */
            .fnScene        = &__pfb_draw_scene_audiomark_handler,
            //.ptDirtyRegion  = (arm_2d_region_list_item_t *)s_tDirtyRegions,
            

            //.fnOnBGStart    = &__on_scene_audiomark_background_start,
            //.fnOnBGComplete = &__on_scene_audiomark_background_complete,
            .fnOnFrameStart = &__on_scene_audiomark_frame_start,
            //.fnBeforeSwitchOut = &__before_scene_audiomark_switching_out,
            .fnOnFrameCPL   = &__on_scene_audiomark_frame_complete,
            .fnDepose       = &__on_scene_audiomark_depose,
        },
        .bUserAllocated = bUserAllocated,
    };

    /* ------------   initialize members of user_scene_audiomark_t begin ---------------*/

    for (uint_fast8_t n = 0; n < dimof(this.Processor); n++) {
        progress_wheel_init(&this.Processor[n].tWheel, 
                            c_tProcessorInfo[n].iWheelSize, 
                            c_tProcessorInfo[n].tColour);
        this.Processor[n].pchName = c_tProcessorInfo[n].pchName;
        this.Processor[n].iProgress = 0;
    }

    arm_foreach(arm_2d_op_fill_cl_msk_opa_trans_t, this.tTransformOP, ptOP) {
        ARM_2D_OP_INIT(*ptOP);
    }

#if __FITNESS_CFG_NEBULA_ENABLE__
    do {
        dynamic_nebula_cfg_t tCFG = {
            .fSpeed = 1.0f,
            .iRadius = 80,
            .iVisibleRingWidth = 80,
            .hwParticleCount = dimof(this.tParticles),
            .ptParticles = this.tParticles,
        };
        dynamic_nebula_init(&this.tNebula, &tCFG);
    } while(0);
#endif

    /* ------------   initialize members of user_scene_audiomark_t end   ---------------*/

    arm_2d_scene_player_append_scenes(  ptDispAdapter, 
                                        &this.use_as__arm_2d_scene_t, 
                                        1);

    return ptThis;
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


