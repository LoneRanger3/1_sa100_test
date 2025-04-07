#include "soc_define.h"
#include "csp.h"
#include "usb.h"
#include "usbd_const.h"

#define PHY2POS(n)	(((n)/2) + ((n)%2 ? 16 : 0))

void csp_usb_reset_controller(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;

	/* reset  controller */
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.rst = USB_SET_RESET;
	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);

	/* wait for completion */
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	while (USB_RESET_DONE != usb_cmd_reg.bits.rst)
		usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
}

void csp_usb_device_mode(void)
{
	reg_usb_usbmode_t usb_mode_reg;
	reg_usb_otgsc_t otgsc_reg;

	/* set device mode */
	usb_mode_reg.val = READREG32(VA_USB_USBMODE);
	usb_mode_reg.bits.cm = USBMODE_DEVICE;
	usb_mode_reg.bits.alp = AUTO_LOWER_POWER_D;
	WRITEREG32(VA_USB_USBMODE, usb_mode_reg.val);

	otgsc_reg.val = READREG32(VA_USB_OTGSC);
	otgsc_reg.bits.ot = OTG_TERMINATION;
	WRITEREG32(VA_USB_OTGSC, otgsc_reg.val);
}

void csp_usb_full_speed(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pfsc = FORCE_FULL_SPEED;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_set_device_add(const u8 address)
{
	/* attention: must set address first, then set advance address */
	reg_usb_deviceaddr_t dev_addr_reg;
	dev_addr_reg.val = READREG32(VA_USB_DEVICEADDR);
	dev_addr_reg.bits.usbadr = address;
	WRITEREG32(VA_USB_DEVICEADDR, dev_addr_reg.val);

	dev_addr_reg.val = READREG32(VA_USB_DEVICEADDR);
	dev_addr_reg.bits.usbadra = ADVANCE_DEV_ADD;
	WRITEREG32(VA_USB_DEVICEADDR, dev_addr_reg.val);
}

u8 csp_usb_is_setup(void)
{
	reg_usb_eptstpstat_t ep_setup_reg;
	ep_setup_reg.val = READREG32(VA_USB_EPTSTPSTAT);
	if (ep_setup_reg.bits.eptstpstat)
		return 1;
	else
		return 0;
}

void csp_usb_clear_setup(void)
{
	reg_usb_eptstpstat_t ep_setup_reg;
	ep_setup_reg.val = READREG32(VA_USB_EPTSTPSTAT);
	WRITEREG32(VA_USB_EPTSTPSTAT, ep_setup_reg.val);
}

void csp_usb_set_stall(u8 ep_num)
{
	u32 addr;
	reg_usb_endptctrl0_t ep_ctrl_reg;
	addr = VA_USB_ENDPTCTRL0 + EP_ADDR_SETP * ep_num;

	ep_ctrl_reg.val = READREG32(addr);
	ep_ctrl_reg.bits.rxs = ENDPT_RX_STALL;
	ep_ctrl_reg.bits.txs = ENDPT_TX_STALL;
	WRITEREG32(addr, ep_ctrl_reg.val);
}

void csp_usb_clear_stall(u8 ep_num)
{
	u32 addr;
	reg_usb_endptctrl0_t ep_ctrl_reg;
	addr = VA_USB_ENDPTCTRL0 + EP_ADDR_SETP * ep_num;

	ep_ctrl_reg.val = READREG32(addr);
	ep_ctrl_reg.bits.rxs = ENDPT_RX_RUN;
	ep_ctrl_reg.bits.txs = ENDPT_TX_RUN;
	WRITEREG32(addr, ep_ctrl_reg.val);
}

u8 csp_usb_is_stall(u8 ep_num)
{
	u32 addr;
	reg_usb_endptctrl0_t ep_ctrl_reg;
	addr = VA_USB_ENDPTCTRL0 + EP_ADDR_SETP * ep_num;

	ep_ctrl_reg.val = READREG32(addr);
	if ((ep_ctrl_reg.bits.rxs == ENDPT_RX_STALL) ||
		(ep_ctrl_reg.bits.txs == ENDPT_TX_STALL))
		return 1;
	else
		return 0;
}

void csp_usb_reset_toggle(u8 ep_num)
{
	u32 addr;
	reg_usb_endptctrl1_t ep_ctrl_reg;
	addr = VA_USB_ENDPTCTRL1 + EP_ADDR_SETP * (ep_num - 1);

	ep_ctrl_reg.val = READREG32(addr);
	ep_ctrl_reg.bits.rxr = ENDPT_RX_RESET_TOGGLE;
	ep_ctrl_reg.bits.txr = ENDPT_TX_RESET_TOGGLE;
	WRITEREG32(addr, ep_ctrl_reg.val);
}

void csp_usb_disable_endpoints(void)
{
	reg_usb_endptctrl0_t ep_ctrl0_reg;
	reg_usb_endptctrl1_t ep_ctrl1_reg;
	reg_usb_endptctrl2_t ep_ctrl2_reg;
	reg_usb_endptctrl3_t ep_ctrl3_reg;

	ep_ctrl0_reg.val = READREG32(VA_USB_ENDPTCTRL0);
	ep_ctrl0_reg.bits.rxe = ENDPT_RX_DISABLE;
	ep_ctrl0_reg.bits.txe = ENDPT_TX_DISABLE;
	WRITEREG32(VA_USB_ENDPTCTRL0, ep_ctrl0_reg.val);

	ep_ctrl1_reg.val = READREG32(VA_USB_ENDPTCTRL1);
	ep_ctrl1_reg.bits.rxe = ENDPT_RX_DISABLE;
	ep_ctrl1_reg.bits.txe = ENDPT_TX_DISABLE;
	WRITEREG32(VA_USB_ENDPTCTRL1, ep_ctrl1_reg.val);

	ep_ctrl2_reg.val = READREG32(VA_USB_ENDPTCTRL2);
	ep_ctrl2_reg.bits.rxe = ENDPT_RX_DISABLE;
	ep_ctrl2_reg.bits.txe = ENDPT_TX_DISABLE;
	WRITEREG32(VA_USB_ENDPTCTRL2, ep_ctrl2_reg.val);

	ep_ctrl3_reg.val = READREG32(VA_USB_ENDPTCTRL3);
	ep_ctrl3_reg.bits.rxe = ENDPT_RX_DISABLE;
	ep_ctrl3_reg.bits.txe = ENDPT_TX_DISABLE;
	WRITEREG32(VA_USB_ENDPTCTRL3, ep_ctrl3_reg.val);
}

void csp_usb_clear_pendings(void)
{
	reg_usb_endptnak_t ep_nak_reg;
	reg_usb_endptnaken_t ep_nak_en_reg;
	reg_usb_usbsts_t usb_sts_reg;
	reg_usb_eptstpstat_t ep_setup_reg;
	reg_usb_epcompl_t ep_compl_reg;
	reg_usb_endptprime_t ep_prime_reg;
	reg_usb_endptflush_t ep_flush_reg;

	ep_nak_reg.val = CLREAR_ALL_NAK;
	WRITEREG32(VA_USB_ENDPTNAK, ep_nak_reg.val);

	ep_nak_en_reg.val = DISABLE_ALL_NAK;
	WRITEREG32(VA_USB_ENDPTNAKEN, ep_nak_en_reg.val);

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	WRITEREG32(VA_USB_USBSTS, usb_sts_reg.val);

	ep_setup_reg.val = READREG32(VA_USB_EPTSTPSTAT);
	WRITEREG32(VA_USB_EPTSTPSTAT, ep_setup_reg.val);

	ep_compl_reg.val = READREG32(VA_USB_EPCOMPL);
	WRITEREG32(VA_USB_EPCOMPL, ep_compl_reg.val);

	ep_prime_reg.val = READREG32(VA_USB_ENDPTPRIME);
	while (ENDPT_NO_PRIME != ep_prime_reg.val)
		ep_prime_reg.val = READREG32(VA_USB_ENDPTPRIME);

	ep_flush_reg.val = FLUSH_ALL_ENDPT;
	WRITEREG32(VA_USB_ENDPTFLUSH, ep_flush_reg.val);
	ep_flush_reg.val = READREG32(VA_USB_ENDPTFLUSH);
	while (FLUSH_ENDPT_DONE != ep_flush_reg.val)
		ep_flush_reg.val = READREG32(VA_USB_ENDPTFLUSH);
}

void csp_usb_set_threshold(void)
{
	reg_usb_usbcmd_t usb_cmd_reg;

	/* set the interrupt threshold control interval to 0 */
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	usb_cmd_reg.bits.itc = ZERO_INT_THRESHOLD;
	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
}

void csp_usb_set_list_address(u32 address)
{
	reg_usb_endpointlistaddr_t ep_list_addr_reg;

	/* make sure it in on 64 byte boundary !!! */
	ep_list_addr_reg.val = address;
	WRITEREG32(VA_USB_ENDPOINTLISTADDR, ep_list_addr_reg.val);
}

void csp_usb_set_interrupt(void)
{
	reg_usb_usbintr_t usb_intr_reg;

	/* enable interrupt : USB interrupt, error, port change, reset, suspend, NAK */
	usb_intr_reg.val = READREG32(VA_USB_USBINTR);
	usb_intr_reg.bits.ue = USB_INT_ENABLE;
	usb_intr_reg.bits.uee = USB_ERR_ENABLE;
	usb_intr_reg.bits.pce = PORT_CHANGE_ENABLE;
	usb_intr_reg.bits.uri = RESET_INT_ENABLE;
	/* usb_intr_reg.bits.sre = SOF_INT_ENABLE; */
	usb_intr_reg.bits.sle = DCSUSPEND_INT_ENABLE;
	usb_intr_reg.bits.nake = NAK_INT_ENABLE;
	WRITEREG32(VA_USB_USBINTR, usb_intr_reg.val);
}

void csp_usb_config_endpoint(u8 ep_num, u8 type, u8 direction)
{
	u32 addr;
	reg_usb_endptctrl0_t ep_ctrl0_reg;
	reg_usb_endptctrl1_t ep_ctrlx_reg;

	if (ep_num != 0) {
		addr = VA_USB_ENDPTCTRL1 + EP_ADDR_SETP * (ep_num - 1);
		ep_ctrlx_reg.val = READREG32(addr);

		if (direction == ENDPOINT_DIR_OUT) {
			ep_ctrlx_reg.bits.rxs = ENDPT_RX_RUN;
			ep_ctrlx_reg.bits.rxd = ENDPT_RX_SOURCE;
			ep_ctrlx_reg.bits.rxt = type;
			ep_ctrlx_reg.bits.rxi = ENDPT_RX_TOGGLE;
			ep_ctrlx_reg.bits.rxr = ENDPT_RX_RESET_TOGGLE;
			ep_ctrlx_reg.bits.rxe = ENDPT_RX_ENABLE;
		} else {
			ep_ctrlx_reg.bits.txs = ENDPT_TX_RUN;
			ep_ctrlx_reg.bits.txd = ENDPT_TX_SOURCE;
			ep_ctrlx_reg.bits.txt = type;
			ep_ctrlx_reg.bits.txi = ENDPT_TX_TOGGLE;
			ep_ctrlx_reg.bits.txr = ENDPT_TX_RESET_TOGGLE;
			ep_ctrlx_reg.bits.txe = ENDPT_TX_ENABLE;
		}

		WRITEREG32(addr, ep_ctrlx_reg.val);
	} else {
		ep_ctrl0_reg.val = READREG32(VA_USB_ENDPTCTRL0);

		if (direction == ENDPOINT_DIR_OUT) {
			ep_ctrl0_reg.bits.rxs = ENDPT_RX_RUN;
			ep_ctrl0_reg.bits.rxt = ENDPT_RX_CONTROL;
			ep_ctrl0_reg.bits.rxe = ENDPT_RX_ENABLE;
		} else {
			ep_ctrl0_reg.bits.txs = ENDPT_TX_RUN;
			ep_ctrl0_reg.bits.txt = ENDPT_TX_CONTROL;
			ep_ctrl0_reg.bits.txe = ENDPT_TX_ENABLE;
		}

		WRITEREG32(VA_USB_ENDPTCTRL0, ep_ctrl0_reg.val);
	}
}

void csp_usb_wait_endpoint_unprimed(u8 ep_phy_num)
{
	reg_usb_endptstatus_t ep_status_reg;
	u32 ep_position_val;

	ep_status_reg.val = READREG32(VA_USB_ENDPTSTATUS);
	ep_position_val = BIT(PHY2POS(ep_phy_num));

	while (ep_status_reg.val & ep_position_val)
		ep_status_reg.val = READREG32(VA_USB_ENDPTSTATUS);
}

void csp_usb_prime_endpoint(u8 ep_phy_num)
{
	reg_usb_endptprime_t ep_prime_reg;

	ep_prime_reg.val = BIT(PHY2POS(ep_phy_num));
	WRITEREG32(VA_USB_ENDPTPRIME, ep_prime_reg.val);
	dsb();
}

void csp_usb_clear_complete(u32 value)
{
	WRITEREG32(VA_USB_EPCOMPL, value);
}

u32 csp_usb_get_int_sts(void)
{
	reg_usb_usbsts_t usb_sts_reg;
	reg_usb_usbintr_t usb_intr_reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	usb_intr_reg.val = READREG32(VA_USB_USBINTR);

	return usb_sts_reg.val & usb_intr_reg.val;
}

void csp_usb_clear_usb_sts(u32 valid_sts)
{
	reg_usb_usbsts_t usb_sts_reg;

	usb_sts_reg.val = valid_sts;
	WRITEREG32(VA_USB_USBSTS, usb_sts_reg.val);
}

u32 csp_usb_get_and_clear_int_active(void)
{
	reg_usb_usbsts_t usb_sts_reg;
	reg_usb_usbintr_t usb_intr_reg;
	u32 reg;

	usb_sts_reg.val = READREG32(VA_USB_USBSTS);
	usb_intr_reg.val = READREG32(VA_USB_USBINTR);
	reg = usb_sts_reg.val & usb_intr_reg.val;
	WRITEREG32(VA_USB_USBSTS, reg);

	return reg;
}

u32 csp_usb_get_setup_sts(void)
{
	reg_usb_eptstpstat_t ep_setup_reg;

	ep_setup_reg.val = READREG32(VA_USB_EPTSTPSTAT);
	return ep_setup_reg.val;
}

u32 cps_usb_get_complete_sts(void)
{
	reg_usb_epcompl_t ep_compl_reg;

	ep_compl_reg.val = READREG32(VA_USB_EPCOMPL);
	return ep_compl_reg.val;
}

u32 csp_usb_clear_nak(void)
{
	u32 valid_nak;
	reg_usb_endptnak_t ep_nak_reg;
	reg_usb_endptnaken_t ep_nak_en_reg;

	ep_nak_reg.val = READREG32(VA_USB_ENDPTNAK);
	ep_nak_en_reg.val = READREG32(VA_USB_ENDPTNAKEN);
	valid_nak = ep_nak_reg.val & ep_nak_en_reg.val;
	WRITEREG32(VA_USB_ENDPTNAK, valid_nak);

	return valid_nak;
}

u8 csp_usb_is_primed(u8 ep_num)
{
	reg_usb_endptstatus_t ep_sts_reg;

	ep_sts_reg.val = READREG32(VA_USB_ENDPTSTATUS);

	return ep_sts_reg.val & BIT(PHY2POS(ep_num));
}

void csp_usb_set_run_stop(u8 con)
{
	reg_usb_usbcmd_t usb_cmd_reg;
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);

	if (con)
		usb_cmd_reg.bits.rs = USB_RUN;
	else
		usb_cmd_reg.bits.rs = USB_STOP;

	WRITEREG32(VA_USB_USBCMD, usb_cmd_reg.val);
}

