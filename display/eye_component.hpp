/*
 * Copyright 2024 JUNE - HAN University of Applied Sciences, Health Concept Lab
 * Project HealthBot
 * Authors: Victor Hogeweij, Marelle Vink and Richard Kroesen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef EYE_COMPONENT_HPP
#define EYE_COMPONENT_HPP

#include <lv_drv_conf.h>
#include <lvgl.h>
#include "lv_port_disp.h"
#include "ui.h"
#include "ui_helpers.h"
#include "lvgl.h"

/* @NOTE: This class is not elegant, and is generated from Squareline Studio based on the design of the S6-2324 Industrial Designers Groupd */

namespace DISPLAY {
class EyeComponent {
public:
    EyeComponent(lv_obj_t *parent, const lv_img_dsc_t *pupil_img, lv_img_dsc_t *iris_img, lv_img_dsc_t *sclera)
        : pupil_img(create_image(parent, pupil_img)),
        iris_img(create_image(parent, iris_img)),
        sclera_img(create_image(parent, sclera)), 
        anim_obj_confused(create_image(parent, &ui_img_vraagteken_png)),
        anim_obj_lid_top(create_image(parent, &ui_img_bovenkant_ooglid_png)),
        anim_obj_lid_bottom(create_image(parent, &ui_img_onderkant_ooglid_png)),
        anim_obj_thinking(lv_spinner_create(parent, 1000, 90)) {
            lv_obj_add_flag(anim_obj_thinking, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_width(anim_obj_thinking, 177);
            lv_obj_set_height(anim_obj_thinking, 179);
            lv_obj_set_align(anim_obj_thinking, LV_ALIGN_CENTER);
            lv_obj_set_style_arc_color(anim_obj_thinking, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_opa(anim_obj_thinking, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_add_flag(anim_obj_lid_top, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(anim_obj_lid_bottom, LV_OBJ_FLAG_HIDDEN);

            lv_obj_add_flag(anim_obj_confused, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_width(anim_obj_confused, LV_SIZE_CONTENT);   /// 26
            lv_obj_set_height(anim_obj_confused, LV_SIZE_CONTENT);    /// 39
            lv_obj_set_align(anim_obj_confused, LV_ALIGN_CENTER);
        }

    ~EyeComponent() {
        if (pupil_img) {
            lv_obj_del(pupil_img);
            pupil_img = nullptr;
        }
        if (iris_img) {
            lv_obj_del(iris_img);
            iris_img = nullptr;
        }
        if (sclera_img) {
            lv_obj_del(sclera_img);
            sclera_img = nullptr;
        }
        if (anim_obj_confused) {
            lv_obj_del(anim_obj_confused);
            anim_obj_confused = nullptr;
        }
        if (anim_obj_lid_top) {
            lv_obj_del(anim_obj_lid_top);
            anim_obj_lid_top = nullptr;
        }
        if (anim_obj_lid_bottom) {
            lv_obj_del(anim_obj_lid_bottom);
            anim_obj_lid_bottom = nullptr;
        }
        if (anim_obj_thinking) {
            lv_obj_del(anim_obj_thinking);
            anim_obj_thinking = nullptr;
        }
    }

    lv_obj_t* getPupil() {
        return pupil_img;
    }

    lv_obj_t* getThinking() {
        return anim_obj_thinking;
    }

    void animate_to_xy(lv_obj_t *obj, int target_x, int target_y, int duration) {
        if (obj == nullptr) return; // Safety check
        
        const int start_x = lv_obj_get_x_aligned(obj);
        lv_anim_t a_x;
        lv_anim_init(&a_x);
        lv_anim_set_var(&a_x, obj);
        lv_anim_set_exec_cb(&a_x, anim_x_cb);
        lv_anim_set_values(&a_x, start_x, target_x);
        lv_anim_set_time(&a_x, duration);
        lv_anim_set_path_cb(&a_x, lv_anim_path_linear);
        lv_anim_start(&a_x);

        const int start_y = lv_obj_get_y_aligned(obj);
        lv_anim_t a_y;
        lv_anim_init(&a_y);
        lv_anim_set_var(&a_y, obj);
        lv_anim_set_exec_cb(&a_y, anim_y_cb);
        lv_anim_set_values(&a_y, start_y, target_y);
        lv_anim_set_time(&a_y, duration);
        lv_anim_set_path_cb(&a_y, lv_anim_path_linear);
        lv_anim_start(&a_y);

        if (obj == pupil_img) {
            last_x_pos = target_x;
            last_y_pos = target_y;
        }
    }

    void animate_confused() {
        animate_to_xy(anim_obj_thinking, last_x_pos, last_y_pos, 0);
        anim_confused_expression(anim_obj_confused);
    }

    void animate_blink() {
        bovenkantooglid_Animation(anim_obj_lid_top, 0);
        Onderkantooglid_Animation(anim_obj_lid_bottom, 0);
    }

    void animate_thinking() {
        animate_to_xy(anim_obj_thinking, last_x_pos, last_y_pos, 0);
        anim_thinking_expression(anim_obj_thinking, 0);
    }

    void disable() {
        lv_obj_add_flag(pupil_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(iris_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(sclera_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(anim_obj_confused, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(anim_obj_lid_top, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(anim_obj_lid_bottom, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(anim_obj_thinking, LV_OBJ_FLAG_HIDDEN);
    }

private:
/* IMG Objects: */
    lv_obj_t* pupil_img;
    lv_obj_t* iris_img;
    lv_obj_t* sclera_img;

/* ANIMATION Objects: */
    lv_obj_t* anim_obj_confused;
    lv_obj_t* anim_obj_lid_top;
    lv_obj_t* anim_obj_lid_bottom;
    lv_obj_t* anim_obj_thinking;

/* Buffered Values*/
    uint8_t last_x_pos = 0;
    uint8_t last_y_pos = 0;

/* Class Functions: */
    static lv_obj_t* create_image(lv_obj_t* parent, const lv_img_dsc_t* src_img) {
        if (src_img == nullptr) return nullptr; // Safety check
        lv_obj_t *img = lv_img_create(parent);
        lv_img_set_src(img, src_img);
        lv_obj_center(img);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        return img;
    }

    static void anim_confused_expression(lv_obj_t * TargetObject, int delay = 0) {
        lv_obj_clear_flag(TargetObject, LV_OBJ_FLAG_HIDDEN);
        ui_anim_user_data_t * PropertyAnimation_0_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
        if (!PropertyAnimation_0_user_data) {
            printf("Failed to allocate memory for PropertyAnimation_0_user_data");
            return;
        }

        PropertyAnimation_0_user_data->target = TargetObject;
        PropertyAnimation_0_user_data->val = -1;
        lv_anim_t PropertyAnimation_0;
        lv_anim_init(&PropertyAnimation_0);
        lv_anim_set_time(&PropertyAnimation_0, 2500);
        lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
        lv_anim_set_values(&PropertyAnimation_0, 0, 255);
        lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);
        lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
        lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_0, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
        lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
        lv_anim_set_repeat_delay(&PropertyAnimation_0, 50000);
        lv_anim_set_early_apply(&PropertyAnimation_0, false);
        lv_anim_start(&PropertyAnimation_0);

        ui_anim_user_data_t * PropertyAnimation_1_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
        if (!PropertyAnimation_1_user_data) {
            printf("Failed to allocate memory for PropertyAnimation_1_user_data");
            return;
        }
        PropertyAnimation_1_user_data->target = TargetObject;
        PropertyAnimation_1_user_data->val = -1;

        lv_anim_t PropertyAnimation_1;
        lv_anim_init(&PropertyAnimation_1);
        lv_anim_set_time(&PropertyAnimation_1, 1000);
        lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_opacity);
        lv_anim_set_ready_cb(&PropertyAnimation_1, hide_obj_anim_ready);
        lv_anim_set_values(&PropertyAnimation_1, 255, 0);
        lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_ease_in);
        lv_anim_set_delay(&PropertyAnimation_1, delay + 5000);
        lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_1, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
        lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
        lv_anim_set_repeat_delay(&PropertyAnimation_1, 50000);
        lv_anim_set_early_apply(&PropertyAnimation_1, false);
        lv_anim_start(&PropertyAnimation_1);
    }

    static void Onderkantooglid_Animation(lv_obj_t * TargetObject, int delay) {
        lv_obj_clear_flag(TargetObject, LV_OBJ_FLAG_HIDDEN);
        ui_anim_user_data_t * PropertyAnimation_0_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
            if (!PropertyAnimation_0_user_data) {
            printf("Failed to allocate memory for PropertyAnimation_0_user_data");
            return;
        }
        PropertyAnimation_0_user_data->target = TargetObject;
        PropertyAnimation_0_user_data->val = -1;
        lv_anim_t PropertyAnimation_0;
        lv_anim_init(&PropertyAnimation_0);
        lv_anim_set_time(&PropertyAnimation_0, 50);
        lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
        lv_anim_set_values(&PropertyAnimation_0, 0, -180);
        lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in_out);
        lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
        lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_0, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
        lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
        lv_anim_set_repeat_delay(&PropertyAnimation_0, 50);
        lv_anim_set_early_apply(&PropertyAnimation_0, false);
        lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y);
        lv_anim_start(&PropertyAnimation_0);
        
        ui_anim_user_data_t * PropertyAnimation_1_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
        if (!PropertyAnimation_1_user_data) {
            printf("Failed to allocate memory for PropertyAnimation_1_user_data");
            return;
        }
        PropertyAnimation_1_user_data->target = TargetObject;
        PropertyAnimation_1_user_data->val = -1;
        lv_anim_t PropertyAnimation_1;
        lv_anim_init(&PropertyAnimation_1);
        lv_anim_set_time(&PropertyAnimation_1, 50);
        lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_y);
        lv_anim_set_values(&PropertyAnimation_1, 0, 180);
        lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_ease_in_out);
        lv_anim_set_delay(&PropertyAnimation_1, delay + 50);
        lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_1, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
        lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
        lv_anim_set_repeat_delay(&PropertyAnimation_1, 50);
        lv_anim_set_early_apply(&PropertyAnimation_1, false);
        lv_anim_start(&PropertyAnimation_1);
    }
    
    static void bovenkantooglid_Animation(lv_obj_t * TargetObject, int delay) {
        lv_obj_clear_flag(TargetObject, LV_OBJ_FLAG_HIDDEN);
        ui_anim_user_data_t * PropertyAnimation_0_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
        if (!PropertyAnimation_0_user_data) {
            LV_LOG_WARN("Failed to allocate memory for PropertyAnimation_0_user_data");
            return;
        }
        PropertyAnimation_0_user_data->target = TargetObject;
        PropertyAnimation_0_user_data->val = -1;
        lv_anim_t PropertyAnimation_0;
        lv_anim_init(&PropertyAnimation_0);
        lv_anim_set_time(&PropertyAnimation_0, 50);
        lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
        lv_anim_set_values(&PropertyAnimation_0, 0, 180);
        lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in_out);
        lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
        lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_0, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
        lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
        lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
        lv_anim_set_early_apply(&PropertyAnimation_0, false);
        lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y);
        lv_anim_start(&PropertyAnimation_0);

        ui_anim_user_data_t * PropertyAnimation_1_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
        if (!PropertyAnimation_1_user_data) {
            LV_LOG_WARN("Failed to allocate memory for PropertyAnimation_1_user_data");
            return;
        }
        PropertyAnimation_1_user_data->target = TargetObject;
        PropertyAnimation_1_user_data->val = -1;
        lv_anim_t PropertyAnimation_1;
        lv_anim_init(&PropertyAnimation_1);
        lv_anim_set_time(&PropertyAnimation_1, 50);
        lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_y);
        lv_anim_set_values(&PropertyAnimation_1, 0, -180);
        lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_ease_in_out);
        lv_anim_set_delay(&PropertyAnimation_1, delay + 50);
        lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_1, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
        lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
        lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
        lv_anim_set_early_apply(&PropertyAnimation_1, false);
        lv_anim_start(&PropertyAnimation_1);
    }

