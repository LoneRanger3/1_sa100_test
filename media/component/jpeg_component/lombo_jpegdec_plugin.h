#ifndef LOMBO_DECPLUGIN_H
#define LOMBO_DECPLUGIN_H

enum OUTPUT_MODE {
	VC_YUV420p = 0,
	VC_YVU420p,
	VC_YUV420semi,
	VC_YVU420semi
};

enum PACKET_TYPE {
	JPEG_ERROR = -1,
	JPEG_NORMAL,
	JPEG_CMYK,
	JPEG_PROGRESSIVE
};

typedef struct {
	int packet_type;
	int packet_len;
	int packet_ts;
} jpeg_dec_packet_header_t;

typedef struct {
	jpeg_dec_packet_header_t head;
	unsigned char *vir_addr;
	unsigned int   phy_addr;
	unsigned char *file_addr;
	unsigned int  file_offset;
	unsigned int  file_buf_len;
} jpeg_dec_packet_t;

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
	int output_width;
	int output_height;
	int width_stride;
	int height_stride;
	int output_mode;
	int time_stamp;
} jpeg_dec_header_t;

typedef struct {
	jpeg_dec_header_t head;
	unsigned char *vir_addr;
	unsigned int phy_addr;
	int fd;
	int index;
} jpeg_dec_buf_handle_t;

typedef struct {
	int sec_output_format;
	int down_scale_ratio;
	int rotation_degree;
	int luma_stride;		/* for luma stride */
	int chroma_stride; /* ChromaStride = LumaStride if format is nv12 or nv21;
			ChromaStride = LumaStride / 2 if format is yv12 or yv21 */
	int chroma_offset;   /* gap between Y and UV */
	int crop_out_width;   /* actual width */
	int crop_out_height;  /* actual height */
	int crop_left_offset; /* always 0 */
	int crop_top_offset;  /* always 0 */
	int output_height;  /* encoding output scaling down */
	int output_width;   /* encoding output scaling down */
	int stride_luma;    /* for display test, actually is luma_stride for hardware */
	int stride_chroma;  /* for display test, actually is chroma_stride for hardware */
	int one_frame_size; /* for display test */
	int offset_luma;    /* for display test */
	int offset_chroma;  /* for display test */
	int size_luma;      /* for display test */
	int size_chroma;    /* for display test */
} jpeg_dec_parm_t;

typedef struct {
	char extension[20];
	void* (*open)(void *dec_parm , void *packet);
	int (*decode_frame)(void *handle, void *packet, void *dec_buf_handle);
	int (*dispose)(void *handle);
	int (*ex_ops)(void *handle, int cmd, void *dec_parm);
	int (*reserve)();
} jpegdec_plugin_t;

void *jpeg_dec_open(void *jpeg_dec_parm, void *jpeg_dec_packet);
int jpeg_dec_frame(void *handle, void *jpeg_dec_packet, void *jpeg_dec_buf_handle);
int jpeg_dec_dispose(void *handle);

#endif
