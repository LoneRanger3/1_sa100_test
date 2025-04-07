#ifndef  _LIBBSD_H_
#define  _LIBBSD_H_
/****************************************************************************************/
                                 /*������������*/
/****************************************************************************************/
#ifndef _IMGSIZEC_
#define _IMGSIZEC_
typedef struct
{
		union {int cols;int width;};
		union {int rows;int height;};
}SIZEC;
#endif
/*****************/
#ifndef _VIEWANGC_
#define _VIEWANGC_
typedef struct
{
		int angH;//��ͷ��ˮƽ�ӽ�
		int angV;//��ͷ�Ĵ�ֱ�ӽ�
}VIEWANGC;
#endif
/*****************/
#ifndef _IPOINTC_
#define _IPOINTC_
typedef	struct
{
		union {int x;int col;};
		union {int y;int row;};
}IPOINTC;
#endif
/*****************/
#ifndef _IMGYUVC_
#define _IMGYUVC_
typedef struct
{
		SIZEC size;         //y�����ĳߴ�
		unsigned char *yP;  //y����ָ��
		unsigned char *uvP; //uv����ָ��
}IMGYUVC;
#endif
/*****************/
#ifndef _IRECTC_
#define _IRECTC_
typedef struct 
{
		union {int x;     int col;};
		union {int y;     int row;};
		union {int width; int cols;};
		union {int height;int rows;};
}IRECTC;
#endif
/***************************************/
#ifndef _UCHAR2DMEMC_
#define _UCHAR2DMEMC_
typedef struct
{
		SIZEC size;       //�������ߴ�
		unsigned char *p; //����ָ��
		int memSize;      //p��size
}UCHAR2DMEMC;
#endif
/****************************************************************************************/
                             /*bsd��ʼ���õ������ݽṹ*/
/****************************************************************************************/

#ifndef _HDMODULEENABLEBSD_
#define _HDMODULEENABLEBSD_
typedef struct
{
		int inGps;        //�Ƿ�����gps����
		int inGsensor;    //�Ƿ�����gsenser��Ϣ��stop��a��v��
		int inObd;        //�Ƿ���������obd����
		int inCamOutPara; //�Ƿ���������ͷ�ⲿ����(��װ����)
}HDModuleEnableBsd;//��ģ��ʹ��
#endif

/*****************/
#ifndef _HDCARPARABSD_
#define _HDCARPARABSD_
typedef struct  
{
		int enable;       //�ò����Ƿ���Ч
		int width;        //�����������ף�
}HDCarParaBsd;//������Ϣ
#endif
/*****************/
#ifndef _HDROIPARABSD_
#define _HDROIPARABSD_
typedef struct
{
		int enable;       //�ò����Ƿ���Ч
		int upRow;        //�ϱ�Ե(�����),laneCrossPoint.y
		int dnRow;        //�±�Ե��������ߣ�

		int ltCol;        //����˫·bsd����߳���ı߽��ߣ����ڰ����ͼ�����㵽�ĳ�������
		int rtCol;        //����˫·bsd���ұ߳���ı߽��ߣ����ڰ��ұ�ͼ�����㵽�ĳ�������


}HDRoiParaBsd;//����Ȥ����
#endif
///*****************/ 
#ifndef _HDCAMOUTPARABSD_
#define _HDCAMOUTPARABSD_
typedef struct
{
		int     enable;          //�ò����Ƿ���Ч
		int     cam2ground;      //�������߶ȣ���λ�����ף���ͬ��   		 Ĭ��ֵ��130��  ��Χ��50 - 400
		int     cam2middle;      //�������������ߣ�����������λ������      Ĭ��ֵ��0��    ��Χ��-100 - 100
		int     cam2head;        //�복ͷ���롣��λ������            		 Ĭ��ֵ��150��  ��Χ��0 - 300
		IPOINTC laneCrossPoint;  //��λ�����ء��궨������Ҫ��ĳ����ߵĽ������꣬ͨ����ͼ�����ĸ���
}HDCamOutParaBsd;//����ͷ�ⲿ����
#endif
/*****************/
#ifndef _HDCAMINPARABSD_
#define _HDCAMINPARABSD_
typedef struct
{
		int      fps;     //֡��
		SIZEC    imgSize; //ͼƬ�ߴ�
		VIEWANGC viewAng; //����ͷˮƽ�ʹ�ֱ�ӽ�
}HDCamInParaBsd;//����ͷ�ڲ�����
#endif
/*****************/
#ifndef _HDSKYLINEBSD_
#define _HDSKYLINEBSD_
typedef struct
{
		int isFullLearn;//�Ƿ���ѧϰ
		int value ;     //����ߵ�ֵ
}HDSkyLineBsd; //�����
#endif
/*****************/
#ifndef _CAMDEFINE_
#define _CAMDEFINE_
  #define MAJOR_CAM 0 //ֻ�����
  #define MINOR_CAM 1 //��ʱ����
  #define CAM_NUM   2

