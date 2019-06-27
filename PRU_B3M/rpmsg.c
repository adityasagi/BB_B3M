
#include "rpmsg.h"
#include "resource_table_0.h"


volatile register uint32_t __R31;


/**********************
 *     RPMsg
 *********************/


/*** RPMsg INITIALIZATION ***/
void RPMsgInit(struct pru_rpmsg_transport* transport) {

	volatile uint8_t *status;

	/* allow OCP master port access by the PRU so the PRU can read external memories */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

	/* clear the status of event MB_INT_NUMBER (the mailbox event) and enable the mailbox event */
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Make sure the Linux drivers are ready for RPMsg communication */
	status = &resourceTable.rpmsg_vdev.status;
	while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

	/* Initialize the RPMsg transport structure */
	pru_rpmsg_init(transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);


	/* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
	while (pru_rpmsg_channel(RPMSG_NS_CREATE, transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);

}
/*** END OF RPMsg INITIALIZATION ***/


uint8_t RPMsgRecv(struct pru_rpmsg_transport *transport, uint16_t *src, uint16_t *dst, uint8_t *payload, uint16_t *len) {

	uint8_t ret = 1;
	
	/* Check bit 30 of register R31 to see if the mailbox interrupt has occurred */
	if (__R31 & HOST_INT) {

		/* Clear the event status, event MB_INT_NUMBER corresponds to the mailbox interrupt */
		CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

		/* Receive the message */
		if (pru_rpmsg_receive(transport, src, dst, payload, len) == PRU_RPMSG_SUCCESS) {


					ret = 0;

		}
	}
	return ret;

}

inline uint8_t RPMsgSend(struct pru_rpmsg_transport *transport, uint16_t dst, uint16_t src, uint8_t *payload, uint16_t len) {

	return	pru_rpmsg_send(transport, dst, src, payload, len);

}
