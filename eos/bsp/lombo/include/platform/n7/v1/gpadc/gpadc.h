/* gpadc.h */

#ifndef ___GPADC___H___
#define ___GPADC___H___

#define BASE_GPADC                0x04013000

#define VA_GPADC_VER                 (0x00000000 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_GPAEN               (0x00000010 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_GPAC                (0x00000014 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_GPAIE               (0x00000018 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_GPAIS               (0x0000001C + BASE_GPADC + VA_GPADC)
#define VA_GPADC_GPAIC               (0x00000020 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_CDAT                (0x00000024 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_AIN0DAT             (0x00000030 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_AUX0HDAT            (0x00000034 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_AUX0TDAT            (0x00000038 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_AIN1DAT             (0x00000040 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_AUX1HDAT            (0x00000044 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_AUX1TDAT            (0x00000048 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_GSC                 (0x00000070 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S0CDAT              (0x00000080 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S1CDAT              (0x00000084 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S2CDAT              (0x00000088 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S0DAT               (0x00000090 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S0UTDAT             (0x00000094 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S0DTDAT             (0x00000098 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S1DAT               (0x000000A0 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S1UTDAT             (0x000000A4 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S1DTDAT             (0x000000A8 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S2DAT               (0x000000B0 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S2UTDAT             (0x000000B4 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_S2DTDAT             (0x000000B8 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_DBG0                (0x000000F0 + BASE_GPADC + VA_GPADC)
#define VA_GPADC_DBG1                (0x000000F4 + BASE_GPADC + VA_GPADC)

#define DATA_GPADC_VER               0x00001100
#define DATA_GPADC_GPAEN             0x00000000
#define DATA_GPADC_GPAC              0x00000000
#define DATA_GPADC_GPAIE             0x00000000
#define DATA_GPADC_GPAIS             0x00000000
#define DATA_GPADC_GPAIC             0x00000000
#define DATA_GPADC_CDAT              0x00000000
#define DATA_GPADC_AIN0DAT           0x00000000
#define DATA_GPADC_AUX0HDAT          0x00000000
#define DATA_GPADC_AUX0TDAT          0x00000000
#define DATA_GPADC_AIN1DAT           0x00000000
#define DATA_GPADC_AUX1HDAT          0x00000000
#define DATA_GPADC_AUX1TDAT          0x00000000
#define DATA_GPADC_GSC               0x00000000
#define DATA_GPADC_S0CDAT            0x00000000
#define DATA_GPADC_S1CDAT            0x00000000
#define DATA_GPADC_S2CDAT            0x00000000
#define DATA_GPADC_S0DAT             0x00000000
#define DATA_GPADC_S0UTDAT           0x00000000
#define DATA_GPADC_S0DTDAT           0x00000000
#define DATA_GPADC_S1DAT             0x00000000
#define DATA_GPADC_S1UTDAT           0x00000000
#define DATA_GPADC_S1DTDAT           0x00000000
#define DATA_GPADC_S2DAT             0x00000000
#define DATA_GPADC_S2UTDAT           0x00000000
#define DATA_GPADC_S2DTDAT           0x00000000
#define DATA_GPADC_DBG0              0x00000000
#define DATA_GPADC_DBG1              0x00000000

/* Version */
typedef union {
	u32 val;
	struct {
	u32 ver_l:5;    /**/
	u32 rsvd0:3;    /**/
	u32 ver_h:3;    /**/
	u32 rsvd1:1;    /**/
	u32 comp:1;     /**/
	u32 rsvd2:19;   /**/
	} bits;
} reg_gpadc_ver_t;

#define GPADC_VER_COMP_0        0x0
#define GPADC_VER_COMP_1        0x1

/* Enable */
typedef union {
	u32 val;
	struct {
	u32 gen:1;      /**/
	u32 cen:1;      /**/
	u32 rsvd0:30;   /**/
	} bits;
} reg_gpadc_gpaen_t;

#define GPADC_GPAEN_GEN_1       0x1
#define GPADC_GPAEN_GEN_0       0x0
#define GPADC_GPAEN_CEN_1       0x1
#define GPADC_GPAEN_CEN_0       0x0

