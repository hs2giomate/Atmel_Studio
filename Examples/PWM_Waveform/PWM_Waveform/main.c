/**
 * \file
 *
 * \brief PWM Waveform Generation Application
 *
 * Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <atmel_start.h>
#include <pwm.h>

/* Macro Definitions   */
#define TCC_DEFAULT_CONFIGURATION PWM_0_init()
#define DEAD_TIME_CHANNEL(n) (1 << n)
#define TCC_PATTERN_PAGE_VAL 0x0F
#define PATTERN_SIZE 4
#define COMPARE_CHANNEL_0 0
#define COMPARE_CHANNEL_1 1
#define COMPARE_CHANNEL_2 2
#define COMPARE_CHANNEL_3 3
#define TCC_EXAMPLES                                                                                                   \
	"************************* TCC PWM Examples ******************\r\n\n"                                              \
	"1. 1KHz PWM Waveform using TCC single slope PWM mode \r\n"                                                        \
	"2. PWM Waveform using TCC Circular buffer feature \r\n"                                                           \
	"3. PWM Waveform using TCC dead time insertion feature \r\n"                                                       \
	"4. Swap PWM Waveforms using TCC swap feature \r\n"                                                                \
	"5. Invert PWM Waveforms using TCC output Invert feature \r\n"                                                     \
	"6. Bipolar Stepper Motor output Waveform pattern generation \r\n"                                                 \
	"7. PWM Waveform using TCC RAMP2 feature \r\n"                                                                     \
	"8. PWM Waveform using TCC RAMP2A feature \r\n"                                                                    \
	"9. PWM Waveform using using Dual slope PWM mode \r\n\n"

/* TCC Period and Compare register value
 * Calculation with TCC Clock
 * of 12MHz and Prescaler of 2
 **/
#define VALUE_FOR_2ms 0x2EE0
#define VALUE_FOR_1ms 0x1770
#define VALUE_FOR_750us 0x1194
#define VALUE_FOR_500us 0xBB8
#define VALUE_FOR_250us 0x5DC
#define DEAD_TIME_10us 0x78
#define OTMX_SETTING_0 0x00
#define OTMX_SETTING_2 0x00
#define DEAD_TIME_CHANNEL_012                                                                                          \
	DEAD_TIME_CHANNEL(COMPARE_CHANNEL_0)                                                                               \
	| DEAD_TIME_CHANNEL(COMPARE_CHANNEL_1) | DEAD_TIME_CHANNEL(COMPARE_CHANNEL_2)

/* Global Variables Definitions  */
uint32_t example_selection       = 1;
bool volatile swap_flag          = false;
bool volatile inv_flag           = false;
uint8_t sm_pattern[PATTERN_SIZE] = {8, 2, 4, 1};
uint8_t iIndex                   = 0;

