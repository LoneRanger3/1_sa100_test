#ifndef  _LIBADAS_H_
#define  _LIBADAS_H_

/****************************************************************************************/
/*������������*/
/****************************************************************************************/
#ifndef _IMGSIZEC_
#define _IMGSIZEC_
typedef struct {
	union {
		int cols;
		int width;
	};
	union {
		int rows;
		int height;
	};
} SIZEC;
#endif
/*****************/
#ifndef _VIEWANGC_
#define _VIEWANGC_
typedef struct {
	int angH;//��ͷ��ˮƽ�ӽ�
	int angV;//��ͷ�Ĵ�ֱ�ӽ�
} VIEWANGC;
#endif
/*****************/
#ifndef _IPOINTC_
#define _IPOINTC_
typedef	struct {
	union {
		int x;
		int col;
	};
	union {
		int y;
		int row;
	};
} IPOINTC;
#endif
/*****************/
#ifndef _IMGYUVC_
#define _IMGYUVC_
typedef struct {
	SIZEC size;         //y�����ĳߴ�
	unsigned char *yP;  //y����ָ��
	unsigned char *uvP; //uv����ָ��
} IMGYUVC;
#endif
/*****************/
#ifndef _IRECTC_
#define _IRECTC_
typedef struct {
	union {
		int x;
		int col;
	};
	union {
		int y;
		int row;
	};
	union {
		int width;
		int cols;
	};
	union {
		int height;
		int rows;
	};
} IRECTC;
#endif
/***************************************/
#ifndef _UCHAR2DMEMC_
#define _UCHAR2DMEMC_
typedef struct {
	SIZEC size;       //�������ߴ�
	unsigned char *p; //����ָ��
	int memSize;      //p��size
} UCHAR2DMEMC;
#endif
/****************************************************************************************/
/*adas��ʼ���õ������ݽṹ*/
/****************************************************************************************/

#ifndef _HDMODULEENABLE_
#define _HDMODULEENABLE_
typedef struct {
	int inGps;        //�Ƿ�����gps����
	int inGsensor;    //�Ƿ�����gsenser��Ϣ��stop��a��v��
	int inObd;        //�Ƿ���������obd����
	int inCamOutPara; //�Ƿ���������ͷ�ⲿ����(��װ����)
} HDModuleEnable; //��ģ��ʹ��
#endif

/*****************/
#ifndef _HDCARPARA_
#define _HDCARPARA_
typedef struct {
	int enable;       //�ò����Ƿ���Ч
	int width;        //�����������ף�
} HDCarPara; //������Ϣ
#endif
/*****************/
#ifndef _HDROIPARA_
#define _HDROIPARA_
typedef struct {
	int enable;       //�ò����Ƿ���Ч
	int upRow;        //�ϱ�Ե(�����),laneCrossPoint.y
	int dnRow;        //�±�Ե��������ߣ�
} HDRoiPara; //����Ȥ����
#endif
///*****************/
#ifndef _HDCAMOUTPARA_
#define _HDCAMOUTPARA_
typedef struct {
	int     enable;          //�ò����Ƿ���Ч
	int     cam2ground;      //�������߶ȣ���λ�����ף���ͬ��   		 Ĭ��ֵ��130��  ��Χ��50 - 400
	int     cam2middle;      //�������������ߣ�����������λ������      Ĭ��ֵ��0��    ��Χ��-100 - 100
	int     cam2head;        //�복ͷ���롣��λ������            		 Ĭ��ֵ��150��  ��Χ��0 - 300
	IPOINTC laneCrossPoint;  //��λ�����ء��궨������Ҫ��ĳ����ߵĽ������꣬ͨ����ͼ�����ĸ���
} HDCamOutPara; //����ͷ�ⲿ����
#endif
/*****************/
#ifndef _HDCAMINPARA_
#define _HDCAMINPARA_
typedef struct {
	int      fps;     //֡��
	SIZEC    imgSize; //ͼƬ�ߴ�
	VIEWANGC viewAng; //����ͷˮƽ�ʹ�ֱ�ӽ�
} HDCamInPara; //����ͷ�ڲ�����
#endif
/*****************/
#ifndef _HDSKYLINE_
#define _HDSKYLINE_
typedef struct {
	int isFullLearn;//�Ƿ���ѧϰ
	int value ;     //����ߵ�ֵ
} HDSkyLine; //�����
#endif
/*****************/
#ifndef _CAMDEFINE_
#define _CAMDEFINE_
#define MAJOR_CAM 0 //ֻ�����
#define MINOR_CAM 1 //��ʱ����
#define CAM_NUM   2

