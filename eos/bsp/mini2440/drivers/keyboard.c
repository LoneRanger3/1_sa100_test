#include <rtthread.h>
#include <s3c24x0.h>
#include <rtgui/event.h>

void report_keyboard_event(RTGUI_KBD_KEY key)
{
	struct rtgui_event_kbd kbd_event;

	RTGUI_EVENT_KBD_INIT(&kbd_event);
	kbd_event.mod  = RTGUI_KMOD_NONE;
	kbd_event.unicode = 0;
	kbd_event.type = RTGUI_KEYDOWN;
	kbd_event.key = key;

	if (kbd_event.key != RTGUIK_UNKNOWN)
	{
		/* post down event */
		rtgui_server_post_event(&(kbd_event.parent), sizeof(kbd_event));
	}
}

void INTEINT0_handler(int irqno, void *param)
{
	rt_uint32_t eint_pend = EINTPEND;

	if(eint_pend & (1<<0))
	{
		rt_kprintf("%s %d! \n", __func__, __LINE__);  /* not run here, maybe cleared before lomboswer */
	}

	report_keyboard_event(RTGUIK_LEFT);
}

void INTEINT2_handler(int irqno, void *param)
{
	rt_uint32_t eint_pend = EINTPEND;

	if(eint_pend & (1<<2))
	{
		rt_kprintf("%s %d! \n", __func__, __LINE__);  /* not run here, maybe cleared before lomboswer */
	}

	report_keyboard_event(RTGUIK_RIGHT);
}

void INTEINT11_handler(int irqno, void *param)
{
	rt_uint32_t eint_pend = EINTPEND;

	if(eint_pend & (1 << 11))
	{
		EINTPEND = eint_pend; /* clear pending */
	}

	report_keyboard_event(RTGUIK_UP);
}

void rt_hw_eint0_init()
{
	/* Set GPF0 as EINT0 */
	GPFCON = GPFCON & (~(3 << 0)) | (2 << 0);
	GPFUP = GPFUP | (1 << 0);

	/* EINT0 falling edge triggered */
	EXTINT0 = (EXTINT0 & (~(0x7 << 0))) | (2 << 0);
	/* Enable EINT0 */
	EINTMASK = EINTMASK & (~(1<<0));

	/* instal interrupt */
	rt_hw_interrupt_install(INTEINT0, INTEINT0_handler, RT_NULL, "EINT0");
	rt_hw_interrupt_umask(INTEINT0);
}

void rt_hw_eint2_init()
{
	/* Set GPF2 as EINT2 */
	GPFCON = GPFCON & (~(3 << 4)) | (2 << 4);
	GPFUP = GPFUP | (1 << 2);

	/* EINT2 falling edge triggered */
	EXTINT0 = (EXTINT0 & (~(0x7 << 8))) | (2 << 8);
	/* Enable EINT2 */
	EINTMASK = EINTMASK & (~(1<<2));

	/* instal interrupt */
	rt_hw_interrupt_install(INTEINT2, INTEINT2_handler, RT_NULL, "EINT2");
	rt_hw_interrupt_umask(INTEINT2);
}

void rt_hw_eint11_init()
{
	/* Set GPG3 as EINT11 */
	GPGCON = GPGCON & (~(3 << 6)) | (2 << 6);
	GPGUP = GPGUP | (1 << 3);

	/* EINT11 falling edge triggered */
	EXTINT1 = (EXTINT1 & (~(0x7 << 12))) | (2 << 12);
	/* Enable EINT11 */
	EINTMASK = EINTMASK & (~(1<<11));

	/* instal interrupt */
	rt_hw_interrupt_install(INTEINT8_23, INTEINT11_handler, RT_NULL, "EINT11");
	rt_hw_interrupt_umask(INTEINT8_23);
}

void rt_hw_keyborad_init()
{
	rt_hw_eint0_init();
	rt_hw_eint2_init();
	rt_hw_eint11_init();
}