/************************************************************************/
/*          2KHz PWM waveform using TCC single slope PWM mode           */
/************************************************************************/
void pwm_1khz_example()
{
	printf("1KHz PWM Waveform output available on WO[0] PIN\r\n");
	tcc_disable(TCC0);
	tcc_set_compare_value(TCC0, VALUE_FOR_500us, COMPARE_CHANNEL_0);
	tcc_enable(TCC0);
}
/************************************************************************/
/*                 Circular Buffer feature of TCC                       */
/************************************************************************/
void circular_buffer_example()
{
	printf("Circular Buffer PWM Waveform output available on WO[0] PIN\r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_enable_circular_buffer_compare(TCC0, COMPARE_CHANNEL_0);
	tcc_set_compare_value(TCC0, VALUE_FOR_500us, COMPARE_CHANNEL_0);
	tcc_set_compare_buffer_value(TCC0, VALUE_FOR_250us, COMPARE_CHANNEL_0);
	tcc_enable(TCC0);
}

/************************************************************************/
/*                    Dead Lock feature of TCC                          */
/************************************************************************/
void dead_time_insertion_example()
{
	printf("Dead Time inserted PWM Waveform output available on WO[0], WO[1], WO[2], WO[4], WO[5] and WO[6] PINs\r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_set_compare_value(TCC0, VALUE_FOR_250us, COMPARE_CHANNEL_0);
	tcc_set_compare_value(TCC0, VALUE_FOR_250us, COMPARE_CHANNEL_1);
	tcc_set_compare_value(TCC0, VALUE_FOR_250us, COMPARE_CHANNEL_2);
	tcc_set_dead_time(TCC0, DEAD_TIME_CHANNEL_012, OTMX_SETTING_0, DEAD_TIME_10us, DEAD_TIME_10us);
	tcc_enable(TCC0);
}

/************************************************************************/
/*             Swap PWM Waveform using Swap feature of TCC              */
/************************************************************************/
static void swap_waveform_on_SW0_press(void)
{
	if (swap_flag) {
		tcc_disable_channel_swap(TCC0, 0);
		swap_flag = false;
	} else {
		tcc_enable_channel_swap(TCC0, 0);
		swap_flag = true;
	}
}

void tcc_swap_feature_example()
{
	printf("PWM Waveform output available on WO[0] and WO[4] PIN\r\n");
	printf("Press SW0 Button on XPlained PRO Board for swapping PWM Waveform between WO[0] and WO[4] PINs\r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_set_compare_value(TCC0, VALUE_FOR_250us, COMPARE_CHANNEL_0);
	tcc_set_dead_time(TCC0, DEAD_TIME_CHANNEL(COMPARE_CHANNEL_0), OTMX_SETTING_0, DEAD_TIME_10us, DEAD_TIME_10us);
	tcc_enable(TCC0);
	ext_irq_register(SW0, swap_waveform_on_SW0_press);
}

/************************************************************************/
/*          Invert PWM Waveform using Inversion feature of TCC          */
/************************************************************************/
static void invert_waveform_on_SW0_press(void)
{
	tcc_disable(TCC0);
	if (inv_flag) {
		tcc_disable_channel_inversion(TCC0, COMPARE_CHANNEL_0);
		inv_flag = false;
	} else {
		tcc_enable_channel_inversion(TCC0, COMPARE_CHANNEL_0);
		inv_flag = true;
	}
	tcc_enable(TCC0);
}

void tcc_invert_feature_example()
{
	printf("PWM Waveform output available on WO[0] PIN\r\n");
	printf("Press SW0 Button on XPlained PRO Board for inverting PWM Waveform \r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_set_compare_value(TCC0, VALUE_FOR_250us, COMPARE_CHANNEL_0);
	tcc_enable(TCC0);
	ext_irq_register(SW0, invert_waveform_on_SW0_press);
}

/************************************************************************/
/*   PWM Waveform Pattern Generation using Pattern Generation of TCC    */
/*            Bipolar Stepper Motor Output Waveform Pattern             */
/************************************************************************/
void pattern_generation(void)
{
	if (iIndex == PATTERN_SIZE) {
		iIndex = 0;
	}
	tcc_set_pattern_buffer(TCC0, TCC_PATTERN_PAGE_VAL, sm_pattern[iIndex++]);
}

void tcc_pattern_generation_example()
{
	printf("TCC Pattern Generation PWM Waveform output available on WO[0], WO[1], WO[2] and WO[3] PINs\r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_set_compare_value(TCC0, VALUE_FOR_500us, COMPARE_CHANNEL_0);
	tcc_set_otmx_mode(TCC0, OTMX_SETTING_2);
	tcc_set_pattern(TCC0, TCC_PATTERN_PAGE_VAL, sm_pattern[iIndex++]);
	tcc_enable_interrupt(TCC0, TCC_INTENSET_MC0, pattern_generation);
	NVIC_EnableIRQ((IRQn_Type)TCC0_1_IRQn);
	tcc_enable(TCC0);
}

/************************************************************************/
/*               PWM Waveform using RAMP2 feature of TCC                */
/************************************************************************/
void tcc_ramp2_feature_example()
{
	printf("TCC RAMP2 feature PWM Waveform Output available on WO[0] and WO[1] PINs\r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_set_compare_value(TCC0, VALUE_FOR_500us, COMPARE_CHANNEL_0);
	tcc_set_compare_value(TCC0, VALUE_FOR_1ms, COMPARE_CHANNEL_1);
	tcc_set_ramp_operation(TCC0, TCC_WAVE_RAMP_RAMP2_Val);
	tcc_enable_circular_buffer_period(TCC0);
	tcc_set_period_buffer_value(TCC0, VALUE_FOR_2ms);
	tcc_enable(TCC0);
}

/************************************************************************/
/*              PWM Waveform using RAMP2A feature of TCC                */
/************************************************************************/
void tcc_ramp2a_feature_example()
{
	printf("TCC RAMP2A feature PWM Waveform Output available on WO[0] and WO[1] PINs\r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_set_compare_value(TCC0, VALUE_FOR_500us, COMPARE_CHANNEL_0);
	tcc_set_ramp_operation(TCC0, TCC_WAVE_RAMP_RAMP2A_Val);
	tcc_enable_circular_buffer_compare(TCC0, COMPARE_CHANNEL_0);
	tcc_enable_circular_buffer_period(TCC0);
	tcc_set_compare_buffer_value(TCC0, VALUE_FOR_1ms, COMPARE_CHANNEL_0);
	tcc_set_period_buffer_value(TCC0, VALUE_FOR_2ms);
	tcc_enable(TCC0);
}

/************************************************************************/
/*      PWM Waveform using Dual Slope PWM Operation feature of TCC      */
/************************************************************************/
void tcc_pwm_dual_slope_mode_example()
{
	printf("TCC Dual Slope Center Aligned PWM Waveform Output available on WO[0], WO[1] and WO[2] PINs\r\n");
	tcc_disable(TCC0);
	tcc_set_period_value(TCC0, VALUE_FOR_1ms);
	tcc_set_compare_value(TCC0, VALUE_FOR_250us, COMPARE_CHANNEL_0);
	tcc_set_compare_value(TCC0, VALUE_FOR_500us, COMPARE_CHANNEL_1);
	tcc_set_compare_value(TCC0, VALUE_FOR_750us, COMPARE_CHANNEL_2);
	tcc_set_wavegen_mode(TCC0, TCC_WAVE_WAVEGEN_DSTOP_Val);
	tcc_enable(TCC0);
}

/************************************************************************/
/*      PWM Waveform using Dual Slope PWM Operation feature of TCC      */
/************************************************************************/
void tcc_examples()
{
	TCC_DEFAULT_CONFIGURATION;
	switch (example_selection) {
	case 1:
		pwm_1khz_example();
		break;
	case 2:
		circular_buffer_example();
		break;
	case 3:
		dead_time_insertion_example();
		break;
	case 4:
		tcc_swap_feature_example();
		break;
	case 5:
		tcc_invert_feature_example();
		break;
	case 6:
		tcc_pattern_generation_example();
		break;
	case 7:
		tcc_ramp2_feature_example();
		break;
	case 8:
		tcc_ramp2a_feature_example();
		break;
	case 9:
		tcc_pwm_dual_slope_mode_example();
		break;
	default:
		pwm_1khz_example();
		break;
	}
}
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	printf(TCC_EXAMPLES);
	tcc_examples();

	while (1) {
		printf("\r\n******************** Please Enter Choice ********************\r\n\n");
		if (scanf("%d", &example_selection) == 0) {
			// If its not a number, flush stdin
			fflush(stdin);
			continue;
		}
		tcc_examples();
	}
}
