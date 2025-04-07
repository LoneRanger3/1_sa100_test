/* gpadc_set.h */

#ifndef ___GPADC__SET___H___
#define ___GPADC__SET___H___

#ifdef DEF_SET_GPADC_VER
void set_gpadc_ver(u32 reg_addr,
		u32 ver_l,
		u32 ver_h,
		u32 comp,
		u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_VER_VER_L
def_set_mod_reg_bit(gpadc, ver, ver_l, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_VER_VER_H
def_set_mod_reg_bit(gpadc, ver, ver_h, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_VER_COMP
def_set_mod_reg_bit(gpadc, ver, comp, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAEN
void set_gpadc_gpaen(u32 reg_addr,
			u32 gen,
			u32 cen,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAEN_GEN
def_set_mod_reg_bit(gpadc, gpaen, gen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAEN_CEN
def_set_mod_reg_bit(gpadc, gpaen, cen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC
void set_gpadc_gpac(u32 reg_addr,
			u32 auxin0e,
			u32 auxin1e,
			u32 sdae,
			u32 src,
			u32 fss,
			u32 cce,
			u32 dive,
			u32 opc,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_AUXIN0E
def_set_mod_reg_bit(gpadc, gpac, auxin0e, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_AUXIN1E
def_set_mod_reg_bit(gpadc, gpac, auxin1e, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_SDAE
def_set_mod_reg_bit(gpadc, gpac, sdae, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_SRC
def_set_mod_reg_bit(gpadc, gpac, src, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_FSS
def_set_mod_reg_bit(gpadc, gpac, fss, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_CCE
def_set_mod_reg_bit(gpadc, gpac, cce, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_DIVE
def_set_mod_reg_bit(gpadc, gpac, dive, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAC_OPC
def_set_mod_reg_bit(gpadc, gpac, opc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE
void set_gpadc_gpaie(u32 reg_addr,
			u32 aux0ie,
			u32 aux1ie,
			u32 s0die,
			u32 s0uie,
			u32 s1die,
			u32 s1uie,
			u32 s2die,
			u32 s2uie,
			u32 ain0datie,
			u32 ain1datie,
			u32 s0datie,
			u32 s1datie,
			u32 s2datie,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_AUX0IE
def_set_mod_reg_bit(gpadc, gpaie, aux0ie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_AUX1IE
def_set_mod_reg_bit(gpadc, gpaie, aux1ie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S0DIE
def_set_mod_reg_bit(gpadc, gpaie, s0die, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S0UIE
def_set_mod_reg_bit(gpadc, gpaie, s0uie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S1DIE
def_set_mod_reg_bit(gpadc, gpaie, s1die, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S1UIE
def_set_mod_reg_bit(gpadc, gpaie, s1uie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S2DIE
def_set_mod_reg_bit(gpadc, gpaie, s2die, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S2UIE
def_set_mod_reg_bit(gpadc, gpaie, s2uie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_AIN0DATIE
def_set_mod_reg_bit(gpadc, gpaie, ain0datie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_AIN1DATIE
def_set_mod_reg_bit(gpadc, gpaie, ain1datie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S0DATIE
def_set_mod_reg_bit(gpadc, gpaie, s0datie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S1DATIE
def_set_mod_reg_bit(gpadc, gpaie, s1datie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIE_S2DATIE
def_set_mod_reg_bit(gpadc, gpaie, s2datie, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS
void set_gpadc_gpais(u32 reg_addr,
			u32 aux0is,
			u32 aux1is,
			u32 s0dis,
			u32 s0uis,
			u32 s1dis,
			u32 s1uis,
			u32 s2dis,
			u32 s2uis,
			u32 ain0datis,
			u32 ain1datis,
			u32 s0datis,
			u32 s1datis,
			u32 s2datis,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_AUX0IS
def_set_mod_reg_bit(gpadc, gpais, aux0is, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_AUX1IS
def_set_mod_reg_bit(gpadc, gpais, aux1is, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S0DIS
def_set_mod_reg_bit(gpadc, gpais, s0dis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S0UIS
def_set_mod_reg_bit(gpadc, gpais, s0uis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S1DIS
def_set_mod_reg_bit(gpadc, gpais, s1dis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S1UIS
def_set_mod_reg_bit(gpadc, gpais, s1uis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S2DIS
def_set_mod_reg_bit(gpadc, gpais, s2dis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S2UIS
def_set_mod_reg_bit(gpadc, gpais, s2uis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_AIN0DATIS
def_set_mod_reg_bit(gpadc, gpais, ain0datis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_AIN1DATIS
def_set_mod_reg_bit(gpadc, gpais, ain1datis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S0DATIS
def_set_mod_reg_bit(gpadc, gpais, s0datis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S1DATIS
def_set_mod_reg_bit(gpadc, gpais, s1datis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIS_S2DATIS
def_set_mod_reg_bit(gpadc, gpais, s2datis, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC
void set_gpadc_gpaic(u32 reg_addr,
			u32 auxin0pc,
			u32 auxin1pc,
			u32 s0dpc,
			u32 s0upc,
			u32 s1dpc,
			u32 s1upc,
			u32 s2dpc,
			u32 s2upc,
			u32 ain0datpc,
			u32 ain1datpc,
			u32 s0datpc,
			u32 s1datpc,
			u32 s2datpc,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_AUXIN0PC
def_set_mod_reg_bit(gpadc, gpaic, auxin0pc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_AUXIN1PC
def_set_mod_reg_bit(gpadc, gpaic, auxin1pc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S0DPC
def_set_mod_reg_bit(gpadc, gpaic, s0dpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S0UPC
def_set_mod_reg_bit(gpadc, gpaic, s0upc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S1DPC
def_set_mod_reg_bit(gpadc, gpaic, s1dpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S1UPC
def_set_mod_reg_bit(gpadc, gpaic, s1upc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S2DPC
def_set_mod_reg_bit(gpadc, gpaic, s2dpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S2UPC
def_set_mod_reg_bit(gpadc, gpaic, s2upc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_AIN0DATPC
def_set_mod_reg_bit(gpadc, gpaic, ain0datpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_AIN1DATPC
def_set_mod_reg_bit(gpadc, gpaic, ain1datpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S0DATPC
def_set_mod_reg_bit(gpadc, gpaic, s0datpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S1DATPC
def_set_mod_reg_bit(gpadc, gpaic, s1datpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GPAIC_S2DATPC
def_set_mod_reg_bit(gpadc, gpaic, s2datpc, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_CDAT
void set_gpadc_cdat(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_CDAT_DAT
def_set_mod_reg_bit(gpadc, cdat, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_AIN0DAT
void set_gpadc_ain0dat(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_AIN0DAT_DAT
def_set_mod_reg_bit(gpadc, ain0dat, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX0HDAT
void set_gpadc_aux0hdat(u32 reg_addr,
			u32 auxin0shdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX0HDAT_AUXIN0SHDAT
def_set_mod_reg_bit(gpadc, aux0hdat, auxin0shdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX0TDAT
void set_gpadc_aux0tdat(u32 reg_addr,
			u32 auxin0tdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX0TDAT_AUXIN0TDAT
def_set_mod_reg_bit(gpadc, aux0tdat, auxin0tdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_AIN1DAT
void set_gpadc_ain1dat(u32 reg_addr,
			u32 dat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_AIN1DAT_DAT
def_set_mod_reg_bit(gpadc, ain1dat, dat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX1HDAT
void set_gpadc_aux1hdat(u32 reg_addr,
			u32 auxin1hdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX1HDAT_AUXIN1HDAT
def_set_mod_reg_bit(gpadc, aux1hdat, auxin1hdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX1TDAT
void set_gpadc_aux1tdat(u32 reg_addr,
			u32 auxin1tdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_AUX1TDAT_AUXIN1TDAT
def_set_mod_reg_bit(gpadc, aux1tdat, auxin1tdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GSC
void set_gpadc_gsc(u32 reg_addr,
		u32 sen,
		u32 ssdaen,
		u32 ssr,
		u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_GSC_SEN
def_set_mod_reg_bit(gpadc, gsc, sen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GSC_SSDAEN
def_set_mod_reg_bit(gpadc, gsc, ssdaen, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_GSC_SSR
def_set_mod_reg_bit(gpadc, gsc, ssr, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0CDAT
void set_gpadc_s0cdat(u32 reg_addr,
			u32 s0cdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0CDAT_S0CDAT
def_set_mod_reg_bit(gpadc, s0cdat, s0cdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1CDAT
void set_gpadc_s1cdat(u32 reg_addr,
			u32 s1cdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1CDAT_S1CDAT
def_set_mod_reg_bit(gpadc, s1cdat, s1cdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2CDAT
void set_gpadc_s2cdat(u32 reg_addr,
			u32 s2cdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2CDAT_S2CDAT
def_set_mod_reg_bit(gpadc, s2cdat, s2cdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0DAT
void set_gpadc_s0dat(u32 reg_addr,
			u32 s0adcdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0DAT_S0ADCDAT
def_set_mod_reg_bit(gpadc, s0dat, s0adcdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0UTDAT
void set_gpadc_s0utdat(u32 reg_addr,
			u32 s0datut,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0UTDAT_S0DATUT
def_set_mod_reg_bit(gpadc, s0utdat, s0datut, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0DTDAT
void set_gpadc_s0dtdat(u32 reg_addr,
			u32 s0datdt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S0DTDAT_S0DATDT
def_set_mod_reg_bit(gpadc, s0dtdat, s0datdt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1DAT
void set_gpadc_s1dat(u32 reg_addr,
			u32 s1adcdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1DAT_S1ADCDAT
def_set_mod_reg_bit(gpadc, s1dat, s1adcdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1UTDAT
void set_gpadc_s1utdat(u32 reg_addr,
			u32 s1datut,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1UTDAT_S1DATUT
def_set_mod_reg_bit(gpadc, s1utdat, s1datut, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1DTDAT
void set_gpadc_s1dtdat(u32 reg_addr,
			u32 s1datdt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S1DTDAT_S1DATDT
def_set_mod_reg_bit(gpadc, s1dtdat, s1datdt, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2DAT
void set_gpadc_s2dat(u32 reg_addr,
			u32 s2adcdat,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2DAT_S2ADCDAT
def_set_mod_reg_bit(gpadc, s2dat, s2adcdat, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2UTDAT
void set_gpadc_s2utdat(u32 reg_addr,
			u32 s2datut,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2UTDAT_S2DATUT
def_set_mod_reg_bit(gpadc, s2utdat, s2datut, addr, val, m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2DTDAT
void set_gpadc_s2dtdat(u32 reg_addr,
			u32 s2datdt,
			u32 m_or_r);
#endif

#ifdef DEF_SET_GPADC_S2DTDAT_S2DATDT
def_set_mod_reg_bit(gpadc, s2dtdat, s2datdt, addr, val, m_or_r);
#endif

#endif /* ___GPADC__SET___H___ */
