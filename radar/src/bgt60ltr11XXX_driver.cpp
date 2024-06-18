#include <bgt60ltr11XXX_driver.hpp>

namespace BGT60 {

    BGT60_DRIVER::BGT60_DRIVER() {
        initialize_spi_interface();
    }

    void BGT60_DRIVER::initCwMode() {
        radarEnableSoftReset();
        clear_Register(REGISTER_ADDRESS::REG0);
        clear_Register(REGISTER_ADDRESS::REG1);
        clear_Register(REGISTER_ADDRESS::REG2);

        clearWrite_MultipleFieldsInRegister(REGISTER_ADDRESS::REG2, {
            {REGISTER_FIELDS::REG2::HPRT, hprt_off},
            {REGISTER_FIELDS::REG2::APRT, aprt_off},
            {REGISTER_FIELDS::REG2::DIR_MODE, dir_mode_2},
            {REGISTER_FIELDS::REG2::THRS, det_thrs_2560},
        });

        clearWrite_MultipleFieldsInRegister(REGISTER_ADDRESS::REG4, {
            {REGISTER_FIELDS::REG4::PLL_PFD_RDT_SEL, pfd_rst_dly_375ps},
            {REGISTER_FIELDS::REG4::PLL_CP_MODE, bias_regulation_loop_active},
            {REGISTER_FIELDS::REG4::PLL_CP_ICP_SEL, charge_pump_current_55uA},
            {REGISTER_FIELDS::REG4::PLL_FBDIV_CNT, feedback_divider_counter_21dec},
            {REGISTER_FIELDS::REG4::XOSC_MODE, xosc_mode_amplitude_1},
            {REGISTER_FIELDS::REG4::PLL_LF_R2_SEL, loopfilter_R2_18_7Kohm},
            {REGISTER_FIELDS::REG4::PLL_CL_LOOP_PMODE, closed_loop_pusled_mode},
            {REGISTER_FIELDS::REG4::PLL_LF_R4_SEL, loopfilter_R4_0_1Kohm},
            {REGISTER_FIELDS::REG4::PLL_LF_ISO, loopfilter_isolation_with_switches_only},
            {REGISTER_FIELDS::REG4::PLL_BIAS_DIS, biassing_on},
            {REGISTER_FIELDS::REG4::PLL_DFT_DMUX, mux_functional_mode},
        });

        clearWrite_RegisterField(REGISTER_ADDRESS::REG5, REGISTER_FIELDS::REG5::PLL_FCW, frequency_61_25Ghz); // RF Frequency
    
        clearWrite_MultipleFieldsInRegister(REGISTER_ADDRESS::REG6, {
            {REGISTER_FIELDS::REG6::PLL_LD_EN, lock_detection_on},
            {REGISTER_FIELDS::REG6::PLL_LD_LEN, lock_detection_24_clock},
            {REGISTER_FIELDS::REG6::PLL_LD_TW_SEL, lock_detection_time_window_1_5ns},
        });

        clearWrite_MultipleFieldsInRegister(REGISTER_ADDRESS::REG7, {
            {REGISTER_FIELDS::REG7::MPA_CTRL, mpa_4_5dBm},
            {REGISTER_FIELDS::REG7::PD_EN, pd_off},
            {REGISTER_FIELDS::REG7::MPA2SH_DLY, map_enable_to_sample_hold_delay_1000ns},
            {REGISTER_FIELDS::REG7::VCO2PLL_DLY, vco_to_pll_delay_1000ns},
            {REGISTER_FIELDS::REG7::DC_ON_PULSE_LEN, duty_cycle_on_pulse_10us},
            {REGISTER_FIELDS::REG7::DC_REP_RATE, duty_cycle_rep_500us},
        });

        clearWrite_MultipleFieldsInRegister(REGISTER_ADDRESS::REG8, {
            {REGISTER_FIELDS::REG8::DIV_OUT_EN, BIT_ONE},
            {REGISTER_FIELDS::REG8::DIV_SEL, divider_select_2_power_20},
        });

        clearWrite_MultipleFieldsInRegister(REGISTER_ADDRESS::REG9, {
            {REGISTER_FIELDS::REG9::BB_CTRL_GAIN, baseband_PGA_gain_50dB},
            {REGISTER_FIELDS::REG9::BB_LPF_BW, low_pass_filter_10kHz},
            {REGISTER_FIELDS::REG9::BB_CLK_CHOP_SEL, clock_chop_freq_200kHz},
            {REGISTER_FIELDS::REG9::BB_HP_RES, high_pass_filter_resistor_1Mohm},
        });

        write_Register(REGISTER_ADDRESS::REG10, REGISTER_FIELDS::REG10::HOLD, hold_time_minimum);
        clear_Register(REGISTER_ADDRESS::REG12);
        clear_Register(REGISTER_ADDRESS::REG13);
        clear_Register(REGISTER_ADDRESS::REG14);

        write_Register(REGISTER_ADDRESS::REG15, REGISTER_FIELDS::REG15::START_PM, start_pm_inactive);
        write_Register(REGISTER_ADDRESS::REG15, REGISTER_FIELDS::REG15::MISO_DRV, miso_drv_high_z); // miso_drv_some_level);

        clear_Register(REGISTER_ADDRESS::REG34);
        clear_Register(REGISTER_ADDRESS::REG35);
        clear_Register(REGISTER_ADDRESS::REG36);

        write_Register(REGISTER_ADDRESS::REG15, REGISTER_FIELDS::REG15::START_CW, start_cw_active);
        write_Register(REGISTER_ADDRESS::REG15, REGISTER_FIELDS::REG15::START_PM, start_pm_active);
    }

