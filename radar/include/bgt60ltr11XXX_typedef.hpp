/* MIT License
* 
* Copyright (c) 2024 Richard Kroesen <kroesen.ri@gmail.com>
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef BGTL60LTR11XXX_TYPEDEF_HPP
#define BGTL60LTR11XXX_TYPEDEF_HPP

#include <stdint.h>

typedef enum
{
	det_thrs_66		= 66,
	det_thrs_80		= 80,
	det_thrs_90		= 90,
	det_thrs_112	= 112,
	det_thrs_136	= 136,
	det_thrs_192	= 192,
	det_thrs_248	= 248,
	det_thrs_320	= 320,
	det_thrs_384	= 384,
	det_thrs_480	= 480,
	det_thrs_640	= 640,
	det_thrs_896	= 896,
	det_thrs_1344	= 1344,
	det_thrs_1920	= 1920,
	det_thrs_2560	= 2560
} Reg02_detector_threshold_level_t;

typedef enum
{
	dir_mode_1	= 0,
	dir_mode_2	= 1
} Reg02_direction_detection_mode_t;

typedef enum
{
	aprt_off	= 0,
	aprt_on		= 1
} Reg02_adaptive_pulse_repetition_time_t;

typedef enum
{
	hprt_off	= 0,
	hprt_on		= 1
} Reg02_high_pulse_repetition_time_t;

typedef enum
{
	pfd_rst_dly_175ps	= 0,
	pfd_rst_dly_275ps	= 1,
	pfd_rst_dly_375ps	= 2,
	pfd_rst_dly_470ps	= 3
} Reg04_pfd_rst_dly_select_t;

typedef enum
{
	bias_regulation_loop_active	= 0,
	bias_regulation_loop_off	= 1
} Reg04_pll_cp_mode_t;

typedef enum
{
	charge_pump_current_20uA	= 0,
	charge_pump_current_25uA	= 1,
	charge_pump_current_30uA	= 2,
	charge_pump_current_35uA	= 3,
	charge_pump_current_40uA	= 4,
	charge_pump_current_45uA	= 5,
	charge_pump_current_50uA	= 6,
	charge_pump_current_55uA	= 7
} Reg04_charge_pump_current_t;

typedef enum
{
	feedback_divider_counter_21dec	= 0,	/* 38.4 MHz */
	feedback_divider_counter_20dec	= 1		/* 40 MHz */
} Reg04_pll_feedback_divider_counter_t;

typedef enum
{
	xosc_mode_amplitude_1	= 0,
	xosc_mode_amplitude_2	= 1
} Reg04_pll_xosc_mode_t;

typedef enum
{
	loopfilter_R2_21_6Kohm	= 0,
	loopfilter_R2_18_7Kohm	= 1
} Reg04_pll_loopfilter_R2_t;

typedef enum
{
	loopfilter_R4_12_4Kohm	= 0,
	loopfilter_R4_0_1Kohm	= 1
} Reg04_pll_loopfilter_R4_t;

typedef enum
{
	open_loop_pusled_mode	= 0,
	closed_loop_pusled_mode	= 1
} Reg04_pll_close_loop_pusled_mode_t;

typedef enum
{
	loopfilter_isolation_with_charge_keeping_buffer_enabled	= 0,
	loopfilter_isolation_with_switches_only					= 1
} Reg04_pll_loopfilter_isolation_t;

typedef enum
{
	biassing_on = 0,
	biassing_off = 1
} Reg04_pll_bias_dis_t;

typedef enum
{
	mux_functional_mode				= 0,
	pll_lock						= 1,
	reference_clock_divided_by_4	= 2,
	divider_clock_divided_by_4		= 3
} Reg04_pll_dft_dmux_t;

