#ifndef  _LIBBSD_H_
#define  _LIBBSD_H_
/****************************************************************************************/
                                 /*基本数据类型*/
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
		int angH;//镜头的水平视角
		int angV;//镜头的垂直视角
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
		SIZEC size;         //y分量的尺寸
		unsigned char *yP;  //y分量指针
		unsigned char *uvP; //uv分量指针
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
		SIZEC size;       //给分量尺寸
		unsigned char *p; //数据指针
		int memSize;      //p的size
}UCHAR2DMEMC;
#endif
/****************************************************************************************/
                             /*bsd初始化用到的数据结构*/
/****************************************************************************************/

#ifndef _HDMODULEENABLEBSD_
#define _HDMODULEENABLEBSD_
typedef struct
{
		int inGps;        //是否输入gps车速
		int inGsensor;    //是否输入gsenser信息（stop、a、v）
		int inObd;        //是否输入输入obd参数
		int inCamOutPara; //是否输入摄像头外部参数(安装参数)
}HDModuleEnableBsd;//各模块使能
#endif

/*****************/
#ifndef _HDCARPARABSD_
#define _HDCARPARABSD_
typedef struct  
{
		int enable;       //该参数是否有效
		int width;        //本车车宽（厘米）
}HDCarParaBsd;//本车信息
#endif
/*****************/
#ifndef _HDROIPARABSD_
#define _HDROIPARABSD_
typedef struct
{
		int enable;       //该参数是否有效
		int upRow;        //上边缘(天际线),laneCrossPoint.y
		int dnRow;        //下边缘（引擎盖线）

		int ltCol;        //用于双路bsd，左边车身的边界线（用于把左边图像拍摄到的车身卡掉）
		int rtCol;        //用于双路bsd，右边车身的边界线（用于把右边图像拍摄到的车身卡掉）


}HDRoiParaBsd;//感兴趣区域
#endif
///*****************/ 
#ifndef _HDCAMOUTPARABSD_
#define _HDCAMOUTPARABSD_
typedef struct
{
		int     enable;          //该参数是否有效
		int     cam2ground;      //距离地面高度，单位：厘米，下同。   		 默认值：130，  范围：50 - 400
		int     cam2middle;      //距离汽车中轴线，左负右正。单位：厘米      默认值：0，    范围：-100 - 100
		int     cam2head;        //与车头距离。单位：厘米            		 默认值：150，  范围：0 - 300
		IPOINTC laneCrossPoint;  //单位：像素。标定界面中要求的车道线的交点坐标，通常在图像中心附近
}HDCamOutParaBsd;//摄像头外部参数
#endif
/*****************/
#ifndef _HDCAMINPARABSD_
#define _HDCAMINPARABSD_
typedef struct
{
		int      fps;     //帧率
		SIZEC    imgSize; //图片尺寸
		VIEWANGC viewAng; //摄像头水平和垂直视角
}HDCamInParaBsd;//摄像头内部参数
#endif
/*****************/
#ifndef _HDSKYLINEBSD_
#define _HDSKYLINEBSD_
typedef struct
{
		int isFullLearn;//是否充分学习
		int value ;     //天际线的值
}HDSkyLineBsd; //天际线
#endif
/*****************/
#ifndef _CAMDEFINE_
#define _CAMDEFINE_
  #define MAJOR_CAM 0 //只用这个
  #define MINOR_CAM 1 //暂时不用
  #define CAM_NUM   2

