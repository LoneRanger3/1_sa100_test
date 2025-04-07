/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-10-01     Yi Qiu       first version
 * 2012-11-25     Heyuanjie87  reduce the memory consumption
 * 2012-12-09     Heyuanjie87  change function and endpoint handler 
 * 2013-07-25     Yi Qiu       update for USB CV test
 */

#include <rtthread.h>
#include <rtservice.h>
#include "drivers/usb_device.h"
#include "mstorage.h"

#ifdef ARCH_LOMBO
/* self scsi start */
#include <csp.h>
#include "lb-ufi.h"
/* self scsi end */
#endif

#ifdef RT_USB_DEVICE_MSTORAGE

enum STAT
{
    STAT_CBW,
    STAT_CMD,        
    STAT_CSW,
    STAT_RECEIVE,
    STAT_SEND,
#ifdef ARCH_LOMBO
/* self scsi start */
    STAT_LB_REV,
    STAT_LB_SEND,
/* self scsi end */
#endif
};

typedef enum
{
    FIXED,
    COUNT,
    BLOCK_COUNT,
}CB_SIZE_TYPE;

typedef enum
{
    DIR_IN,
    DIR_OUT,
    DIR_NONE,
}CB_DIR;

typedef rt_size_t (*cbw_handler)(ufunction_t func, ustorage_cbw_t cbw);

struct scsi_cmd
{
    rt_uint16_t cmd;
    cbw_handler handler;    
    rt_size_t cmd_len;
    CB_SIZE_TYPE type;
    rt_size_t data_size;
    CB_DIR dir;
};

#ifdef ARCH_LOMBO
#define W_BUF_PAGE_NUM	10
#define R_BUF_PAGE_NUM	2

/* self  scsi start */
#define SCSI_UFI_CMD	0xF8

static int lb_cmd;
rt_uint8_t *in_buf;
rt_uint8_t *out_buf;
struct cmd_reg_xfer g_reg_cmd;
struct cmd_mem_xfer g_mem_cmd;

#ifdef ARCH_LOMBO_N7V0
#define VA_ADDR		VA_OFF_N7V0
#elif defined ARCH_LOMBO_N7V1
#define VA_ADDR		VA_OFF_N7V1
#else
#define VA_ADDR		(0x0)
#endif

void rt_pm_reboot(void);
/* self scsi end */

enum buf_page_status {
	BUF_PAGE_IDLE,
	BUF_PAGE_READY,
	BUF_PAGE_BUSY
};

struct buf_page {
	rt_uint32_t status;
	rt_uint8_t *buf;
	rt_uint32_t block;
	rt_uint32_t sec_num;
};
#endif

struct mstorage
{    
    struct ustorage_csw csw_response;
    uep_t ep_in;
    uep_t ep_out;    
    int status;
    rt_uint32_t cb_data_size;
    rt_device_t disk;
    rt_uint32_t block;
    rt_int32_t count;
    rt_int32_t size;
    struct scsi_cmd* processing;
    struct rt_device_blk_geometry geometry;    
#ifdef ARCH_LOMBO
	struct buf_page wpage[W_BUF_PAGE_NUM];
	rt_uint32_t windex;
	rt_uint32_t page_free;
	rt_mq_t page_mq;
	rt_sem_t wpage_sem;
	struct rt_thread *usb_write_thread;
	rt_mutex_t rw_mtx;
	struct rt_thread *usb_read_thread;
	rt_bool_t continue_read;
	rt_mq_t rpage_mq;
	rt_sem_t rpage_sem;
	rt_uint32_t rindex;
	struct buf_page rpage[R_BUF_PAGE_NUM];
#endif
};

ALIGN(4)
static struct udevice_descriptor dev_desc =
{
    USB_DESC_LENGTH_DEVICE,     //bLength;
    USB_DESC_TYPE_DEVICE,       //type;
    USB_BCD_VERSION,            //bcdUSB;
    USB_CLASS_MASS_STORAGE,     //bDeviceClass;
    0x06,                       //bDeviceSubClass;
    0x50,                       //bDeviceProtocol;
    0x40,                       //bMaxPacketSize0;
    _VENDOR_ID,                 //idVendor;
    _PRODUCT_ID,                //idProduct;
    USB_BCD_DEVICE,             //bcdDevice;
    USB_STRING_MANU_INDEX,      //iManufacturer;
    USB_STRING_PRODUCT_INDEX,   //iProduct;
    USB_STRING_SERIAL_INDEX,    //iSerialNumber;
    USB_DYNAMIC,                //bNumConfigurations;
};

//FS and HS needed
ALIGN(4)
static struct usb_qualifier_descriptor dev_qualifier =
{
    sizeof(dev_qualifier),          //bLength
    USB_DESC_TYPE_DEVICEQUALIFIER,  //bDescriptorType
    0x0200,                         //bcdUSB
    USB_CLASS_MASS_STORAGE,         //bDeviceClass
    0x06,                           //bDeviceSubClass
    0x50,                           //bDeviceProtocol
    64,                             //bMaxPacketSize0
    0x01,                           //bNumConfigurations
    0,
};


ALIGN(4)
const static struct umass_descriptor _mass_desc =
{
#ifdef RT_USB_DEVICE_COMPOSITE
    /* Interface Association Descriptor */
    {
        USB_DESC_LENGTH_IAD,
        USB_DESC_TYPE_IAD,
        USB_DYNAMIC,
        0x01,
        USB_CLASS_MASS_STORAGE,
        0x06,
        0x50,
        0x00,
    },
#endif
    {
        USB_DESC_LENGTH_INTERFACE,  //bLength;
        USB_DESC_TYPE_INTERFACE,    //type;
        USB_DYNAMIC,                //bInterfaceNumber;
        0x00,                       //bAlternateSetting;
        0x02,                       //bNumEndpoints
        USB_CLASS_MASS_STORAGE,     //bInterfaceClass;
        0x06,                       //bInterfaceSubClass;
        0x50,                       //bInterfaceProtocol;
        0x00,                       //iInterface;
    },

    {
        USB_DESC_LENGTH_ENDPOINT,   //bLength;
        USB_DESC_TYPE_ENDPOINT,     //type;
        USB_DYNAMIC | USB_DIR_OUT,  //bEndpointAddress;
        USB_EP_ATTR_BULK,           //bmAttributes;
        USB_DYNAMIC,                //wMaxPacketSize;
        0x00,                       //bInterval;
    },

    {
        USB_DESC_LENGTH_ENDPOINT,   //bLength;
        USB_DESC_TYPE_ENDPOINT,     //type;
        USB_DYNAMIC | USB_DIR_IN,   //bEndpointAddress;
        USB_EP_ATTR_BULK,           //bmAttributes;
        USB_DYNAMIC,                //wMaxPacketSize;
        0x00,                       //bInterval;
    },
};

ALIGN(4)
const static char* _ustring[] =
{
    "Language",
    "RT-Thread Team.",
    "RTT Mass Storage",
    "320219198301",
    "Configuration",
    "Interface",
};

