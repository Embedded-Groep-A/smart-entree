#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-1"  // I2C bus on Raspberry Pi (use "/dev/i2c-0" for older models)
#define I2C_ADDR 0x08         // STM32 I2C slave address

int main() {
    int fd;
    char buffer[1];

    // Open I2C bus
    if ((fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the I2C bus");
        return 1;
    }

    // Specify the I2C slave device
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
        perror("Failed to set I2C address");
        close(fd);
        return 1;
    }

    // Send a command byte (e.g., request sensor data)
    buffer[0] = 0x01;  // Example command
    if (write(fd, buffer, 1) != 1) {
        perror("Failed to write to I2C device");
        close(fd);
        return 1;
    }
    printf("Sent command: 0x%02X\n", buffer[0]);

    // Small delay to allow the STM32 to process the request
    usleep(10000);

    // Read response (1 byte)
    if (read(fd, buffer, 1) != 1) {
        perror("Failed to read from I2C device");
        close(fd);
        return 1;
    }
    printf("Received data: 0x%02X (%d)\n", buffer[0], buffer[0]);

    close(fd);
    return 0;
}
