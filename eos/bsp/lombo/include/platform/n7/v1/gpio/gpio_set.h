/* gpio_set.h */

#ifndef ___GPIO__SET___H___
#define ___GPIO__SET___H___

#ifdef DEF_SET_GPIO_GPIOC_VER
void set_gpio_gpioc_ver(u32 reg_addr,
			u32 ver_l,
			u32 ver_h,
			u32 comp,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_VER_VER_L
def_set_mod_reg_bit(gpio, gpioc_ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_VER_VER_H
def_set_mod_reg_bit(gpio, gpioc_ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_VER_COMP
def_set_mod_reg_bit(gpio, gpioc_ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0
void set_gpio_gpioa_func_r0(u32 reg_addr,
				u32 gpioa0,
				u32 gpioa1,
				u32 gpioa2,
				u32 gpioa3,
				u32 gpioa4,
				u32 gpioa5,
				u32 gpioa6,
				u32 gpioa7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA0
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA1
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA2
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA3
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA4
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA5
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA6
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R0_GPIOA7
def_set_mod_reg_bit(gpio, gpioa_func_r0, gpioa7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1
void set_gpio_gpioa_func_r1(u32 reg_addr,
				u32 gpioa8,
				u32 gpioa9,
				u32 gpioa10,
				u32 gpioa11,
				u32 gpioa12,
				u32 gpioa13,
				u32 gpioa14,
				u32 gpioa15,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA8
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA9
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA10
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA11
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA12
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA13
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA14
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_FUNC_R1_GPIOA15
def_set_mod_reg_bit(gpio, gpioa_func_r1, gpioa15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0
void set_gpio_gpioa_pud_r0(u32 reg_addr,
			u32 gpioa0,
			u32 gpioa1,
			u32 gpioa2,
			u32 gpioa3,
			u32 gpioa4,
			u32 gpioa5,
			u32 gpioa6,
			u32 gpioa7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA0
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA1
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA2
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA3
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA4
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA5
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA6
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R0_GPIOA7
def_set_mod_reg_bit(gpio, gpioa_pud_r0, gpioa7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1
void set_gpio_gpioa_pud_r1(u32 reg_addr,
			u32 gpioa8,
			u32 gpioa9,
			u32 gpioa10,
			u32 gpioa11,
			u32 gpioa12,
			u32 gpioa13,
			u32 gpioa14,
			u32 gpioa15,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA8
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA9
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA10
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA11
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA12
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA13
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA14
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PUD_R1_GPIOA15
def_set_mod_reg_bit(gpio, gpioa_pud_r1, gpioa15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0
void set_gpio_gpioa_drv_r0(u32 reg_addr,
			u32 gpioa0,
			u32 gpioa1,
			u32 gpioa2,
			u32 gpioa3,
			u32 gpioa4,
			u32 gpioa5,
			u32 gpioa6,
			u32 gpioa7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA0
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA1
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA2
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA3
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA4
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA5
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA6
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R0_GPIOA7
def_set_mod_reg_bit(gpio, gpioa_drv_r0, gpioa7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1
void set_gpio_gpioa_drv_r1(u32 reg_addr,
			u32 gpioa8,
			u32 gpioa9,
			u32 gpioa10,
			u32 gpioa11,
			u32 gpioa12,
			u32 gpioa13,
			u32 gpioa14,
			u32 gpioa15,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA8
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA9
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA10
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA11
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA12
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA13
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA14
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DRV_R1_GPIOA15
def_set_mod_reg_bit(gpio, gpioa_drv_r1, gpioa15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DATA
void set_gpio_gpioa_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_DATA_DAT
def_set_mod_reg_bit(gpio, gpioa_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PR_R
void set_gpio_gpioa_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOA_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpioa_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R0
void set_gpio_gpiob_func_r0(u32 reg_addr,
				u32 gpiob2,
				u32 gpiob3,
				u32 gpiob4,
				u32 gpiob5,
				u32 gpiob6,
				u32 gpiob7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R0_GPIOB2
def_set_mod_reg_bit(gpio, gpiob_func_r0, gpiob2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R0_GPIOB3
def_set_mod_reg_bit(gpio, gpiob_func_r0, gpiob3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R0_GPIOB4
def_set_mod_reg_bit(gpio, gpiob_func_r0, gpiob4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R0_GPIOB5
def_set_mod_reg_bit(gpio, gpiob_func_r0, gpiob5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R0_GPIOB6
def_set_mod_reg_bit(gpio, gpiob_func_r0, gpiob6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R0_GPIOB7
def_set_mod_reg_bit(gpio, gpiob_func_r0, gpiob7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R1
void set_gpio_gpiob_func_r1(u32 reg_addr,
				u32 gpiob10,
				u32 gpiob11,
				u32 gpiob12,
				u32 gpiob13,
				u32 gpiob14,
				u32 gpiob15,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R1_GPIOB10
def_set_mod_reg_bit(gpio, gpiob_func_r1, gpiob10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R1_GPIOB11
def_set_mod_reg_bit(gpio, gpiob_func_r1, gpiob11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R1_GPIOB12
def_set_mod_reg_bit(gpio, gpiob_func_r1, gpiob12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R1_GPIOB13
def_set_mod_reg_bit(gpio, gpiob_func_r1, gpiob13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R1_GPIOB14
def_set_mod_reg_bit(gpio, gpiob_func_r1, gpiob14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R1_GPIOB15
def_set_mod_reg_bit(gpio, gpiob_func_r1, gpiob15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R2
void set_gpio_gpiob_func_r2(u32 reg_addr,
				u32 gpiob18,
				u32 gpiob19,
				u32 gpiob20,
				u32 gpiob21,
				u32 gpiob22,
				u32 gpiob23,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R2_GPIOB18
def_set_mod_reg_bit(gpio, gpiob_func_r2, gpiob18, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R2_GPIOB19
def_set_mod_reg_bit(gpio, gpiob_func_r2, gpiob19, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R2_GPIOB20
def_set_mod_reg_bit(gpio, gpiob_func_r2, gpiob20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R2_GPIOB21
def_set_mod_reg_bit(gpio, gpiob_func_r2, gpiob21, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R2_GPIOB22
def_set_mod_reg_bit(gpio, gpiob_func_r2, gpiob22, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R2_GPIOB23
def_set_mod_reg_bit(gpio, gpiob_func_r2, gpiob23, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R3
void set_gpio_gpiob_func_r3(u32 reg_addr,
				u32 gpiob24,
				u32 gpiob25,
				u32 gpiob26,
				u32 gpiob27,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R3_GPIOB24
def_set_mod_reg_bit(gpio, gpiob_func_r3, gpiob24, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R3_GPIOB25
def_set_mod_reg_bit(gpio, gpiob_func_r3, gpiob25, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R3_GPIOB26
def_set_mod_reg_bit(gpio, gpiob_func_r3, gpiob26, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_FUNC_R3_GPIOB27
def_set_mod_reg_bit(gpio, gpiob_func_r3, gpiob27, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R0
void set_gpio_gpiob_pud_r0(u32 reg_addr,
			u32 gpiob2,
			u32 gpiob3,
			u32 gpiob4,
			u32 gpiob5,
			u32 gpiob6,
			u32 gpiob7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R0_GPIOB2
def_set_mod_reg_bit(gpio, gpiob_pud_r0, gpiob2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R0_GPIOB3
def_set_mod_reg_bit(gpio, gpiob_pud_r0, gpiob3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R0_GPIOB4
def_set_mod_reg_bit(gpio, gpiob_pud_r0, gpiob4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R0_GPIOB5
def_set_mod_reg_bit(gpio, gpiob_pud_r0, gpiob5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R0_GPIOB6
def_set_mod_reg_bit(gpio, gpiob_pud_r0, gpiob6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R0_GPIOB7
def_set_mod_reg_bit(gpio, gpiob_pud_r0, gpiob7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R1
void set_gpio_gpiob_pud_r1(u32 reg_addr,
			u32 gpiob10,
			u32 gpiob11,
			u32 gpiob12,
			u32 gpiob13,
			u32 gpiob14,
			u32 gpiob15,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R1_GPIOB10
def_set_mod_reg_bit(gpio, gpiob_pud_r1, gpiob10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R1_GPIOB11
def_set_mod_reg_bit(gpio, gpiob_pud_r1, gpiob11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R1_GPIOB12
def_set_mod_reg_bit(gpio, gpiob_pud_r1, gpiob12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R1_GPIOB13
def_set_mod_reg_bit(gpio, gpiob_pud_r1, gpiob13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R1_GPIOB14
def_set_mod_reg_bit(gpio, gpiob_pud_r1, gpiob14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R1_GPIOB15
def_set_mod_reg_bit(gpio, gpiob_pud_r1, gpiob15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R2
void set_gpio_gpiob_pud_r2(u32 reg_addr,
			u32 gpiob18,
			u32 gpiob19,
			u32 gpiob20,
			u32 gpiob21,
			u32 gpiob22,
			u32 gpiob23,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R2_GPIOB18
def_set_mod_reg_bit(gpio, gpiob_pud_r2, gpiob18, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R2_GPIOB19
def_set_mod_reg_bit(gpio, gpiob_pud_r2, gpiob19, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R2_GPIOB20
def_set_mod_reg_bit(gpio, gpiob_pud_r2, gpiob20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R2_GPIOB21
def_set_mod_reg_bit(gpio, gpiob_pud_r2, gpiob21, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R2_GPIOB22
def_set_mod_reg_bit(gpio, gpiob_pud_r2, gpiob22, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R2_GPIOB23
def_set_mod_reg_bit(gpio, gpiob_pud_r2, gpiob23, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R3
void set_gpio_gpiob_pud_r3(u32 reg_addr,
			u32 gpiob24,
			u32 gpiob25,
			u32 gpiob26,
			u32 gpiob27,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R3_GPIOB24
def_set_mod_reg_bit(gpio, gpiob_pud_r3, gpiob24, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R3_GPIOB25
def_set_mod_reg_bit(gpio, gpiob_pud_r3, gpiob25, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R3_GPIOB26
def_set_mod_reg_bit(gpio, gpiob_pud_r3, gpiob26, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PUD_R3_GPIOB27
def_set_mod_reg_bit(gpio, gpiob_pud_r3, gpiob27, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R0
void set_gpio_gpiob_drv_r0(u32 reg_addr,
			u32 gpiob2,
			u32 gpiob3,
			u32 gpiob4,
			u32 gpiob5,
			u32 gpiob6,
			u32 gpiob7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R0_GPIOB2
def_set_mod_reg_bit(gpio, gpiob_drv_r0, gpiob2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R0_GPIOB3
def_set_mod_reg_bit(gpio, gpiob_drv_r0, gpiob3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R0_GPIOB4
def_set_mod_reg_bit(gpio, gpiob_drv_r0, gpiob4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R0_GPIOB5
def_set_mod_reg_bit(gpio, gpiob_drv_r0, gpiob5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R0_GPIOB6
def_set_mod_reg_bit(gpio, gpiob_drv_r0, gpiob6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R0_GPIOB7
def_set_mod_reg_bit(gpio, gpiob_drv_r0, gpiob7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R1
void set_gpio_gpiob_drv_r1(u32 reg_addr,
			u32 gpiob10,
			u32 gpiob11,
			u32 gpiob12,
			u32 gpiob13,
			u32 gpiob14,
			u32 gpiob15,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R1_GPIOB10
def_set_mod_reg_bit(gpio, gpiob_drv_r1, gpiob10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R1_GPIOB11
def_set_mod_reg_bit(gpio, gpiob_drv_r1, gpiob11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R1_GPIOB12
def_set_mod_reg_bit(gpio, gpiob_drv_r1, gpiob12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R1_GPIOB13
def_set_mod_reg_bit(gpio, gpiob_drv_r1, gpiob13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R1_GPIOB14
def_set_mod_reg_bit(gpio, gpiob_drv_r1, gpiob14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R1_GPIOB15
def_set_mod_reg_bit(gpio, gpiob_drv_r1, gpiob15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R2
void set_gpio_gpiob_drv_r2(u32 reg_addr,
			u32 gpiob18,
			u32 gpiob19,
			u32 gpiob20,
			u32 gpiob21,
			u32 gpiob22,
			u32 gpiob23,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R2_GPIOB18
def_set_mod_reg_bit(gpio, gpiob_drv_r2, gpiob18, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R2_GPIOB19
def_set_mod_reg_bit(gpio, gpiob_drv_r2, gpiob19, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R2_GPIOB20
def_set_mod_reg_bit(gpio, gpiob_drv_r2, gpiob20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R2_GPIOB21
def_set_mod_reg_bit(gpio, gpiob_drv_r2, gpiob21, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R2_GPIOB22
def_set_mod_reg_bit(gpio, gpiob_drv_r2, gpiob22, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R2_GPIOB23
def_set_mod_reg_bit(gpio, gpiob_drv_r2, gpiob23, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R3
void set_gpio_gpiob_drv_r3(u32 reg_addr,
			u32 gpiob24,
			u32 gpiob25,
			u32 gpiob26,
			u32 gpiob27,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R3_GPIOB24
def_set_mod_reg_bit(gpio, gpiob_drv_r3, gpiob24, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R3_GPIOB25
def_set_mod_reg_bit(gpio, gpiob_drv_r3, gpiob25, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R3_GPIOB26
def_set_mod_reg_bit(gpio, gpiob_drv_r3, gpiob26, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DRV_R3_GPIOB27
def_set_mod_reg_bit(gpio, gpiob_drv_r3, gpiob27, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DATA
void set_gpio_gpiob_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_DATA_DAT
def_set_mod_reg_bit(gpio, gpiob_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PR_R
void set_gpio_gpiob_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpiob_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0
void set_gpio_gpioc_func_r0(u32 reg_addr,
				u32 gpioc0,
				u32 gpioc1,
				u32 gpioc2,
				u32 gpioc3,
				u32 gpioc4,
				u32 gpioc5,
				u32 gpioc6,
				u32 gpioc7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC0
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC1
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC2
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC3
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC4
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC5
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC6
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R0_GPIOC7
def_set_mod_reg_bit(gpio, gpioc_func_r0, gpioc7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1
void set_gpio_gpioc_func_r1(u32 reg_addr,
				u32 gpioc8,
				u32 gpioc9,
				u32 gpioc10,
				u32 gpioc11,
				u32 gpioc12,
				u32 gpioc13,
				u32 gpioc14,
				u32 gpioc15,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC8
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC9
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC10
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC11
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC12
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC13
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC14
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R1_GPIOC15
def_set_mod_reg_bit(gpio, gpioc_func_r1, gpioc15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2
void set_gpio_gpioc_func_r2(u32 reg_addr,
				u32 gpioc16,
				u32 gpioc17,
				u32 gpioc18,
				u32 gpioc19,
				u32 gpioc20,
				u32 gpioc21,
				u32 gpioc22,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2_GPIOC16
def_set_mod_reg_bit(gpio, gpioc_func_r2, gpioc16, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2_GPIOC17
def_set_mod_reg_bit(gpio, gpioc_func_r2, gpioc17, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2_GPIOC18
def_set_mod_reg_bit(gpio, gpioc_func_r2, gpioc18, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2_GPIOC19
def_set_mod_reg_bit(gpio, gpioc_func_r2, gpioc19, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2_GPIOC20
def_set_mod_reg_bit(gpio, gpioc_func_r2, gpioc20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2_GPIOC21
def_set_mod_reg_bit(gpio, gpioc_func_r2, gpioc21, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_FUNC_R2_GPIOC22
def_set_mod_reg_bit(gpio, gpioc_func_r2, gpioc22, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0
void set_gpio_gpioc_pud_r0(u32 reg_addr,
			u32 gpioc0,
			u32 gpioc1,
			u32 gpioc2,
			u32 gpioc3,
			u32 gpioc4,
			u32 gpioc5,
			u32 gpioc6,
			u32 gpioc7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC0
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC1
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC2
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC3
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC4
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC5
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC6
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R0_GPIOC7
def_set_mod_reg_bit(gpio, gpioc_pud_r0, gpioc7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1
void set_gpio_gpioc_pud_r1(u32 reg_addr,
			u32 gpioc8,
			u32 gpioc9,
			u32 gpioc10,
			u32 gpioc11,
			u32 gpioc12,
			u32 gpioc13,
			u32 gpioc14,
			u32 gpioc15,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC8
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC9
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC10
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC11
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC12
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC13
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC14
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R1_GPIOC15
def_set_mod_reg_bit(gpio, gpioc_pud_r1, gpioc15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2
void set_gpio_gpioc_pud_r2(u32 reg_addr,
			u32 gpioc16,
			u32 gpioc17,
			u32 gpioc18,
			u32 gpioc19,
			u32 gpioc20,
			u32 gpioc21,
			u32 gpioc22,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2_GPIOC16
def_set_mod_reg_bit(gpio, gpioc_pud_r2, gpioc16, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2_GPIOC17
def_set_mod_reg_bit(gpio, gpioc_pud_r2, gpioc17, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2_GPIOC18
def_set_mod_reg_bit(gpio, gpioc_pud_r2, gpioc18, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2_GPIOC19
def_set_mod_reg_bit(gpio, gpioc_pud_r2, gpioc19, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2_GPIOC20
def_set_mod_reg_bit(gpio, gpioc_pud_r2, gpioc20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2_GPIOC21
def_set_mod_reg_bit(gpio, gpioc_pud_r2, gpioc21, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PUD_R2_GPIOC22
def_set_mod_reg_bit(gpio, gpioc_pud_r2, gpioc22, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0
void set_gpio_gpioc_drv_r0(u32 reg_addr,
			u32 gpioc0,
			u32 gpioc1,
			u32 gpioc2,
			u32 gpioc3,
			u32 gpioc4,
			u32 gpioc5,
			u32 gpioc6,
			u32 gpioc7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC0
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC1
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC2
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC3
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC4
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC5
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC6
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R0_GPIOC7
def_set_mod_reg_bit(gpio, gpioc_drv_r0, gpioc7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1
void set_gpio_gpioc_drv_r1(u32 reg_addr,
			u32 gpioc8,
			u32 gpioc9,
			u32 gpioc10,
			u32 gpioc11,
			u32 gpioc12,
			u32 gpioc13,
			u32 gpioc14,
			u32 gpioc15,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC8
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC9
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC10
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC11
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC12
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC13
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC14
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R1_GPIOC15
def_set_mod_reg_bit(gpio, gpioc_drv_r1, gpioc15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2
void set_gpio_gpioc_drv_r2(u32 reg_addr,
			u32 gpioc16,
			u32 gpioc17,
			u32 gpioc18,
			u32 gpioc19,
			u32 gpioc20,
			u32 gpioc21,
			u32 gpioc22,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2_GPIOC16
def_set_mod_reg_bit(gpio, gpioc_drv_r2, gpioc16, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2_GPIOC17
def_set_mod_reg_bit(gpio, gpioc_drv_r2, gpioc17, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2_GPIOC18
def_set_mod_reg_bit(gpio, gpioc_drv_r2, gpioc18, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2_GPIOC19
def_set_mod_reg_bit(gpio, gpioc_drv_r2, gpioc19, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2_GPIOC20
def_set_mod_reg_bit(gpio, gpioc_drv_r2, gpioc20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2_GPIOC21
def_set_mod_reg_bit(gpio, gpioc_drv_r2, gpioc21, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DRV_R2_GPIOC22
def_set_mod_reg_bit(gpio, gpioc_drv_r2, gpioc22, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DATA
void set_gpio_gpioc_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_DATA_DAT
def_set_mod_reg_bit(gpio, gpioc_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PR_R
void set_gpio_gpioc_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOC_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpioc_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_FUNC_R0
void set_gpio_gpiod_func_r0(u32 reg_addr,
				u32 gpiod0,
				u32 gpiod1,
				u32 gpiod2,
				u32 gpiod3,
				u32 gpiod4,
				u32 gpiod5,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_FUNC_R0_GPIOD0
def_set_mod_reg_bit(gpio, gpiod_func_r0, gpiod0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_FUNC_R0_GPIOD1
def_set_mod_reg_bit(gpio, gpiod_func_r0, gpiod1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_FUNC_R0_GPIOD2
def_set_mod_reg_bit(gpio, gpiod_func_r0, gpiod2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_FUNC_R0_GPIOD3
def_set_mod_reg_bit(gpio, gpiod_func_r0, gpiod3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_FUNC_R0_GPIOD4
def_set_mod_reg_bit(gpio, gpiod_func_r0, gpiod4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_FUNC_R0_GPIOD5
def_set_mod_reg_bit(gpio, gpiod_func_r0, gpiod5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PUD_R0
void set_gpio_gpiod_pud_r0(u32 reg_addr,
			u32 gpiod0,
			u32 gpiod1,
			u32 gpiod2,
			u32 gpiod3,
			u32 gpiod4,
			u32 gpiod5,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PUD_R0_GPIOD0
def_set_mod_reg_bit(gpio, gpiod_pud_r0, gpiod0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PUD_R0_GPIOD1
def_set_mod_reg_bit(gpio, gpiod_pud_r0, gpiod1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PUD_R0_GPIOD2
def_set_mod_reg_bit(gpio, gpiod_pud_r0, gpiod2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PUD_R0_GPIOD3
def_set_mod_reg_bit(gpio, gpiod_pud_r0, gpiod3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PUD_R0_GPIOD4
def_set_mod_reg_bit(gpio, gpiod_pud_r0, gpiod4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PUD_R0_GPIOD5
def_set_mod_reg_bit(gpio, gpiod_pud_r0, gpiod5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DRV_R0
void set_gpio_gpiod_drv_r0(u32 reg_addr,
			u32 gpiod0,
			u32 gpiod1,
			u32 gpiod2,
			u32 gpiod3,
			u32 gpiod4,
			u32 gpiod5,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DRV_R0_GPIOD0
def_set_mod_reg_bit(gpio, gpiod_drv_r0, gpiod0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DRV_R0_GPIOD1
def_set_mod_reg_bit(gpio, gpiod_drv_r0, gpiod1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DRV_R0_GPIOD2
def_set_mod_reg_bit(gpio, gpiod_drv_r0, gpiod2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DRV_R0_GPIOD3
def_set_mod_reg_bit(gpio, gpiod_drv_r0, gpiod3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DRV_R0_GPIOD4
def_set_mod_reg_bit(gpio, gpiod_drv_r0, gpiod4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DRV_R0_GPIOD5
def_set_mod_reg_bit(gpio, gpiod_drv_r0, gpiod5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DATA
void set_gpio_gpiod_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_DATA_DAT
def_set_mod_reg_bit(gpio, gpiod_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PR_R
void set_gpio_gpiod_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOD_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpiod_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0
void set_gpio_gpioe_func_r0(u32 reg_addr,
				u32 gpioe0,
				u32 gpioe1,
				u32 gpioe2,
				u32 gpioe3,
				u32 gpioe4,
				u32 gpioe5,
				u32 gpioe6,
				u32 gpioe7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE0
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE1
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE2
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE3
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE4
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE5
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE6
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R0_GPIOE7
def_set_mod_reg_bit(gpio, gpioe_func_r0, gpioe7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1
void set_gpio_gpioe_func_r1(u32 reg_addr,
				u32 gpioe8,
				u32 gpioe9,
				u32 gpioe10,
				u32 gpioe11,
				u32 gpioe12,
				u32 gpioe13,
				u32 gpioe14,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1_GPIOE8
def_set_mod_reg_bit(gpio, gpioe_func_r1, gpioe8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1_GPIOE9
def_set_mod_reg_bit(gpio, gpioe_func_r1, gpioe9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1_GPIOE10
def_set_mod_reg_bit(gpio, gpioe_func_r1, gpioe10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1_GPIOE11
def_set_mod_reg_bit(gpio, gpioe_func_r1, gpioe11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1_GPIOE12
def_set_mod_reg_bit(gpio, gpioe_func_r1, gpioe12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1_GPIOE13
def_set_mod_reg_bit(gpio, gpioe_func_r1, gpioe13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_FUNC_R1_GPIOE14
def_set_mod_reg_bit(gpio, gpioe_func_r1, gpioe14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0
void set_gpio_gpioe_pud_r0(u32 reg_addr,
			u32 gpioe0,
			u32 gpioe1,
			u32 gpioe2,
			u32 gpioe3,
			u32 gpioe4,
			u32 gpioe5,
			u32 gpioe6,
			u32 gpioe7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE0
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE1
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE2
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE3
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE4
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE5
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE6
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R0_GPIOE7
def_set_mod_reg_bit(gpio, gpioe_pud_r0, gpioe7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1
void set_gpio_gpioe_pud_r1(u32 reg_addr,
			u32 gpioe8,
			u32 gpioe9,
			u32 gpioe10,
			u32 gpioe11,
			u32 gpioe12,
			u32 gpioe13,
			u32 gpioe14,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1_GPIOE8
def_set_mod_reg_bit(gpio, gpioe_pud_r1, gpioe8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1_GPIOE9
def_set_mod_reg_bit(gpio, gpioe_pud_r1, gpioe9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1_GPIOE10
def_set_mod_reg_bit(gpio, gpioe_pud_r1, gpioe10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1_GPIOE11
def_set_mod_reg_bit(gpio, gpioe_pud_r1, gpioe11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1_GPIOE12
def_set_mod_reg_bit(gpio, gpioe_pud_r1, gpioe12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1_GPIOE13
def_set_mod_reg_bit(gpio, gpioe_pud_r1, gpioe13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PUD_R1_GPIOE14
def_set_mod_reg_bit(gpio, gpioe_pud_r1, gpioe14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0
void set_gpio_gpioe_drv_r0(u32 reg_addr,
			u32 gpioe0,
			u32 gpioe1,
			u32 gpioe2,
			u32 gpioe3,
			u32 gpioe4,
			u32 gpioe5,
			u32 gpioe6,
			u32 gpioe7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE0
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE1
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE2
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE3
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE4
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE5
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE6
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R0_GPIOE7
def_set_mod_reg_bit(gpio, gpioe_drv_r0, gpioe7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1
void set_gpio_gpioe_drv_r1(u32 reg_addr,
			u32 gpioe8,
			u32 gpioe9,
			u32 gpioe10,
			u32 gpioe11,
			u32 gpioe12,
			u32 gpioe13,
			u32 gpioe14,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1_GPIOE8
def_set_mod_reg_bit(gpio, gpioe_drv_r1, gpioe8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1_GPIOE9
def_set_mod_reg_bit(gpio, gpioe_drv_r1, gpioe9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1_GPIOE10
def_set_mod_reg_bit(gpio, gpioe_drv_r1, gpioe10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1_GPIOE11
def_set_mod_reg_bit(gpio, gpioe_drv_r1, gpioe11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1_GPIOE12
def_set_mod_reg_bit(gpio, gpioe_drv_r1, gpioe12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1_GPIOE13
def_set_mod_reg_bit(gpio, gpioe_drv_r1, gpioe13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DRV_R1_GPIOE14
def_set_mod_reg_bit(gpio, gpioe_drv_r1, gpioe14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DATA
void set_gpio_gpioe_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_DATA_DAT
def_set_mod_reg_bit(gpio, gpioe_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PR_R
void set_gpio_gpioe_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpioe_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0
void set_gpio_gpiof_func_r0(u32 reg_addr,
				u32 gpiof0,
				u32 gpiof1,
				u32 gpiof2,
				u32 gpiof3,
				u32 gpiof4,
				u32 gpiof5,
				u32 gpiof6,
				u32 gpiof7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF0
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF1
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF2
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF3
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF4
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF5
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF6
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R0_GPIOF7
def_set_mod_reg_bit(gpio, gpiof_func_r0, gpiof7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R1
void set_gpio_gpiof_func_r1(u32 reg_addr,
				u32 gpiof8,
				u32 gpiof9,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R1_GPIOF8
def_set_mod_reg_bit(gpio, gpiof_func_r1, gpiof8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_FUNC_R1_GPIOF9
def_set_mod_reg_bit(gpio, gpiof_func_r1, gpiof9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0
void set_gpio_gpiof_pud_r0(u32 reg_addr,
			u32 gpiof0,
			u32 gpiof1,
			u32 gpiof2,
			u32 gpiof3,
			u32 gpiof4,
			u32 gpiof5,
			u32 gpiof6,
			u32 gpiof7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF0
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF1
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF2
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF3
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF4
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF5
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF6
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R0_GPIOF7
def_set_mod_reg_bit(gpio, gpiof_pud_r0, gpiof7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R1
void set_gpio_gpiof_pud_r1(u32 reg_addr,
			u32 gpiof8,
			u32 gpiof9,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R1_GPIOF8
def_set_mod_reg_bit(gpio, gpiof_pud_r1, gpiof8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PUD_R1_GPIOF9
def_set_mod_reg_bit(gpio, gpiof_pud_r1, gpiof9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0
void set_gpio_gpiof_drv_r0(u32 reg_addr,
			u32 gpiof0,
			u32 gpiof1,
			u32 gpiof2,
			u32 gpiof3,
			u32 gpiof4,
			u32 gpiof5,
			u32 gpiof6,
			u32 gpiof7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF0
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF1
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF2
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF3
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF4
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF5
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF6
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R0_GPIOF7
def_set_mod_reg_bit(gpio, gpiof_drv_r0, gpiof7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R1
void set_gpio_gpiof_drv_r1(u32 reg_addr,
			u32 gpiof8,
			u32 gpiof9,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R1_GPIOF8
def_set_mod_reg_bit(gpio, gpiof_drv_r1, gpiof8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DRV_R1_GPIOF9
def_set_mod_reg_bit(gpio, gpiof_drv_r1, gpiof9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DATA
void set_gpio_gpiof_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_DATA_DAT
def_set_mod_reg_bit(gpio, gpiof_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PR_R
void set_gpio_gpiof_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpiof_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0
void set_gpio_gpiog_func_r0(u32 reg_addr,
				u32 gpiog0,
				u32 gpiog1,
				u32 gpiog2,
				u32 gpiog3,
				u32 gpiog4,
				u32 gpiog5,
				u32 gpiog6,
				u32 gpiog7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG0
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG1
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG2
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG3
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG4
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG5
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG6
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R0_GPIOG7
def_set_mod_reg_bit(gpio, gpiog_func_r0, gpiog7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R1
void set_gpio_gpiog_func_r1(u32 reg_addr,
				u32 gpiog8,
				u32 gpiog9,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R1_GPIOG8
def_set_mod_reg_bit(gpio, gpiog_func_r1, gpiog8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_FUNC_R1_GPIOG9
def_set_mod_reg_bit(gpio, gpiog_func_r1, gpiog9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0
void set_gpio_gpiog_pud_r0(u32 reg_addr,
			u32 gpiog0,
			u32 gpiog1,
			u32 gpiog2,
			u32 gpiog3,
			u32 gpiog4,
			u32 gpiog5,
			u32 gpiog6,
			u32 gpiog7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG0
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG1
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG2
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG3
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG4
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG5
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG6
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R0_GPIOG7
def_set_mod_reg_bit(gpio, gpiog_pud_r0, gpiog7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R1
void set_gpio_gpiog_pud_r1(u32 reg_addr,
			u32 gpiog8,
			u32 gpiog9,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R1_GPIOG8
def_set_mod_reg_bit(gpio, gpiog_pud_r1, gpiog8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PUD_R1_GPIOG9
def_set_mod_reg_bit(gpio, gpiog_pud_r1, gpiog9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0
void set_gpio_gpiog_drv_r0(u32 reg_addr,
			u32 gpiog0,
			u32 gpiog1,
			u32 gpiog2,
			u32 gpiog3,
			u32 gpiog4,
			u32 gpiog5,
			u32 gpiog6,
			u32 gpiog7,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG0
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG1
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG2
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG3
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG4
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG5
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG6
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R0_GPIOG7
def_set_mod_reg_bit(gpio, gpiog_drv_r0, gpiog7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R1
void set_gpio_gpiog_drv_r1(u32 reg_addr,
			u32 gpiog8,
			u32 gpiog9,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R1_GPIOG8
def_set_mod_reg_bit(gpio, gpiog_drv_r1, gpiog8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DRV_R1_GPIOG9
def_set_mod_reg_bit(gpio, gpiog_drv_r1, gpiog9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DATA
void set_gpio_gpiog_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_DATA_DAT
def_set_mod_reg_bit(gpio, gpiog_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PR_R
void set_gpio_gpiog_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpiog_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_FUNC_R0
void set_gpio_gpioj_func_r0(u32 reg_addr,
				u32 gpioj0,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_FUNC_R0_GPIOJ0
def_set_mod_reg_bit(gpio, gpioj_func_r0, gpioj0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_PUD_R0
void set_gpio_gpioj_pud_r0(u32 reg_addr,
			u32 gpioj0,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_PUD_R0_GPIOJ0
def_set_mod_reg_bit(gpio, gpioj_pud_r0, gpioj0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_DRV_R0
void set_gpio_gpioj_drv_r0(u32 reg_addr,
			u32 gpioj0,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_DRV_R0_GPIOJ0
def_set_mod_reg_bit(gpio, gpioj_drv_r0, gpioj0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_DATA
void set_gpio_gpioj_data(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_DATA_DAT
def_set_mod_reg_bit(gpio, gpioj_data, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_PR_R
void set_gpio_gpioj_pr_r(u32 reg_addr,
			u32 pull_cfg,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOJ_PR_R_PULL_CFG
def_set_mod_reg_bit(gpio, gpioj_pr_r, pull_cfg, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R0
void set_gpio_gpiob_int_tri_r0(u32 reg_addr,
				u32 gpiob2,
				u32 gpiob3,
				u32 gpiob4,
				u32 gpiob5,
				u32 gpiob6,
				u32 gpiob7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R0_GPIOB2
def_set_mod_reg_bit(gpio, gpiob_int_tri_r0, gpiob2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R0_GPIOB3
def_set_mod_reg_bit(gpio, gpiob_int_tri_r0, gpiob3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R0_GPIOB4
def_set_mod_reg_bit(gpio, gpiob_int_tri_r0, gpiob4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R0_GPIOB5
def_set_mod_reg_bit(gpio, gpiob_int_tri_r0, gpiob5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R0_GPIOB6
def_set_mod_reg_bit(gpio, gpiob_int_tri_r0, gpiob6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R0_GPIOB7
def_set_mod_reg_bit(gpio, gpiob_int_tri_r0, gpiob7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R1
void set_gpio_gpiob_int_tri_r1(u32 reg_addr,
				u32 gpiob10,
				u32 gpiob11,
				u32 gpiob12,
				u32 gpiob13,
				u32 gpiob14,
				u32 gpiob15,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R1_GPIOB10
def_set_mod_reg_bit(gpio, gpiob_int_tri_r1, gpiob10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R1_GPIOB11
def_set_mod_reg_bit(gpio, gpiob_int_tri_r1, gpiob11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R1_GPIOB12
def_set_mod_reg_bit(gpio, gpiob_int_tri_r1, gpiob12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R1_GPIOB13
def_set_mod_reg_bit(gpio, gpiob_int_tri_r1, gpiob13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R1_GPIOB14
def_set_mod_reg_bit(gpio, gpiob_int_tri_r1, gpiob14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R1_GPIOB15
def_set_mod_reg_bit(gpio, gpiob_int_tri_r1, gpiob15, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R2
void set_gpio_gpiob_int_tri_r2(u32 reg_addr,
				u32 gpiob18,
				u32 gpiob19,
				u32 gpiob20,
				u32 gpiob21,
				u32 gpiob22,
				u32 gpiob23,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R2_GPIOB18
def_set_mod_reg_bit(gpio, gpiob_int_tri_r2, gpiob18, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R2_GPIOB19
def_set_mod_reg_bit(gpio, gpiob_int_tri_r2, gpiob19, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R2_GPIOB20
def_set_mod_reg_bit(gpio, gpiob_int_tri_r2, gpiob20, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R2_GPIOB21
def_set_mod_reg_bit(gpio, gpiob_int_tri_r2, gpiob21, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R2_GPIOB22
def_set_mod_reg_bit(gpio, gpiob_int_tri_r2, gpiob22, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R2_GPIOB23
def_set_mod_reg_bit(gpio, gpiob_int_tri_r2, gpiob23, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R3
void set_gpio_gpiob_int_tri_r3(u32 reg_addr,
				u32 gpiob24,
				u32 gpiob25,
				u32 gpiob26,
				u32 gpiob27,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R3_GPIOB24
def_set_mod_reg_bit(gpio, gpiob_int_tri_r3, gpiob24, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R3_GPIOB25
def_set_mod_reg_bit(gpio, gpiob_int_tri_r3, gpiob25, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R3_GPIOB26
def_set_mod_reg_bit(gpio, gpiob_int_tri_r3, gpiob26, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_TRI_R3_GPIOB27
def_set_mod_reg_bit(gpio, gpiob_int_tri_r3, gpiob27, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_MASK
void set_gpio_gpiob_int_mask(u32 reg_addr,
				u32 mask_en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_MASK_MASK_EN
def_set_mod_reg_bit(gpio, gpiob_int_mask, mask_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_PEND
void set_gpio_gpiob_int_pend(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_PEND_PENDING
def_set_mod_reg_bit(gpio, gpiob_int_pend, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_SAMP_CTRL
void set_gpio_gpiob_int_samp_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_SAMP_CTRL_SRC_SEL
def_set_mod_reg_bit(gpio, gpiob_int_samp_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_SAMP_CTRL_DIV
def_set_mod_reg_bit(gpio, gpiob_int_samp_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOB_INT_SAMP_CTRL_EN
def_set_mod_reg_bit(gpio, gpiob_int_samp_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0
void set_gpio_gpioe_int_tri_r0(u32 reg_addr,
				u32 gpioe0,
				u32 gpioe1,
				u32 gpioe2,
				u32 gpioe3,
				u32 gpioe4,
				u32 gpioe5,
				u32 gpioe6,
				u32 gpioe7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE0
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE1
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE2
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE3
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE4
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE5
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE6
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R0_GPIOE7
def_set_mod_reg_bit(gpio, gpioe_int_tri_r0, gpioe7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1
void set_gpio_gpioe_int_tri_r1(u32 reg_addr,
				u32 gpioe8,
				u32 gpioe9,
				u32 gpioe10,
				u32 gpioe11,
				u32 gpioe12,
				u32 gpioe13,
				u32 gpioe14,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1_GPIOE8
def_set_mod_reg_bit(gpio, gpioe_int_tri_r1, gpioe8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1_GPIOE9
def_set_mod_reg_bit(gpio, gpioe_int_tri_r1, gpioe9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1_GPIOE10
def_set_mod_reg_bit(gpio, gpioe_int_tri_r1, gpioe10, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1_GPIOE11
def_set_mod_reg_bit(gpio, gpioe_int_tri_r1, gpioe11, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1_GPIOE12
def_set_mod_reg_bit(gpio, gpioe_int_tri_r1, gpioe12, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1_GPIOE13
def_set_mod_reg_bit(gpio, gpioe_int_tri_r1, gpioe13, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_TRI_R1_GPIOE14
def_set_mod_reg_bit(gpio, gpioe_int_tri_r1, gpioe14, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_MASK
void set_gpio_gpioe_int_mask(u32 reg_addr,
				u32 mask_en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_MASK_MASK_EN
def_set_mod_reg_bit(gpio, gpioe_int_mask, mask_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_PEND
void set_gpio_gpioe_int_pend(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_PEND_PENDING
def_set_mod_reg_bit(gpio, gpioe_int_pend, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_SAMP_CTRL
void set_gpio_gpioe_int_samp_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_SAMP_CTRL_SRC_SEL
def_set_mod_reg_bit(gpio, gpioe_int_samp_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_SAMP_CTRL_DIV
def_set_mod_reg_bit(gpio, gpioe_int_samp_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOE_INT_SAMP_CTRL_EN
def_set_mod_reg_bit(gpio, gpioe_int_samp_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0
void set_gpio_gpiof_int_tri_r0(u32 reg_addr,
				u32 gpiof0,
				u32 gpiof1,
				u32 gpiof2,
				u32 gpiof3,
				u32 gpiof4,
				u32 gpiof5,
				u32 gpiof6,
				u32 gpiof7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF0
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF1
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF2
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF3
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF4
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF5
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF6
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R0_GPIOF7
def_set_mod_reg_bit(gpio, gpiof_int_tri_r0, gpiof7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R1
void set_gpio_gpiof_int_tri_r1(u32 reg_addr,
				u32 gpiof8,
				u32 gpiof9,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R1_GPIOF8
def_set_mod_reg_bit(gpio, gpiof_int_tri_r1, gpiof8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_TRI_R1_GPIOF9
def_set_mod_reg_bit(gpio, gpiof_int_tri_r1, gpiof9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_MASK
void set_gpio_gpiof_int_mask(u32 reg_addr,
				u32 mask_en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_MASK_MASK_EN
def_set_mod_reg_bit(gpio, gpiof_int_mask, mask_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_PEND
void set_gpio_gpiof_int_pend(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_PEND_PENDING
def_set_mod_reg_bit(gpio, gpiof_int_pend, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_SAMP_CTRL
void set_gpio_gpiof_int_samp_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_SAMP_CTRL_SRC_SEL
def_set_mod_reg_bit(gpio, gpiof_int_samp_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_SAMP_CTRL_DIV
def_set_mod_reg_bit(gpio, gpiof_int_samp_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOF_INT_SAMP_CTRL_EN
def_set_mod_reg_bit(gpio, gpiof_int_samp_ctrl, en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0
void set_gpio_gpiog_int_tri_r0(u32 reg_addr,
				u32 gpiog0,
				u32 gpiog1,
				u32 gpiog2,
				u32 gpiog3,
				u32 gpiog4,
				u32 gpiog5,
				u32 gpiog6,
				u32 gpiog7,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG0
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog0, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG1
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog1, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG2
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog2, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG3
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog3, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG4
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog4, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG5
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog5, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG6
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog6, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R0_GPIOG7
def_set_mod_reg_bit(gpio, gpiog_int_tri_r0, gpiog7, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R1
void set_gpio_gpiog_int_tri_r1(u32 reg_addr,
				u32 gpiog8,
				u32 gpiog9,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R1_GPIOG8
def_set_mod_reg_bit(gpio, gpiog_int_tri_r1, gpiog8, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_TRI_R1_GPIOG9
def_set_mod_reg_bit(gpio, gpiog_int_tri_r1, gpiog9, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_MASK
void set_gpio_gpiog_int_mask(u32 reg_addr,
				u32 mask_en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_MASK_MASK_EN
def_set_mod_reg_bit(gpio, gpiog_int_mask, mask_en, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_PEND
void set_gpio_gpiog_int_pend(u32 reg_addr,
				u32 pending,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_PEND_PENDING
def_set_mod_reg_bit(gpio, gpiog_int_pend, pending, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_SAMP_CTRL
void set_gpio_gpiog_int_samp_ctrl(u32 reg_addr,
				u32 src_sel,
				u32 div,
				u32 en,
				u32 m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_SAMP_CTRL_SRC_SEL
def_set_mod_reg_bit(gpio, gpiog_int_samp_ctrl, src_sel, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_SAMP_CTRL_DIV
def_set_mod_reg_bit(gpio, gpiog_int_samp_ctrl, div, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPIO_GPIOG_INT_SAMP_CTRL_EN
def_set_mod_reg_bit(gpio, gpiog_int_samp_ctrl, en, addr, val, m_or_r);
#endif

#endif /* ___GPIO__SET___H___ */
