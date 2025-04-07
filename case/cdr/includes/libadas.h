#ifndef  _LIBADAS_H_
#define  _LIBADAS_H_

/****************************************************************************************/
/*基本数据类型*/
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
	int angH;//镜头的水平视角
	int angV;//镜头的垂直视角
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
	SIZEC size;         //y分量的尺寸
	unsigned char *yP;  //y分量指针
	unsigned char *uvP; //uv分量指针
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
	SIZEC size;       //给分量尺寸
	unsigned char *p; //数据指针
	int memSize;      //p的size
} UCHAR2DMEMC;
#endif
/****************************************************************************************/
/*adas初始化用到的数据结构*/
/****************************************************************************************/

#ifndef _HDMODULEENABLE_
#define _HDMODULEENABLE_
typedef struct {
	int inGps;        //是否输入gps车速
	int inGsensor;    //是否输入gsenser信息（stop、a、v）
	int inObd;        //是否输入输入obd参数
	int inCamOutPara; //是否输入摄像头外部参数(安装参数)
} HDModuleEnable; //各模块使能
#endif

/*****************/
#ifndef _HDCARPARA_
#define _HDCARPARA_
typedef struct {
	int enable;       //该参数是否有效
	int width;        //本车车宽（厘米）
} HDCarPara; //本车信息
#endif
/*****************/
#ifndef _HDROIPARA_
#define _HDROIPARA_
typedef struct {
	int enable;       //该参数是否有效
	int upRow;        //上边缘(天际线),laneCrossPoint.y
	int dnRow;        //下边缘（引擎盖线）
} HDRoiPara; //感兴趣区域
#endif
///*****************/
#ifndef _HDCAMOUTPARA_
#define _HDCAMOUTPARA_
typedef struct {
	int     enable;          //该参数是否有效
	int     cam2ground;      //距离地面高度，单位：厘米，下同。   		 默认值：130，  范围：50 - 400
	int     cam2middle;      //距离汽车中轴线，左负右正。单位：厘米      默认值：0，    范围：-100 - 100
	int     cam2head;        //与车头距离。单位：厘米            		 默认值：150，  范围：0 - 300
	IPOINTC laneCrossPoint;  //单位：像素。标定界面中要求的车道线的交点坐标，通常在图像中心附近
} HDCamOutPara; //摄像头外部参数
#endif
/*****************/
#ifndef _HDCAMINPARA_
#define _HDCAMINPARA_
typedef struct {
	int      fps;     //帧率
	SIZEC    imgSize; //图片尺寸
	VIEWANGC viewAng; //摄像头水平和垂直视角
} HDCamInPara; //摄像头内部参数
#endif
/*****************/
#ifndef _HDSKYLINE_
#define _HDSKYLINE_
typedef struct {
	int isFullLearn;//是否充分学习
	int value ;     //天际线的值
} HDSkyLine; //天际线
#endif
/*****************/
#ifndef _CAMDEFINE_
#define _CAMDEFINE_
#define MAJOR_CAM 0 //只用这个
#define MINOR_CAM 1 //暂时不用
#define CAM_NUM   2