#endif
/*****************/
#ifndef _HDCAMERAPARA_
#define _HDCAMERAPARA_
typedef struct {
	HDCamOutPara    camOutPara;    //����ͷ�ⲿ����(��װ����)
	HDCamInPara     camInPara;     //����ͷ�ڲ�����
} HDCameraPara;
#endif
/*****************/
#ifndef _HDINISETDATA_
#define _HDINISETDATA_
typedef struct {
	HDModuleEnable   moduleEnable;  //��ģ��ʹ��
	HDCarPara        carPara;       //��������
	HDRoiPara        roiPara;       //roi����
	HDCameraPara     cameraPara[CAM_NUM];
	int
	laneWarnDist;  //����ƫ�뱨�����루���־೵��laneWarnDist���뼴����������-30 �� +30�����ף���Ĭ��ֵΪ 0,�� ��δԽ�ߣ�����Խ��
	char            *dataFormat;    //���ݸ�ʽ
} HDIniSetData; //��ʼ���������ݣ����ø�adas��
#endif
/*******************************************/
#ifndef _HDREFERLINE_
#define _HDREFERLINE_
typedef struct {
	IPOINTC lt2Points[2];    //���У׼�ߵ������˵�
	IPOINTC rt2Points[2];    //�Ҳ�У׼�ߵ������˵�
} HDReferLine;
#endif
/*****************/
#ifndef _HDINIGETDATA_
#define _HDINIGETDATA_
typedef struct {
	char        *version;  //�㷨�汾��
	HDReferLine referLine; //�ο��ߣ�У׼�ߣ��ǹ̶���װʹ�ã����г���¼�ǣ�
} HDIniGetData; //��ʼ����ȡ���ݣ���adas������
#endif
/****************************************************************************************/
/*adas�ص������õ������ݽṹ*/
/****************************************************************************************/
#ifndef _HDGPS_
#define _HDGPS_
typedef struct {
	int   enable;   //�ò����Ƿ���Ч�����ź�ʱ��1�����ź�ʱ��0
	float speed;    //gps���٣�����/ʱ��
} HDGps; //inGpsΪ��ʱ����
#endif
/*****************/
#ifndef _HDGSENSOR_
#define _HDGSENSOR_
typedef struct {
	int   acct;     //���ٶ�(m/s^2)
	float speed;    //���٣�����/ʱ��
	int   stop;     //�����Ƿ�ֹͣ
} HDGsensor; //inGsensorΪ��ʱ����
#endif
/*****************/
#ifndef _HDCAROPERATE_
#define _HDCAROPERATE_
typedef struct {
	struct {
		int enable; //�ò����Ƿ���Ч
		int isBrake;//�Ƿ���ɲ��
	} brake;
	struct {
		int enable; //�ò����Ƿ���Ч
		int isAccel;//�Ƿ��ڲ�����
	} accelerato;
	struct {
		int enable; //�ò����Ƿ���Ч
		int isWiper;//�Ƿ��ڴ���ˢ
	} wiper;
	struct {
		int enable; //�ò����Ƿ���Ч
		int ang;    //�����̽Ƕȣ�-540~540��
	} wheel;
} HDCarOperate; //�����ٿ���Ϣ
#endif
/*****************/
#ifndef _HDCARSTATUS_
#define _HDCARSTATUS_
typedef struct {
	struct {
		int   enable; //�ò����Ƿ���Ч
		float value;  //�������٣�����/ʱ��
	} speed;
	struct {
		int   enable;    //�ò����Ƿ���Ч
		int   isObstacle;//��û���ϰ���
		float dist;      //������ϰ��Ϊ���ϰ���ľ��루��λ���ף�
	} radar;
} HDCarStatus; //����״̬��Ϣ
#endif
/*****************/
#ifndef _HDCARLAME_
#define _HDCARLAME_
typedef struct {
	struct {
		int enable;    //�ò����Ƿ���Ч
		int value;     //-1:��ת��0����ת��1����ת
	} corner; //ת���
	struct {
		int enable;    //�ò����Ƿ���Ч
		int value;     //0���رգ�1������ƣ�2��Զ��ƣ�3��פ����
	} common; //���
	struct {
		int enable;    //�ò����Ƿ���Ч
		int value;     //0���رգ�1������
	} fog; //���
	struct {
		int enable;    //�ò����Ƿ���Ч
		int value;     //0���رգ�1������
	} danger; //Σ�վ�ʾ��
} HDCarLame; //�ƹ���Ϣ
#endif
/*****************/
#ifndef _HDOBD_
#define _HDOBD_
typedef struct {
	HDCarOperate operate;//�����ٿ���Ϣ
	HDCarStatus  status; //����״̬��Ϣ
	HDCarLame    lame;   //�ƹ���Ϣ
} HDObd; //HDObd��ȡ�ĳ�����Ϣ
#endif
/*****************/
#ifndef _HDWARNSENSITY_
#define _HDWARNSENSITY_
typedef struct
{
		int fcwSensity;//ǰ����ײ������  ��ǰ fcwSensity �뱨�� 
		int ldwSensity;//����ƫ��������  ƫ�� ldwSensity �뱨��
		int fpwSensity;//���˷�ײ������  ��ǰ fpwSensity �뱨��
}HDWarnSensity;
#endif
/*****************/
#ifndef _HDFRAMESETDATA_
#define _HDFRAMESETDATA_
typedef struct {
	long long     timeStamp;      //ʱ���
	int           isDay;          //1�����죬-1��ҹ��
	int
	cpuGear;        //cpu��λ����(Ĭ��ֵ��Ϊ0)  // -1:�͵���cpuռ���ʵ�,  0:�е���cpuռ������,  1:�ߵ���cpuռ���ʸ�
	IMGYUVC       imgYuv[CAM_NUM];//�����yuvͼ��
	HDGps         gps;            //gps����
	HDGsensor     gsensor;        //gsensor��õĳ�����Ϣ
	HDObd         obd;            //obd����
	HDWarnSensity warnSensity;    //����������
	int           runPed;        //�Ƿ������˼�� 1:�ǣ�0����
} HDFrameSetData;
#endif
/*******************************************/
#ifndef _HDOBJECT_
#define _HDOBJECT_
typedef struct {
	int    status;          //Ŀ��״̬  -1:���Ŀ��  0������Ŀ��   1���ҵ�Ŀ��  2������������������豨���л���3   3��ע����ײ  4: ǰ�����뿪  5��С��ǰ��  6�����ֳ��� 7: С����ײ
	IRECTC loc;             //Ŀ����ͼ���е�λ��
	float  dist;            //Ŀ�����
	float  time;            //Ԥ����ײʱ�䣬��Ϊ�������汾����Ϊ�ǹ�������Υ�泵����ͣ��ʱ��
} HDObject;
#endif
/*****************/
#ifndef _HDWARMOBJECT_
#define _HDWARMOBJECT_
typedef struct {
	int warnIdx;    //��ҪԤ����Ŀ����±�
	int warnStatus; //������״̬������ʲô���͵ı�����0��û����ҪԤ����Ŀ�꣬���������Ӧ�� ����Ŀ�� HDObject �� status
} HDWarmObject;
#endif
/*****************/
#ifndef _HDPLATE_
#define _HDPLATE_
typedef struct {
	int          isExist;
	IRECTC       loc;      //Ŀ����ͼ���е�λ��
} HDPlate;
#endif
/*****************/
#ifndef _HDCAR_
#define _HDCAR_
typedef struct {
	HDObject object;//���е�status ΪĿ��״̬  -1:���Ŀ��  0������Ŀ��   1���ҵ�Ŀ��  2������������������豨���л���3   3��ע����ײ  4: ǰ�����뿪  5��С��ǰ��  6�����ֳ��� 7: С����ײ
	HDPlate  plate;//����
} HDCar; //ÿ�����������Ϣ
#endif
/*****************/
typedef struct {
	HDWarmObject  warmCar;//��ҪԤ���ĳ�����Ϣ
	int           num;    //��������
	HDCar         *p;     //������г�����Ϣ��ָ��
	int           memSize;//���ٵ�pָ���size
} HDCars;
/*****************/
#ifndef _HDLANE_
#define _HDLANE_