#endif
/*****************/
#ifndef _HDCAMERAPARABSD_
#define _HDCAMERAPARABSD_
typedef struct 
{
		HDCamOutParaBsd    camOutPara;    //摄像头外部参数(安装参数)
		HDCamInParaBsd     camInPara;     //摄像头内部参数
}HDCameraParaBsd;
#endif
/*****************/
#ifndef _HDWARNAREABSD_
#define _HDWARNAREABSD_		
#define LT_UP     0 //左上角
#define RT_UP     1 //右上角
#define RT_DN     2 //右下角
#define LT_DN     3 //左下角
#define POINT_NUM 4
typedef struct
{
		int enable;
		IPOINTC ltArea[POINT_NUM]; //  左边报警区域
		IPOINTC rtArea[POINT_NUM]; //  右边报警区域
}HDWarnAreaBsd;
#endif
/*****************/
#ifndef _HDINISETDATABSD_
#define _HDINISETDATABSD_
typedef struct
{
		int                 bsdSensity;    //报警灵敏度 : 可设范围：-100 ~ 10。
		                                   //当 后车车速 - 本车车速 < bsdSensity 公里/小时，该后车不报警 status = -11 转为 status = -12，status = 11 转为 status = 12
		HDModuleEnableBsd   moduleEnable;  //各模块使能
		HDCarParaBsd        carPara;       //本车参数
		HDRoiParaBsd        roiPara;       //roi参数
		HDWarnAreaBsd       warnArea;      //盲区报警区域
		HDCameraParaBsd     cameraPara[CAM_NUM];		
		int                 laneWarnDist;  //车道偏离报警距离（车轮距车道laneWarnDist距离即报警）设置-30 到 +30（厘米），默认值为 0,负 ：未越线，正：越线
		char *              dataFormat;    //数据格式
}HDIniSetBsd;//初始化设置数据（设置给bsd）
#endif
/*******************************************/
#ifndef _HDREFERLINEBSD_
#define _HDREFERLINEBSD_
typedef struct
{
		IPOINTC lt2Points[2];    //左侧校准线的两个端点
		IPOINTC rt2Points[2];    //右侧校准线的两个端点
}HDReferLineBsd;
#endif
/*****************/
#ifndef _HDINIGETBSD_
#define _HDINIGETBSD_
typedef struct
{
		char        *version;  //算法版本号
		HDReferLineBsd referLine; //参考线（校准线，非固定安装使用，如行车记录仪）
}HDIniGetBsd;//初始化读取数据（从bsd读出）
#endif
/****************************************************************************************/
                            /*bsd回调函数用到的数据结构*/
