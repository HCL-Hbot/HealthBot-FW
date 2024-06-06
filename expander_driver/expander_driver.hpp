#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <hardware/i2c.h>


// Define the register addresses for the TCA9534
#define INPUT_PORT_REG 0x00
#define OUTPUT_PORT_REG 0x01
#define POLARITY_INV_REG 0x02
#define CONFIG_REG 0x03



namespace EXPANDER
{
    class TCA9534DWR
    {
    public:
        TCA9534DWR(i2c_inst_t *i2c, uint8_t address, uint8_t sda_pin, uint8_t scl_pin, uint32_t baudrate)
            : i2c_(i2c), address_(address), sda_pin_(sda_pin), scl_pin_(scl_pin), baudrate_(baudrate) {}

        bool init()
        {
            // Initialize the I2C interface
            i2c_init(i2c_, baudrate_);
            i2c_set_slave_mode(i2c_, false, address_);

            // Configure the SCL and SDA pins
            gpio_set_function(scl_pin_, GPIO_FUNC_I2C);
            gpio_set_function(sda_pin_, GPIO_FUNC_I2C);
            gpio_pull_up(scl_pin_);
            gpio_pull_up(sda_pin_);

            writeRegister(CONFIG_REG, 0b11110011); // MUX_A, MUX_B, LED_Cheeks, LED_Mouth, MD1_EN, MD2_EN to output. MD_SW1, MD_SW2 to input

            return true; // Return true if initialization succeeds, false otherwise
        }

        // Set the Motor Driver 1 enable pin
        void setMD1_EN(bool enable)
        {
            setPin(0, enable);
        }

        // Set the Motor Driver 2 enable pin
        void setMD2_EN(bool enable)
        {
            setPin(1, enable);
        }

        // Read the Motor Driver Limit Switch 1 pin
        bool MD_SW1()
        {
            return getPin(2);
        }

        // Read the Motor Driver Limit Switch 2 pin
        bool MD_SW2()
        {
            return getPin(3);
        }

        // Set the LED Mouth pin
        void setLEDMouth(bool on)
        {
            setPin(4, on);
        }

        // Set the LED Cheeks pins
        void setLEDCheeks(bool on)
        {
            setPin(5, on);
        }

    private:
        i2c_inst_t *i2c_;
        uint8_t address_;
        uint8_t sda_pin_;
        uint8_t scl_pin_;
        uint32_t baudrate_;

        void writeRegister(uint8_t reg, uint8_t value)
        {
            uint8_t buffer[2] = {reg, value};
            i2c_write_blocking(i2c_, address_, buffer, 2, false);
        }

        uint8_t readRegister(uint8_t reg)
        {
            i2c_write_blocking(i2c_, address_, &reg, 1, false);
            uint8_t value;
            i2c_read_blocking(i2c_, address_, &value, 1, false);
            return value;
        }

        void setPin(uint8_t pin, bool value)
        {
            uint8_t reg = (value ? readRegister(OUTPUT_PORT_REG) | (1 << pin) : readRegister(OUTPUT_PORT_REG) & ~(1 << pin));
            writeRegister(OUTPUT_PORT_REG, reg);
        }

        bool getPin(uint8_t pin)
        {
            return (readRegister(INPUT_PORT_REG) & (1 << pin)) != 0;
        }
    };
}

#endif // EXPANDER_HPP
