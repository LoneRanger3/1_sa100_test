/* gpio_dump.h */

#ifndef ___GPIO___DUMP__H___
#define ___GPIO___DUMP__H___

#ifndef TMP_STR_LEN
#define TMP_STR_LEN  256
#endif

#ifndef REG_INFO_DEF
#define REG_INFO_DEF

typedef u32 (*pfn_dump)(u32 addr, u32 data, u32 mode, char *buffer);

typedef struct tag_reg_info {
	u32      addr;           /*address         */
	u32      reset;          /*reset value     */
	pfn_dump dump;           /*reg dump func   */
	char     name[28];       /*reg name        */
	u32      res;            /*0               */
} reg_info_t;

#endif /* REG_INFO_DEF */

u32 dump_gpio_gpioc_ver(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_func_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_pud_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_drv_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioa_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_func_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_func_r2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_func_r3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_pud_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_pud_r2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_pud_r3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_drv_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_drv_r2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_drv_r3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_func_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_func_r2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_pud_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_pud_r2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_drv_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_drv_r2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioc_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiod_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiod_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiod_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiod_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiod_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_func_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_pud_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_drv_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_func_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_pud_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_drv_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_func_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_pud_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_drv_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioj_func_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioj_pud_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioj_drv_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioj_data(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioj_pr_r(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_int_tri_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_int_tri_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_int_tri_r2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_int_tri_r3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_int_mask(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_int_pend(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiob_int_samp_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_int_tri_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_int_tri_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_int_mask(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_int_pend(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpioe_int_samp_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_int_tri_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_int_tri_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_int_mask(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_int_pend(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiof_int_samp_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_int_tri_r0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_int_tri_r1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_int_mask(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_int_pend(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gpio_gpiog_int_samp_ctrl(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___GPIO___DUMP__H___ */