#endif
/*****************/
#ifndef _HDCAMERAPARA_
#define _HDCAMERAPARA_
typedef struct {
	HDCamOutPara    camOutPara;    //摄像头外部参数(安装参数)
	HDCamInPara     camInPara;     //摄像头内部参数
} HDCameraPara;
#endif
/*****************/
#ifndef _HDINISETDATA_
#define _HDINISETDATA_
typedef struct {
	HDModuleEnable   moduleEnable;  //各模块使能
	HDCarPara        carPara;       //本车参数
	HDRoiPara        roiPara;       //roi参数
	HDCameraPara     cameraPara[CAM_NUM];
	int
	laneWarnDist;  //车道偏离报警距离（车轮距车道laneWarnDist距离即报警）设置-30 到 +30（厘米），默认值为 0,负 ：未越线，正：越线
	char            *dataFormat;    //数据格式
} HDIniSetData; //初始化设置数据（设置给adas）
#endif
/*******************************************/
#ifndef _HDREFERLINE_
#define _HDREFERLINE_
typedef struct {
	IPOINTC lt2Points[2];    //左侧校准线的两个端点
	IPOINTC rt2Points[2];    //右侧校准线的两个端点
} HDReferLine;
#endif
/*****************/
#ifndef _HDINIGETDATA_
#define _HDINIGETDATA_
typedef struct {
	char        *version;  //算法版本号
	HDReferLine referLine; //参考线（校准线，非固定安装使用，如行车记录仪）
} HDIniGetData; //初始化读取数据（从adas读出）
#endif
/****************************************************************************************/
/*adas回调函数用到的数据结构*/
/****************************************************************************************/
#ifndef _HDGPS_
#define _HDGPS_
typedef struct {
	int   enable;   //该参数是否有效，有信号时置1，无信号时置0
	float speed;    //gps车速（公里/时）
} HDGps; //inGps为真时输入
#endif
/*****************/
#ifndef _HDGSENSOR_
#define _HDGSENSOR_
typedef struct {
	int   acct;     //加速度(m/s^2)
	float speed;    //车速（公里/时）
	int   stop;     //车辆是否停止
} HDGsensor; //inGsensor为真时输入
#endif
/*****************/
#ifndef _HDCAROPERATE_
#define _HDCAROPERATE_
typedef struct {
	struct {
		int enable; //该参数是否有效
		int isBrake;//是否在刹车
	} brake;
	struct {
		int enable; //该参数是否有效
		int isAccel;//是否在踩油门
	} accelerato;
	struct {
		int enable; //该参数是否有效
		int isWiper;//是否在打雨刷
	} wiper;
	struct {
		int enable; //该参数是否有效
		int ang;    //方向盘角度（-540~540）
	} wheel;
} HDCarOperate; //车辆操控信息
#endif
/*****************/
#ifndef _HDCARSTATUS_
#define _HDCARSTATUS_
typedef struct {
	struct {
		int   enable; //该参数是否有效
		float value;  //本车车速（公里/时）
	} speed;
	struct {
		int   enable;    //该参数是否有效
		int   isObstacle;//有没有障碍物
		float dist;      //如果有障碍物，为离障碍物的距离（单位：米）
	} radar;
} HDCarStatus; //车辆状态信息
#endif
/*****************/
#ifndef _HDCARLAME_
#define _HDCARLAME_
typedef struct {
	struct {
		int enable;    //该参数是否有效
		int value;     //-1:左转，0：不转，1：右转
	} corner; //转向灯
	struct {
		int enable;    //该参数是否有效
		int value;     //0：关闭，1：近光灯，2：远光灯，3：驻车灯
	} common; //大灯
	struct {
		int enable;    //该参数是否有效
		int value;     //0：关闭，1：开启
	} fog; //雾灯
	struct {
		int enable;    //该参数是否有效
		int value;     //0：关闭，1：开启
	} danger; //危险警示灯
} HDCarLame; //灯光信息
#endif
/*****************/
#ifndef _HDOBD_
#define _HDOBD_
typedef struct {
	HDCarOperate operate;//车辆操控信息
	HDCarStatus  status; //车辆状态信息
	HDCarLame    lame;   //灯光信息
} HDObd; //HDObd读取的车辆信息
#endif
/*****************/
#ifndef _HDWARNSENSITY_
#define _HDWARNSENSITY_
typedef struct
{
		int fcwSensity;//前车防撞灵敏度  提前 fcwSensity 秒报警 
		int ldwSensity;//车道偏离灵敏度  偏离 ldwSensity 秒报警
		int fpwSensity;//行人防撞灵敏度  提前 fpwSensity 秒报警
}HDWarnSensity;
#endif
/*****************/
#ifndef _HDFRAMESETDATA_
#define _HDFRAMESETDATA_
typedef struct {
	long long     timeStamp;      //时间戳
	int           isDay;          //1：白天，-1：夜晚
	int
	cpuGear;        //cpu档位设置(默认值设为0)  // -1:低档：cpu占用率低,  0:中档：cpu占用率中,  1:高档：cpu占用率高
	IMGYUVC       imgYuv[CAM_NUM];//输入的yuv图像
	HDGps         gps;            //gps车速
	HDGsensor     gsensor;        //gsensor获得的车辆信息
	HDObd         obd;            //obd参数
	HDWarnSensity warnSensity;    //报警灵敏度
	int           runPed;        //是否开启行人检测 1:是，0：否
} HDFrameSetData;
#endif
/*******************************************/
#ifndef _HDOBJECT_
#define _HDOBJECT_
typedef struct {
	int    status;          //目标状态  -1:左道目标  0：本道目标   1：右道目标  2：本道最近车辆，如需报警切换成3   3：注意碰撞  4: 前车已离开  5：小心前车  6：保持车距 7: 小心碰撞
	IRECTC loc;             //目标在图像中的位置
	float  dist;            //目标距离
	float  time;            //预判碰撞时间，若为公交车版本，则为非公交车在违规车道的停留时间
} HDObject;
#endif
/*****************/
#ifndef _HDWARMOBJECT_
#define _HDWARMOBJECT_
typedef struct {
	int warnIdx;    //需要预警的目标的下标
	int warnStatus; //报警的状态，即：什么类型的报警，0：没有需要预警的目标，大于零则对应到 报警目标 HDObject 的 status
} HDWarmObject;
#endif
/*****************/
#ifndef _HDPLATE_
#define _HDPLATE_
typedef struct {
	int          isExist;
	IRECTC       loc;      //目标在图像中的位置
} HDPlate;
#endif
/*****************/
#ifndef _HDCAR_
#define _HDCAR_
typedef struct {
	HDObject object;//其中的status 为目标状态  -1:左道目标  0：本道目标   1：右道目标  2：本道最近车辆，如需报警切换成3   3：注意碰撞  4: 前车已离开  5：小心前车  6：保持车距 7: 小心碰撞
	HDPlate  plate;//车牌
} HDCar; //每辆车的输出信息
#endif
/*****************/
typedef struct {
	HDWarmObject  warmCar;//需要预警的车辆信息
	int           num;    //车辆个数
	HDCar         *p;     //存放所有车辆信息的指针
	int           memSize;//开辟的p指针的size
} HDCars;
/*****************/
#ifndef _HDLANE_
#define _HDLANE_

