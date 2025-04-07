/* gicd_dump.h */

#ifndef ___GICD___DUMP__H___
#define ___GICD___DUMP__H___

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

u32 dump_gicd_ctlr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_typer(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_iidr(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_igroupr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_igroupr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_igroupr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_igroupr3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_igroupr4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_igroupr5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_igroupr6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isenabler0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isenabler1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isenabler2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isenabler3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isenabler4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isenabler5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isenabler6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icenabler0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icenabler1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icenabler2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icenabler3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icenabler4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icenabler5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icenabler6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ispendr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ispendr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ispendr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ispendr3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ispendr4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ispendr5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ispendr6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icpendr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icpendr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icpendr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icpendr3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icpendr4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icpendr5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icpendr6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isactiver0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isactiver1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isactiver2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isactiver3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isactiver4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isactiver5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_isactiver6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icactiver0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icactiver1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icactiver2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icactiver3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icactiver4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icactiver5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icactiver6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr7(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr8(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr9(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr10(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr11(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr12(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr13(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr14(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr15(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr16(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr17(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr18(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr19(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr20(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr21(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr22(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr23(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr24(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr25(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr26(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr27(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr28(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr29(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr30(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr31(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr32(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr33(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr34(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr35(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr36(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr37(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr38(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr39(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr40(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr41(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr42(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr43(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr44(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr45(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr46(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr47(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr48(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr49(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr50(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr51(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr52(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr53(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr54(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_ipriorityr55(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr7(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr8(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr9(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr10(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr11(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr12(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr13(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr14(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr15(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr16(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr17(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr18(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr19(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr20(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr21(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr22(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr23(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr24(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr25(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr26(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr27(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr28(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr29(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr30(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr31(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr32(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr33(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr34(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr35(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr36(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr37(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr38(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr39(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr40(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr41(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr42(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr43(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr44(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr45(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr46(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr47(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr48(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr49(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr50(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr51(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr52(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr53(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr54(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_itargetsr55(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr4(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr5(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr6(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr7(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr8(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr9(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr10(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr11(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr12(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_icfgr13(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_sgir(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_cpendsgir0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_cpendsgir1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_cpendsgir2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_cpendsgir3(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_spendsgir0(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_spendsgir1(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_spendsgir2(u32 addr, u32 data, u32 mode, char *outbuf);

u32 dump_gicd_spendsgir3(u32 addr, u32 data, u32 mode, char *outbuf);

#endif /* ___GICD___DUMP__H___ */
