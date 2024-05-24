
#ifndef _REGISTER_H_
#define _REGISTER_H_


#define SIM_CTRL_BASE       (0x0200)
#define DRAM_CTRL_BASE      (0x0300)
#define TIME_BASE_BASE      (0x0400)
#define ANALOG_CTRL_BASE    (0x0500)
#define RC_CTRL_BASE        (0x0600)
#define PIN_CTRL_BASE       (0x0700)
#define INT_CTRL_BASE       (0x0800)
#define PWM_CTRL_BASE       (0x0900)
#define PM_REG_BASE         (0x0a00)
#define ADDRESS_MAP_BASE    (0x0b00)
#define E2P_CTRL_BASE       (0x0c00)
#define WAKE_UP_BASE        (0x0d00)


// SIM_CTRL
#define SIMC0               (XBYTE[(SIM_CTRL_BASE + 0x00)])
#define SIMC1               (XBYTE[(SIM_CTRL_BASE + 0x01)])
#define SIMD                (XBYTE[(SIM_CTRL_BASE + 0x02)])
#define SIMC2               (XBYTE[(SIM_CTRL_BASE + 0x03)])

// DRAM_CTRL
#define MTP_CTRL            (XBYTE[(DRAM_CTRL_BASE + 0x00)])
#define MTP_PROTECT         (XBYTE[(DRAM_CTRL_BASE + 0x01)])
#define DRAM_WR_REV_TM      (XBYTE[(DRAM_CTRL_BASE + 0x02)])



// TIME_BASE
#define TBC                 (XBYTE[(TIME_BASE_BASE + 0x00)])
#define TBC_INT             (XBYTE[(TIME_BASE_BASE + 0x01)])
#define BZ_CTRL             (XBYTE[(TIME_BASE_BASE + 0x02)])


// ANALOG_CTRL
#define ADRL                (XBYTE[(ANALOG_CTRL_BASE + 0x00)])
#define ADRH        		(XBYTE[(ANALOG_CTRL_BASE + 0x01)])
#define ADCR         		(XBYTE[(ANALOG_CTRL_BASE + 0x02)])
#define ADIFG         		(XBYTE[(ANALOG_CTRL_BASE + 0x03)])
#define OPA1C0        		(XBYTE[(ANALOG_CTRL_BASE + 0x04)])
#define OPA1C1       	    (XBYTE[(ANALOG_CTRL_BASE + 0x05)])
#define OPA1MODE      	    (XBYTE[(ANALOG_CTRL_BASE + 0x06)])
#define OPA2C0        		(XBYTE[(ANALOG_CTRL_BASE + 0x07)])
#define OPA2C1          	(XBYTE[(ANALOG_CTRL_BASE + 0x08)])
#define OPA2C2          	(XBYTE[(ANALOG_CTRL_BASE + 0x09)])
#define CMP1C0              (XBYTE[(ANALOG_CTRL_BASE + 0x0A)])
#define CMP1C1              (XBYTE[(ANALOG_CTRL_BASE + 0x0B)])
#define CMP2C0              (XBYTE[(ANALOG_CTRL_BASE + 0x0C)])
#define CMP2C1              (XBYTE[(ANALOG_CTRL_BASE + 0x0D)])
#define CMPOF_SEL           (XBYTE[(ANALOG_CTRL_BASE + 0x0E)])
#define BANDGAP_TEMP        (XBYTE[(ANALOG_CTRL_BASE + 0x0F)])
#define LDOC                (XBYTE[(ANALOG_CTRL_BASE + 0x10)])
#define LVDC                (XBYTE[(ANALOG_CTRL_BASE + 0x11)])
#define DAL                 (XBYTE[(ANALOG_CTRL_BASE + 0x12)])
#define DAH                 (XBYTE[(ANALOG_CTRL_BASE + 0x13)])
#define DACTRL              (XBYTE[(ANALOG_CTRL_BASE + 0x14)])
#define IRED_CTRL           (XBYTE[(ANALOG_CTRL_BASE + 0x15)])
#define LXT_START           (XBYTE[(ANALOG_CTRL_BASE + 0x16)])
#define LDO2_CTRL           (XBYTE[(ANALOG_CTRL_BASE + 0x1B)])


// RC_CTRL
#define MCU_HIRCL_TRIM      (XBYTE[(RC_CTRL_BASE + 0x00)])
#define MCU_HIRCH_TRIM      (XBYTE[(RC_CTRL_BASE + 0x01)])
#define MCU_HIRC16M_TRIM    (XBYTE[(RC_CTRL_BASE + 0x02)])
#define MCU_LIRC_TRIM       (XBYTE[(RC_CTRL_BASE + 0x03)])
#define RC_CTRL1            (XBYTE[(RC_CTRL_BASE + 0x04)])
#define RC_CTRL2            (XBYTE[(RC_CTRL_BASE + 0x05)])
#define MCU_TRIM_PTCT       (XBYTE[(RC_CTRL_BASE + 0x06)])


