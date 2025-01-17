/****************************************************************************
*  Copyright 2023 Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)       *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

/*============================ INCLUDES ======================================*/

#include "arm_2d.h"

#define __USER_SCENE_GENSHIN_CLOCK_IMPLEMENT__
#include "arm_2d_scene_genshin_clock.h"

#include "arm_2d_helper.h"

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
#elif __IS_COMPILER_IAR__
#   pragma diag_suppress=Pa089,Pe188,Pe177,Pe174
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#   pragma GCC diagnostic ignored "-Wunused-value"
#endif

/*============================ MACROS ========================================*/

#if __GLCD_CFG_COLOUR_DEPTH__ == 8
#   define c_tileGenshinBackground   c_tileGenshinBackgroundGRAY8

#elif __GLCD_CFG_COLOUR_DEPTH__ == 16
#   define c_tileGenshinBackground   c_tileGenshinBackgroundRGB565

#elif __GLCD_CFG_COLOUR_DEPTH__ == 32
#   define c_tileGenshinBackground   c_tileGenshinBackgroundCCCN888

#else
#   error Unsupported colour depth!
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#undef this
#define this (*ptThis)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

extern const arm_2d_tile_t c_tileGenshinClockPanelMask;
extern const arm_2d_tile_t c_tileGenshinBackground;
extern const arm_2d_tile_t c_tileClockGradientPanelMask;
extern const arm_2d_tile_t c_tileCircleBackgroundMask;
extern const arm_2d_tile_t c_tileClockInnerPanelMask;
extern const arm_2d_tile_t c_tileInnerGearMask;
extern const arm_2d_tile_t c_tileInnerGearMidMask;
extern const arm_2d_tile_t c_tileGenshinPointerMask;

/*============================ PROTOTYPES ====================================*/
static 
IMPL_PFB_ON_DRAW(__pfb_draw_clock_inner_panel);

/*============================ LOCAL VARIABLES ===============================*/

dcl_fb(s_tileInnerPanel);

/*============================ IMPLEMENTATION ================================*/ 
impl_fb(s_tileInnerPanel, 220, 220, __arm_2d_color_t);

static void __on_scene_genshin_clock_depose(arm_2d_scene_t *ptScene)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    ptScene->ptPlayer = NULL;
    
    arm_foreach(int64_t,this.lTimestamp, ptItem) {
        *ptItem = 0;
    }

    arm_foreach(arm_2d_op_fill_cl_msk_opa_trans_t, this.tTransformOP, ptOP) {
        ARM_2D_OP_DEPOSE(*ptOP);
    }

    if (!this.bUserAllocated) {
        free(ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene genshin_clock                                                        *
 *----------------------------------------------------------------------------*/

static void __on_scene_genshin_clock_background_start(arm_2d_scene_t *ptScene)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene_genshin_clock_background_complete(arm_2d_scene_t *ptScene)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}


static void __on_scene_genshin_clock_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    __pfb_draw_clock_inner_panel(ptThis, &s_tileInnerPanel, true);

}

static void __on_scene_genshin_clock_frame_complete(arm_2d_scene_t *ptScene)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
}

