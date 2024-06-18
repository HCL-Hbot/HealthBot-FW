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

#ifndef BGT60LTR11XXX_REGS_HPP
#define BGT60LTR11XXX_REGS_HPP

#include <stdint.h>
#include <bgt60ltr11XXX_typedef.hpp>

#define SET_SPI_ADDR(addr, read_wr_bit) (((reg & 0x7F) << 1) | SPI_READ_BIT)

/* Datasheet: https://www.infineon.com/dgdl/Infineon-Users_guide_to_BGT60LTR11SAIP-ApplicationNotes-v01_05-EN.pdf?fileId=8ac78c8c821f3890018268a7f22e1c54
*/
namespace BGT60 {

const static constexpr uint8_t BIT_ONE        = 0x01;
const static constexpr uint8_t SPI_READ_BIT   = 0x01;
const static constexpr uint8_t BYTE           = 8;
const static constexpr uint8_t BYTE_MASK      = 0xFF;

// SPI Configuration Structure:
typedef struct {
    uint16_t clkFrequencyMHz;   // SPI clock frequency in MHz.
    uint8_t bitsPerTransfer;    // Number of bits per transfer.
} SPIConfig_t;

// Register Field Structure:
struct RegisterField {
    const uint16_t mask;
    const uint16_t shift;
};

/* BGT60LTR11XXX SPI Register Address Map */
enum class REGISTER_ADDRESS : uint8_t {
    REG0    = 0x00,     /* Direct Access Register 1 */
    REG1    = 0x01,     /* Direct Access Register 2 */
    REG2    = 0x02,     /* Threshold Configuration */
    REG3    = 0x03,     /* Reserved */
    REG4    = 0x04,     /* PLL Configuration 1 */
    REG5    = 0x05,     /* PLL Configuration 2 */
    REG6    = 0x06,     /* PLL Configuration 3 */
    REG7    = 0x07,     /* Duty Cycling Timing PD MPA */
    REG8    = 0x08,     /* Divider Configuration */
    REG9    = 0x09,     /* Baseband Configuration */
    REG10   = 0x0A,     /* Hold Time Configuration */
    REG11   = 0x0B,     /* Reserved */
    REG12   = 0x0C,     /* BITE (Built-In Test Equipment) */
    REG13   = 0x0D,     /* Algorithm Configuration 1 */
    REG14   = 0x0E,     /* Algorithm Configuration 2 */
    REG15   = 0x0F,     /* Digital Control */
/* Others Reserved */
    REG34   = 0x22,     /* ADC Start */
    REG35   = 0x23,     /* ADC Convert */
    REG36   = 0x24,     /* ADC Status */
    REG38   = 0x26,
    REG_ADC_CHANNELS_START = 0x26,
    /* ... 16x ADC Channels: ... */
    REG_ADC_CHANNELS_END = 0x36,
    /* ... Reserved ... */
    REG56   = 0x38,     /* Status and Chip Version */
    GSR0    = 0x39      /* SPI Status Register - Datasheet does not explicitly state this. */
};

struct REGISTER_FIELDS {
    struct Reg0 {
        static constexpr RegisterField RXBUF_EN         = {0x0001, 0};
        static constexpr RegisterField LNA_EN           = {0x0002, 0};
        static constexpr RegisterField MIXI_EN          = {0x0004, 0};
        static constexpr RegisterField MIXQ_EN          = {0x0008, 0};
        static constexpr RegisterField TXBUF_EN         = {0x0010, 0};
        static constexpr RegisterField MPA_EN           = {0x0020, 0};

        static constexpr RegisterField PLL_EN           = {0x0001, 8};
        static constexpr RegisterField PLL_ACTIVE       = {0x0002, 8};
        static constexpr RegisterField PLL_CLK_GATE_EN  = {0x0004, 8};
        static constexpr RegisterField PLL_OPEN_LOOP    = {0x0008, 8};
        static constexpr RegisterField VCO_EN           = {0x0010, 8};
        static constexpr RegisterField VCOBUF_EN        = {0x0020, 8};
    };

    struct REG1 {
        static constexpr RegisterField DIV_BIAS_EN      = {0x0001, 12};
        static constexpr RegisterField QS_RD_EN         = {0x0001, 8};
        static constexpr RegisterField BB_DIG_DET_EN    = {0x0001, 7};
        static constexpr RegisterField BB_BOOST_DIS     = {0x0001, 5}; 
        static constexpr RegisterField BB_CLK_C_HOP_EN  = {0x0001, 4};
        static constexpr RegisterField BB_STRUP_HP      = {0x0001, 2}; 
        static constexpr RegisterField BB_AMP_EN        = {0x0001, 1}; 
        static constexpr RegisterField BB_SAMPLE_EN     = {0x0001, 0};  
    };

