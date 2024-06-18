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

#ifndef BGT60LTR11XXX_DRIVER_HPP
#define BGT60LTR11XXX_DRIVER_HPP

#include <FreeRTOS.h>
#include <task.h>

#include <vector>
#include <type_traits>  //for std::underlying_type
#include <utility>      // For std::forward
#include "hardware/spi.h"
#include "pico/stdlib.h"

#include "bgt60ltr11XXX_regs.hpp"
#include "board_defines.h"

namespace BGT60 {

class BGT60_DRIVER {
public:
    BGT60_DRIVER();
    ~BGT60_DRIVER() {}

    static void initCwMode();
    static void radarEnableSoftReset();

    // Variadic Functions: 
    template<typename... FieldValuePairs>
    static void clearWrite_MultipleRegisterFields(FieldValuePairs&&... pairs);
    template<typename... FieldValuePairs>
    static void write_MultipleRegisterFields(FieldValuePairs&&... pairs);

    static void clearWrite_MultipleFieldsInRegister(const REGISTER_ADDRESS registerAddress, 
        const std::vector<std::pair<RegisterField, uint16_t>>& fieldsValues);

    // Functions to interact with the hardware
    static void clearWrite_RegisterField(const REGISTER_ADDRESS registerAddress, 
        const RegisterField& field, const uint16_t value);

    static void write_Register(const REGISTER_ADDRESS registerAddress,
        const RegisterField& field, const uint16_t value);

    static void clear_Register(REGISTER_ADDRESS registerAddress);
    static void clear_RegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field);

    const static uint16_t read_RegisterField(REGISTER_ADDRESS registerAddress, const RegisterField& field);
    const static uint16_t read_RegisterField(uint8_t registerAddress, const RegisterField& field);

    const static uint16_t read_AdChannel(const ADC_REG_CHANNELS channel);

private:
    /* CLASS VARIABLES: */
    const static inline SPIConfig_t spi_config  = {5, 8};
    const static inline uint16_t INIT_TIMEOUT   = 0x0800; // In base10: 2048
    static inline uint16_t current_status_GSR0  = 0x0000;

    const static inline uint8_t spi_clk_pin     = SPI_SCK_RADAR;
    const static inline uint8_t spi_mosi_pin    = SPI_MOSI_RADAR; 
    const static inline uint8_t spi_miso_pin    = SPI_MISO_RADAR; 
    const static inline uint8_t spi_select_pin  =  SPI_CS_RADAR; 
    const static inline uint8_t rs_line         = SPI_RS_RADAR;

    /* CLASS FUNCTIONS: */
    // Initialize SPI inteface: Reference User Manual: 3.1.1 of BGT60LTR11AIP.
    static void initialize_spi_interface();
    static void spi_write_register(const uint8_t reg, const uint16_t value);
    const static uint16_t spi_read_register(const uint8_t reg);

    /* Helper functions for field manipulation*/
    const static inline constexpr uint16_t read_field(const uint16_t reg, const RegisterField field);
    const static inline uint8_t check_adc_result_flag();
};

} // End of BGT60 namespace. 
#endif // BGT60LTR11XXX_DRIVER_HPP