void csp_usb_ulpi_transceiver(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pts2 = PTS2_OTHER;
	portsc1_reg.bits.pts = PTS_ULPI;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

void csp_usb_utmi_transceiver(void)
{
	reg_usb_portsc1_t portsc1_reg;

	portsc1_reg.val = READREG32(VA_USB_PORTSC1);
	portsc1_reg.bits.pts2 = PTS2_OTHER;
	portsc1_reg.bits.pts = PTS_UTMI_UTMIP;
	WRITEREG32(VA_USB_PORTSC1, portsc1_reg.val);
}

#ifdef ARCH_LOMBO_N7
void csp_usb_force_cfg(void)
{
	reg_usb_usbdbg_t usbdbg;
	reg_usb_phycfg_t phycfg;

	/* for device: using internal vbus valid signal due to usb device must
	* see a valid vbus signal before it runs and normally vbus pin has no use
	*/
	/* TODO: using internal vbus valid is valid in FPGA PLAT? have to verify */
	/* force select internal vbus valid signal */
	phycfg.val = READREG32(VA_USB_PHYCFG);
	phycfg.bits.vbusvldextsel = 1;
	phycfg.bits.vbusvldext = 1;
	WRITEREG32(VA_USB_PHYCFG, phycfg.val);

	/* force a/b valid, for id to 1(device) */
	usbdbg.val = READREG32(VA_USB_USBDBG);
	usbdbg.bits.f_id = 3;
	usbdbg.bits.f_avalid = 3;
	usbdbg.bits.f_bvalid = 3;
	WRITEREG32(VA_USB_USBDBG, usbdbg.val);
}
#endif

int csp_usb_check_init(void)
{
	reg_usb_usbmode_t usb_mode_reg;
	reg_usb_usbcmd_t usb_cmd_reg;

	/* check auto low power */
	usb_mode_reg.val = READREG32(VA_USB_USBMODE);
	if (usb_mode_reg.bits.alp != AUTO_LOWER_POWER_D)
		return -1;

	/* check whether the controller is running */
	usb_cmd_reg.val = READREG32(VA_USB_USBCMD);
	if (usb_cmd_reg.bits.rs != USB_RUN)
		return -1;

	return 0;
}

/* deassert phy siddq reset */
void csp_usb_clear_siddq(void)
{
	reg_usb_phycfg_t usb_phy_reg;

	usb_phy_reg.val = READREG32(VA_USB_PHYCFG);
	usb_phy_reg.bits.siddq = SIDDQ_CLEAR;
	WRITEREG32(VA_USB_PHYCFG, usb_phy_reg.val);
}

/* set AHB port to sdram or sram */
void csp_usb_select_ahb_port(u8 value)
{
	reg_usb_usbctrl_t usb_ctrl_reg;

	usb_ctrl_reg.val = READREG32(VA_USB_USBCTRL);
	usb_ctrl_reg.bits.mhport_sel = value;
	WRITEREG32(VA_USB_USBCTRL, usb_ctrl_reg.val);
}
