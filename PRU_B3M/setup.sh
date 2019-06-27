#!/bin/bash

# Copy the PRU firmware into a central location
# from where it can be picked up by the rproc driver.
cp gen/PRU_B3M.out /lib/firmware/am335x-pru0-fw 


# Until the device tree is setup to perform the 
# pin-mux configure the pins from the command line.
#configure the GPIO pin
config-pin P9.27 pruout

#configure the UART port
config-pin P9.24 pru_uart
config-pin P9.26 pru_uart






# Restart the PRU core
echo "stop" > /sys/class/remoteproc/remoteproc1/state
echo "start" > /sys/class/remoteproc/remoteproc1/state
