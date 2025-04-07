#ifndef __USBD_CSP_H__
#define __USBD_CSP_H__

/* Port select */
#define AHB_PORT_DRAM		0x0
#define AHB_PORT_SRAM		0x1

void csp_usb_reset_controller(void);
void csp_usb_device_mode(void);
void csp_usb_full_speed(void);
void csp_usb_set_device_add(const u8 address);
u8 csp_usb_is_setup(void);
void csp_usb_clear_setup(void);
void csp_usb_set_stall(u8 ep_num);
void csp_usb_clear_stall(u8 ep_num);
u8 csp_usb_is_stall(u8 ep_num);
void csp_usb_reset_toggle(u8 ep_num);
void csp_usb_disable_endpoints(void);
void csp_usb_clear_pendings(void);
void csp_usb_set_threshold(void);
void csp_usb_set_list_address(u32 address);
void csp_usb_set_interrupt(void);
void csp_usb_config_endpoint(u8 ep_num, u8 type, u8 direction);
void csp_usb_wait_endpoint_unprimed(u8 ep_phy_num);
void csp_usb_prime_endpoint(u8 ep_phy_num);
void csp_usb_clear_complete(u32 value);
u32  csp_usb_get_int_sts(void);
void csp_usb_clear_usb_sts(u32 valid_sts);
u32 csp_usb_get_and_clear_int_active(void);
u32  csp_usb_get_setup_sts(void);
u32  cps_usb_get_complete_sts(void);
u32  csp_usb_clear_nak(void);
u8 csp_usb_is_primed(u8 ep_num);
void csp_usb_set_run_stop(u8 con);
void csp_usb_ulpi_transceiver(void);
void csp_usb_utmi_transceiver(void);
int csp_usb_check_init(void);
void csp_usb_clear_siddq(void);
void csp_usb_select_ahb_port(u8 value);
void csp_usb_init(void);
#ifdef ARCH_LOMBO_N7
void csp_usb_force_cfg(void);
#endif

#endif /* __USBD_CSP_H__ */
