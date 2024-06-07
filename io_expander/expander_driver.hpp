#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <hardware/i2c.h>
#include <cstdio>

// Define the register addresses for the TCA9534
#define INPUT_PORT_REG      0x00
#define OUTPUT_PORT_REG     0x01
#define POLARITY_INV_REG    0x02
#define CONFIG_REG          0x03
#define INIT_COMMAND        0b11110011 // Not sure if it actually a init command.

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