    static void anim_thinking_expression(lv_obj_t * TargetObject, int delay) {
        lv_obj_clear_flag(TargetObject, LV_OBJ_FLAG_HIDDEN);
        ui_anim_user_data_t * PropertyAnimation_0_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
        if (!PropertyAnimation_0_user_data) {
            LV_LOG_WARN("Failed to allocate memory for PropertyAnimation_0_user_data");
            return;
        }
        PropertyAnimation_0_user_data->target = TargetObject;
        PropertyAnimation_0_user_data->val = -1;
        lv_anim_t PropertyAnimation_0;
        lv_anim_init(&PropertyAnimation_0);
        lv_anim_set_time(&PropertyAnimation_0, 100);
        lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
        lv_anim_set_values(&PropertyAnimation_0, 0, 255);
        lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);
        lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
        lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_0, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
        lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
        // lv_anim_set_repeat_delay(&PropertyAnimation_0, 50000);
        lv_anim_set_early_apply(&PropertyAnimation_0, false);
        lv_anim_start(&PropertyAnimation_0);

        /* ROTARY OPACITY */
        ui_anim_user_data_t * PropertyAnimation_1_user_data = (ui_anim_user_data_t *)lv_mem_alloc(sizeof(ui_anim_user_data_t));
        if (!PropertyAnimation_1_user_data) {
            LV_LOG_WARN("Failed to allocate memory for PropertyAnimation_1_user_data");
            return;
        }

