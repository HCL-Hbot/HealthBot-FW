/* 
    Datasheet URL: https://www.ti.com/lit/ds/symlink/tca9534.pdf
*/
#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <hardware/i2c.h>
#include <cstdio>

namespace EXPANDER {

enum class COMMANDS : uint8_t {
    INPUT_PORT      = 0x00,
    OUTPUT_PORT     = 0x01,
    POLARITY_INV    = 0x02,
    CONFIG_PORT     = 0x03,
};

enum class DIRECTION : uint8_t {
    INPUT   = 1,
    OUTPUT  = 0,
};

enum class LEVEL : uint8_t {
    LOW     = 0,
    HIGH    = 1
};

typedef uint8_t TCA9534XXXConfig;

class TCA9534XXX {
public:
    TCA9534XXX(const TCA9534XXX&) = delete;
    TCA9534XXX& operator=(const TCA9534XXX&) = delete;

    static TCA9534XXX& getInstance(i2c_inst_t *i2c = nullptr, 
        uint8_t address = 0, uint8_t sda_pin = 0, uint8_t scl_pin = 0, uint32_t baudrate = 0) {
        static TCA9534XXX instance(i2c, address, sda_pin, scl_pin, baudrate);
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

        if (! writeRegister(COMMANDS::CONFIG_PORT, TCA9534DWRConfigInput)) {
            return false;
        }

        initialized_ = true;
        return true;
    }

    void setPin(const uint8_t pin, const LEVEL value) {
        uint8_t reg_value = readRegister(COMMANDS::OUTPUT_PORT);
        if (value == LEVEL::HIGH) {
            reg_value |= (1 << pin);
        }
        else {
            reg_value &= ~(1 << pin);
        }
        writeRegister(COMMANDS::OUTPUT_PORT, reg_value);
    }

    const LEVEL getPin(const uint8_t pin) {
        uint8_t reg_value = readRegister(COMMANDS::INPUT_PORT);
        return static_cast<LEVEL>((reg_value & (1 << pin)) != 0);
    }

    void setPinDirection(const uint8_t pin, const DIRECTION isInput) {
        uint8_t reg_value = readRegister(COMMANDS::CONFIG_PORT);
        if (isInput == DIRECTION::INPUT) {
            reg_value |= (1 << pin);
        } else {
            reg_value &= ~(1 << pin);
        }
        writeRegister(COMMANDS::CONFIG_PORT, reg_value);
    }

    void configureAllPins(const uint8_t config) {
        writeRegister(COMMANDS::CONFIG_PORT, config);
    }

private:
// Singleton constructor principle: 
    TCA9534XXX(i2c_inst_t *i2c, uint8_t address, uint8_t sda_pin, uint8_t scl_pin, uint32_t baudrate)
        : i2c_(i2c), address_(address), sda_pin_(sda_pin), scl_pin_(scl_pin), baudrate_(baudrate), initialized_(false) {}

    i2c_inst_t *i2c_;
    uint8_t address_;
    uint8_t sda_pin_;
    uint8_t scl_pin_;
    uint32_t baudrate_;
    bool initialized_;
    const TCA9534XXXConfig TCA9534DWRConfigInput = 0b00000000;

    const bool writeRegister(const COMMANDS reg, const uint8_t value) {
        const uint8_t buffer[2] = {
            static_cast<uint8_t>(reg), 
            value
        };
        const int result = i2c_write_blocking(i2c_, address_, buffer, 2, false);
        return result >= 0;
    }

    const uint8_t readRegister(const COMMANDS reg) {
        const uint8_t converted_reg = static_cast<uint8_t>(reg);
        int result = i2c_write_blocking(i2c_, address_, &converted_reg, 1, false);
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
}; // class TCA9534DWR
} // namespace EXPANDER
#endif // EXPANDER_HPP