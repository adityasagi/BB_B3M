
Code to be run on the BeagleBone.
Compilation:
$ export PRU_CGT=/usr/share/ti/cgt-pru
$ export PRU_SOFTWARE_PACKAGE=/usr/lib/ti/pru-software-support-package/
$ make 

To run the code:
$ source setup.sh


The setup.sh script copies the firmware to the /lib/firmware directory
where the rproc driver finds it and loads the FW to the PRU.

UART0
------
P9.24 : Tx
P9.26 : Rx

P9.27 : EN 
