#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <hardware/i2c.h>
#include <cstdio>
// https://www.ti.com/lit/ds/symlink/tca9534.pdf?ts=1717841612499&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTCA9534%252Fpart-details%252FTCA9534DWR%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Docb-tistore-promo-app_opn_en-cpc-storeic-google-eu%2526utm_content%253DDevice%2526ds_k%253DTCA9534DWR%2526DCM%253Dyes%2526gad_source%253D1%2526gclid%253DCjwKCAjwmYCzBhA6EiwAxFwfgJVm_uUhwJaAEAKAOJb5Y71rkLtJ0pM1oBkuKKFtShp-oto-MnDVXxoCTzwQAvD_BwE%2526gclsrc%253Daw.ds
// Define the register addresses for the TCA9534

// Verified: 2021-09-15 from datasheet
#define INPUT_PORT_REG      0x00
#define OUTPUT_PORT_REG     0x01
#define POLARITY_INV_REG    0x02
#define CONFIG_REG          0x03

#define INIT_COMMAND        0b0 // Not sure if it actually a init command.

namespace EXPANDER {

class TCA9534DWR {
public:
    // Deleted copy constructor and assignment operator to prevent copying
    TCA9534DWR(const TCA9534DWR&) = delete;
    TCA9534DWR& operator=(const TCA9534DWR&) = delete;

    static TCA9534DWR& getInstance(i2c_inst_t *i2c = nullptr, 
        uint8_t address = 0, uint8_t sda_pin = 0, uint8_t scl_pin = 0, uint32_t baudrate = 0) {
        static TCA9534DWR instance(i2c, address, sda_pin, scl_pin, baudrate);
        return instance;
    }

    const bool init() {
        if (initialized_) return true;

        if (i2c_init(i2c_, baudrate_) < 0) {
            return false;
        }

        i2c_set_slave_mode(i2c_, false, address_);
        gpio_set_function(scl_pin_, GPIO_FUNC_I2C);
        gpio_set_function(sda_pin_, GPIO_FUNC_I2C);
        gpio_pull_up(scl_pin_);
        gpio_pull_up(sda_pin_);

        if (! writeRegister(CONFIG_REG, INIT_COMMAND)) {
            return false;
        }

        initialized_ = true;
        return true;
    }

    void setPin(const uint8_t pin, const bool value) {
        uint8_t reg_value = readRegister(OUTPUT_PORT_REG);
        if (value) {
            reg_value |= (1 << pin);
        }
        else {
            reg_value &= ~(1 << pin);
        }
        writeRegister(OUTPUT_PORT_REG, reg_value);
    }

    const bool getPin(const uint8_t pin) {
        uint8_t reg_value = readRegister(INPUT_PORT_REG);
        return (reg_value & (1 << pin)) != 0;
    }

private:
// Singleton constructor principle: 
    TCA9534DWR(i2c_inst_t *i2c, uint8_t address, uint8_t sda_pin, uint8_t scl_pin, uint32_t baudrate)
        : i2c_(i2c), address_(address), sda_pin_(sda_pin), scl_pin_(scl_pin), baudrate_(baudrate), initialized_(false) {}

    i2c_inst_t *i2c_;
    uint8_t address_;
    uint8_t sda_pin_;
    uint8_t scl_pin_;
    uint32_t baudrate_;
    bool initialized_;

    const bool writeRegister(const uint8_t reg, const uint8_t value) {
        uint8_t buffer[2] = {reg, value};
        int result = i2c_write_blocking(i2c_, address_, buffer, 2, false);
        return result >= 0;
    }

    const uint8_t readRegister(const uint8_t reg) {
        int result = i2c_write_blocking(i2c_, address_, &reg, 1, false);
        uint8_t value;

        if (result < 0) {
            printf("Failed to write register address\n");
            return 0;
        }

        result = i2c_read_blocking(i2c_, address_, &value, 1, false);
        
        if (result < 0)
        {
            printf("Failed to read register value\n");
            return 0;
        }
        return value;
    }
};
}

#endif // EXPANDER_HPP
