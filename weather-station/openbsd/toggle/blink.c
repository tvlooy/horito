/*
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/gpio.h>
#include <fcntl.h>

int main(void)
{
    int device;
    static struct gpio_pin_op op;

    device = open("/dev/gpio1", O_RDWR);
    op.gp_pin = 17;
    ioctl(device, GPIOPINTOGGLE, &op);
    close(device);

    exit(0);
}