#endif
/*****************/
#ifndef _HDCAMERAPARABSD_
#define _HDCAMERAPARABSD_
typedef struct 
{
		HDCamOutParaBsd    camOutPara;    //����ͷ�ⲿ����(��װ����)
		HDCamInParaBsd     camInPara;     //����ͷ�ڲ�����
}HDCameraParaBsd;
#endif
/*****************/
#ifndef _HDWARNAREABSD_
#define _HDWARNAREABSD_		
#define LT_UP     0 //���Ͻ�
#define RT_UP     1 //���Ͻ�
#define RT_DN     2 //���½�
#define LT_DN     3 //���½�
#define POINT_NUM 4
typedef struct
{
		int enable;
		IPOINTC ltArea[POINT_NUM]; //  ��߱�������
		IPOINTC rtArea[POINT_NUM]; //  �ұ߱�������
}HDWarnAreaBsd;
#endif
/*****************/
#ifndef _HDINISETDATABSD_
#define _HDINISETDATABSD_
typedef struct
{
		int                 bsdSensity;    //���������� : ���跶Χ��-100 ~ 10��
		                                   //�� �󳵳��� - �������� < bsdSensity ����/Сʱ���ú󳵲����� status = -11 תΪ status = -12��status = 11 תΪ status = 12
		HDModuleEnableBsd   moduleEnable;  //��ģ��ʹ��
		HDCarParaBsd        carPara;       //��������
		HDRoiParaBsd        roiPara;       //roi����
		HDWarnAreaBsd       warnArea;      //ä����������
		HDCameraParaBsd     cameraPara[CAM_NUM];		
		int                 laneWarnDist;  //����ƫ�뱨�����루���־೵��laneWarnDist���뼴����������-30 �� +30�����ף���Ĭ��ֵΪ 0,�� ��δԽ�ߣ�����Խ��
		char *              dataFormat;    //���ݸ�ʽ
}HDIniSetBsd;//��ʼ���������ݣ����ø�bsd��
#endif
/*******************************************/
#ifndef _HDREFERLINEBSD_
#define _HDREFERLINEBSD_
typedef struct
{
		IPOINTC lt2Points[2];    //���У׼�ߵ������˵�
		IPOINTC rt2Points[2];    //�Ҳ�У׼�ߵ������˵�
}HDReferLineBsd;
#endif
/*****************/
#ifndef _HDINIGETBSD_
#define _HDINIGETBSD_
typedef struct
{
		char        *version;  //�㷨�汾��
		HDReferLineBsd referLine; //�ο��ߣ�У׼�ߣ��ǹ̶���װʹ�ã����г���¼�ǣ�
}HDIniGetBsd;//��ʼ����ȡ���ݣ���bsd������
#endif
/****************************************************************************************/
                            /*bsd�ص������õ������ݽṹ*/