/* Conversion Control */
typedef union {
	u32 val;
	struct {
	u32 auxin0e:1;  /**/
	u32 auxin1e:1;  /**/
	u32 rsvd0:5;    /**/
	u32 sdae:1;     /**/
	u32 src:3;      /**/
	u32 rsvd1:1;    /**/
	u32 fss:1;      /**/
	u32 cce:1;      /**/
	u32 dive:1;     /**/
	u32 opc:3;      /**/
	u32 rsvd2:14;   /**/
	} bits;
} reg_gpadc_gpac_t;

#define GPADC_GPAC_AUXIN0E_0    0x0
#define GPADC_GPAC_AUXIN0E_1    0x1
#define GPADC_GPAC_AUXIN1E_0    0x0
#define GPADC_GPAC_AUXIN1E_1    0x1
#define GPADC_GPAC_SDAE_0       0x0
#define GPADC_GPAC_SDAE_1       0x1
#define GPADC_GPAC_SRC_0        0x0
#define GPADC_GPAC_SRC_1        0x1
#define GPADC_GPAC_SRC_2        0x2
#define GPADC_GPAC_SRC_3        0x3
#define GPADC_GPAC_SRC_4        0x4
#define GPADC_GPAC_SRC_5        0x5
#define GPADC_GPAC_FSS_0        0x0
#define GPADC_GPAC_FSS_1        0x1
#define GPADC_GPAC_CCE_0        0x0
#define GPADC_GPAC_CCE_1        0x1
#define GPADC_GPAC_DIVE_0       0x0
#define GPADC_GPAC_DIVE_1       0x1

/* Interrupt Enable */
typedef union {
	u32 val;
	struct {
	u32 aux0ie:1;   /**/
	u32 aux1ie:1;   /**/
	u32 rsvd0:2;    /**/
	u32 s0die:1;    /**/
	u32 s0uie:1;    /**/
	u32 s1die:1;    /**/
	u32 s1uie:1;    /**/
	u32 s2die:1;    /**/
	u32 s2uie:1;    /**/
	u32 rsvd1:6;    /**/
	u32 ain0datie:1;/**/
	u32 ain1datie:1;/**/
	u32 rsvd2:2;    /**/
	u32 s0datie:1;  /**/
	u32 s1datie:1;  /**/
	u32 s2datie:1;  /**/
	u32 rsvd3:9;    /**/
	} bits;
} reg_gpadc_gpaie_t;

#define GPADC_GPAIE_AUX0IE_1    0x1
#define GPADC_GPAIE_AUX0IE_0    0x0
#define GPADC_GPAIE_AUX1IE_1    0x1
#define GPADC_GPAIE_AUX1IE_0    0x0
#define GPADC_GPAIE_S0DIE_1     0x1
#define GPADC_GPAIE_S0DIE_0     0x0
#define GPADC_GPAIE_S0UIE_1     0x1
#define GPADC_GPAIE_S0UIE_0     0x0
#define GPADC_GPAIE_S1DIE_1     0x1
#define GPADC_GPAIE_S1DIE_0     0x0
#define GPADC_GPAIE_S1UIE_1     0x1
#define GPADC_GPAIE_S1UIE_0     0x0
#define GPADC_GPAIE_S2DIE_1     0x1
#define GPADC_GPAIE_S2DIE_0     0x0
#define GPADC_GPAIE_S2UIE_1     0x1
#define GPADC_GPAIE_S2UIE_0     0x0
#define GPADC_GPAIE_AIN0DATIE_1 0x1
#define GPADC_GPAIE_AIN0DATIE_0 0x0
#define GPADC_GPAIE_AIN1DATIE_1 0x1
#define GPADC_GPAIE_AIN1DATIE_0 0x0
#define GPADC_GPAIE_S0DATIE_1   0x1
#define GPADC_GPAIE_S0DATIE_0   0x0
#define GPADC_GPAIE_S1DATIE_1   0x1
#define GPADC_GPAIE_S1DATIE_0   0x0
#define GPADC_GPAIE_S2DATIE_1   0x1
#define GPADC_GPAIE_S2DATIE_0   0x0