#define LANE_POINT_NUM  6
typedef struct {
	IPOINTC pOfLane[LANE_POINT_NUM];//车道线上的两点
	int     color;     //车道线颜色。0：不明确，1：白线，2：黄线
	int     shape;     //车道线形状。0：不明确， 1：直单实线,  2：直虚线， 3：直双实线,  4：直内虚外实， 5: 直内实外虚
	//                       -1：曲单实线, -2：曲虚线，-3：曲双实线, -4：曲内虚外实，-5: 曲内实外虚
} HDLane;

#endif
/*****************/
#ifndef _HDLANES_
#define _HDLANES_
typedef struct {
	int isDisp;//是否显示车道线
	int status;
	//-2:显示左侧偏离，语音提醒左侧偏离
	//-1:显示左侧偏离，铃声提醒左侧偏离
	// 1:显示右侧偏离，铃声提醒右侧偏离
	// 2:显示右侧偏离，语音提醒右侧偏离
	//或者不区分左右：-2、-1、1、2 统一播报车道偏离
	//4：本车占用了公交车道
	HDLane lt;   //左侧车道线
	HDLane rt;   //右侧车道线
} HDLanes;
#endif
/*****************/
#ifndef _HDFIATIGUE_
#define _HDFIATIGUE_
typedef struct {
	int   isWarn;//是否报警
	float time;//连续驾驶时长(单位：小时)
} HDFiatigue; //疲劳驾驶
#endif
/*****************/
#ifndef _HDLAMEON_
#define _HDLAMEON_
typedef struct {
	int value;//0：不提示，1：开近光灯，2：开远光灯
} HDLameOn; //灯光开启提示
#endif
/*****************/
#ifndef _HDBLINDDETECT_
#define _HDBLINDDETECT_
typedef struct {
	int value;//-1:左侧危险，0：安全，1：右侧危险
} HDBlindDetect; //盲区检测
#endif
/*****************/
#ifndef _HDPED_
#define _HDPED_
typedef struct {
	HDObject object;//其中的status 为目标状态  -1:左道目标  0：本道目标   1：右道目标  2：本道最近行人，如需报警切换成3   3：小心行人
} HDPed; //行人目标
#endif
/*****************/
#ifndef _HDPEDS_
#define _HDPEDS_
typedef struct {
	HDWarmObject  warmPed;  //需要预警的行人信息
	int           num;      //行人个数
	HDPed         *p;       //存放所有信息信息的指针
	int           memSize;  //开辟的pedestrianP指针的size
} HDPeds;
#endif
/*****************/
#ifndef _HDFRAMEGETDATA_
#define _HDFRAMEGETDATA_
typedef struct {
	int            reCalibrate;   //重新校准标志
	int            isDay;         //1：白天，-1：夜晚
	long long      timeStamp;     //时间戳
	HDLanes        lanes;         //LDWS
	HDCars         cars;          //FCWS
	HDPeds         peds;          //行人预警
	HDFiatigue     fiatigue;      //疲劳驾驶
	HDLameOn       lameOn;        //车灯开启提示
	HDSkyLine      skyLine;       //天际线
	void           *testP;        //测试用的指针，外部无效
} HDFrameGetData;
#endif

/****************************************************************************************/
/*adas接口函数*/
/****************************************************************************************/
void CreatAdas(HDIniSetData *iniSetData, HDIniGetData *iniGetData,
	void **dev); //初始化adas
void DeleteAdas(void);                                                       //释放adas
/*******************************************/
typedef void (*SETADASDATA)(HDFrameSetData *frameSetData,
	void *dv);         //给adas输入参数(回调函数)
typedef void (*GETADASDATA)(HDFrameGetData *frameGetData,
	void *dv);         //获取adas的计算结果(回调函数)

extern  SETADASDATA SetAdasData;
extern  GETADASDATA GetAdasData;

#endif
