/**
 * \file
 *
 * \brief Application implement
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
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
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
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <atmel_start.h>

#define CONF_IDLE 2
#define CONF_IDLE_STA 0
#define CONF_IDLE_PRE

#define CONF_STDBY 4
#define CONF_STDBY_STA 2
#define CONF_STDBY_PRE hri_pm_write_STDBYCFG_reg(PM, PM_STDBYCFG_FASTWKUP(0) | PM_STDBYCFG_RAMCFG(0x1))

#define CONF_HIB 5
#define CONF_HIB_STA 4
#define CONF_HIB_PRE hri_pm_write_HIBCFG_reg(PM, PM_HIBCFG_RAMCFG(0x1) | PM_HIBCFG_BRAMCFG(0x1))

#define CONF_BACKUP 6
#define CONF_BACKUP_STA 5
#define CONF_BACKUP_PRE hri_pm_write_BKUPCFG_reg(PM, PM_BKUPCFG_BRAMCFG(0x1))

#define CONF_OFF 7
#define CONF_OFF_STA 6
#define CONF_OFF_PRE

#define CONF_PL_CONFIG
#define CONF_DFLL_CONFIG hri_oscctrl_write_DFLLCTRLA_reg(OSCCTRL, 0)
#define CONF_BOD_CONFIG hri_supc_clear_BOD33_ENABLE_bit(SUPC)
#define CONF_VREG_CONFIG
#define CONF_NVM_CONFIG hri_nvmctrl_write_CTRLA_PRM_bf(NVMCTRL, 1)
#define CONF_PINS_CONFIG                                                                                               \
	gpio_set_pin_direction(GPIO(2, 21), GPIO_DIRECTION_OUT);                                                           \
	gpio_set_pin_level(GPIO(2, 21), false)

#define CONF_RCAUSE_RD hri_rstc_read_RCAUSE_reg(RSTC)
#define CONF_TSTATE_RD hri_rtcmode0_read_BKUP_reg(RTC, 0)
#define CONF_TSTATE_WR(a)                                                                                              \
	hri_rtcmode0_write_BKUP_reg(RTC, 0, (a));                                                                          \
	tstate = (a)

#define CONF_WKUP_EN                                                                                                   \
	hri_rtcmode0_set_CTRLA_ENABLE_bit(RTC);                                                                            \
	hri_rtcmode0_set_INTEN_CMP0_bit(RTC)
#define CONF_WKUP_DIS                                                                                                  \
	hri_rtcmode0_clear_INTEN_CMP0_bit(RTC);                                                                            \
	hri_rtcmode0_clear_CTRLA_ENABLE_bit(RTC)
#define CONF_WKUP_CLR hri_rtcmode0_clear_INTFLAG_CMP0_bit(RTC);

/* Before entering Standby, Hibernate or Backup mode, software must
 * ensure that the INTFLAG.SLEEPRDY bit is set.
 */
#define CONF_WAIT_RDY                                                                                                  \
	while (!hri_pm_read_INTFLAG_reg(PM)) {                                                                             \
	}

volatile bool rtc_interrupt_flag = false;
void          RTC_Handler(void)
{
	/* Clear CMP0 interrupt flag */
	hri_rtcmode0_clear_INTFLAG_CMP0_bit(RTC);

	/* Set software RTC Flag */
	rtc_interrupt_flag = true;
}

static volatile uint32_t tstate = 0;

/*
 * This example demonstrates the different low power modes.
 * An RTC is used to enter/exit the different modes every 15
 * seconds (can be configured).
 */
int main(void)
{
	tstate = CONF_TSTATE_RD;
	if (hri_rtcmode0_read_SYNCBUSY_reg(RTC)) {
		hri_rtcmode0_write_CTRLA_reg(RTC, RTC_MODE0_CTRLA_SWRST);
	}

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Related configurations */
	CONF_PL_CONFIG;
	CONF_DFLL_CONFIG;
	CONF_BOD_CONFIG;
	CONF_VREG_CONFIG;
	CONF_NVM_CONFIG;
	CONF_PINS_CONFIG;

	CONF_WAIT_RDY;

	/* ACTIVE mode & sleep modes */
	if (tstate == 0) {
		/* Active */
		NVIC_EnableIRQ(RTC_IRQn);
		rtc_interrupt_flag = false;
		while (!rtc_interrupt_flag) {
		}
	}

#if CONF_IDLE
	if (tstate == CONF_IDLE_STA) {
		/* Idle */
		CONF_TSTATE_WR(CONF_IDLE);
		CONF_IDLE_PRE;
		_set_sleep_mode(CONF_IDLE);
		__DSB();
		__WFI();
	}
#endif
#if CONF_STDBY
	if (tstate == CONF_STDBY_STA) {
		/* Standby */
		CONF_TSTATE_WR(CONF_STDBY);
		CONF_STDBY_PRE;
		_set_sleep_mode(CONF_STDBY);
		__DSB();
		__WFI();
	}
#endif

	/* Sleep modes (active reset) */
	CONF_WAIT_RDY;

/* Hibernate/Backup */
#if CONF_HIB
	if (tstate == CONF_HIB_STA) {
		CONF_TSTATE_WR(CONF_HIB);
		CONF_HIB_PRE;
		_set_sleep_mode(CONF_HIB);
		__DSB();
		__WFI();
		while (1) {
		} /* Should wakeup reset and never here */
	}
#endif
#if CONF_BACKUP
	if (tstate == CONF_BACKUP_STA) {
		CONF_TSTATE_WR(CONF_BACKUP);
		CONF_WKUP_CLR;
		CONF_BACKUP_PRE;
		_set_sleep_mode(CONF_BACKUP);
		__DSB();
		__WFI();
		while (1) {
		} /* Should wakeup reset and never here */
	}
#endif
#if CONF_OFF
	if (tstate == CONF_OFF_STA) {
		CONF_TSTATE_WR(0);
		CONF_WKUP_CLR;
		CONF_WKUP_DIS;
		CONF_OFF_PRE;
		_set_sleep_mode(CONF_OFF);
		__DSB();
		__WFI();
		while (1) {
		} /* Should never here */
	}
#endif
	while (1) {
	}
}