#define LANE_POINT_NUM  6
typedef struct {
	IPOINTC pOfLane[LANE_POINT_NUM];//�������ϵ�����
	int     color;     //��������ɫ��0������ȷ��1�����ߣ�2������
	int     shape;     //��������״��0������ȷ�� 1��ֱ��ʵ��,  2��ֱ���ߣ� 3��ֱ˫ʵ��,  4��ֱ������ʵ�� 5: ֱ��ʵ����
	//                       -1������ʵ��, -2�������ߣ�-3����˫ʵ��, -4����������ʵ��-5: ����ʵ����
} HDLane;

#endif
/*****************/
#ifndef _HDLANES_
#define _HDLANES_
typedef struct {
	int isDisp;//�Ƿ���ʾ������
	int status;
	//-2:��ʾ���ƫ�룬�����������ƫ��
	//-1:��ʾ���ƫ�룬�����������ƫ��
	// 1:��ʾ�Ҳ�ƫ�룬���������Ҳ�ƫ��
	// 2:��ʾ�Ҳ�ƫ�룬���������Ҳ�ƫ��
	//���߲��������ң�-2��-1��1��2 ͳһ��������ƫ��
	//4������ռ���˹�������
	HDLane lt;   //��೵����
	HDLane rt;   //�Ҳ೵����
} HDLanes;
#endif
/*****************/
#ifndef _HDFIATIGUE_
#define _HDFIATIGUE_
typedef struct {
	int   isWarn;//�Ƿ񱨾�
	float time;//������ʻʱ��(��λ��Сʱ)
} HDFiatigue; //ƣ�ͼ�ʻ
#endif
/*****************/
#ifndef _HDLAMEON_
#define _HDLAMEON_
typedef struct {
	int value;//0������ʾ��1��������ƣ�2����Զ���
} HDLameOn; //�ƹ⿪����ʾ
#endif
/*****************/
#ifndef _HDBLINDDETECT_
#define _HDBLINDDETECT_
typedef struct {
	int value;//-1:���Σ�գ�0����ȫ��1���Ҳ�Σ��
} HDBlindDetect; //ä�����
#endif
/*****************/
#ifndef _HDPED_
#define _HDPED_
typedef struct {
	HDObject object;//���е�status ΪĿ��״̬  -1:���Ŀ��  0������Ŀ��   1���ҵ�Ŀ��  2������������ˣ����豨���л���3   3��С������
} HDPed; //����Ŀ��
#endif
/*****************/
#ifndef _HDPEDS_
#define _HDPEDS_
typedef struct {
	HDWarmObject  warmPed;  //��ҪԤ����������Ϣ
	int           num;      //���˸���
	HDPed         *p;       //���������Ϣ��Ϣ��ָ��
	int           memSize;  //���ٵ�pedestrianPָ���size
} HDPeds;
#endif
/*****************/
#ifndef _HDFRAMEGETDATA_
#define _HDFRAMEGETDATA_
typedef struct {
	int            reCalibrate;   //����У׼��־
	int            isDay;         //1�����죬-1��ҹ��
	long long      timeStamp;     //ʱ���
	HDLanes        lanes;         //LDWS
	HDCars         cars;          //FCWS
	HDPeds         peds;          //����Ԥ��
	HDFiatigue     fiatigue;      //ƣ�ͼ�ʻ
	HDLameOn       lameOn;        //���ƿ�����ʾ
	HDSkyLine      skyLine;       //�����
	void           *testP;        //�����õ�ָ�룬�ⲿ��Ч
} HDFrameGetData;
#endif

/****************************************************************************************/
/*adas�ӿں���*/
/****************************************************************************************/
void CreatAdas(HDIniSetData *iniSetData, HDIniGetData *iniGetData,
	void **dev); //��ʼ��adas
void DeleteAdas(void);                                                       //�ͷ�adas
/*******************************************/
typedef void (*SETADASDATA)(HDFrameSetData *frameSetData,
	void *dv);         //��adas�������(�ص�����)
typedef void (*GETADASDATA)(HDFrameGetData *frameGetData,
	void *dv);         //��ȡadas�ļ�����(�ص�����)

extern  SETADASDATA SetAdasData;
extern  GETADASDATA GetAdasData;

#endif