    void BGT60_DRIVER::radarEnableSoftReset() {
        const REGISTER_ADDRESS reg15 = REGISTER_ADDRESS::REG15;
        const REGISTER_ADDRESS reg56 = REGISTER_ADDRESS::REG56; 

        clearWrite_RegisterField(reg15, REGISTER_FIELDS::REG15::SOFT_RESET, BIT_ONE); // Perfom a reset

        // Register Pooling with timeout.
        for (uint16_t i = 0; i < INIT_TIMEOUT; i++) {
            uint16_t init_flag = read_RegisterField(reg56, REGISTER_FIELDS::REG56::INIT_DONE);
            uint16_t reg0_done = spi_read_register(static_cast<uint16_t>(REGISTER_ADDRESS::REG0));

            if ((init_flag == 1) && (reg0_done == 0)) {
                break; // Stop Pooling
            }
            vTaskDelay(pdMS_TO_TICKS(20)); 
        }
    }

    // Variadic Function: 
    template<typename... FieldValuePairs>
    void BGT60_DRIVER::clearWrite_MultipleRegisterFields(FieldValuePairs&&... pairs) {
        (setRegisterField(std::forward<FieldValuePairs>(pairs).first, 
            std::forward<FieldValuePairs>(pairs).second.first, 
            std::forward<FieldValuePairs>(pairs).second.second), ...);
    }

    template<typename... FieldValuePairs>
    void BGT60_DRIVER::write_MultipleRegisterFields(FieldValuePairs&&... pairs) {
        (writeRegister(std::forward<FieldValuePairs>(pairs).first, 
            std::forward<FieldValuePairs>(pairs).second.first, 
            std::forward<FieldValuePairs>(pairs).second.second), ...);
    }

    void BGT60_DRIVER::clearWrite_MultipleFieldsInRegister(const REGISTER_ADDRESS registerAddress, const std::vector<std::pair<RegisterField, uint16_t>>& fieldsValues) {
        uint16_t regValue = spi_read_register(static_cast<uint8_t>(registerAddress));
        
        // Apply all modifications to regValue
        for (const auto& fv : fieldsValues) {
            const RegisterField& field = fv.first;
            uint16_t value = fv.second;
            regValue = (regValue & ~(field.mask << field.shift)) | ((value & field.mask) << field.shift);
        }

        // Write back the modified register value
        spi_write_register(static_cast<uint8_t>(registerAddress), regValue);
    }

    // Functions to interact with the hardware
    void BGT60_DRIVER::clearWrite_RegisterField(const REGISTER_ADDRESS registerAddress, const RegisterField& field, const uint16_t value) {
        uint16_t regValue = spi_read_register(static_cast<uint8_t>(registerAddress));
        regValue = (regValue & ~(field.mask << field.shift)) | ((value & field.mask) << field.shift);
        spi_write_register(static_cast<uint8_t>(registerAddress), regValue);
    }

    void BGT60_DRIVER::write_Register(const REGISTER_ADDRESS registerAddress, const RegisterField& field, const uint16_t value) {
        uint16_t regValue = 0x0000;
        regValue = ((value & field.mask) << field.shift);
        spi_write_register(static_cast<uint8_t>(registerAddress), regValue);
    }

    void BGT60_DRIVER::clear_Register(REGISTER_ADDRESS registerAddress) {
        spi_write_register(static_cast<uint8_t>(registerAddress), 0x0000);
    }