    struct REG2 {
        static constexpr RegisterField HPRT             = {0x0001, 15};
        static constexpr RegisterField APRT             = {0x0001, 14};
        static constexpr RegisterField DIR_MODE         = {0x0001, 13};
        static constexpr RegisterField THRS             = {0x1FFF, 0};  
    };

    struct REG3 {
        static constexpr RegisterField Reserved         = {0xFFFF, 0};  
    };

    struct REG4 {
        static constexpr RegisterField PLL_DFT_DMUX     = {0x00C0, 8};
        static constexpr RegisterField PLL_BIAS_DIS     = {0x0001, 12};
        static constexpr RegisterField PLL_LF_ISO       = {0x0001, 11};
        static constexpr RegisterField PLL_LF_R4_SEL    = {0x0001, 10};
        static constexpr RegisterField PLL_CL_LOOP_PMODE= {0x0001, 9};
        static constexpr RegisterField PLL_LF_R2_SEL    = {0x0001, 8};
        static constexpr RegisterField XOSC_MODE        = {0x0001, 7};
        static constexpr RegisterField PLL_FBDIV_CNT    = {0x0001, 6};  
        static constexpr RegisterField PLL_CP_ICP_SEL   = {0x0038, 0};
        static constexpr RegisterField PLL_CP_MODE      = {0x0001, 2};
        static constexpr RegisterField PLL_PFD_RDT_SEL  = {0x0003, 0};
    };

    struct REG5 {
        static constexpr RegisterField PLL_FCW          = {0x0FFF, 0};
    };

    struct REG6 {
        static constexpr RegisterField PLL_LD_TW_SEL    = {0xE000, 0};
        static constexpr RegisterField PLL_LD_LEN       = {0x0010, 8};
        static constexpr RegisterField PLL_LD_EN        = {0x0008, 8};
    };

    struct REG7 {
        static constexpr RegisterField DC_REP_RATE      = {0x0003, 10};
        static constexpr RegisterField DC_ON_PULSE_LEN  = {0x0003, 8};
        static constexpr RegisterField VCO2PLL_DLY      = {0x0001, 6};
        static constexpr RegisterField MPA2SH_DLY       = {0x0003, 4};
        static constexpr RegisterField PD_EN            = {0x0001, 3};
        static constexpr RegisterField MPA_CTRL         = {0x0007, 0};
    };

    struct REG8 {
        static constexpr RegisterField DIV_SEL          = {0x0003, 2};
        static constexpr RegisterField DIV_OUT_EN       = {0x0002, 0};
        static constexpr RegisterField DIV_TEST_MODE_EN = {0x0001, 0};
    };

    struct REG9 {
        static constexpr RegisterField BB_HP_RES        = {0x0003, 6};
        static constexpr RegisterField BB_CLK_CHOP_SEL  = {0x0001, 5};
        static constexpr RegisterField BB_LPF_BW        = {0x0001, 4};
        static constexpr RegisterField BB_CTRL_GAIN     = {0x000F, 0};
    };

    struct REG10 {
        static constexpr RegisterField HOLD             = {0xFFFF, 0};
    };

    struct REG12 {
        static constexpr RegisterField BB_AMUX_CTRL     = {0x0003, 6}; 
        static constexpr RegisterField BB_AMUX_EN       = {0x0001, 5}; 
        static constexpr RegisterField BB_AMUX_PD_EN    = {0x0001, 4}; 
        static constexpr RegisterField BITE_CTRL        = {0x000E, 0}; 
        static constexpr RegisterField BITE_EN          = {0x0001, 0}; 
    };

    struct REG13 {
        static constexpr RegisterField PHASE_WIN_LEN    = {0x0007, 5};
        static constexpr RegisterField MEAN_WIN_LEN     = {0x0007, 2};
        static constexpr RegisterField PRT_MULT         = {0x0003, 0};
    };

    struct REG14 {
        static constexpr RegisterField THRS_OFFSET      = {0xFF00, 0}; 
        static constexpr RegisterField DIR_HYS_DIS      = {0x0001, 7};
        static constexpr RegisterField DIR_KEEP         = {0x0001, 6};
        static constexpr RegisterField HOLD_X32         = {0x0001, 5};
        static constexpr RegisterField SWAP_IQ          = {0x0001, 4};
        static constexpr RegisterField AUTOBLIND_DIS    = {0x0001, 3};
        static constexpr RegisterField PULSE_MON        = {0x0001, 2};
        static constexpr RegisterField PHASE_THRS       = {0x0003, 0};
    };

    struct REG15 {
        static constexpr RegisterField SOFT_RESET       = {0x0001, 15};
        static constexpr RegisterField START_PM         = {0x0001, 14};
        static constexpr RegisterField CLK_EXT_DIS      = {0x0001, 13};
        static constexpr RegisterField START_CW         = {0x0001, 12};
        static constexpr RegisterField FAST_PHASE       = {0x0001, 11};
        static constexpr RegisterField DIR_C2_1         = {0x0600, 0};
        static constexpr RegisterField FASTMODE         = {0x0001, 8};