/* Interrupt Status */
typedef union {
	u32 val;
	struct {
	u32 aux0is:1;   /**/
	u32 aux1is:1;   /**/
	u32 rsvd0:2;    /**/
	u32 s0dis:1;    /**/
	u32 s0uis:1;    /**/
	u32 s1dis:1;    /**/
	u32 s1uis:1;    /**/
	u32 s2dis:1;    /**/
	u32 s2uis:1;    /**/
	u32 rsvd1:6;    /**/
	u32 ain0datis:1;/**/
	u32 ain1datis:1;/**/
	u32 rsvd2:2;    /**/
	u32 s0datis:1;  /**/
	u32 s1datis:1;  /**/
	u32 s2datis:1;  /**/
	u32 rsvd3:9;    /**/
	} bits;
} reg_gpadc_gpais_t;

#define GPADC_GPAIS_AUX0IS_1    0x1
#define GPADC_GPAIS_AUX0IS_0    0x0
#define GPADC_GPAIS_AUX1IS_1    0x1
#define GPADC_GPAIS_AUX1IS_0    0x0
#define GPADC_GPAIS_S0DIS_1     0x1
#define GPADC_GPAIS_S0DIS_0     0x0
#define GPADC_GPAIS_S0UIS_1     0x1
#define GPADC_GPAIS_S0UIS_0     0x0
#define GPADC_GPAIS_S1DIS_1     0x1
#define GPADC_GPAIS_S1DIS_0     0x0
#define GPADC_GPAIS_S1UIS_1     0x1
#define GPADC_GPAIS_S1UIS_0     0x0
#define GPADC_GPAIS_S2DIS_1     0x1
#define GPADC_GPAIS_S2DIS_0     0x0
#define GPADC_GPAIS_S2UIS_1     0x1
#define GPADC_GPAIS_S2UIS_0     0x0
#define GPADC_GPAIS_AIN0DATIS_1 0x1
#define GPADC_GPAIS_AIN0DATIS_0 0x0
#define GPADC_GPAIS_AIN1DATIS_1 0x1
#define GPADC_GPAIS_AIN1DATIS_0 0x0
#define GPADC_GPAIS_S0DATIS_1   0x1
#define GPADC_GPAIS_S0DATIS_0   0x0
#define GPADC_GPAIS_S1DATIS_1   0x1
#define GPADC_GPAIS_S1DATIS_0   0x0
#define GPADC_GPAIS_S2DATIS_1   0x1
#define GPADC_GPAIS_S2DATIS_0   0x0

/* Interrupt Clear */
typedef union {
	u32 val;
	struct {
	u32 auxin0pc:1; /**/
	u32 auxin1pc:1; /**/
	u32 rsvd0:2;    /**/
	u32 s0dpc:1;    /**/
	u32 s0upc:1;    /**/
	u32 s1dpc:1;    /**/
	u32 s1upc:1;    /**/
	u32 s2dpc:1;    /**/
	u32 s2upc:1;    /**/
	u32 rsvd1:6;    /**/
	u32 ain0datpc:1;/**/
	u32 ain1datpc:1;/**/
	u32 rsvd2:2;    /**/
	u32 s0datpc:1;  /**/
	u32 s1datpc:1;  /**/
	u32 s2datpc:1;  /**/
	u32 rsvd3:9;    /**/
	} bits;
} reg_gpadc_gpaic_t;