    void BGT60_DRIVER::clear_RegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field) {
        uint16_t regValue = spi_read_register(static_cast<uint8_t>(registerAddress));
        regValue &= ~(field.mask << field.shift);
        spi_write_register(static_cast<uint8_t>(registerAddress), regValue);
    }

    const uint16_t BGT60_DRIVER::read_RegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field) {
        uint16_t regValue = spi_read_register(static_cast<uint8_t>(registerAddress));
        return read_field(regValue, field);
    }

    const uint16_t BGT60_DRIVER::read_RegisterField(uint8_t registerAddress, const RegisterField& field) {
        uint16_t regValue = spi_read_register(registerAddress);
        return read_field(regValue, field);
    }

    const uint16_t BGT60_DRIVER::read_AdChannel(const ADC_REG_CHANNELS channel) {
        const uint8_t result_reg = getAdcRegisterAddress(channel);
        uint16_t channel_result = read_RegisterField(static_cast<REGISTER_ADDRESS>(result_reg), REGISTER_FIELDS::ADC_REGS::ADC_RESULT);
        if (current_status_GSR0 != 0xFF) {
            current_status_GSR0 = 0;
            return channel_result;
        }
        return 0;
    }

    // Initialize SPI inteface: Reference User Manual: 3.1.1 of BGT60LTR11AIP.
    void BGT60_DRIVER::initialize_spi_interface() {
        const uint8_t dataFieldSize = spi_config.bitsPerTransfer;

        spi_init(SPI_INST_RADAR, SPI_BAUD_RATE_RADAR);
        spi_set_format(SPI_INST_RADAR, 
            dataFieldSize, 
            SPI_CPOL_0,         // Clock low in idle.
            SPI_CPHA_0,         // CLock phase rising. 
            SPI_MSB_FIRST
        );

        gpio_set_function(spi_miso_pin, GPIO_FUNC_SPI);
        gpio_set_function(spi_clk_pin, GPIO_FUNC_SPI);
        gpio_set_function(spi_mosi_pin, GPIO_FUNC_SPI);

        // Initialize CS pin
        gpio_init(spi_select_pin);
        gpio_set_dir(spi_select_pin, GPIO_OUT);
        gpio_put(spi_select_pin, 1);

        // Reset Bus
        gpio_init(rs_line);
        gpio_set_dir(rs_line, GPIO_OUT);
        vTaskDelay(pdMS_TO_TICKS(50)); 
        gpio_put(SPI_RS_RADAR, 0);
        vTaskDelay(pdMS_TO_TICKS(50)); 
        gpio_put(SPI_RS_RADAR, 1);
    }

    void BGT60_DRIVER::spi_write_register(const uint8_t reg, const uint16_t value) {
        uint8_t data_to_send[3] = {0, 0, 0};
        
        // 7-bit address + RW bit set to 1 for write operation
        data_to_send[0] = SET_SPI_ADDR(reg, SPI_READ_BIT);

        // 16-bit payload sent MSB first
        data_to_send[1] = (value >> BYTE) & BYTE_MASK; // MSB of the value to write
        data_to_send[2] = value & BYTE_MASK;        // LSB of the value to write

        gpio_put(SPI_CS_RADAR, 0);
        spi_write_blocking(SPI_INST_RADAR, data_to_send, 3);
        gpio_put(SPI_CS_RADAR, BIT_ONE);
    }

    const uint16_t BGT60_DRIVER::spi_read_register(const uint8_t reg) {
        // 7-bit address + RW bit set to 0 for read operation
        const uint8_t spiAddr = SET_SPI_ADDR(reg, ~SPI_READ_BIT);
        const uint8_t writeBuffer[3] = {spiAddr, 0, 0};
        uint8_t received_data[3] = {0, 0, 0};

        gpio_put(SPI_CS_RADAR, 0);
        spi_write_read_blocking(SPI_INST_RADAR, writeBuffer, received_data, 3);
        gpio_put(SPI_CS_RADAR, 1); 

        current_status_GSR0 = received_data[0];

        return (static_cast<uint16_t>((received_data[1]) << BYTE) | static_cast<uint16_t>(received_data[2]));
    }

    const inline uint8_t BGT60_DRIVER::check_adc_result_flag() {
        uint16_t dummyValue = read_RegisterField(REGISTER_ADDRESS::REG56, REGISTER_FIELDS::REG56::CHIP_VERSION);
        dummyValue = 0;
        if (current_status_GSR0 == 4) {
            current_status_GSR0 = 0;
            return BIT_ONE;
        }
        
        return 0;
    }

    /* Helper functions for field manipulation*/
    const inline constexpr uint16_t BGT60_DRIVER::read_field(const uint16_t reg, const RegisterField field) {
        return ((reg  >> field.shift) & field.mask);
    }
}