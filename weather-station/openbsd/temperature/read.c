/*                    
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program is derived from OpenBSD sysctl(8), the latter finds the sensor
 * device by getting all sensor devices and compare the names. I keep it easy
 * and hardcoded mib[2] to what I found by running "sysctl hw.sensors".
 *
 * mib[4] is the sensor index, the calculation to obtain the value of the
 * sensor was taken directly from the sysctl(8) source. 
 */
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/sensors.h>
#include <stdio.h>

int main(void)
{
    int mib[5];
    struct sensor s;
    size_t slen = sizeof(s);

    mib[0] = CTL_HW;
    mib[1] = HW_SENSORS;
    mib[2] = 1; // sensor device
    mib[3] = SENSOR_TEMP;
    mib[4] = 0; // sensor index

    sysctl(mib, 5, &s, &slen, NULL, 0);
    printf("temperature = %.2f degC\n", (s.value - 273150000) / 1000000.0);

    exit(0);
}