typedef enum : uint16_t
{
	frequency_60_6Ghz	= 0xea2,
	frequency_60_7Ghz	= 0xecc,
	frequency_60_8Ghz	= 0xef5,
	frequency_60_9Ghz	= 0xf1f,
	frequency_61_1Ghz	= 0xf72,
	frequency_61_2Ghz	= 0xf9c,
	frequency_61_25Ghz	= 0xfb0,
	frequency_61_3Ghz	= 0xfc6,
	frequency_61_4Ghz	= 0xfef
} Reg05_pll_frequency_word_t;


typedef enum
{
	lock_detection_off	= 0,
	lock_detection_on	= 1
} Reg06_pll_lock_detect_en_t;

typedef enum
{
	lock_detection_24_clock	= 0,
	lock_detection_16_clock	= 1
} Reg06_pll_lock_detect_len_t;

typedef enum
{
	lock_detection_time_window_0_26ns	= 0,
	lock_detection_time_window_0_5ns	= 1,
	lock_detection_time_window_1ns		= 2,
	lock_detection_time_window_1_5ns	= 3,
	lock_detection_time_window_2ns		= 4,
	lock_detection_time_window_2_8ns	= 5,
	lock_detection_time_window_3_8ns	= 6,
	lock_detection_time_window_4_6ns	= 7
} Reg06_pll_lock_detect_time_window_t;

typedef enum
{
	mpa_minus_34dBm		= 0,
	mpa_minus_31_5dBm	= 1,
	mpa_minus_25dBm		= 2,
	mpa_minus_18dBm		= 3,
	mpa_minus_11dBm		= 4,
	mpa_minus_5dBm		= 5,
	mpa_0dBm			= 6,
	mpa_4_5dBm			= 7
} Reg07_mpa_ctrl_t;

typedef enum
{
	pd_off	= 0,
	pd_on	= 1
} Reg07_pd_en_t;

typedef enum
{
	map_enable_to_sample_hold_delay_500ns	= 0,
	map_enable_to_sample_hold_delay_1000ns	= 1,
	map_enable_to_sample_hold_delay_2000ns	= 2,
	map_enable_to_sample_hold_delay_4000ns	= 3
} Reg07_mpa_2sh_dly_t;

typedef enum
{
	vco_to_pll_delay_500ns	= 0,
	vco_to_pll_delay_1000ns	= 1
} Reg07_vco_to_pll_delay_t;

typedef enum
{
	duty_cycle_on_pulse_5us		= 0,
	duty_cycle_on_pulse_10us	= 1,
	duty_cycle_on_pulse_20us	= 2,
	duty_cycle_on_pulse_40us	= 3
} Reg07_duty_cycle_on_pulse_length_t;

typedef enum
{
	duty_cycle_rep_250us	= 0,
	duty_cycle_rep_500us	= 1,
	duty_cycle_rep_1000us	= 2,
	duty_cycle_rep_2000us	= 3
} Reg07_duty_cycle_repetition_rate_t;

typedef enum
{
	internal_9_6Mhz_clock_out	= 0,
	divider_select_2_power_13	= 1,
	divider_select_2_power_16	= 2,
	divider_select_2_power_20	= 3
} Reg08_divider_select_t;

typedef enum : uint16_t
{
	baseband_PGA_gain_10dB	= 0,
	baseband_PGA_gain_15dB	= 1,
	baseband_PGA_gain_20dB	= 2,
	baseband_PGA_gain_25dB	= 3,
	baseband_PGA_gain_30dB	= 4,
	baseband_PGA_gain_35dB	= 5,
	baseband_PGA_gain_40dB	= 6,
	baseband_PGA_gain_45dB	= 7,
	baseband_PGA_gain_50dB	= 8,
} Reg09_baseband_ctrl_t;

typedef enum
{
	low_pass_filter_10kHz	= 0,
	low_pass_filter_60kHz	= 1
} Reg09_low_pass_filter_t;

typedef enum
{
	clock_chop_freq_100kHz	= 0,
	clock_chop_freq_200kHz	= 1
} Reg09_bb_clock_chop_freq_t;