/****************************************************************************************/
#ifndef _HDGPSBSD_
#define _HDGPSBSD_
typedef struct
{
		int   enable;   //�ò����Ƿ���Ч�����ź�ʱ��1�����ź�ʱ��0
		float speed;    //gps���٣�����/ʱ��
}HDGpsBsd;//inGpsΪ��ʱ����
#endif
/*****************/
#ifndef _HDGSENSORBSD_
#define _HDGSENSORBSD_
typedef struct
{
		int   acct;     //���ٶ�(m/s^2)
		float speed;    //���٣�����/ʱ��
		int   stop;     //�����Ƿ�ֹͣ
}HDGsensorBsd;//inGsensorΪ��ʱ����
#endif
/*****************/
#ifndef _HDCAROPERATEBSD_
#define _HDCAROPERATEBSD_
typedef struct
{
		struct
		{
				int enable; //�ò����Ƿ���Ч
				int isBrake;//�Ƿ���ɲ��
		}brake;
		struct
		{
				int enable; //�ò����Ƿ���Ч
				int isAccel;//�Ƿ��ڲ�����
		}accelerato;
		struct
		{
				int enable; //�ò����Ƿ���Ч
				int isWiper;//�Ƿ��ڴ���ˢ
		}wiper;
		struct
		{
				int enable; //�ò����Ƿ���Ч
				int ang;    //�����̽Ƕȣ�-540~540��
		}wheel;
}HDCarOperateBsd;//�����ٿ���Ϣ
#endif
/*****************/
#ifndef _HDCARSTATUSBSD_
#define _HDCARSTATUSBSD_
typedef struct
{
		struct
		{
				int   enable; //�ò����Ƿ���Ч
				float value;  //�������٣�����/ʱ��
		}speed;
		struct
		{
				int   enable;    //�ò����Ƿ���Ч
				int   isObstacle;//��û���ϰ���
				float dist;      //������ϰ��Ϊ���ϰ���ľ��루��λ���ף�
		}radar;
}HDCarStatusBsd;//����״̬��Ϣ
#endif
/*****************/
#ifndef _HDCARLAMEBSD_
#define _HDCARLAMEBSD_
typedef struct
{
		struct
		{
				int enable;    //�ò����Ƿ���Ч
				int value;     //-1:��ת��0����ת��1����ת
		}corner;//ת���
		struct
		{
				int enable;    //�ò����Ƿ���Ч
				int value;     //0���رգ�1������ƣ�2��Զ��ƣ�3��פ����
		}common;//���
		struct
		{
				int enable;    //�ò����Ƿ���Ч
				int value;     //0���رգ�1������
		}fog;//���
		struct
		{
				int enable;    //�ò����Ƿ���Ч
				int value;     //0���رգ�1������
		}danger;//Σ�վ�ʾ��
}HDCarLameBsd;//�ƹ���Ϣ
#endif
/*****************/
#ifndef _HDOBDBSD_
#define _HDOBDBSD_
typedef struct
{
		HDCarOperateBsd operate;//�����ٿ���Ϣ
		HDCarStatusBsd  status; //����״̬��Ϣ
		HDCarLameBsd    lame;   //�ƹ���Ϣ
}HDObdBsd;//HDObdBsd��ȡ�ĳ�����Ϣ
#endif
/*****************/
#ifndef _HDWARNSENSITYBSD_
#define _HDWARNSENSITYBSD_
typedef struct
{
		int fcwSensity;//ǰ����ײ������  ��ǰ fcwSensity �뱨�� 
		int ldwSensity;//����ƫ��������  ƫ�� ldwSensity �뱨��
		int fpwSensity;//���˷�ײ������  ��ǰ fpwSensity �뱨��
}HDWarnSensityBsd;
#endif
/*****************/
#ifndef _HDFRAMESETBSD_
#define _HDFRAMESETBSD_
typedef struct
{
		long long        timeStamp;      //ʱ���
		int              isDay;          //1�����죬-1��ҹ��  ,0 :��Ч
		int              cpuGear;        //cpu��λ����(Ĭ��ֵ��Ϊ0)  // -1:�͵���cpuռ���ʵ�,  0:�е���cpuռ������,  1:�ߵ���cpuռ���ʸ�
		IMGYUVC          imgYuv[CAM_NUM];//�����yuvͼ��
		HDGpsBsd         gps;            //gps����
		HDGsensorBsd     gsensor;        //gsensor��õĳ�����Ϣ
		HDObdBsd         obd;            //obd����
		HDWarnSensityBsd warnSensity;    //����������
		int              runPed;        //�Ƿ������˼�� 1:�ǣ�0����
}HDFrameSetBsd;
#endif
/*******************************************/
#ifndef _HDOBJECTBSD_
#define _HDOBJECTBSD_
typedef struct
{
		int    status;//Ŀ��״̬ //-2: ���ڶ���Ŀ�� 
		                         //-1: ����һ��Ŀ�꣨����뱨���������� -11 �����뱨�����򣬺󳵳��� - �������� < bsdSensity ����/Сʱ ���� -12��
		                         // 0������Ŀ��   
		                         // 1���ҵ���һ��Ŀ�꣨����뱨���������� 11 �� ���뱨�����򣬺󳵳��� - �������� < bsdSensity ����/Сʱ ����  12��
		                         // 2���ҵ��ڶ���Ŀ��

		IRECTC loc;   //Ŀ����ͼ���е�λ��
		float  dist;  //Ŀ�����
		float  time;  //Ԥ����ײʱ�䣬��Ϊ�������汾����Ϊ�ǹ�������Υ�泵����ͣ��ʱ��
}HDObjectBsd;
#endif
/*****************/
#ifndef _HDWARMOBJECTBSD_
#define _HDWARMOBJECTBSD_
typedef struct
{
		int warnIdx;    //��ҪԤ����Ŀ����±�
		int warnStatus; //������״̬������ʲô���͵ı�����0��û����ҪԤ����Ŀ�꣬���������Ӧ�� ����Ŀ�� HDObjectBsd �� status
}HDWarmObjectBsd;
#endif
/*****************/
#ifndef _HDPLATEBSD_
#define _HDPLATEBSD_
typedef struct
{
		int          isExist;
		IRECTC       loc;      //Ŀ����ͼ���е�λ��
}HDPlateBsd;
#endif
/*****************/
#ifndef _HDCARBSD_
#define _HDCARBSD_
typedef struct
{
		HDObjectBsd object;
		HDPlateBsd  plate;//����
}HDCarBsd;//ÿ�����������Ϣ
#endif
/*****************/
#ifndef _HDCARSBSD_
#define _HDCARSBSD_
typedef struct
{
		HDWarmObjectBsd  warmCar;//��ҪԤ���ĳ�����Ϣ
		int              num;    //��������
		HDCarBsd         *p;     //������г�����Ϣ��ָ��
		int              memSize;//���ٵ�pָ���size
}HDCarsBsd;
#endif
/*****************/
#ifndef _HDLANEBSD_
#define _HDLANEBSD_