        static constexpr RegisterField ADC_MON          = {0x0001, 7};
        static constexpr RegisterField MISO_DRV         = {0x0001, 6};
        static constexpr RegisterField MOT_POL          = {0x0001, 5};
        static constexpr RegisterField DIR_POL          = {0x0001, 4};
        static constexpr RegisterField STAT_MUX         = {0x000F, 0};
    };

    struct REG34 {
        static constexpr RegisterField ADC_EN           = {0x0001, 2};
        static constexpr RegisterField BANDGAP_EN       = {0x0001, 1};
        static constexpr RegisterField ADC_CLK_EN       = {0x0001, 0};
    };

    struct REG35 {
        static constexpr RegisterField LV_GAIN          = {0x0001, 7};
        static constexpr RegisterField CHNR_ALL         = {0x0001, 4};
        static constexpr RegisterField CHNR             = {0x000F, 0};
    };

    struct REG36 {
        static constexpr RegisterField ADC_READY        = {0x0001, 1};
        static constexpr RegisterField BANDGAP_UP       = {0x0001, 0};
    };

    struct ADC_REGS {
        static constexpr RegisterField ADC_RESULT       = {0x00FF, 2};
    };

    struct REG56 {
        static constexpr RegisterField QS1_S            = {0x0003, 14};
        static constexpr RegisterField INIT_DONE        = {0x0001, 13};
        static constexpr RegisterField QS2_S            = {0x0003, 11};
        static constexpr RegisterField QS3_S            = {0x0003, 9};
        static constexpr RegisterField QS4_S            = {0x0003, 7};
        static constexpr RegisterField ADVANCED_MODE    = {0x0001, 6};
        static constexpr RegisterField PLL_LOCK_DETECT  = {0x0001, 3};
        static constexpr RegisterField CHIP_VERSION     = {0x0003, 0};
    };

    struct GSPR0 {
        static constexpr RegisterField ADC_RESULT_READY = {0x0001, 2};
    };
};

struct FIELDS_OPTIONS {
    // Programmable Gain Amplifier (PGA) Options:
    struct PGA_GAIN {
        static constexpr uint8_t GAIN_10DB      = 0;    // +10 dBm
        static constexpr uint8_t GAIN_15DB      = 1;    // +15 dBm
        static constexpr uint8_t GAIN_20DB      = 2;    // +20 dBm
        static constexpr uint8_t GAIN_25DB      = 3;    // +25 dBm
        static constexpr uint8_t GAIN_30DB      = 4;    // +30 dBm
        static constexpr uint8_t GAIN_35DB      = 5;    // +35 dBm
        static constexpr uint8_t GAIN_40DB      = 6;    // +40 dBm
        static constexpr uint8_t GAIN_45DB      = 7;    // +45 dBm
        static constexpr uint8_t GAIN_50DB      = 8;    // +50 dBm
    };

    struct MEDIUM_PWR_GAIN {
        static constexpr uint8_t GAIN_N34DB     = 0;    // -34.5 dBm
        static constexpr uint8_t GAIN_N31DB     = 1;    // -31.5 dBm
        static constexpr uint8_t GAIN_N25B      = 2;    // -25 dBm
        static constexpr uint8_t GAIN_N18DB     = 3;    // -18 dBm
        static constexpr uint8_t GAIN_N11DB     = 4;    // -11 dBm
        static constexpr uint8_t GAIN_N5DB      = 5;    // -5 dBm
        static constexpr uint8_t GAIN_0DB       = 6;    // 0 dBm
        static constexpr uint8_t GAIN_4DB       = 7;    // 4.5 dBm
    };
};

enum class ADC_REG_CHANNELS : uint8_t {
    POWER_SENSOR_MPA_OUTPUT     = 0,
    POWER_SENSOR_MPAX_OUTPUT    = 1,
    IFI                         = 2,
    IFQ                         = 3,
    POWER_SENSOR_BITE_PD_OUT    = 4,
    POWER_SENSOR_BITE_PD_OUTX   = 5,
    QS2                         = 6,
    QS3                         = 7,
    COMMON_MODE_VOLTAGE_IFI     = 8,
    COMMON_MODE_VOLTAGE_IFQ     = 9,
    VDD_RF_CLOSE_TO_SPI         = 10,
    GND                         = 11,
    TEMPERATURE_SENSOR          = 12,
    PLL_BANDGAP_VOLTAGE         = 13,
    ADC_BANDGAP_VOLTAGE         = 14,
    ABB_BANDGAP_VOLTAGE         = 15
};

const constexpr uint8_t getAdcRegisterAddress(const ADC_REG_CHANNELS channel) {
    return (static_cast<uint8_t>(REGISTER_ADDRESS::REG_ADC_CHANNELS_START) + static_cast<uint8_t>(channel));
}

} // End of BGT60 namespace. 
#endif // BGT60LTR11XXX_REGS_HPP