/****************************************************************************************/
#ifndef _HDGPSBSD_
#define _HDGPSBSD_
typedef struct
{
		int   enable;   //该参数是否有效，有信号时置1，无信号时置0
		float speed;    //gps车速（公里/时）
}HDGpsBsd;//inGps为真时输入
#endif
/*****************/
#ifndef _HDGSENSORBSD_
#define _HDGSENSORBSD_
typedef struct
{
		int   acct;     //加速度(m/s^2)
		float speed;    //车速（公里/时）
		int   stop;     //车辆是否停止
}HDGsensorBsd;//inGsensor为真时输入
#endif
/*****************/
#ifndef _HDCAROPERATEBSD_
#define _HDCAROPERATEBSD_
typedef struct
{
		struct
		{
				int enable; //该参数是否有效
				int isBrake;//是否在刹车
		}brake;
		struct
		{
				int enable; //该参数是否有效
				int isAccel;//是否在踩油门
		}accelerato;
		struct
		{
				int enable; //该参数是否有效
				int isWiper;//是否在打雨刷
		}wiper;
		struct
		{
				int enable; //该参数是否有效
				int ang;    //方向盘角度（-540~540）
		}wheel;
}HDCarOperateBsd;//车辆操控信息
#endif
/*****************/
#ifndef _HDCARSTATUSBSD_
#define _HDCARSTATUSBSD_
typedef struct
{
		struct
		{
				int   enable; //该参数是否有效
				float value;  //本车车速（公里/时）
		}speed;
		struct
		{
				int   enable;    //该参数是否有效
				int   isObstacle;//有没有障碍物
				float dist;      //如果有障碍物，为离障碍物的距离（单位：米）
		}radar;
}HDCarStatusBsd;//车辆状态信息
#endif
/*****************/
#ifndef _HDCARLAMEBSD_
#define _HDCARLAMEBSD_
typedef struct
{
		struct
		{
				int enable;    //该参数是否有效
				int value;     //-1:左转，0：不转，1：右转
		}corner;//转向灯
		struct
		{
				int enable;    //该参数是否有效
				int value;     //0：关闭，1：近光灯，2：远光灯，3：驻车灯
		}common;//大灯
		struct
		{
				int enable;    //该参数是否有效
				int value;     //0：关闭，1：开启
		}fog;//雾灯
		struct
		{
				int enable;    //该参数是否有效
				int value;     //0：关闭，1：开启
		}danger;//危险警示灯
}HDCarLameBsd;//灯光信息
#endif
/*****************/
#ifndef _HDOBDBSD_
#define _HDOBDBSD_
typedef struct
{
		HDCarOperateBsd operate;//车辆操控信息
		HDCarStatusBsd  status; //车辆状态信息
		HDCarLameBsd    lame;   //灯光信息
}HDObdBsd;//HDObdBsd读取的车辆信息
#endif
/*****************/
#ifndef _HDWARNSENSITYBSD_
#define _HDWARNSENSITYBSD_
typedef struct
{
		int fcwSensity;//前车防撞灵敏度  提前 fcwSensity 秒报警 
		int ldwSensity;//车道偏离灵敏度  偏离 ldwSensity 秒报警
		int fpwSensity;//行人防撞灵敏度  提前 fpwSensity 秒报警
}HDWarnSensityBsd;
#endif
/*****************/
#ifndef _HDFRAMESETBSD_
#define _HDFRAMESETBSD_
typedef struct
{
		long long        timeStamp;      //时间戳
		int              isDay;          //1：白天，-1：夜晚  ,0 :无效
		int              cpuGear;        //cpu档位设置(默认值设为0)  // -1:低档：cpu占用率低,  0:中档：cpu占用率中,  1:高档：cpu占用率高
		IMGYUVC          imgYuv[CAM_NUM];//输入的yuv图像
		HDGpsBsd         gps;            //gps车速
		HDGsensorBsd     gsensor;        //gsensor获得的车辆信息
		HDObdBsd         obd;            //obd参数
		HDWarnSensityBsd warnSensity;    //报警灵敏度
		int              runPed;        //是否开启行人检测 1:是，0：否
}HDFrameSetBsd;
#endif
/*******************************************/
#ifndef _HDOBJECTBSD_
#define _HDOBJECTBSD_
typedef struct
{
		int    status;//目标状态 //-2: 左侧第二道目标 
		                         //-1: 左侧第一道目标（如进入报警区域则变成 -11 ，进入报警区域，后车车速 - 本车车速 < bsdSensity 公里/小时 则变成 -12）
		                         // 0：本道目标   
		                         // 1：右道第一道目标（如进入报警区域则变成 11 ， 进入报警区域，后车车速 - 本车车速 < bsdSensity 公里/小时 则变成  12）
		                         // 2：右道第二道目标

		IRECTC loc;   //目标在图像中的位置
		float  dist;  //目标距离
		float  time;  //预判碰撞时间，若为公交车版本，则为非公交车在违规车道的停留时间
}HDObjectBsd;
#endif
/*****************/
#ifndef _HDWARMOBJECTBSD_
#define _HDWARMOBJECTBSD_
typedef struct
{
		int warnIdx;    //需要预警的目标的下标
		int warnStatus; //报警的状态，即：什么类型的报警，0：没有需要预警的目标，大于零则对应到 报警目标 HDObjectBsd 的 status
}HDWarmObjectBsd;
#endif
/*****************/
#ifndef _HDPLATEBSD_
#define _HDPLATEBSD_
typedef struct
{
		int          isExist;
		IRECTC       loc;      //目标在图像中的位置
}HDPlateBsd;
#endif
/*****************/
#ifndef _HDCARBSD_
#define _HDCARBSD_
typedef struct
{
		HDObjectBsd object;
		HDPlateBsd  plate;//车牌
}HDCarBsd;//每辆车的输出信息
#endif
/*****************/
#ifndef _HDCARSBSD_
#define _HDCARSBSD_
typedef struct
{
		HDWarmObjectBsd  warmCar;//需要预警的车辆信息
		int              num;    //车辆个数
		HDCarBsd         *p;     //存放所有车辆信息的指针
		int              memSize;//开辟的p指针的size
}HDCarsBsd;
#endif
/*****************/
#ifndef _HDLANEBSD_
#define _HDLANEBSD_

