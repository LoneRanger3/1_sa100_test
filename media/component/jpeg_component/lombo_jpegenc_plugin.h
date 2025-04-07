#ifndef LOMBO_ENCPLUGIN_H
#define LOMBO_ENCPLUGIN_H

#define MAX_JPG_BLENDING_NUM  4

enum ENCODE_CMD_T {
	SET_BLENDING_PICTURE,
	DISABLE_BLENDING
};

enum INPUT_MODE {
	VC_YUV420P = 0,
	VC_YVU420P,
	VC_YUV420SP,
	VC_YVU420SP,
	VC_YUYV422,
	VC_UYVY422,
	VC_YVYU422,
	VC_VYUY422,
	VC_YUV422P,
	VC_YVU422P,
	VC_YUV422SP,
	VC_YVU422SP,
	VC_ARGB8888 = 100,
	VC_RGBA8888,
	VC_ABGR8888,
	VC_BGRA8888,
};

typedef struct {
	int in_width;
	int in_height;
	int l_stride;
	int out_width;
	int out_height;
	int quality;
	int input_mode;
	unsigned char *buf_info;
	int buf_info_size;
	int data_length;
} jpeg_enc_parm_t;

typedef struct {
	unsigned char *vir_addr[3];
	unsigned int phy_addr[3];
	unsigned int fd[3];
	int time_stamp;
} capbuf_t;

typedef struct {
	unsigned char *vir_addr;
	unsigned int phy_addr;
	unsigned int fd;
	int buf_size;
	int data_size;
	int time_stamp;
	numeral_output_t blendings[MAX_JPG_BLENDING_NUM];
} jpeg_enc_packet_t;

typedef struct {
	char extension[16];
	void *(*open)(void *enc_parm, void *port);
	int (*encode_frame)(void *handle, void *capbuf, void *packet, void *enc_parm);
	int (*dispose)(void *handle);
	int (*ex_ops)(void *handle, int cmd, void *arg);
	int (*reserve)();
} jpegenc_plugin_t;

void *jpeg_enc_open(void *jpeg_enc_parm, void *port);
int jpeg_enc_frame(void *handle, void *capbuf, void *jpeg_enc_packet,
					void *jpeg_enc_parm);
int jpeg_enc_dispose(void *handle);
int jpeg_enc_ops(void *handle, int cmd, void *arg);

#endif