#define LANE_POINT_NUM  6
typedef struct
{
		IPOINTC pOfLane[LANE_POINT_NUM];//�������ϵ�����
		int     color;     //��������ɫ��0������ȷ��1�����ߣ�2������
		int     shape;     //��������״��0������ȷ�� 1��ֱ��ʵ��,  2��ֱ���ߣ� 3��ֱ˫ʵ��,  4��ֱ������ʵ�� 5: ֱ��ʵ����
		                   //                       -1������ʵ��, -2�������ߣ�-3����˫ʵ��, -4����������ʵ��-5: ����ʵ����
}HDLaneBsd;

#endif
/*****************/
#ifndef _HDLANESBSD_
#define _HDLANESBSD_
typedef struct
{
		int isDisp;//�Ƿ���ʾ������
		int status;
		//-2:��ʾ���ƫ�룬�����������ƫ��
		//-1:��ʾ���ƫ�룬�����������ƫ��
		// 1:��ʾ�Ҳ�ƫ�룬���������Ҳ�ƫ��
		// 2:��ʾ�Ҳ�ƫ�룬���������Ҳ�ƫ��
		//���߲��������ң�-2��-1��1��2 ͳһ��������ƫ��
		//4������ռ���˹�������
		HDLaneBsd lt;   //��೵����
		HDLaneBsd rt;   //�Ҳ೵����
}HDLanesBsd;
#endif
/*****************/
#ifndef _HDFIATIGUEBSD_
#define _HDFIATIGUEBSD_
typedef struct
{
		int   isWarn;//�Ƿ񱨾�
		float time;//������ʻʱ��(��λ��Сʱ)
}HDFiatigueBsd;//ƣ�ͼ�ʻ
#endif
/*****************/
#ifndef _HDLAMEON_
#define _HDLAMEON_
typedef struct
{
		int value;//0������ʾ��1��������ƣ�2����Զ���
}HDLameOn;//�ƹ⿪����ʾ
#endif
/*****************/
#ifndef _HDBLINDDETECTBSD_
#define _HDBLINDDETECTBSD_
typedef struct
{
		int ltWarn;//-1:���Σ�գ�0����ȫ
		int rtWarn;//-1:�Ҳ�Σ�գ�0����ȫ
}HDBlindDetectBsd;//ä�����
#endif
/*****************/
#ifndef _HDPEDBSD_
#define _HDPEDBSD_
typedef struct
{
		HDObjectBsd object;//���е�status ΪĿ��״̬  -1:���Ŀ��  0������Ŀ��   1���ҵ�Ŀ��  2������������ˣ����豨���л���3   3��С������
}HDPedBsd;//����Ŀ��
#endif
/*****************/
#ifndef _HDPEDSBSD_
#define _HDPEDSBSD_
typedef struct
{
		HDWarmObjectBsd  warmPed;  //��ҪԤ����������Ϣ
		int              num;      //���˸���
		HDPedBsd         *p;       //���������Ϣ��Ϣ��ָ��
		int              memSize;  //���ٵ�pedestrianPָ���size
}HDPedsBsd;
#endif
/*****************/
#ifndef _HDFRAMEGETBSD_
#define _HDFRAMEGETBSD_
typedef struct
{
		int              reCalibrate;   //����У׼��־
		int              isDay;         //1�����죬-1��ҹ��
		long long        timeStamp;     //ʱ���

		HDLanesBsd       lanes;         //LDWS
		HDCarsBsd        cars;          //FCWS
		HDPedsBsd        peds;          //����Ԥ��

		HDBlindDetectBsd bsd;           //ä�������

		HDWarnAreaBsd    warnArea;      //�㷨������ä����������		
		HDFiatigueBsd    fiatigue;      //ƣ�ͼ�ʻ
		HDLameOn         lameOn;        //���ƿ�����ʾ
		HDSkyLineBsd     skyLine;       //�����
		void             *testP;        //�����õ�ָ�룬�ⲿ��Ч
}HDFrameGetBsd;
#endif

/****************************************************************************************/
                                 /*bsd�ӿں���*/
/****************************************************************************************/
void CreatBsd(HDIniSetBsd *iniSetBsd,HDIniGetBsd *iniGetBsd,void **dev);//��ʼ��bsd
void DeleteBsd(void);                                                       //�ͷ�bsd
/*******************************************/
typedef void (*SETBSDDATA)(HDFrameSetBsd *frameSetBsd,void *dv);          //��bsd�������(�ص�����)
typedef void (*GETBSDDATA)(HDFrameGetBsd *frameGetBsd,void *dv);          //��ȡbsd�ļ�����(�ص�����)

extern  SETBSDDATA SetBsdData;
extern  GETBSDDATA GetBsdData;






#endif