// PIN_CTRL
#define PAPU                (XBYTE[(PIN_CTRL_BASE + 0x00)])
#define PBPU                (XBYTE[(PIN_CTRL_BASE + 0x01)])
#define PCPU                (XBYTE[(PIN_CTRL_BASE + 0x02)])
#define PDPU                (XBYTE[(PIN_CTRL_BASE + 0x03)])
#define PEPU                (XBYTE[(PIN_CTRL_BASE + 0x04)])
#define MISC                (XBYTE[(PIN_CTRL_BASE + 0x05)])
#define LCDC                (XBYTE[(PIN_CTRL_BASE + 0x06)])
#define ADPCR               (XBYTE[(PIN_CTRL_BASE + 0x07)])
#define EE_PINSEL           (XBYTE[(PIN_CTRL_BASE + 0x08)])


// INT_CTRL
#define IRQSTATL            (XBYTE[(INT_CTRL_BASE + 0x00)])
#define IRQSTATH            (XBYTE[(INT_CTRL_BASE + 0x01)])
#define INTML               (XBYTE[(INT_CTRL_BASE + 0x02)])
#define INTMH         		(XBYTE[(INT_CTRL_BASE + 0x03)])
#define IRQ_MSKDL        	(XBYTE[(INT_CTRL_BASE + 0x04)])
#define IRQ_MSKDH        	(XBYTE[(INT_CTRL_BASE + 0x05)])
#define INTSEL0L         	(XBYTE[(INT_CTRL_BASE + 0x06)])
#define INTSEL0H         	(XBYTE[(INT_CTRL_BASE + 0x07)])


// PWM_CTRL
#define PWM_PERIOD0L        (XBYTE[(PWM_CTRL_BASE + 0x00)])
#define PWM_PERIOD0H        (XBYTE[(PWM_CTRL_BASE + 0x01)])
#define PWM_PULSE0L         (XBYTE[(PWM_CTRL_BASE + 0x02)])
#define PWM_PULSE0H         (XBYTE[(PWM_CTRL_BASE + 0x03)])
#define PWM_PERIOD1L        (XBYTE[(PWM_CTRL_BASE + 0x04)])
#define PWM_PERIOD1H        (XBYTE[(PWM_CTRL_BASE + 0x05)])
#define PWM_PULSE1L         (XBYTE[(PWM_CTRL_BASE + 0x06)])
#define PWM_PULSE1H         (XBYTE[(PWM_CTRL_BASE + 0x07)])
#define DEAD_ZONE0          (XBYTE[(PWM_CTRL_BASE + 0x08)])
#define DEAD_ZONE1          (XBYTE[(PWM_CTRL_BASE + 0x09)])
#define PWM_CTRL            (XBYTE[(PWM_CTRL_BASE + 0x0A)])
#define DZ_CTRL             (XBYTE[(PWM_CTRL_BASE + 0x0B)])		 


// PM_REG
#define SYS_REG1            (XBYTE[(PM_REG_BASE + 0x00)])
#define RESET_FALG          (XBYTE[(PM_REG_BASE + 0x01)])
#define SYS_REG2            (XBYTE[(PM_REG_BASE + 0x02)])
#define ADC_DIV             (XBYTE[(PM_REG_BASE + 0x03)])
#define DAC_DIV 	        (XBYTE[(PM_REG_BASE + 0x04)])
#define SYS_REG_MASK        (XBYTE[(PM_REG_BASE + 0x05)])
#define SYS_REG3            (XBYTE[(PM_REG_BASE + 0x06)])
#define SYS_REG4            (XBYTE[(PM_REG_BASE + 0x07)])


// ADDRESS_MAP
#define ERREN               (XBYTE[(ADDRESS_MAP_BASE + 0x00)])

// E2P_CTRL
#define E2P_CTRL            (XBYTE[(E2P_CTRL_BASE + 0x00)])
#define E2P_FLAG            (XBYTE[(E2P_CTRL_BASE + 0x01)])
#define E2P_DATA            (XBYTE[(E2P_CTRL_BASE + 0x02)])
#define E2P_ADDRL           (XBYTE[(E2P_CTRL_BASE + 0x03)])
#define E2P_ADDRH           (XBYTE[(E2P_CTRL_BASE + 0x04)])
#define E2P_PREDIV          (XBYTE[(E2P_CTRL_BASE + 0x06)])

// WAKE_UP
#define WAKE_EN             (XBYTE[(WAKE_UP_BASE + 0x00)])
#define WAKEUP_EDGE_REG     (XBYTE[(WAKE_UP_BASE + 0x01)])



#endif