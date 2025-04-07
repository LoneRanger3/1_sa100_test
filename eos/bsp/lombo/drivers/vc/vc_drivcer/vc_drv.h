#ifndef __VC_DRIVE_H
#define __VC_DRIVE_H

struct reg_p {
	void *register_p;
	int size;
};

#ifdef __KERNEL__
	#define VC_S_TOPREG_PARAMS   _IOWR('x', 0xf0, struct reg_p)
	#define VC_S_PREREG_PARAMS   _IOWR('x', 0xf1, struct reg_p)
	#define VC_S_ENCREG_PARAMS   _IOWR('x', 0xf2, struct reg_p)
	#define VC_S_DECREG_PARAMS   _IOWR('x', 0xf3, struct reg_p)
	#define VC_S_JPEGREG_PARAMS  _IOWR('x', 0xf4, struct reg_p)
	#define VC_S_ENC_DEC_TABLE   _IOWR('x', 0xf5, struct reg_p)
	#define VC_S_SCALE_PARAMS    _IOWR('x', 0xf6, struct reg_p)
	#define VC_RUN               _IOWR('x', 0xf7, unsigned int)
	#define VC_QUERY             _IOWR('x', 0xf8, unsigned int)
#elif defined __EOS__
	#define VC_S_TOPREG_PARAMS   0x1
	#define VC_S_PREREG_PARAMS   0x2
	#define VC_S_ENCREG_PARAMS   0x3
	#define VC_S_DECREG_PARAMS   0x4
	#define VC_S_JPEGREG_PARAMS  0x5
	#define VC_S_ENC_DEC_TABLE   0x6
	#define VC_S_SCALE_PARAMS    0x7
	#define VC_RUN               0x8
	#define VC_QUERY             0x9
#else
#endif

#endif