static void __before_scene_genshin_clock_switching_out(arm_2d_scene_t *ptScene)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static 
IMPL_PFB_ON_DRAW(__pfb_draw_clock_inner_panel)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)pTarget;

    int32_t iResult;
    float fAngle;


    arm_2d_canvas(ptTile, __top_canvas) {
        
        arm_2d_align_centre(__top_canvas, 220, 220) {

            /* we draw everything in a 220 * 220 region */
            arm_2d_container(ptTile, __inner_panel, &__centre_region) {

                arm_2d_fill_colour(&__inner_panel, NULL, GLCD_COLOR_BLACK);

                dynamic_nebula_show(&this.tNebula,
                                    &__inner_panel,
                                    &__inner_panel_canvas,
                                    GLCD_COLOR_WHITE,
                                    255,
                                    bIsNewFrame);

                do {
                    static const arm_2d_location_t tSourceCentre = {110, 110};
                    static const arm_2d_location_t tTargetCentre = {150, 90};

                    if (arm_2d_helper_time_liner_slider(0, 3600, 50 * 1000, &iResult, &this.lTimestamp[TRANSFORM_INNER_GEAR_0])) {
                        this.lTimestamp[TRANSFORM_INNER_GEAR_0] = 0;
                    }
                    fAngle = ARM_2D_ANGLE((float)iResult / 10.0f);

                    arm_2dp_fill_colour_with_mask_opacity_and_transform(&this.tTransformOP[TRANSFORM_INNER_GEAR_0],
                                                                        &c_tileInnerGearMask,
                                                                        &__inner_panel,
                                                                        &__inner_panel_canvas,
                                                                        tSourceCentre,
                                                                        fAngle,
                                                                        1.0f,
                                                                        GLCD_COLOR_OLIVE,
                                                                        256-32,
                                                                        &tTargetCentre);
                } while(0);

                
                do {
                    static const arm_2d_location_t tSourceCentre = {90, 90};
                    static const arm_2d_location_t tTargetCentre = {80, 140};

                    if (arm_2d_helper_time_liner_slider(0, 3600,40 * 1000, &iResult, &this.lTimestamp[TRANSFORM_INNER_GEAR_1])) {
                        this.lTimestamp[TRANSFORM_INNER_GEAR_1] = 0;
                    }
                    fAngle = ARM_2D_ANGLE((float)iResult / 10.0f);

                    arm_2dp_fill_colour_with_mask_opacity_and_transform(&this.tTransformOP[TRANSFORM_INNER_GEAR_1],
                                                                        &c_tileInnerGearMidMask,
                                                                        &__inner_panel,
                                                                        &__inner_panel_canvas,
                                                                        tSourceCentre,
                                                                        fAngle,
                                                                        1.0f,
                                                                        GLCD_COLOR_OLIVE,
                                                                        256-64,
                                                                        &tTargetCentre);
                } while(0);

                do {
                    static const arm_2d_location_t tSourceCentre = {90, 90};

                    if (arm_2d_helper_time_liner_slider(0, 3600,13 * 1000, &iResult, &this.lTimestamp[TRANSFORM_INNER_GEAR_2])) {
                        this.lTimestamp[TRANSFORM_INNER_GEAR_2] = 0;
                    }
                    fAngle = ARM_2D_ANGLE((float)iResult / 10.0f);

                    arm_2dp_fill_colour_with_mask_opacity_and_transform(&this.tTransformOP[TRANSFORM_INNER_GEAR_2],
                                                                        &c_tileInnerGearMidMask,
                                                                        &__inner_panel,
                                                                        &__inner_panel_canvas,
                                                                        tSourceCentre,
                                                                        fAngle,
                                                                        0.3f,
                                                                        GLCD_COLOR_OLIVE,
                                                                        256-64);
                } while(0);

                do {
                    static const arm_2d_location_t tSourceCentre = {90, 90};
                    static const arm_2d_location_t tTargetCentre = {200, 200};

                    if (arm_2d_helper_time_liner_slider(0, 3600,20 * 1000, &iResult, &this.lTimestamp[TRANSFORM_INNER_GEAR_3])) {
                        this.lTimestamp[TRANSFORM_INNER_GEAR_3] = 0;
                    }
                    fAngle = ARM_2D_ANGLE((float)iResult / 10.0f);

                    arm_2dp_fill_colour_with_mask_opacity_and_transform(&this.tTransformOP[TRANSFORM_INNER_GEAR_3],
                                                                        &c_tileInnerGearMidMask,
                                                                        &__inner_panel,
                                                                        &__inner_panel_canvas,
                                                                        tSourceCentre,
                                                                        -fAngle,
                                                                        0.5f,
                                                                        GLCD_COLOR_OLIVE,
                                                                        256-128,
                                                                        &tTargetCentre);
                } while(0);
            }
        }
    }

    arm_2d_op_wait_async(NULL);

    return arm_fsm_rt_cpl;

}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene_genshin_clock_handler)
{
    user_scene_genshin_clock_t *ptThis = (user_scene_genshin_clock_t *)pTarget;

    int32_t iResult;
    float fAngle;

    arm_2d_canvas(ptTile, __top_canvas) {
    /*-----------------------draw the foreground begin-----------------------*/
        
        arm_2d_fill_colour(ptTile, &__top_canvas, GLCD_COLOR_BLACK);

        /* draw background */
        arm_using(arm_2d_region_t __background_canvas = __top_canvas) {
            if (bIsNewFrame) {

                arm_2d_helper_time_cos_slider(  0, 
                                                c_tileGenshinBackground.tRegion.tSize.iWidth - __top_canvas.tSize.iWidth, 
                                                10 * 1000, 
                                                0,
                                                &iResult, 
                                                &this.lTimestamp[TIMER_BACKGROUND_X]);

                this.tBackgroundOffset.iX = iResult;

                arm_2d_helper_time_cos_slider(  0, 
                                                c_tileGenshinBackground.tRegion.tSize.iHeight - __top_canvas.tSize.iHeight, 
                                                10 * 1000, 
                                                ARM_2D_ANGLE(90.0f),
                                                &iResult, 
                                                &this.lTimestamp[TIMER_BACKGROUND_Y]);

                this.tBackgroundOffset.iY = iResult;

            }

            __background_canvas.tLocation.iX -= this.tBackgroundOffset.iX;
            __background_canvas.tLocation.iY -= this.tBackgroundOffset.iY;
            __background_canvas.tSize = c_tileGenshinBackground.tRegion.tSize;

            arm_2d_tile_copy_only(  &c_tileGenshinBackground, 
                                    ptTile,
                                    &__background_canvas);
            
            ARM_2D_OP_WAIT_ASYNC();

        }

        arm_2d_layout(__top_canvas, RIGHT_TO_LEFT) {

            __item_line_dock_horizontal(c_tileGenshinClockPanelMask.tRegion.tSize.iWidth) {

                arm_2d_fill_colour_with_opacity(
                    ptTile,
                    &__item_region,
                    (__arm_2d_color_t){GLCD_COLOR_BLACK},
                    128);
                
                ARM_2D_OP_WAIT_ASYNC();

                arm_2d_align_centre(__item_region, c_tileClockInnerPanelMask.tRegion.tSize) {
                        arm_2d_tile_copy_with_src_mask_only(&s_tileInnerPanel, 
                                                            &c_tileClockInnerPanelMask,
                                                            ptTile, 
                                                            &__centre_region);
                }
                
                ARM_2D_OP_WAIT_ASYNC();

                arm_2d_fill_colour_with_mask_and_opacity(   
                                                    ptTile, 
                                                    &__item_region, 
                                                    &c_tileGenshinClockPanelMask, 
                                                    (__arm_2d_color_t){__RGB(255, 255, 128)},
                                                    256 - 64);

                ARM_2D_OP_WAIT_ASYNC();

                arm_2d_align_centre(__item_region, c_tileClockInnerPanelMask.tRegion.tSize) {
                    static const arm_2d_location_t tSourceCentre = {17, 90};

                    if (arm_2d_helper_time_liner_slider(0, 3600, 60 * 1000, &iResult, &this.lTimestamp[TRANSFORM_POINTER_0])) {
                        this.lTimestamp[TRANSFORM_POINTER_0] = 0;
                    }
                    fAngle = ARM_2D_ANGLE((float)iResult / 10.0f);

                    arm_2dp_fill_colour_with_mask_opacity_and_transform(&this.tTransformOP[TRANSFORM_POINTER_0],
                                                                        &c_tileGenshinPointerMask,
                                                                        ptTile,
                                                                        &__centre_region,
                                                                        tSourceCentre,
                                                                        fAngle,
                                                                        1.0f,
                                                                        __RGB(255, 255, 128),
                                                                        255);
                }
            }

            __item_line_dock_horizontal(64) {

                arm_2d_fill_colour_with_horizontal_alpha_gradient(
                    ptTile,
                    &__item_region,
                    (__arm_2d_color_t){GLCD_COLOR_BLACK}, 
                    (arm_2d_alpha_samples_2pts_t) {
                        {0, 128},
                    });

            }
        }

    /*-----------------------draw the foreground end  -----------------------*/
    }
    ARM_2D_OP_WAIT_ASYNC();

    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
user_scene_genshin_clock_t *__arm_2d_scene_genshin_clock_init(   
                                        arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_genshin_clock_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);
    
    /* get the screen region */
    arm_2d_region_t tScreen
        = arm_2d_helper_pfb_get_display_area(
            &ptDispAdapter->use_as__arm_2d_helper_pfb_t);
    
    if (NULL == ptThis) {
        ptThis = (user_scene_genshin_clock_t *)malloc(sizeof(user_scene_genshin_clock_t));
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }

    memset(ptThis, 0, sizeof(user_scene_genshin_clock_t));

    *ptThis = (user_scene_genshin_clock_t){
        .use_as__arm_2d_scene_t = {
            /* Please uncommon the callbacks if you need them
             */
            .fnScene        = &__pfb_draw_scene_genshin_clock_handler,
            
            //.fnOnBGStart    = &__on_scene_genshin_clock_background_start,
            //.fnOnBGComplete = &__on_scene_genshin_clock_background_complete,
            .fnOnFrameStart = &__on_scene_genshin_clock_frame_start,
            //.fnBeforeSwitchOut = &__before_scene_genshin_clock_switching_out,
            .fnOnFrameCPL   = &__on_scene_genshin_clock_frame_complete,
            .fnDepose       = &__on_scene_genshin_clock_depose,
        },
        .bUserAllocated = bUserAllocated,
    };

    arm_foreach(arm_2d_op_fill_cl_msk_opa_trans_t, this.tTransformOP, ptOP) {
        ARM_2D_OP_INIT(*ptOP);
    }

    do {

        dynamic_nebula_cfg_t tCFG = {
            .fSpeed = 0.3f,
            .iRadius = 110,
            .iVisibleRingWidth = 50,
            .hwParticleCount = dimof(this.tParticles),
            .ptParticles = this.tParticles,
        };

        dynamic_nebula_init(&this.tNebula, &tCFG);
    } while(0);

    arm_2d_scene_player_append_scenes(  ptDispAdapter, 
                                        &this.use_as__arm_2d_scene_t, 
                                        1);

    return ptThis;
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


