Weather station
===============

## Station
The first version of the station is be based on a Soekris Engineering net4801.
You can find details about the net4801 (which is end of life by the way) at:

* http://www.soekris.com/net4801.htm
* http://www.soekris.com/manuals/net4801_manual.pdf

Basically it's an i386 device with an AMD Geode CPU (234 MHz) and 128 MB RAM.

The weather station relies on sensors to measure temperature and other stuff.
What I'm interested in is the board's GPIO pins. It should be relatively
easy to attach sensors to these pins. 

The following is taken from the net4801 manual: JP5, User I/O. 20 pins 0.050"
header with 12 programmable general purpose input/output pins and com2 serial
port access at TTL level. They are connected directly to the PC87366 Multi-IO
controller, please see the PC87366 datasheet for programming information and
electrical specifications. When the board is viewed as on the illustration
(ethernet ports facing you), pin 1 is the top left pin, at the "JP5" text.

The pinout that is in the 4801 manual seems incorrect, you need to refer to
the pinout that is on the wiki of the "net5501 JP5 GPIO mappings" for working
values:

* http://wiki.soekris.info/Net5501_JP5_GPIO_Mappings

<pre>
+-------------+-------------+------------+-----------+-------------+
| PC87366 Pin | Function    | Pin Number | Function  | PC87366 Pin |
+-------------+-------------+-----+------+-----------+-------------+
| --          | +3.3V Power |   1 |    2 | +5V Power | --          |
| GPIO 16     | GPIO 3      |   3 |    4 | GPIO 4    | GPIO 17     |
| GPIO 18     | GPIO 5      |   5 |    6 | GPIO 6    | GPIO 19     |
| GPIO 20     | GPIO 7      |   7 |    8 | GPIO 8    | GPIO 21     |
| GPIO 22     | GPIO 9      |   9 |   10 | GPIO 10   | GPIO 23     |
| --          | GND         |  11 |   12 | GPIO 12   | GPIO 04     |
| GPIO 05     | GPIO 13     |  13 |   14 | GND       | --          |
| GPIO 11     | GPIO 15     |  15 |   16 | GPIO 16   | GPIO 16     |
| --          | GND         |  17 |   18 | RXD       | --          |
| --          | TXD         |  19 |   20 | GND       | --          |
+-------------+-------------+-----+------+-----------+-------------+
</pre>
## OpenBSD
The device is running OpenBSD 4.7. It's an old release, but that's not really
relevant for what I'm doing with it at the moment. Let's see what information
dmesg has about GPIO:

```
[root@casper ~]                                                    
# dmesg | grep -i gpio
gpio0 at gscpcib0: 64 pins
nsclpcsio0 at isa0 port 0x2e/2: NSC PC87366 rev 9: GPIO VLM TMS
gpio1 at nsclpcsio0: 29 pins
```

We don't need device gpio0. But that's awesome, the PC87366 chip is found at
gpio1. Here are some relevant manpages:

* http://www.openbsd.org/cgi-bin/man.cgi?query=gpio
* http://www.openbsd.org/cgi-bin/man.cgi?query=nsclpcsio

On OpenBSD, GPIO pins have to be enabled in securelevel lower than 1. The
default securelevel is 1, so you have to enable the pin in /etc/rc.securelevel:

```
gpioctl gpio1 17 set out pp
```

As you can see, you have to use the GPIO Pin number (17) and not the JP5 pin
number. Then, reboot. After reboot, the gpioctl command will notice the pin:

```
# gpioctl /dev/gpio1
/dev/gpio1: 1 pins
```

To test if this worked, I attached an LED to the pin. To set the LED value,
you can use the following command with parameter 'on', 'off' or 'toggle'.

```
gpioctl gpio1 17 on
```

## Sensor
The DS18B20 is a digital thermometer with a 1-Wire interface. I attached the
data pin of the thermometer to gpio pin 21 and attached a onewire(4) bus on
a gpioow(4) device on pin 21. The following was added in /etc/rc.securelevel:

```
gpioctl gpio1 attach gpioow 21 0x01
```

When I reboot the system, the device is found:

```
gpioow0 at gpio1: DATA[21] open-drain pull-up
onewire0 at gpioow0
```

And:

```
owtemp0 at onewire0 "Temperature" sn 000001b3da23
```

Reading the temperature is very easy now, you can just read it with the sysctl(8)
command. The device is listed under hw.sensors.owtemp0:

```
# sysctl hw.sensors.owtemp0
hw.sensors.owtemp0.temp0=24.19 degC (sn 000001b3da23)
```

Here is some reference material on the 1-Wire stuff:

* http://en.wikipedia.org/wiki/Onewire
* http://www.openbsd.org/cgi-bin/man.cgi?query=onewire
* http://www.openbsd.org/cgi-bin/man.cgi?query=gpioow
* http://www.openbsd.org/cgi-bin/man.cgi?query=owtemp

The exact details on how to connect the sensor's pins can be found in the
DS18B20 datasheet, which can be downloaded at:

* http://www.datasheetcatalog.org/datasheet/maxim/DS18B20.pdf

To read the temperature from a C program, you can use the sysctl(3) subroutine.
Information on how to use the routine is in the manpage:

* http://www.openbsd.org/cgi-bin/man.cgi?query=sysctl&sektion=3

Take a look at the file "openbsd/temperature/read.c" for an example.
An example to toggle a LED can be found in "openbsd/toggle/blink.c".

