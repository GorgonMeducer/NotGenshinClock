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

#ifndef __ARM_2D_SCENE_GENSHIN_CLOCK_H__
#define __ARM_2D_SCENE_GENSHIN_CLOCK_H__

/*============================ INCLUDES ======================================*/

#include "arm_2d.h"

#include "arm_2d_helper_scene.h"
#include "arm_2d_example_controls.h"

#ifdef   __cplusplus
extern "C" {
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wpadded"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wpadded"
#endif

/*============================ MACROS ========================================*/

/* OOC header, please DO NOT modify  */
#ifdef __USER_SCENE_GENSHIN_CLOCK_IMPLEMENT__
#   undef __USER_SCENE_GENSHIN_CLOCK_IMPLEMENT__
#   define __ARM_2D_IMPL__
#endif
#include "arm_2d_utils.h"

/*============================ MACROFIED FUNCTIONS ===========================*/

/*!
 * \brief initalize scene_genshin_clock and add it to a user specified scene player
 * \param[in] __DISP_ADAPTER_PTR the target display adatper (i.e. scene player)
 * \param[in] ... this is an optional parameter. When it is NULL, a new 
 *            user_scene_genshin_clock_t will be allocated from HEAP and freed on
 *            the deposing event. When it is non-NULL, the life-cycle is managed
 *            by user.
 * \return user_scene_genshin_clock_t* the user_scene_genshin_clock_t instance
 */
#define arm_2d_scene_genshin_clock_init(__DISP_ADAPTER_PTR, ...)                    \
            __arm_2d_scene_genshin_clock_init((__DISP_ADAPTER_PTR), (NULL, ##__VA_ARGS__))

/*============================ TYPES =========================================*/

enum {

    TRANSFORM_INNER_GEAR_0 = 0,
    TRANSFORM_INNER_GEAR_1,
    TRANSFORM_INNER_GEAR_2,
    TRANSFORM_INNER_GEAR_3,
    TRANSFORM_POINTER_0,

    TIMER_BACKGROUND_X,
    TIMER_BACKGROUND_Y,
};

/*!
 * \brief a user class for scene genshin_clock
 */
typedef struct user_scene_genshin_clock_t user_scene_genshin_clock_t;

struct user_scene_genshin_clock_t {
    implement(arm_2d_scene_t);                                                  //! derived from class: arm_2d_scene_t

ARM_PRIVATE(
    /* place your private member here, following two are examples */
    int64_t lTimestamp[7];
    arm_2d_location_t tBackgroundOffset;
    dynamic_nebula_t tNebula;
    dynamic_nebula_particle_t tParticles[200];
    bool bUserAllocated;
    
    arm_2d_op_fill_cl_msk_opa_trans_t tTransformOP[5];

)
    /* place your public member here */
    
};

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

ARM_NONNULL(1)
extern
user_scene_genshin_clock_t *__arm_2d_scene_genshin_clock_init(   arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_genshin_clock_t *ptScene);

#if defined(__clang__)
#   pragma clang diagnostic pop
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

#endif