#define LANE_POINT_NUM  6
typedef struct
{
		IPOINTC pOfLane[LANE_POINT_NUM];//车道线上的两点
		int     color;     //车道线颜色。0：不明确，1：白线，2：黄线
		int     shape;     //车道线形状。0：不明确， 1：直单实线,  2：直虚线， 3：直双实线,  4：直内虚外实， 5: 直内实外虚
		                   //                       -1：曲单实线, -2：曲虚线，-3：曲双实线, -4：曲内虚外实，-5: 曲内实外虚
}HDLaneBsd;

#endif
/*****************/
#ifndef _HDLANESBSD_
#define _HDLANESBSD_
typedef struct
{
		int isDisp;//是否显示车道线
		int status;
		//-2:显示左侧偏离，语音提醒左侧偏离
		//-1:显示左侧偏离，铃声提醒左侧偏离
		// 1:显示右侧偏离，铃声提醒右侧偏离
		// 2:显示右侧偏离，语音提醒右侧偏离
		//或者不区分左右：-2、-1、1、2 统一播报车道偏离
		//4：本车占用了公交车道
		HDLaneBsd lt;   //左侧车道线
		HDLaneBsd rt;   //右侧车道线
}HDLanesBsd;
#endif
/*****************/
#ifndef _HDFIATIGUEBSD_
#define _HDFIATIGUEBSD_
typedef struct
{
		int   isWarn;//是否报警
		float time;//连续驾驶时长(单位：小时)
}HDFiatigueBsd;//疲劳驾驶
#endif
/*****************/
#ifndef _HDLAMEON_
#define _HDLAMEON_
typedef struct
{
		int value;//0：不提示，1：开近光灯，2：开远光灯
}HDLameOn;//灯光开启提示
#endif
/*****************/
#ifndef _HDBLINDDETECTBSD_
#define _HDBLINDDETECTBSD_
typedef struct
{
		int ltWarn;//-1:左侧危险，0：安全
		int rtWarn;//-1:右侧危险，0：安全
}HDBlindDetectBsd;//盲区检测
#endif
/*****************/
#ifndef _HDPEDBSD_
#define _HDPEDBSD_
typedef struct
{
		HDObjectBsd object;//其中的status 为目标状态  -1:左道目标  0：本道目标   1：右道目标  2：本道最近行人，如需报警切换成3   3：小心行人
}HDPedBsd;//行人目标
#endif
/*****************/
#ifndef _HDPEDSBSD_
#define _HDPEDSBSD_
typedef struct
{
		HDWarmObjectBsd  warmPed;  //需要预警的行人信息
		int              num;      //行人个数
		HDPedBsd         *p;       //存放所有信息信息的指针
		int              memSize;  //开辟的pedestrianP指针的size
}HDPedsBsd;
#endif
/*****************/
#ifndef _HDFRAMEGETBSD_
#define _HDFRAMEGETBSD_
typedef struct
{
		int              reCalibrate;   //重新校准标志
		int              isDay;         //1：白天，-1：夜晚
		long long        timeStamp;     //时间戳

		HDLanesBsd       lanes;         //LDWS
		HDCarsBsd        cars;          //FCWS
		HDPedsBsd        peds;          //行人预警

		HDBlindDetectBsd bsd;           //盲区检测结果

		HDWarnAreaBsd    warnArea;      //算法修正的盲区报警区域		
		HDFiatigueBsd    fiatigue;      //疲劳驾驶
		HDLameOn         lameOn;        //车灯开启提示
		HDSkyLineBsd     skyLine;       //天际线
		void             *testP;        //测试用的指针，外部无效
}HDFrameGetBsd;
#endif

/****************************************************************************************/
                                 /*bsd接口函数*/
/****************************************************************************************/
void CreatBsd(HDIniSetBsd *iniSetBsd,HDIniGetBsd *iniGetBsd,void **dev);//初始化bsd
void DeleteBsd(void);                                                       //释放bsd
/*******************************************/
typedef void (*SETBSDDATA)(HDFrameSetBsd *frameSetBsd,void *dv);          //给bsd输入参数(回调函数)
typedef void (*GETBSDDATA)(HDFrameGetBsd *frameGetBsd,void *dv);          //获取bsd的计算结果(回调函数)

extern  SETBSDDATA SetBsdData;
extern  GETBSDDATA GetBsdData;






#endif


