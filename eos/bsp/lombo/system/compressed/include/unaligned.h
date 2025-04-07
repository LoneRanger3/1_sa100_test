#ifndef __UNALIGNED_H
#define __UNALIGNED_H

struct __una_u16 { u16 x; } __packed;
struct __una_u32 { u32 x; } __packed;
struct __una_u64 { u64 x; } __packed;

static inline u16 __get_unaligned_cpu16(const void *p)
{
	const struct __una_u16 *ptr = (const struct __una_u16 *)p;
	return ptr->x;
}

static inline u16 get_unaligned_le16(const void *p)
{
	return __get_unaligned_cpu16((const u8 *)p);
}

static inline u16 __get_unaligned_be16(const u8 *p)
{
	return p[0] << 8 | p[1];
}

static inline u16 get_unaligned_be16(const void *p)
{
	return __get_unaligned_be16((const u8 *)p);
}

static inline u32 __get_unaligned_be32(const u8 *p)
{
	return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

static inline u32 get_unaligned_be32(const void *p)
{
	return __get_unaligned_be32((const u8 *)p);
}

#endif /* __UNALIGNED_H */