/* ADC Calibration Data */
typedef union {
	u32 val;
	struct {
	u32 dat:12;     /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_cdat_t;

/* AUXIN0 Data */
typedef union {
	u32 val;
	struct {
	u32 dat:12;     /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_ain0dat_t;

/* AUXIN0 Sample Hold Data */
typedef union {
	u32 val;
	struct {
	u32 auxin0shdat:12;/**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_aux0hdat_t;

/* AUXIN0 Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 auxin0tdat:12;/**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_aux0tdat_t;

/* AUXIN1 Data */
typedef union {
	u32 val;
	struct {
	u32 dat:12;     /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_ain1dat_t;

/* AUXIN1 Sample Hold Data */
typedef union {
	u32 val;
	struct {
	u32 auxin1hdat:12;/**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_aux1hdat_t;

/* AUXIN1 Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 auxin1tdat:12;/**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_aux1tdat_t;

/* Sensor Control */
typedef union {
	u32 val;
	struct {
	u32 sen:1;      /**/
	u32 ssdaen:1;   /**/
	u32 rsvd0:6;    /**/
	u32 ssr:2;      /**/
	u32 rsvd1:22;   /**/
	} bits;
} reg_gpadc_gsc_t;

#define GPADC_GSC_SEN_0         0x0
#define GPADC_GSC_SEN_1         0x1
#define GPADC_GSC_SSDAEN_0      0x0
#define GPADC_GSC_SSDAEN_1      0x1
#define GPADC_GSC_SSR_0         0x0
#define GPADC_GSC_SSR_1         0x1
#define GPADC_GSC_SSR_2         0x2
#define GPADC_GSC_SSR_3         0x3

/* Sensor0 Calibration Data */
typedef union {
	u32 val;
	struct {
	u32 s0cdat:12;  /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s0cdat_t;

/* Sensor1 Calibration Data */
typedef union {
	u32 val;
	struct {
	u32 s1cdat:12;  /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s1cdat_t;

/* Sensor2 Calibration Data */
typedef union {
	u32 val;
	struct {
	u32 s2cdat:12;  /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s2cdat_t;

/* Sensor0 Data Register */
typedef union {
	u32 val;
	struct {
	u32 s0adcdat:12;/**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s0dat_t;

/* Sensor0 Up Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 s0datut:12; /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s0utdat_t;

/* Sensor0 Down Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 s0datdt:12; /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s0dtdat_t;

/* Sensor1 Data Register */
typedef union {
	u32 val;
	struct {
	u32 s1adcdat:12;/**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s1dat_t;

/* Sensor1 Up Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 s1datut:12; /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s1utdat_t;

/* Sensor1 Down Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 s1datdt:12; /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s1dtdat_t;

/* Sensor2 Data Register */
typedef union {
	u32 val;
	struct {
	u32 s2adcdat:12;/**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s2dat_t;

/* Sensor2 Up Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 s2datut:12; /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s2utdat_t;

/* Sensor2 Down Threshold Data */
typedef union {
	u32 val;
	struct {
	u32 s2datdt:12; /**/
	u32 rsvd0:20;   /**/
	} bits;
} reg_gpadc_s2dtdat_t;

/* reg_gpadc_t bank */
typedef struct tag_gpadc {
	reg_gpadc_ver_t        ver;        /* 0000 */
	u32                    res0[3];
	reg_gpadc_gpaen_t      gpaen;      /* 0010 */
	reg_gpadc_gpac_t       gpac;       /* 0014 */
	reg_gpadc_gpaie_t      gpaie;      /* 0018 */
	reg_gpadc_gpais_t      gpais;      /* 001C */
	reg_gpadc_gpaic_t      gpaic;      /* 0020 */
	reg_gpadc_cdat_t       cdat;       /* 0024 */
	u32                    res1[2];
	reg_gpadc_ain0dat_t    ain0dat;    /* 0030 */
	reg_gpadc_aux0hdat_t   aux0hdat;   /* 0034 */
	reg_gpadc_aux0tdat_t   aux0tdat;   /* 0038 */
	u32                    res2[1];
	reg_gpadc_ain1dat_t    ain1dat;    /* 0040 */
	reg_gpadc_aux1hdat_t   aux1hdat;   /* 0044 */
	reg_gpadc_aux1tdat_t   aux1tdat;   /* 0048 */
	u32                    res3[9];
	reg_gpadc_gsc_t        gsc;        /* 0070 */
	u32                    res4[3];
	reg_gpadc_s0cdat_t     s0cdat;     /* 0080 */
	reg_gpadc_s1cdat_t     s1cdat;     /* 0084 */
	reg_gpadc_s2cdat_t     s2cdat;     /* 0088 */
	u32                    res5[1];
	reg_gpadc_s0dat_t      s0dat;      /* 0090 */
	reg_gpadc_s0utdat_t    s0utdat;    /* 0094 */
	reg_gpadc_s0dtdat_t    s0dtdat;    /* 0098 */
	u32                    res6[1];
	reg_gpadc_s1dat_t      s1dat;      /* 00A0 */
	reg_gpadc_s1utdat_t    s1utdat;    /* 00A4 */
	reg_gpadc_s1dtdat_t    s1dtdat;    /* 00A8 */
	u32                    res7[1];
	reg_gpadc_s2dat_t      s2dat;      /* 00B0 */
	reg_gpadc_s2utdat_t    s2utdat;    /* 00B4 */
	reg_gpadc_s2dtdat_t    s2dtdat;    /* 00B8 */
	u32                    res8[13];
	u32                    prv_00F0;   /* 00F0 */
	u32                    prv_00F4;   /* 00F4 */
} reg_gpadc_t;

#endif /* ___GPADC___H___ */