        PropertyAnimation_1_user_data->target = TargetObject;
        PropertyAnimation_1_user_data->val = -1;
        lv_anim_t PropertyAnimation_1;
        lv_anim_init(&PropertyAnimation_1);
        lv_anim_set_time(&PropertyAnimation_1, 10);
        lv_anim_set_user_data(&PropertyAnimation_1, PropertyAnimation_1_user_data);
        lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_opacity);
        lv_anim_set_values(&PropertyAnimation_1, 255, 0);
        lv_anim_set_path_cb(&PropertyAnimation_1, lv_anim_path_ease_in);
        lv_anim_set_delay(&PropertyAnimation_1, delay + 5000);
        lv_anim_set_deleted_cb(&PropertyAnimation_1, _ui_anim_callback_free_user_data);
        lv_anim_set_playback_time(&PropertyAnimation_1, 0);
        lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
        // lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
        // lv_anim_set_repeat_delay(&PropertyAnimation_1, 50000);
        lv_anim_set_early_apply(&PropertyAnimation_1, false);
        lv_anim_start(&PropertyAnimation_1);

    }

/* Helper Functions: */
    static void hide_obj_anim_ready(lv_anim_t *anim) {
        auto *user_data = static_cast<ui_anim_user_data_t *>(lv_anim_get_user_data(anim));
        lv_obj_add_flag(user_data->target, LV_OBJ_FLAG_HIDDEN);
    }


    static void anim_x_cb(void *var, const int32_t v) {
        lv_obj_set_x((lv_obj_t*)var, v);
    }

    static void anim_y_cb(void *var, const int32_t v) {
        lv_obj_set_y((lv_obj_t*)var, v);
    }  

};
    
} // namespace DISPLAY
#endif // EYE_COMPONENT_HPP