static rt_size_t _test_unit_ready(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _request_sense(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _inquiry_cmd(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _allow_removal(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _start_stop(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _mode_sense_6(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _read_capacities(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _read_capacity(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _read_10(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _write_10(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _verify_10(ufunction_t func, ustorage_cbw_t cbw);
#ifdef ARCH_LOMBO
/* self scsi start */
static rt_size_t _lb_get_status(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _lb_get_version(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _lb_get_swinfo(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _lb_get_hwinfo(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _lb_do_update(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _lb_reg_ops(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _lb_mem_ops(ufunction_t func, ustorage_cbw_t cbw);
static rt_size_t _lb_prg_run(ufunction_t func, ustorage_cbw_t cbw);
/* self scsi end */
#endif

ALIGN(4)
static struct scsi_cmd cmd_data[] =
{
    {SCSI_TEST_UNIT_READY, _test_unit_ready, 6,  FIXED,       0, DIR_NONE},
    {SCSI_REQUEST_SENSE,   _request_sense,   6,  COUNT,       0, DIR_IN},
    {SCSI_INQUIRY_CMD,     _inquiry_cmd,     6,  COUNT,       0, DIR_IN},
    {SCSI_ALLOW_REMOVAL,   _allow_removal,   6,  FIXED,       0, DIR_NONE},
    {SCSI_MODE_SENSE_6,    _mode_sense_6,    6,  COUNT,       0, DIR_IN},             
    {SCSI_START_STOP,      _start_stop,      6,  FIXED,       0, DIR_NONE},            
    {SCSI_READ_CAPACITIES, _read_capacities, 10, COUNT,       0, DIR_NONE},            
    {SCSI_READ_CAPACITY,   _read_capacity,   10, FIXED,       8, DIR_IN},
    {SCSI_READ_10,         _read_10,         10, BLOCK_COUNT, 0, DIR_IN},
    {SCSI_WRITE_10,        _write_10,        10, BLOCK_COUNT, 0, DIR_OUT},
    {SCSI_VERIFY_10,       _verify_10,       10, FIXED,       0, DIR_NONE},
#ifdef ARCH_LOMBO
    /* self scsi start */
    {UFI_DEV_CMD,          _lb_get_status,   16, COUNT,       0, DIR_IN},
    {UFI_VER_CMD,          _lb_get_version,  16, COUNT,       0, DIR_IN},
    {UFI_SWI_CMD,          _lb_get_swinfo,   16, COUNT,       0, DIR_IN},
    {UFI_HWI_CMD,          _lb_get_hwinfo,   16, COUNT,       0, DIR_IN},
    {UFI_UPD_CMD,          _lb_do_update,    16, COUNT,       0, DIR_NONE},
    {UFI_REG_CMD,          _lb_reg_ops,      16, COUNT,       0, DIR_NONE},
    {UFI_MEM_CMD,          _lb_mem_ops,      16, COUNT,       0, DIR_NONE},
    {UFI_RUN_CMD,          _lb_prg_run,      16, COUNT,       0, DIR_NONE},
     /* self scsi end */
#endif
};

static void _send_status(ufunction_t func)
{
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_send_status\n"));

    data = (struct mstorage*)func->user_data;   
    data->ep_in->request.buffer = (rt_uint8_t*)&data->csw_response;
    data->ep_in->request.size = SIZEOF_CSW;    
    data->ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
    data->status = STAT_CSW;
}

static rt_size_t _test_unit_ready(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_test_unit_ready\n"));

    data = (struct mstorage*)func->user_data;
    data->csw_response.status = 0;
        
    return 0;
}

static rt_size_t _allow_removal(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_allow_removal\n"));

    data = (struct mstorage*)func->user_data;
    data->csw_response.status = 0;

    return 0;
}

/**
 * This function will handle inquiry command request.
 *
 * @param func the usb function object. 
 * @param cbw the command block wrapper.
 *
 * @return RT_EOK on successful.
 */

static rt_size_t _inquiry_cmd(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;
    rt_uint8_t *buf;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 
    RT_ASSERT(cbw != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_inquiry_cmd\n"));

    data = (struct mstorage*)func->user_data;   
    buf = data->ep_in->buffer;

    *(rt_uint32_t*)&buf[0] = 0x0 | (0x80 << 8);
    *(rt_uint32_t*)&buf[4] = 31;

    rt_memset(&buf[8], 0x20, 28);
    rt_memcpy(&buf[8], "RTT", 3);
    rt_memcpy(&buf[16], "USB Disk", 8);

    data->cb_data_size = MIN(data->cb_data_size, SIZEOF_INQUIRY_CMD);
    data->ep_in->request.buffer = buf;
    data->ep_in->request.size = data->cb_data_size;
    data->ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
    data->status = STAT_CMD;

    return data->cb_data_size;
}

/**
 * This function will handle sense request.
 *
 * @param func the usb function object. 
 * @param cbw the command block wrapper.
 *
 * @return RT_EOK on successful.
 */
static rt_size_t _request_sense(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;
    struct request_sense_data *buf;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 
    RT_ASSERT(cbw != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_request_sense\n"));
    
    data = (struct mstorage*)func->user_data;   
    buf = (struct request_sense_data *)data->ep_in->buffer;

    buf->ErrorCode = 0x70;
    buf->Valid = 0;
    buf->SenseKey = 2;
    buf->Information[0] = 0;
    buf->Information[1] = 0;
    buf->Information[2] = 0;
    buf->Information[3] = 0;
    buf->AdditionalSenseLength = 0x0a;
    buf->AdditionalSenseCode   = 0x3a;
    buf->AdditionalSenseCodeQualifier = 0;

    data->cb_data_size = MIN(data->cb_data_size, SIZEOF_REQUEST_SENSE);
    data->ep_in->request.buffer = (rt_uint8_t*)data->ep_in->buffer;
    data->ep_in->request.size = data->cb_data_size;
    data->ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
    data->status = STAT_CMD;

    return data->cb_data_size;
}

/**
 * This function will handle mode_sense_6 request.
 *
 * @param func the usb function object.
 * @param cbw the command block wrapper. 
 *
 * @return RT_EOK on successful.
 */
static rt_size_t _mode_sense_6(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;
    rt_uint8_t *buf;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 
    RT_ASSERT(cbw != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_mode_sense_6\n"));

    data = (struct mstorage*)func->user_data;   
    buf = data->ep_in->buffer;
    buf[0] = 3;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;

    data->cb_data_size = MIN(data->cb_data_size, SIZEOF_MODE_SENSE_6);
    data->ep_in->request.buffer = buf;
    data->ep_in->request.size = data->cb_data_size;
    data->ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
    data->status = STAT_CMD;

    return data->cb_data_size;
}

/**
 * This function will handle read_capacities request.
 *
 * @param func the usb function object. 
 * @param cbw the command block wrapper.
 *
 * @return RT_EOK on successful.
 */
static rt_size_t _read_capacities(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;
    rt_uint8_t *buf;
    rt_uint32_t sector_count, sector_size;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 
    RT_ASSERT(cbw != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_read_capacities\n"));

    data = (struct mstorage*)func->user_data;   
    buf = data->ep_in->buffer;
    sector_count = data->geometry.sector_count;
    sector_size = data->geometry.bytes_per_sector;

    *(rt_uint32_t*)&buf[0] = 0x08000000;
    buf[4] = sector_count >> 24;
    buf[5] = 0xff & (sector_count >> 16);
    buf[6] = 0xff & (sector_count >> 8);
    buf[7] = 0xff & (sector_count);
    buf[8] = 0x02;
    buf[9] = 0xff & (sector_size >> 16);
    buf[10] = 0xff & (sector_size >> 8);
    buf[11] = 0xff & sector_size;

    data->cb_data_size = MIN(data->cb_data_size, SIZEOF_READ_CAPACITIES);
    data->ep_in->request.buffer = buf;
    data->ep_in->request.size = data->cb_data_size;
    data->ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
    data->status = STAT_CMD;

    return data->cb_data_size;
}

/**
 * This function will handle read_capacity request.
 *
 * @param func the usb function object. 
 * @param cbw the command block wapper.
 *
 * @return RT_EOK on successful.
 */
static rt_size_t _read_capacity(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;

    rt_uint8_t *buf;
    rt_uint32_t sector_count, sector_size;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 
    RT_ASSERT(cbw != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_read_capacity\n"));

    data = (struct mstorage*)func->user_data;   
    buf = data->ep_in->buffer;    
    sector_count = data->geometry.sector_count;
    sector_size = data->geometry.bytes_per_sector;

    buf[0] = sector_count >> 24;
    buf[1] = 0xff & (sector_count >> 16);
    buf[2] = 0xff & (sector_count >> 8);
    buf[3] = 0xff & (sector_count);
    buf[4] = 0x0;
    buf[5] = 0xff & (sector_size >> 16);
    buf[6] = 0xff & (sector_size >> 8);
    buf[7] = 0xff & sector_size;

    data->cb_data_size = MIN(data->cb_data_size, SIZEOF_READ_CAPACITY);
    data->ep_in->request.buffer = buf;
    data->ep_in->request.size = data->cb_data_size;
    data->ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
    data->status = STAT_CMD;

    return data->cb_data_size;
}

/**
 * This function will handle read_10 request.
 *
 * @param func the usb function object.
 * @param cbw the command block wrapper.
 *
 * @return RT_EOK on successful.
 */
static rt_size_t _read_10(ufunction_t func, ustorage_cbw_t cbw)
{
#ifdef ARCH_LOMBO
	struct mstorage *data;
	rt_size_t size;
	rt_size_t sec_count;
	struct buf_page *page;
	rt_err_t ret;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	data = (struct mstorage *)func->user_data;
	data->block = cbw->cb[2] << 24 | cbw->cb[3] << 16 | cbw->cb[4] << 8 |
	     cbw->cb[5] << 0;
	data->count = cbw->cb[7] << 8 | cbw->cb[8] << 0;
	data->csw_response.data_reside = data->cb_data_size;

	RT_ASSERT(data->count < data->geometry.sector_count);

	/* get read/write lock */
	rt_mutex_take(data->rw_mtx, RT_WAITING_FOREVER);
	page = &data->rpage[data->rindex];

	/* Check whether read buf is idle */
	if (page->status != BUF_PAGE_IDLE) {
		/* wait ready */
		rt_sem_take(data->rpage_sem, RT_WAITING_FOREVER);
		if (page->status != BUF_PAGE_READY)
			rt_kprintf("buf page should be ready!\n");

		/* buf is exactly what we want and last bulk transaction is in */
		if (data->count == EP_R_NUM && page->block == data->block
			&& data->continue_read) {
			/* set buffer idle */
			page->status = BUF_PAGE_IDLE;
			data->ep_in->request.buffer = page->buf;
			data->ep_in->request.size = data->geometry.bytes_per_sector
							* page->sec_num;
			data->ep_in->request.req_type = UIO_REQUEST_WRITE;
			rt_usbd_io_request(func->device, data->ep_in,
							&data->ep_in->request);
			data->status = STAT_SEND;

			/* pre-read next EP_R_NUM block */
			if (++data->rindex == R_BUF_PAGE_NUM)
				data->rindex = 0;
			page = &data->rpage[data->rindex];
			page->block = data->block + EP_R_NUM;
			page->sec_num = EP_R_NUM;
			page->status = BUF_PAGE_BUSY;
			ret = rt_mq_send(data->rpage_mq, (void *)page,
						sizeof(struct buf_page));
			if (ret)
				rt_kprintf("rt send mq failed");

			return data->geometry.bytes_per_sector;
		} else
			page->status = BUF_PAGE_IDLE;
	}

	/* warning !*/
	if (page->status != BUF_PAGE_IDLE)
		rt_kprintf("page is not idle!!");

	if (data->count >= EP_R_NUM)
		sec_count = EP_R_NUM;
	else
		sec_count = data->count;
	size = rt_device_read(data->disk, data->block, page->buf,
				sec_count);
	if (size == 0)
		rt_kprintf("read data error\n");

	data->ep_in->request.buffer = page->buf;
	data->ep_in->request.size = data->geometry.bytes_per_sector * sec_count;
	data->ep_in->request.req_type = UIO_REQUEST_WRITE;
	rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
	data->status = STAT_SEND;
	data->continue_read = RT_TRUE;

	/* pre-read next EP_R_NUM block */
	if (data->count == EP_R_NUM) {
		if (++data->rindex == R_BUF_PAGE_NUM)
			data->rindex = 0;
		page = &data->rpage[data->rindex];
		page->block = data->block + EP_R_NUM;
		page->sec_num = EP_R_NUM;
		page->status = BUF_PAGE_BUSY;
		ret = rt_mq_send(data->rpage_mq, (void *)page, sizeof(struct buf_page));
		if (ret)
			rt_kprintf("rt send mq failed");
	}

	return data->geometry.bytes_per_sector;
#else
    struct mstorage *data;
    rt_size_t size;
    
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);    
    RT_ASSERT(cbw != RT_NULL);

    data = (struct mstorage*)func->user_data;           
    data->block = cbw->cb[2]<<24 | cbw->cb[3]<<16 | cbw->cb[4]<<8  |
             cbw->cb[5]<<0;
    data->count = cbw->cb[7]<<8 | cbw->cb[8]<<0;

    RT_ASSERT(data->count < data->geometry.sector_count);

    data->csw_response.data_reside = data->cb_data_size;    
    size = rt_device_read(data->disk, data->block, data->ep_in->buffer, 1);
    if(size == 0)
    {
        rt_kprintf("read data error\n");
    }

    data->ep_in->request.buffer = data->ep_in->buffer;
    data->ep_in->request.size = data->geometry.bytes_per_sector;    
    data->ep_in->request.req_type = UIO_REQUEST_WRITE;    
    rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
    data->status = STAT_SEND;
    
    return data->geometry.bytes_per_sector;
#endif
}

#ifdef ARCH_LOMBO
static struct buf_page *_get_write_page(struct mstorage *data,
				 rt_uint32_t block, rt_uint32_t sec_count)
{
	struct buf_page *page;
	rt_base_t level;
	rt_bool_t need_lock = RT_FALSE;

	/* get page resource (total value of semaphore is W_BUF_PAGE_NUM) */
	rt_sem_take(data->wpage_sem, RT_WAITING_FOREVER);

	level = rt_hw_interrupt_disable();
	if (data->page_free == W_BUF_PAGE_NUM) {
		data->page_free--;
		need_lock = RT_TRUE;
	}
	rt_hw_interrupt_enable(level);

	/* get read/write lock */
	if (need_lock)
		rt_mutex_take(data->rw_mtx, RT_WAITING_FOREVER);

	data->windex++;
	if (data->windex == W_BUF_PAGE_NUM)
		data->windex = 0;

	page = (struct buf_page *)&data->wpage[data->windex];
	page->block = block;
	page->sec_num = sec_count;

	return page;
}

static void _return_write_page(struct mstorage *data)
{
	rt_base_t level;
	rt_err_t ret;

	/* release buffer */
	ret = rt_sem_release(data->wpage_sem);
	if (ret)
		rt_kprintf("release sem failed");

	level = rt_hw_interrupt_disable();
	data->page_free++;
	if (data->page_free == W_BUF_PAGE_NUM)
		rt_mutex_release(data->rw_mtx);
	rt_hw_interrupt_enable(level);
}
#endif

/**
 * This function will handle write_10 request.
 *
 * @param func the usb function object.
 * @param cbw the command block wrapper.
 *
 * @return RT_EOK on successful.
 */
static rt_size_t _write_10(ufunction_t func, ustorage_cbw_t cbw)
{
#ifdef ARCH_LOMBO
	struct mstorage *data;
	rt_size_t sec_count;
	struct buf_page *page;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	data = (struct mstorage *)func->user_data;

	data->block = cbw->cb[2]<<24 | cbw->cb[3]<<16 | cbw->cb[4]<<8  |
	cbw->cb[5]<<0;
	data->count = cbw->cb[7]<<8 | cbw->cb[8];
	data->csw_response.data_reside = cbw->xfer_len;
	data->size = data->count * data->geometry.bytes_per_sector;

	RT_DEBUG_LOG(RT_DEBUG_USB, ("_write_10 count 0x%x block 0x%x 0x%x\n",
	data->count, data->block, data->geometry.sector_count));

	data->csw_response.data_reside = data->cb_data_size;

	if (data->count > EP_W_NUM)
		sec_count = EP_W_NUM;
	else
		sec_count = data->count;

	page = _get_write_page(data, data->block, sec_count);
	data->continue_read = RT_FALSE;

	data->ep_out->request.buffer = page->buf;
	data->ep_out->request.size = data->geometry.bytes_per_sector * sec_count;
	data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;
	rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
	data->status = STAT_RECEIVE;

	return data->geometry.bytes_per_sector;
#else
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    RT_ASSERT(cbw != RT_NULL);

    data = (struct mstorage*)func->user_data;   

    data->block = cbw->cb[2]<<24 | cbw->cb[3]<<16 | cbw->cb[4]<<8  |
             cbw->cb[5]<<0;
    data->count = cbw->cb[7]<<8 | cbw->cb[8];
    data->csw_response.data_reside = cbw->xfer_len;
    data->size = data->count * data->geometry.bytes_per_sector;

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_write_10 count 0x%x block 0x%x 0x%x\n",
                                data->count, data->block, data->geometry.sector_count));

    data->csw_response.data_reside = data->cb_data_size;
    
    data->ep_out->request.buffer = data->ep_out->buffer;
    data->ep_out->request.size = data->geometry.bytes_per_sector;    
    data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;
    rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
    data->status = STAT_RECEIVE;
    
    return data->geometry.bytes_per_sector;
#endif
}

/**
 * This function will handle verify_10 request.
 *
 * @param func the usb function object.
 *
 * @return RT_EOK on successful.
 */
static rt_size_t _verify_10(ufunction_t func, ustorage_cbw_t cbw)
{
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_verify_10\n"));

    data = (struct mstorage*)func->user_data;
    data->csw_response.status = 0;
        
    return 0;
}

#ifdef ARCH_LOMBO
/* self scsi start */
static rt_size_t _lb_do_update(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	struct cmd_pkg_update	*pkg;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	data = (struct mstorage *)func->user_data;
	pkg = (struct cmd_pkg_update *)&cbw->cb[0];
	RT_DEBUG_LOG(RT_DEBUG_USB, ("0x%x, param0: 0x%x, param1: 0x%x\n",
		lb_cmd, pkg->param0, pkg->param1));

	/* update interface */
	rtc_pol_flag_set();
	rt_pm_reboot();
	data->status = STAT_LB_SEND;
	lb_cmd = UFI_UPD_CMD;

	return 0;
}

static rt_size_t _lb_prg_run(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	struct cmd_prg_run	*prg;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	data = (struct mstorage *)func->user_data;
	prg = (struct cmd_prg_run *)&cbw->cb[0];
	/* TODO! run software function */
	RT_DEBUG_LOG(RT_DEBUG_USB, ("0x%x, param0: 0x%x, param1: 0x%x\n",
		lb_cmd, prg->param0, prg->param1));

	data->status = STAT_LB_SEND;
	lb_cmd = UFI_RUN_CMD;

	return 0;
}

static rt_size_t _lb_get_status(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	struct data_get_status status_data;
	rt_size_t count;
	rt_uint8_t *buf;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	/* status data size is 16 byte */
	count = cbw->xfer_len;
	if (count != sizeof(struct data_get_status)) {
		rt_kprintf("%s status size error %d - %d\n",
			   __func__, count, sizeof(struct data_get_status));
		return 0;
	}

	data = (struct mstorage*)func->user_data;
	buf = data->ep_in->buffer;

	/* todo! need status interface */
	status_data.status = LB_STATUS_READY;
	status_data.interval = 0x200000;

	rt_memcpy(buf, &status_data, sizeof(struct data_get_status));

	data->cb_data_size = count;
	data->ep_in->request.buffer = buf;
	data->ep_in->request.size = data->cb_data_size;
	data->ep_in->request.req_type = UIO_REQUEST_WRITE;
	rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
	data->status = STAT_LB_SEND;
	lb_cmd = UFI_DEV_CMD;

	return data->cb_data_size;
}

static rt_size_t _lb_get_version(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	struct data_get_ver_info version_data;
	rt_size_t count;
	rt_uint8_t *buf;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	/* version data size is 32 byte */
	count = cbw->xfer_len;
	if (count != sizeof(struct data_get_ver_info)) {
		rt_kprintf("%s version size error %d - %d\n",
			__func__, count, sizeof(struct data_get_ver_info));
		return 0;
	}

	data = (struct mstorage*)func->user_data;
	buf = data->ep_in->buffer;

	/*  TODO! need the version interface */
	version_data.ver = 0x1000;
	version_data.sw_id = 0x2000;
	version_data.hw_id = 0xe302;

	rt_memcpy(buf, &version_data, sizeof(struct data_get_ver_info));

	data->cb_data_size = count;
	data->ep_in->request.buffer = buf;
	data->ep_in->request.size = data->cb_data_size;
	data->ep_in->request.req_type = UIO_REQUEST_WRITE;
	rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
	data->status = STAT_LB_SEND;
	lb_cmd = UFI_VER_CMD;

	return data->cb_data_size;
}

static rt_size_t _lb_get_swinfo(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	rt_size_t count;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	/* swinfo data size is 4 kbyte */
	count = cbw->xfer_len;
	if (count != sizeof(struct data_get_swinfo)) {
		rt_kprintf("%s version size error %d - %d\n",
			   __func__, count, sizeof(struct data_get_swinfo));
		return 0;
	}

	data = (struct mstorage*)func->user_data;

	/*  TODO! need the swinfo interface */
	rt_memset(in_buf, 0xee, sizeof(struct data_get_swinfo));

	data->cb_data_size = count;
	data->ep_in->request.buffer = in_buf;
	data->ep_in->request.size = data->cb_data_size;
	data->ep_in->request.req_type = UIO_REQUEST_WRITE;
	rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
	data->status = STAT_LB_SEND;
	lb_cmd = UFI_SWI_CMD;

	return data->cb_data_size;
}

static rt_size_t _lb_get_hwinfo(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	rt_size_t count;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	/* hwinfo data size is 4 kbyte */
	count = cbw->xfer_len;
	if (count != sizeof(struct data_get_hwinfo)) {
		rt_kprintf("%s version size error %d - %d\n",
			   __func__, count, sizeof(struct data_get_hwinfo));
		return 0;
	}

	data = (struct mstorage*)func->user_data;

	/*  TODO! need the swinfo interface */
	rt_memset(in_buf, 0xaa, sizeof(struct data_get_hwinfo));

	data->cb_data_size = count;
	data->ep_in->request.buffer = in_buf;
	data->ep_in->request.size = data->cb_data_size;
	data->ep_in->request.req_type = UIO_REQUEST_WRITE;
	rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
	data->status = STAT_LB_SEND;
	lb_cmd = UFI_HWI_CMD;

	return data->cb_data_size;
}

static rt_size_t _lb_reg_write(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	rt_size_t sec_count;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	data = (struct mstorage *)func->user_data;
	data->csw_response.data_reside = cbw->xfer_len;
	data->size = cbw->xfer_len;

	/*  trans size large than MAX_TRANS_LEN */
	if (cbw->xfer_len > MAX_TRANS_LEN) {
		sec_count = MAX_TRANS_LEN;
		rt_kprintf("%s data size 0x%x\n", __func__);
	} else
		sec_count = cbw->xfer_len;

	rt_memset(out_buf, 0x0, sec_count);

	data->ep_out->request.buffer = out_buf;
	data->ep_out->request.size = sec_count;
	data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;
	rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
	data->status = STAT_LB_REV;
	lb_cmd = UFI_REG_CMD;

	return sec_count;
}

static rt_size_t _lb_reg_read(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	rt_size_t count;
	rt_size_t sec_count;
	rt_size_t reg_num;
	rt_size_t i;
	struct data_reg_xfer *reg_data;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	/* register size */
	count = cbw->xfer_len;
	if (count != g_reg_cmd.len) {
		rt_kprintf("%s read reg size error %d - %d\n",
			   __func__, count, g_reg_cmd.len);
		return 0;
	}

	data = (struct mstorage*)func->user_data;
	data->csw_response.data_reside = cbw->xfer_len;
	data->size = cbw->xfer_len;

	/*  trans size large than MAX_TRANS_LEN */
	if (cbw->xfer_len > MAX_TRANS_LEN) {
		sec_count = MAX_TRANS_LEN;
		rt_kprintf("%s data size 0x%x\n", __func__);
	} else
		sec_count = cbw->xfer_len;

	rt_memset(in_buf, 0, sec_count);
	reg_data = (struct data_reg_xfer *)in_buf;

	/* get register number */
	reg_num = sec_count / sizeof(struct data_reg_xfer);

	for (i = 0; i < reg_num; i++) {
		reg_data[i].addr = g_reg_cmd.addr + 4 * i;
		reg_data[i].val = readl(VA_ADDR | reg_data[i].addr);
	}

	data->cb_data_size = sec_count;
	data->ep_in->request.buffer = in_buf;
	data->ep_in->request.size = data->cb_data_size;
	data->ep_in->request.req_type = UIO_REQUEST_WRITE;
	rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
	data->status = STAT_LB_SEND;
	lb_cmd = UFI_REG_CMD;

	return data->cb_data_size;
}

static rt_size_t _lb_reg_ops(ufunction_t func, ustorage_cbw_t cbw)
{
	rt_size_t sec_count = 0;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	memcpy(&g_reg_cmd, &cbw->cb[0], sizeof(struct cmd_reg_xfer));

	if (cbw->dflags == USB_DIR_OUT) {
		/* write register */
		sec_count = _lb_reg_write(func, cbw);
	} else if (cbw->dflags == USB_DIR_IN) {
		/* read register */
		sec_count = _lb_reg_read(func, cbw);
	} else
		rt_kprintf("mem cmd dir err 0x%x\n", cbw->dflags);

	return sec_count;
}

static rt_size_t _lb_mem_write(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	rt_size_t sec_count;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	data = (struct mstorage *)func->user_data;
	data->csw_response.data_reside = g_mem_cmd.len;
	data->size = g_mem_cmd.len;

	/*  trans size large than MAX_TRANS_LEN */
	if (g_mem_cmd.len > MAX_TRANS_LEN) {
		sec_count = MAX_TRANS_LEN;
		rt_kprintf("%s data size 0x%x\n", __func__, sec_count);
	} else
		sec_count = g_mem_cmd.len;

	rt_memset(out_buf, 0x0, sec_count);

	data->ep_out->request.buffer = out_buf;
	data->ep_out->request.size = sec_count;
	data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;
	rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
	data->status = STAT_LB_REV;
	lb_cmd = UFI_MEM_CMD;

	return sec_count;
}

static rt_size_t _lb_mem_read(ufunction_t func, ustorage_cbw_t cbw)
{
	struct mstorage *data;
	rt_size_t sec_count;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	data = (struct mstorage*)func->user_data;
	data->csw_response.data_reside = g_mem_cmd.len;
	data->size = g_mem_cmd.len;

	/*  trans size large than MAX_TRANS_LEN */
	if (g_mem_cmd.len > MAX_TRANS_LEN) {
		sec_count = MAX_TRANS_LEN;
		rt_kprintf("%s note: data size 0x%x\n", __func__, g_mem_cmd.len);
	} else
		sec_count = g_mem_cmd.len;

	rt_memset(in_buf, 0, sec_count);
	/* send memory to PC, buffer start addr is  in_buf, */
	/* buffer data size is sec_count */
	/* todo! copy data to expect scenarios */
	isp_usb_read(&g_mem_cmd, in_buf);
	/* for test. */
	/* rt_memset(in_buf, 0x3, sec_count); */

	data->cb_data_size = sec_count;
	data->ep_in->request.buffer = in_buf;
	data->ep_in->request.size = data->cb_data_size;
	data->ep_in->request.req_type = UIO_REQUEST_WRITE;
	rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
	data->status = STAT_LB_SEND;
	lb_cmd = UFI_MEM_CMD;

	return data->cb_data_size;
}

static rt_size_t _lb_mem_ops(ufunction_t func, ustorage_cbw_t cbw)
{
	rt_size_t sec_count = 0;

	RT_ASSERT(func != RT_NULL);
	RT_ASSERT(func->device != RT_NULL);
	RT_ASSERT(cbw != RT_NULL);

	memcpy(&g_mem_cmd, &cbw->cb[0], sizeof(struct cmd_mem_xfer));

	if (cbw->dflags == USB_DIR_OUT) {
		/* write mem */
		sec_count = _lb_mem_write(func, cbw);
	} else if (cbw->dflags == USB_DIR_IN) {
		/* read mem */
		sec_count = _lb_mem_read(func, cbw);
	} else
		rt_kprintf("mem cmd dir err 0x%x\n", cbw->dflags);
	return sec_count;
}
/* self scsi end */
#endif

static rt_size_t _start_stop(ufunction_t func, 
    ustorage_cbw_t cbw)
{
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL); 

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_start_stop\n"));

    data = (struct mstorage*)func->user_data;
    data->csw_response.status = 0;
        
    return 0;
}

static rt_err_t _ep_in_handler(ufunction_t func, rt_size_t size)
{
    struct mstorage *data;
#ifdef ARCH_LOMBO
	rt_size_t sec_count;
	struct buf_page *page;
	rt_err_t ret;
	/* self scsi start */
	rt_size_t tmp_len;
	rt_size_t start_num;
	rt_size_t reg_num;
	rt_size_t i;
	struct data_reg_xfer *reg_data;
	/* self scsi end */
#endif
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_ep_in_handler\n"));
    
    data = (struct mstorage*)func->user_data;   

    switch(data->status)
    {
    case STAT_CSW:
        if(data->ep_in->request.size != SIZEOF_CSW)
        {
            rt_kprintf("Size of csw command error\n");
            rt_usbd_ep_set_stall(func->device, data->ep_in);            
        }
        else
        {
            RT_DEBUG_LOG(RT_DEBUG_USB, ("return to cbw status\n"));
            data->ep_out->request.buffer = data->ep_out->buffer;
            data->ep_out->request.size = SIZEOF_CBW;
            data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;    
            rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);            
            data->status = STAT_CBW;
        }
        break;
     case STAT_CMD:
        if(data->csw_response.data_reside == 0xFF)
        {
            data->csw_response.data_reside = 0;
        }
        else
        {
            data->csw_response.data_reside -= data->ep_in->request.size;
            if(data->csw_response.data_reside != 0)
            {            
                RT_DEBUG_LOG(RT_DEBUG_USB, ("data_reside %d, request %d\n", 
                    data->csw_response.data_reside, data->ep_in->request.size));
                if(data->processing->dir == DIR_OUT)
                {
                    rt_usbd_ep_set_stall(func->device, data->ep_out);
                }
                else
                {
                    rt_usbd_ep_set_stall(func->device, data->ep_in);                    
                }
                data->csw_response.data_reside = 0;
            }
            }
        _send_status(func);
        break;
     case STAT_SEND:        
#ifdef ARCH_LOMBO
	sec_count = data->ep_in->request.size / data->geometry.bytes_per_sector;
	data->csw_response.data_reside -= data->ep_in->request.size;
	data->count -= sec_count;
	data->block += sec_count;

	if (data->count > 0 && data->csw_response.data_reside > 0) {

		/* Unlikely go here */

		page = &data->rpage[data->rindex];
		/* Check whether read buf is idle */
		if (page->status != BUF_PAGE_IDLE) {
			/* wait ready */
			rt_sem_take(data->rpage_sem, RT_WAITING_FOREVER);
			if (page->status == BUF_PAGE_READY)
				rt_kprintf("buf page should be ready!");

			/* buf is exactly what we want */
			if (data->count == EP_R_NUM && page->block == data->block) {
				/* set buffer idle */
				page->status = BUF_PAGE_IDLE;
				data->ep_in->request.buffer = page->buf;
				data->ep_in->request.size =
					data->geometry.bytes_per_sector * page->sec_num;
				data->ep_in->request.req_type = UIO_REQUEST_WRITE;
				rt_usbd_io_request(func->device, data->ep_in,
							&data->ep_in->request);
				data->status = STAT_SEND;

				/* pre-read next EP_R_NUM block */
				if (++data->rindex == R_BUF_PAGE_NUM)
					data->rindex = 0;
				page = &data->rpage[data->rindex];
				page->block = data->block + EP_R_NUM;
				page->sec_num = EP_R_NUM;
				page->status = BUF_PAGE_BUSY;
				ret = rt_mq_send(data->rpage_mq, (void *)page,
							sizeof(struct buf_page));
				if (ret)
					rt_kprintf("rt send mq failed");

				return data->geometry.bytes_per_sector;
			} else
				page->status = BUF_PAGE_IDLE;
		}

		if (data->count >= EP_R_NUM)
			sec_count = EP_R_NUM;
		else
			sec_count = data->count;
		size = rt_device_read(data->disk, data->block, page->buf,
					sec_count);
		if (size == 0)
			rt_kprintf("read data error\n");

		data->ep_in->request.buffer = page->buf;
		data->ep_in->request.size = data->geometry.bytes_per_sector * sec_count;
		data->ep_in->request.req_type = UIO_REQUEST_WRITE;
		rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);
		data->status = STAT_SEND;

		/* pre-read next EP_R_NUM block */
		if (data->count == EP_R_NUM) {
			if (++data->rindex == R_BUF_PAGE_NUM)
				data->rindex = 0;
			page = &data->rpage[data->rindex];
			page->block = data->block + EP_R_NUM;
			page->sec_num = EP_R_NUM;
			page->status = BUF_PAGE_BUSY;
			ret = rt_mq_send(data->rpage_mq, (void *)page,
							sizeof(struct buf_page));
			if (ret)
				rt_kprintf("rt send mq failed");
		}
	} else {
		rt_mutex_release(data->rw_mtx);
		_send_status(func);
	}
	break;
#else
        data->csw_response.data_reside -= data->ep_in->request.size;
        data->count--;    
        data->block++;        
        if(data->count > 0 && data->csw_response.data_reside > 0)
        {
            if(rt_device_read(data->disk, data->block, data->ep_in->buffer, 1) == 0)
            {
                rt_kprintf("disk read error\n");
                rt_usbd_ep_set_stall(func->device, data->ep_in);
                return -RT_ERROR;                
            }

            data->ep_in->request.buffer = data->ep_in->buffer;
            data->ep_in->request.size = data->geometry.bytes_per_sector;    
            data->ep_in->request.req_type = UIO_REQUEST_WRITE;    
            rt_usbd_io_request(func->device, data->ep_in, &data->ep_in->request);            
        }
        else
        {
            _send_status(func);            
        }        
        break;
#endif
#ifdef ARCH_LOMBO
/* self scsi start */
    case STAT_LB_SEND:
	data->csw_response.data_reside -= data->ep_in->request.size;
	RT_DEBUG_LOG(RT_DEBUG_USB, ("%s cmd: 0x%x, reside data: %d, size: %d\n",
		     __func__, lb_cmd, data->csw_response.data_reside,
		     data->ep_in->request.size));

	if (lb_cmd == UFI_REG_CMD) {
		if (data->csw_response.data_reside > 0)  {
			tmp_len = g_reg_cmd.len - data->csw_response.data_reside;
			start_num = tmp_len / sizeof(struct data_reg_xfer);

			/*  trans size large than MAX_TRANS_LEN */
			if (data->csw_response.data_reside > MAX_TRANS_LEN) {
				sec_count = MAX_TRANS_LEN;
				rt_kprintf("%s reg data size 0x%x\n", __func__);
			} else
				sec_count = data->csw_response.data_reside;

			rt_memset(in_buf, 0, sec_count);
			reg_data = (struct data_reg_xfer *)in_buf;

			/* get register number */
			reg_num = sec_count / sizeof(struct data_reg_xfer);

			for (i = 0; i < reg_num; i++) {
				reg_data[i].addr = g_reg_cmd.addr + 4 * (i + start_num);
				reg_data[i].val = readl(VA_ADDR | reg_data[i].addr);
			}

			data->cb_data_size = sec_count;
			data->ep_in->request.buffer = in_buf;
			data->ep_in->request.size = data->cb_data_size;
			data->ep_in->request.req_type = UIO_REQUEST_WRITE;
			rt_usbd_io_request(func->device, data->ep_in,
					   &data->ep_in->request);
		} else
			_send_status(func);
	} else if (lb_cmd == UFI_MEM_CMD) {
		if (data->csw_response.data_reside > 0)  {
			/*  trans size large than MAX_TRANS_LEN */
			if (data->csw_response.data_reside > MAX_TRANS_LEN)
				sec_count = MAX_TRANS_LEN;
			else
				sec_count = data->csw_response.data_reside;

			rt_memset(in_buf, 0, sec_count);
			/* send memory to PC, buffer start addr is  in_buf, */
			/* buffer data size is sec_count */

			/* todo! copy data to expect scenarios */
			/* isp_usb_read(&g_mem_cmd, in_buf); */
			/* for test. */
			/* rt_memset(in_buf, 0x2, sec_count); */

			data->cb_data_size = sec_count;
			data->ep_in->request.buffer = in_buf;
			data->ep_in->request.size = data->cb_data_size;
			data->ep_in->request.req_type = UIO_REQUEST_WRITE;
			rt_usbd_io_request(func->device, data->ep_in,
					   &data->ep_in->request);
		} else
			_send_status(func);
	} else
		_send_status(func);

        break;
/* self scsi end */
#endif
     }

     return RT_EOK;
}

#ifdef  MASS_CBW_DUMP
static void cbw_dump(struct ustorage_cbw* cbw)
{
    RT_ASSERT(cbw != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("signature 0x%x\n", cbw->signature));
    RT_DEBUG_LOG(RT_DEBUG_USB, ("tag 0x%x\n", cbw->tag));
    RT_DEBUG_LOG(RT_DEBUG_USB, ("xfer_len 0x%x\n", cbw->xfer_len));
    RT_DEBUG_LOG(RT_DEBUG_USB, ("dflags 0x%x\n", cbw->dflags));
    RT_DEBUG_LOG(RT_DEBUG_USB, ("lun 0x%x\n", cbw->lun));
    RT_DEBUG_LOG(RT_DEBUG_USB, ("cb_len 0x%x\n", cbw->cb_len));
    RT_DEBUG_LOG(RT_DEBUG_USB, ("cb[0] 0x%x\n", cbw->cb[0]));
}
#endif

static struct scsi_cmd* _find_cbw_command(rt_uint16_t cmd)
{
    int i;

    for(i=0; i<sizeof(cmd_data)/sizeof(struct scsi_cmd); i++)
    {
        if(cmd_data[i].cmd == cmd)
            return &cmd_data[i];
    }    

    return RT_NULL;
}

static void _cb_len_calc(ufunction_t func, struct scsi_cmd* cmd,
    ustorage_cbw_t cbw)
{
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(cmd != RT_NULL);
    RT_ASSERT(cbw != RT_NULL);

    data = (struct mstorage*)func->user_data;   
    if(cmd->cmd_len == 6)
    {
        switch(cmd->type)
        {
        case COUNT:
            data->cb_data_size = cbw->cb[4];
            break;
        case BLOCK_COUNT:
            data->cb_data_size = cbw->cb[4] * data->geometry.bytes_per_sector;
            break;
        case FIXED:
            data->cb_data_size = cmd->data_size;
            break;
        default:       
            break;
        }        
    }
    else if(cmd->cmd_len == 10)
    {
        switch(cmd->type)
        {
        case COUNT:
            data->cb_data_size = cbw->cb[7]<<8 | cbw->cb[8];
            break;
        case BLOCK_COUNT:
            data->cb_data_size = (cbw->cb[7]<<8 | cbw->cb[8]) * 
                data->geometry.bytes_per_sector;
            break;
        case FIXED:
            data->cb_data_size = cmd->data_size;
            break;
        default:      
            break;
        }
    }
#ifdef ARCH_LOMBO
    /* self scsi start */
    else if(cmd->cmd_len == 16)
    {
        data->cb_data_size = cbw->xfer_len;
    }
    /* self scsi end */
#endif
    else
    {
	rt_kprintf("cmd_len error %d\n", cmd->cmd_len);
    }
}

static rt_bool_t _cbw_verify(ufunction_t func, struct scsi_cmd* cmd,
    ustorage_cbw_t cbw)
{
    struct mstorage *data;

    RT_ASSERT(cmd != RT_NULL);
    RT_ASSERT(cbw != RT_NULL);
    RT_ASSERT(func != RT_NULL);

    data = (struct mstorage*)func->user_data;   
    if(cmd->cmd_len != cbw->cb_len)
    {
	cbw->cb_len = cmd->cmd_len;
    }

    if(cbw->xfer_len > 0 && data->cb_data_size == 0)
    {
        rt_kprintf("xfer_len > 0 && data_size == 0\n");
        return RT_FALSE;
    }

    if(cbw->xfer_len == 0 && data->cb_data_size > 0)
    {
        rt_kprintf("xfer_len == 0 && data_size > 0");
        return RT_FALSE;
    }

    if(((cbw->dflags & USB_DIR_IN) && (cmd->dir == DIR_OUT)) ||
        (!(cbw->dflags & USB_DIR_IN) && (cmd->dir == DIR_IN)))
    {
        rt_kprintf("dir error\n");
        return RT_FALSE;
    }

    if(cbw->xfer_len > data->cb_data_size)
    {
	RT_DEBUG_LOG(RT_DEBUG_USB, ("xfer_len(%d) > data_size(%d)\n",
				cbw->xfer_len, data->cb_data_size));
        return RT_FALSE;
    }
    
    if(cbw->xfer_len < data->cb_data_size)
    {
        data->cb_data_size = cbw->xfer_len;
        data->csw_response.status = 1;
    }

    return RT_TRUE; 
}

static rt_size_t _cbw_handler(ufunction_t func, struct scsi_cmd* cmd,
    ustorage_cbw_t cbw)
{    
    struct mstorage *data;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(cbw != RT_NULL);
    RT_ASSERT(cmd->handler != RT_NULL);
        
    data = (struct mstorage*)func->user_data;
    data->processing = cmd;
    return cmd->handler(func, cbw);
}

/**
 * This function will handle mass storage bulk out endpoint request.
 *
 * @param func the usb function object.
 * @param size request size.
 *
 * @return RT_EOK.
 */
static rt_err_t _ep_out_handler(ufunction_t func, rt_size_t size)
{
    struct mstorage *data;
    struct scsi_cmd* cmd;
    rt_size_t len;
    struct ustorage_cbw* cbw;
    
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("_ep_out_handler %d\n", size));
    
    data = (struct mstorage*)func->user_data;
    cbw = (struct ustorage_cbw*)data->ep_out->buffer;    
    if(data->status == STAT_CBW)
    {
        /* dump cbw information */
        if(cbw->signature != CBW_SIGNATURE || size != SIZEOF_CBW)
        {
            goto exit;
        }        

        data->csw_response.signature = CSW_SIGNATURE;
        data->csw_response.tag = cbw->tag;
        data->csw_response.data_reside = cbw->xfer_len;
        data->csw_response.status = 0;

	RT_DEBUG_LOG(RT_DEBUG_USB, ("cmd[0x%x], ep_out reside %d\n",
				cbw->cb[0], data->csw_response.data_reside));

        cmd = _find_cbw_command(cbw->cb[0]);
        if(cmd == RT_NULL)
        {
            rt_kprintf("can't find cbw command\n");
            goto exit;
        }        

        _cb_len_calc(func, cmd, cbw);
        if(!_cbw_verify(func, cmd, cbw))
        {
            goto exit;
        }
        
        len = _cbw_handler(func, cmd, cbw);
        if(len == 0)
        {
            _send_status(func);
        }      
        
        return RT_EOK;        
    }
    else if(data->status == STAT_RECEIVE)
    {
#ifdef ARCH_LOMBO
	rt_size_t sec_count;
	rt_err_t ret;
	struct buf_page *page;

	RT_DEBUG_LOG(RT_DEBUG_USB, ("\nwrite size %d block 0x%x oount 0x%x\n",
			size, data->block, data->size));
	data->size -= size;
	data->csw_response.data_reside -= size;
	sec_count = size / data->geometry.bytes_per_sector;
	data->count -= sec_count;
	data->block += sec_count;

	/* get current page buffer */
	page = (struct buf_page *)&data->wpage[data->windex];
	ret = rt_mq_send(data->page_mq, (void *)page, sizeof(struct buf_page));
	if (ret)
		rt_kprintf("rt send mq failed");

	if (data->csw_response.data_reside != 0) {
		if (data->count > EP_W_NUM)
			sec_count = EP_W_NUM;
		else
			sec_count = data->count;

		page = _get_write_page(data, data->block, sec_count);
		data->ep_out->request.buffer = page->buf;
		data->ep_out->request.size = data->geometry.bytes_per_sector * sec_count;
		data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;
		rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
	} else
		_send_status(func);

	return RT_EOK;
#else
        RT_DEBUG_LOG(RT_DEBUG_USB, ("\nwrite size %d block 0x%x oount 0x%x\n",
                                    size, data->block, data->size));
        
        data->size -= size;
        data->csw_response.data_reside -= size;

        rt_device_write(data->disk, data->block, data->ep_out->buffer, 1);

        if(data->csw_response.data_reside != 0)
        {
            data->ep_out->request.buffer = data->ep_out->buffer;
            data->ep_out->request.size = data->geometry.bytes_per_sector;    
            data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;    
            rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
            data->block ++;                        
        }
        else
        {
            _send_status(func);
        }

        return RT_EOK;
#endif
#ifdef ARCH_LOMBO
/* self scsi start */
    }else if(data->status == STAT_LB_REV) {
	data->size -= size;
	data->csw_response.data_reside -= size;

	RT_DEBUG_LOG(RT_DEBUG_USB, ("%s %d, size: %d, data size: %d, data reside: %d\n",
		  __func__, __LINE__, size, data->size, data->csw_response.data_reside));

	if (lb_cmd == UFI_REG_CMD) {
		struct data_reg_xfer *reg_data;
		rt_size_t reg_num, i;
		rt_uint32_t reg_base = 0;
		rt_size_t sec_count;

		reg_num = size / sizeof(struct data_reg_xfer);
		reg_data = (struct data_reg_xfer *)out_buf;
		for (i = 0; i < reg_num; i++) {
			reg_base = reg_data[i].addr | VA_ADDR;
			writel(reg_data[i].val, reg_base);
		}

		if (data->csw_response.data_reside != 0) {
			if (data->csw_response.data_reside > MAX_TRANS_LEN)
				sec_count = MAX_TRANS_LEN;
			else
				sec_count = data->csw_response.data_reside;

			rt_memset(out_buf, 0x0, sec_count);
			data->ep_out->request.buffer = out_buf;
			data->ep_out->request.size = sec_count;
			data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;
			rt_usbd_io_request(func->device, data->ep_out,
					   &data->ep_out->request);
		} else
			_send_status(func);

		return RT_EOK;
	} else if (lb_cmd == UFI_MEM_CMD) {
		rt_uint8_t *data_buf;
		rt_size_t sec_count;

		/* receive memory form PC, buffer start addr is  data_buf, */
		/* buffer data size is sec_count */
		data_buf = out_buf;
		sec_count = size;
		/* todo! copy data to expect scenarios */
		isp_usb_write(&g_mem_cmd, data_buf);

		if (data->csw_response.data_reside != 0) {
			if (data->csw_response.data_reside > MAX_TRANS_LEN)
				sec_count = MAX_TRANS_LEN;
			else
				sec_count = data->csw_response.data_reside;

			rt_memset(out_buf, 0x0, sec_count);
			data->ep_out->request.buffer = out_buf;
			data->ep_out->request.size = sec_count;
			data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;
			rt_usbd_io_request(func->device, data->ep_out,
					   &data->ep_out->request);
		} else
			_send_status(func);

		return RT_EOK;
	} else {
		_send_status(func);
		return RT_EOK;
	}
/* scsi self end */
#endif
    }

exit:
    if(data->csw_response.data_reside)
    {
        if(cbw->dflags & USB_DIR_IN)
        {
            rt_usbd_ep_set_stall(func->device, data->ep_in);
        }
        else
        {
            rt_usbd_ep_set_stall(func->device, data->ep_in);
            rt_usbd_ep_set_stall(func->device, data->ep_out);
        }
    }
    data->csw_response.status = 1;
    _send_status(func);
    
    return -RT_ERROR;
}

/**
 * This function will handle mass storage interface request.
 *
 * @param func the usb function object.
 * @param setup the setup request.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _interface_handler(ufunction_t func, ureq_t setup)
{
	static rt_uint8_t lun;
    
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);    
    RT_ASSERT(setup != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("mstorage_interface_handler\n"));

    switch(setup->bRequest)
    {
    case USBREQ_GET_MAX_LUN:        
        
        RT_DEBUG_LOG(RT_DEBUG_USB, ("USBREQ_GET_MAX_LUN\n"));
        
        if(setup->wValue || setup->wLength != 1)
        {        
            rt_usbd_ep0_set_stall(func->device);
        }
        else
        {
            rt_usbd_ep0_write(func->device, &lun, setup->wLength);
        }
        break;
    case USBREQ_MASS_STORAGE_RESET:

        RT_DEBUG_LOG(RT_DEBUG_USB, ("USBREQ_MASS_STORAGE_RESET\n"));
        
        if(setup->wValue || setup->wLength != 0)
        {
            rt_usbd_ep0_set_stall(func->device);
        }
        else
        {   
            dcd_ep0_send_status(func->device->dcd);     
        }
        break;
    default:
        rt_kprintf("unknown interface request\n");
        break;
    }

    return RT_EOK;
}

#ifdef ARCH_LOMBO
static void rt_usbd_ms_write_thread(void *parameter)
{
	struct mstorage *data = (struct mstorage *)parameter;
	struct buf_page page;
	rt_err_t ret;

	do {
		ret = rt_mq_recv(data->page_mq, &page, sizeof(struct buf_page),
				RT_WAITING_FOREVER);
		if (ret) {
			rt_kprintf("mq recv failed");
			continue;
		}

		rt_device_write(data->disk, page.block, page.buf, page.sec_num);

		_return_write_page(data);
	} while (1);
}

static void rt_usbd_ms_read_thread(void *parameter)
{
	struct mstorage *data = (struct mstorage *)parameter;
	struct buf_page page;
	rt_err_t ret;

	do {
		ret = rt_mq_recv(data->rpage_mq, &page, sizeof(struct buf_page),
				RT_WAITING_FOREVER);
		if (ret) {
			rt_kprintf("read mq recv failed");
			continue;
		}

		rt_device_read(data->disk, page.block, page.buf, page.sec_num);
		data->rpage[data->rindex].status = BUF_PAGE_READY;

		/* release buffer */
		ret = rt_sem_release(data->rpage_sem);
		if (ret)
			rt_kprintf("release sem failed");
	} while (1);
}
#endif

/**
 * This function will run mass storage function, it will be called on handle set configuration request.
 *
 * @param func the usb function object.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _function_enable(ufunction_t func)
{
    struct mstorage *data;
    RT_ASSERT(func != RT_NULL);
    RT_DEBUG_LOG(RT_DEBUG_USB, ("Mass storage function enabled\n"));
    data = (struct mstorage*)func->user_data;   
#ifdef ARCH_LOMBO
	int i;
	struct buf_page *page;
#endif
    data->disk = rt_device_find(RT_USB_MSTORAGE_DISK_NAME);
    if(data->disk == RT_NULL)
    {
        rt_kprintf("no data->disk named %s\n", RT_USB_MSTORAGE_DISK_NAME);
        return -RT_ERROR;
    }
#ifndef ARCH_LOMBO
    if(rt_device_open(data->disk, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("disk open error\n");
        return -RT_ERROR;
    }
#endif

#ifdef ARCH_LOMBO
    /*  self scsi start */
    lb_cmd = UFI_NONE_CMD;
    rt_memset(&g_reg_cmd, 0, sizeof(struct cmd_reg_xfer));
    rt_memset(&g_mem_cmd, 0, sizeof(struct cmd_mem_xfer));

    in_buf = (rt_uint8_t *)rt_malloc(MAX_TRANS_LEN);
    if (in_buf == RT_NULL)
    {
        rt_kprintf("%s malloc in buf %d fail\n", __func__, MAX_TRANS_LEN);
        return -RT_ENOMEM;
    }

    out_buf = (rt_uint8_t *)rt_malloc(MAX_TRANS_LEN);
    if (out_buf == RT_NULL)
    {
        rt_kprintf("%s malloc out buf %d fail\n", __func__, MAX_TRANS_LEN);
        return -RT_ENOMEM;
    }
    /*  self scsi end */
#endif
    if(rt_device_control(data->disk, RT_DEVICE_CTRL_BLK_GETGEOME, 
        (void*)&data->geometry) != RT_EOK)
    {
        rt_kprintf("get disk info error\n");
        return -RT_ERROR;
    }
#ifdef ARCH_LOMBO
	data->ep_in->buffer = (rt_uint8_t *)rt_malloc(
			data->geometry.bytes_per_sector * EP_W_NUM);
#else
    data->ep_in->buffer = (rt_uint8_t*)rt_malloc(data->geometry.bytes_per_sector);
#endif
    if(data->ep_in->buffer == RT_NULL)
    {
        rt_kprintf("no memory\n");
        return -RT_ENOMEM;
    }
#ifdef ARCH_LOMBO
	data->windex = 0;
	for (i = 0; i < W_BUF_PAGE_NUM; i++) {
		page = &data->wpage[i];
		page->buf = (rt_uint8_t *)rt_malloc(data->geometry.bytes_per_sector
				* EP_W_NUM);
		if (page->buf == RT_NULL) {
			rt_kprintf("alloc page buf failed");
			return -RT_ENOMEM;
		}
	}

	data->rindex = 0;
	for (i = 0; i < R_BUF_PAGE_NUM; i++) {
		page = &data->rpage[i];
		page->buf = rt_malloc(data->geometry.bytes_per_sector * EP_W_NUM);
		if (page->buf == RT_NULL) {
			rt_kprintf("alloc read page buf failed");
			return -RT_EIO;
		}
	}

	data->continue_read = RT_FALSE;
	data->page_free = W_BUF_PAGE_NUM;
	data->page_mq = rt_mq_create("usbd_wmq", sizeof(struct buf_page),
					W_BUF_PAGE_NUM, RT_IPC_FLAG_FIFO);
	if (data->page_mq == RT_NULL) {
		rt_kprintf("usb mq create failed");
		return -RT_EIO;
	}

	data->rpage_mq = rt_mq_create("usbd_rmq", sizeof(struct buf_page),
					W_BUF_PAGE_NUM, RT_IPC_FLAG_FIFO);
	if (data->rpage_mq == RT_NULL) {
		rt_kprintf("usb mq create failed");
		return -RT_EIO;
	}

	data->wpage_sem = rt_sem_create("usbd_ws", W_BUF_PAGE_NUM, RT_IPC_FLAG_FIFO);
	if (data->wpage_sem == RT_NULL) {
		rt_kprintf("usb sem create failed");
		return -RT_EIO;
	}

	data->rpage_sem = rt_sem_create("usbd_rs", 0, RT_IPC_FLAG_FIFO);
	if (data->rpage_sem == RT_NULL) {
		rt_kprintf("usb sem create failed");
		return -RT_EIO;
	}

	data->rw_mtx = rt_mutex_create("usbd_mtx", RT_IPC_FLAG_FIFO);
	if (data->rw_mtx == RT_NULL) {
		rt_kprintf("usb mutex create failed");
		return -RT_EIO;
	}

	/* init usb device write thread */
	data->usb_write_thread = rt_thread_create("usbd_write",
			rt_usbd_ms_write_thread, data,
			RT_USBD_THREAD_STACK_SZ, RT_USBD_THREAD_PRIO + 2, 20);
	/* rt_thread_init should always be OK, so start the thread
	* without further * checking.
	*/
	rt_thread_startup(data->usb_write_thread);

	/* init usb device read thread */
	data->usb_read_thread = rt_thread_create("usbd_read",
			rt_usbd_ms_read_thread, data,
			RT_USBD_THREAD_STACK_SZ, RT_USBD_THREAD_PRIO + 1, 20);
	/* rt_thread_init should always be OK, so start the thread
	* without further * checking.
	*/
	rt_thread_startup(data->usb_read_thread);

	data->ep_out->buffer = (rt_uint8_t *)rt_malloc(data->geometry.bytes_per_sector);
#else
    data->ep_out->buffer = (rt_uint8_t*)rt_malloc(data->geometry.bytes_per_sector);
#endif
    if(data->ep_out->buffer == RT_NULL)
    {
        rt_free(data->ep_in->buffer);
        rt_kprintf("no memory\n");
        return -RT_ENOMEM;
    }    
 
    /* prepare to read CBW request */
    data->ep_out->request.buffer = data->ep_out->buffer;
    data->ep_out->request.size = SIZEOF_CBW;    
    data->ep_out->request.req_type = UIO_REQUEST_READ_FULL;    
    rt_usbd_io_request(func->device, data->ep_out, &data->ep_out->request);
    
    return RT_EOK;
}

/**
 * This function will stop mass storage function, it will be called on handle set configuration request.
 *
 * @param device the usb device object.
 *
 * @return RT_EOK on successful.
 */
static rt_err_t _function_disable(ufunction_t func)
{
    struct mstorage *data;
    RT_ASSERT(func != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("Mass storage function disabled\n"));

    data = (struct mstorage*)func->user_data;   
#ifdef ARCH_LOMBO
	struct buf_page *page;
	rt_err_t ret;
	int i;

	ret = rt_thread_delete(data->usb_read_thread);
	if (ret)
		rt_kprintf("delete usb read thread failed");

	ret = rt_thread_delete(data->usb_write_thread);
	if (ret)
		rt_kprintf("delete usb read thread failed");

	ret = rt_mutex_delete(data->rw_mtx);
	if (ret)
		rt_kprintf("delete usb rw mutex failed");

	ret = rt_sem_delete(data->wpage_sem);
	if (ret)
		rt_kprintf("delete usb write semaphore failed");

	ret = rt_sem_delete(data->rpage_sem);
	if (ret)
		rt_kprintf("delete usb read semaphore failed");

	ret = rt_mq_delete(data->page_mq);
	if (ret)
		rt_kprintf("delete usb mq failed");

	for (i = 0; i < W_BUF_PAGE_NUM; i++) {
		page = &data->wpage[i];
		rt_free(page->buf);
	}

	for (i = 0; i < R_BUF_PAGE_NUM; i++) {
		page = &data->rpage[i];
		rt_free(page->buf);
	}
	/* self scsi start */
	if (in_buf) {
		rt_free(in_buf);
		in_buf = RT_NULL;
	}

	if (out_buf) {
		rt_free(out_buf);
		out_buf = RT_NULL;
	}
	/* self scsi end */
#endif
    if(data->ep_in->buffer != RT_NULL)
    {
        rt_free(data->ep_in->buffer);
        data->ep_in->buffer = RT_NULL;               
    }

    if(data->ep_out->buffer != RT_NULL)
    {
        rt_free(data->ep_out->buffer);
        data->ep_out->buffer = RT_NULL;
    }
#ifndef ARCH_LOMBO
    if(data->disk != RT_NULL)
    {
        rt_device_close(data->disk);
        data->disk = RT_NULL;
    }
#endif
    
    data->status = STAT_CBW;
    
    return RT_EOK;
}

static struct ufunction_ops ops =
{
    _function_enable,
    _function_disable,
    RT_NULL,
};
static rt_err_t _mstorage_descriptor_config(umass_desc_t desc, rt_uint8_t cintf_nr, rt_uint8_t device_is_hs)
{
#ifdef RT_USB_DEVICE_COMPOSITE
    desc->iad_desc.bFirstInterface = cintf_nr;
#endif
    desc->ep_out_desc.wMaxPacketSize = device_is_hs ? 512 : 64;
    desc->ep_in_desc.wMaxPacketSize = device_is_hs ? 512 : 64;
    return RT_EOK;
}
/**
 * This function will create a mass storage function instance.
 *
 * @param device the usb device object.
 *
 * @return RT_EOK on successful.
 */
ufunction_t rt_usbd_function_mstorage_create(udevice_t device)
{
    uintf_t intf;
    struct mstorage *data;
    ufunction_t func;
    ualtsetting_t setting;
    umass_desc_t mass_desc;

    /* parameter check */
    RT_ASSERT(device != RT_NULL);

    /* set usb device string description */
    rt_usbd_device_set_string(device, _ustring);

    /* create a mass storage function */
    func = rt_usbd_function_new(device, &dev_desc, &ops);
    device->dev_qualifier = &dev_qualifier;
    
    /* allocate memory for mass storage function data */
    data = (struct mstorage*)rt_malloc(sizeof(struct mstorage));
    rt_memset(data, 0, sizeof(struct mstorage));
    func->user_data = (void*)data;

    /* create an interface object */
    intf = rt_usbd_interface_new(device, _interface_handler);

    /* create an alternate setting object */
    setting = rt_usbd_altsetting_new(sizeof(struct umass_descriptor));
    
    /* config desc in alternate setting */
    rt_usbd_altsetting_config_descriptor(setting, &_mass_desc, (rt_off_t)&((umass_desc_t)0)->intf_desc);

    /* configure the msc interface descriptor */
    _mstorage_descriptor_config(setting->desc, intf->intf_num, device->dcd->device_is_hs);

    /* create a bulk out and a bulk in endpoint */
    mass_desc = (umass_desc_t)setting->desc;
    data->ep_in = rt_usbd_endpoint_new(&mass_desc->ep_in_desc, _ep_in_handler);
    data->ep_out = rt_usbd_endpoint_new(&mass_desc->ep_out_desc, _ep_out_handler);

    /* add the bulk out and bulk in endpoint to the alternate setting */
    rt_usbd_altsetting_add_endpoint(setting, data->ep_out);
    rt_usbd_altsetting_add_endpoint(setting, data->ep_in);

    /* add the alternate setting to the interface, then set default setting */
    rt_usbd_interface_add_altsetting(intf, setting);
    rt_usbd_set_altsetting(intf, 0);

    /* add the interface to the mass storage function */
    rt_usbd_function_add_interface(func, intf);

    return func;
}
struct udclass msc_class = 
{
    .rt_usbd_function_create = rt_usbd_function_mstorage_create
};

int rt_usbd_msc_class_register(void)
{
    rt_usbd_class_register(&msc_class);
    return 0;
}
INIT_PREV_EXPORT(rt_usbd_msc_class_register);

#endif
