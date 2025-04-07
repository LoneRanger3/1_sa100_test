OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS
{
    . = START_ADDR;

    /* -------- text section -------- */
    __text_start = .;
    .text :
    {
        *(.vectors)
        *(.text)
        *(.text.*)

	/* section information for utest */
	. = ALIGN(4);
	__rt_utest_tc_tab_start = .;
	KEEP(*(UtestTcTab))
	__rt_utest_tc_tab_end = .;

        /* section information for finsh shell */
        . = ALIGN(4);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;
        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;
        . = ALIGN(4);

        /* section information for modules */
        . = ALIGN(4);
        __rtmsymtab_start = .;
        KEEP(*(RTMSymTab))
        __rtmsymtab_end = .;

        /* section information for initialization */
        . = ALIGN(4);
        __rt_init_start = .;
        KEEP(*(SORT(.rti_fn*)))
        __rt_init_end = .;
    } =0

    . = ALIGN(4);
    .ctors :
    {
        PROVIDE(__ctors_start__ = .);
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
	KEEP (*(SORT(.init_array.*)))
	KEEP (*(.init_array))
        PROVIDE(__ctors_end__ = .);
    }

    . = ALIGN(4);
    .dtors :
    {
        PROVIDE(__dtors_start__ = .);
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
        PROVIDE(__dtors_end__ = .);
    }

    __text_end = .;

    /* -------- read only data section -------- */
    . = ALIGN(4 * 1024);
    __rodata_start = .;

    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
    }

    . = ALIGN(4 * 1024);
    __rodata_end = .;

    /* -------- add for c++ -------- */
    . = ALIGN(4);
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
		PROVIDE(_ZSt24__throw_out_of_range_fmtPKcz = .);
		PROVIDE(_ZSt20__throw_length_errorPKc = .);
		PROVIDE(_ZSt17__throw_bad_allocv  = .);
		PROVIDE(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1Ev  = .);
		PROVIDE(_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev   = .);
		PROVIDE(_ZNSt8ios_base4InitC1Ev  = .);
		PROVIDE(_ZNSt8ios_base4InitD1Ev   = .);
    }

    . = ALIGN(4);
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)

        /* This is used by the startup in order to initialize the .data secion */
        _sidata = .;
    }
    __exidx_end = .;

    /* -------- data section -------- */
    . = ALIGN(4);
    __data_start = .;
    .data :
    {
        *(.data.share.isr)
        *(.data)
        *(.data.*)
    }
    __data_end = .;

    /* -------- bss section -------- */
    . = ALIGN(4);
    __bss_start = .;
    .bss :
    {
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
    }
    . = ALIGN(4 * 1024); /* __bss_end is heap begin, so PAGE_SIZE align */
    __bss_end = .;

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }

    /* DWARF debug sections.
     * Symbols in the DWARF debugging sections are relative to the beginning
     * of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }

    _end = .;
}
