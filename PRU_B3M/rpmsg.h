#ifndef RPMSG_H
#define RPMSG_H

#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_virtqueue.h>
#include <pru_rpmsg.h>
#include <sys_mailbox.h>

/* Host-0 Interrupt sets bit 30 in register R31 */
#define HOST_INT			((uint32_t) 1 << 30)

/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST			16
#define FROM_ARM_HOST			17

/*
 * Using the name 'rpmsg-pru' will probe the rpmsg_pru driver found
 * at linux-x.y.z/drivers/rpmsg/rpmsg_pru.c
 */
#define CHAN_NAME					"rpmsg-pru"
#define CHAN_DESC					"Channel 30"
#define CHAN_PORT					30

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4


void RPMsgInit(struct pru_rpmsg_transport*);

uint8_t RPMsgRecv(struct pru_rpmsg_transport *transport, uint16_t *src, uint16_t *dst, uint8_t *payload, uint16_t *len); 

inline uint8_t RPMsgSend(struct pru_rpmsg_transport *transport, uint16_t dst, uint16_t src, uint8_t *payload, uint16_t len);

#endif // RPMSG_H