typedef enum
{
	high_pass_filter_resistor_8Mohm	= 0,
	high_pass_filter_resistor_4Mohm	= 1,
	high_pass_filter_resistor_2Mohm	= 2,
	high_pass_filter_resistor_1Mohm	= 3
} Reg09_high_pass_filter_resistor_t;

typedef enum
{
	hold_time_minimum	= 0,
	hold_time_512msec	= 4,
	hold_time_1_024sec	= 8,
	hold_time_2_048sec 	= 16,
	hold_time_3_072sec	= 24,
	hold_time_5_1sec 	= 40,
	hold_time_10_11sec 	= 79,
	hold_time_20_08sec 	= 235,
	hold_time_40_05sec 	= 352,
	hold_time_60_032sec = 469,
	hold_time_90_112sec = 704,
	hold_time_2min 		= 938,
	hold_time_5min 		= 2345,
	hold_time_10min 	= 4690,
	hold_time_15min 	= 7035,
	hold_time_30_016min = 14070
} Reg10_hold_time_t;

typedef enum
{
	detector_hit_counter_10		= 0,
	detector_hit_counter_20		= 1,
	detector_hit_counter_40		= 2,
	detector_hit_counter_80		= 3
} Reg11_detector_hit_counter_t;

typedef enum
{
	detector_hit_counter_reset_10ms		= 0,
	detector_hit_counter_reset_50ms		= 1,
	detector_hit_counter_reset_100ms	= 2,
	detector_hit_counter_reset_200ms	= 3
} Reg11_detector_hit_counter_reset_t;

typedef enum
{
	detector_direction_as_same_as_detector_hit_counter			= 0,
	detector_direction_as_same_as_half_of_detector_hit_counter	= 1
} Reg11_detector_direction_hit_counter_t;


typedef enum
{
	dir_pol_tdet_low_when_departing		= 0,
	dir_pol_tdet_low_when_approaching	= 1
} Reg15_direction_polarity_t;

typedef enum
{
	mot_pol_tdet_active_low		= 0,
	mot_pol_tdet_active_high	= 1
} Reg15_motion_polarity_t;

typedef enum
{
	miso_drv_high_z		= 0,
	miso_drv_some_level	= 1
} Reg15_spi_force_miso_driver_t;

typedef enum
{
	fast_phase_start_when_target_detected	= 0,
	fast_phase_start_before_target_detected	= 1
} Reg15_spi_fast_mode_t;

typedef enum
{
	fastmode_spi_miso_changes_rising_edge	= 0,
	fastmode_spi_miso_changes_falling_edge	= 1
} Reg15_faster_phase_evaluation_t;

typedef enum
{
	start_cw_inactive	= 0,
	start_cw_active		= 1
} Reg15_start_cw_mode_t;

typedef enum
{
	clk_ext_not_disabled	= 0,
	clk_ext_disabled		= 1
} Reg15_start_pulsed_mode_t;

typedef enum
{
	start_pm_inactive	= 0,
	start_pm_active		= 1
} Reg15_disable_external_clock_t;

typedef enum
{
	soft_reset_inactive	= 0,
	soft_reset_active	= 1
} Reg15_soft_reset_t;

typedef struct
{
	Reg05_pll_frequency_word_t			output_frequency;
	Reg07_mpa_ctrl_t 					output_power;
	Reg07_pd_en_t 						pd;
	Reg07_mpa_2sh_dly_t 				map_enable_to_sample_hold_delay;
	Reg07_vco_to_pll_delay_t 			vco_to_pll_delay;
	Reg07_duty_cycle_on_pulse_length_t 	pulse_on_time;
	Reg07_duty_cycle_repetition_rate_t 	pulse_repetition_time;
	Reg09_baseband_ctrl_t 				baseband_gain;
	Reg09_low_pass_filter_t 			low_pass_filter;
	Reg09_bb_clock_chop_freq_t 			clock_chop_freq;
	Reg09_high_pass_filter_resistor_t 	high_pass_filter_resistor;
} S2GO_RADAR_BGT60LTR11_Config_t;

#endif /* BGTL60LTR11XXX_TYPEDEF_HPP */