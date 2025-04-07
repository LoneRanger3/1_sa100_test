/*
 * lb_pano.c - pano interface.
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _LB_PANO_H_
#define _LB_PANO_H_

/****************************************************************************************/
				/* base data type*/
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

#ifndef _FPOINTC_
#define _FPOINTC_
typedef	struct {
	union {
		float x;
		float col;
	};
	union {
		float y;
		float row;
	};
} FPOINTC;
#endif

/*****************/
#ifndef _IMGYUVC_
#define _IMGYUVC_
typedef struct {
	SIZEC size;         /* y plane size */
	unsigned char *yP;  /* y buffer pointer */
	unsigned char *uvP; /* uv buffer pointer */
} IMGYUVC;
#endif

/*****************/
#ifndef _IBORDERC_
#define _IBORDERC_
typedef struct {
	int up;
	int dn;
	int lt;
	int rt;
} IBORDERC;
#endif

/***************************************/
#ifndef _UCHAR2DMEMC_
#define _UCHAR2DMEMC_
typedef struct {
	SIZEC         size;       /* plane size */
	unsigned char *p;         /* data pointer */
	int           memSize;    /* p buffer size */
} UCHAR2DMEMC;
#endif

/*****************************************
 * data structure for pano init
 *****************************************/
#ifndef _HDMODULEENABLEPANO_
#define _HDMODULEENABLEPANO_
typedef struct {
	int inGps;	/* whether input gps speed */
	int inObd;	/* whether input obd para */
} HDModuleEnablePano;	/* all modules enable */
#endif

/*****************/
#ifndef _HDCARPARAPANO_
#define _HDCARPARAPANO_
typedef struct {
	int enable;	/* whether the HDCarParaPano para is valid */
	int width;	/* car width unit:cm */
} HDCarParaPano;		/* car information */
#endif

/*****************/
#ifndef _HDCAMINPARAPANO_
#define _HDCAMINPARAPANO_
typedef struct {
	int      fps;		/* frame rate */
	SIZEC    imgSize;	/* image size */
} HDCamInParaPano;		/* camera internal para */
#endif

/*****************/
#ifndef _CAMDEFINE_
#define _CAMDEFINE_
#define MAJOR_CAM 0 /* use only this */
#define MINOR_CAM 1 /* not use */
#define CAM_NUM   2
#endif

/*****************/
#ifndef _HDCAMPARAPANO_
#define _HDCAMPARAPANO_
typedef struct {
	HDCamInParaPano     camInPara;     /* camera internal para */
} HDCamParaPano;
#endif

/*****************/
#ifndef _HDCALIBINPARAPANO_
#define _HDCALIBINPARAPANO_
typedef struct {
	int     modeName;
	float   f;
	FPOINTC cp;
} HDCalibInParaPano;
#endif

/*****************/
#ifndef _HDCALIBOUTPARAPANO_
#define _HDCALIBOUTPARAPANO_
  #define  CALIB_OUT_NUM 8
typedef struct {
	double hmgp[CALIB_OUT_NUM];
} HDCalibOutParaPano;
#endif

typedef struct {
	int rearAppear;		/* whether the image at the rear of the car */
	int cutLineUpThr;	/* cut line up limit, unit: pixel */
	int cutLine;		/* cut line, unit:pixel */
	int cutLineDnThr;	/* cut line down limit, unit: pixel */
} HDCalibCutLinePara;		/* cut line para */

/*****************/
typedef struct {
	int                preViewWidth; /* preview width */
	int                preViewHeight;/* preview heitht */
	IBORDERC           carBorder;    /* car image int preview position */
	int                rearDist;     /* rear distance, unit: cm */
	HDCalibCutLinePara cutLinePara;  /* cut line para */
} HDCalibPreViewPano;

/*****************/
#ifndef _HDCALIBPARAPANO_
#define _HDCALIBPARAPANO_
typedef struct {
	HDCalibInParaPano   calibInPara;  /* get camera internal para after calibration */
	HDCalibOutParaPano  calibOutPara; /* get camera external para after calibration */
	HDCalibPreViewPano  preView;      /* calibration output preview */
} HDCalibParaPano;
#endif

/*****************/
#define WARN_LINE_NUM 3

/*****************/
#ifndef _HDINISETPANO_
#define _HDINISETPANO_
typedef struct {
	HDModuleEnablePano   moduleEnable;		/* all modules enable */
	HDCarParaPano        carPara;			/* car para */
	HDCamParaPano        cameraPara[CAM_NUM];	/* camera para */
	HDCalibParaPano      calibPara;			/* calibration para */
	IMGYUVC              carBodyImg;		/* car image */
	/*
	 * The warning lines at the rear
	 * eg:warnLine[0] = 30 :The first warning line at 30cm behind the rear of the car
	 *    warnLine[1] = 60 :The second warning line at 60cm behind the rear of the car
	 *    warnLine[1] = 90 :The third warning line at 90cm behind the rear of the car
	 */
	int                  warnLines[WARN_LINE_NUM];
	char                *dataFormat;		/* data format */
} HDIniSetPano;						/* pano init para*/
#endif

/*****************/
#ifndef _HDINIGETPANO_
#define _HDINIGETPANO_
typedef struct {
	char   *version;  /* pano version */
} HDIniGetPano;
#endif

/****************************************************************************************/
/* pano callback data struct */
/****************************************************************************************/
#ifndef _HDGPSPANO_
#define _HDGPSPANO_
typedef struct {
	int   enable;
	float speed;   /* gps speed km/h */
} HDGpsPano;
#endif
/*****************/
#ifndef _HDCAROPERATEPANO_
#define _HDCAROPERATEPANO_
typedef struct {
	struct {
		int enable;
		int isBrake; /* whether brake */
	} brake;
	struct {
		int enable;
		int isAccel; /* whether accel */
	} accelerato;
	struct {
		int enable;
		int ang;  /* steering wheel angle £¨-540~540£© */
	} wheel;
} HDCarOperatePano; /*   car operate information */
#endif

/*****************/
#ifndef _HDCARSTATUSPANO_
#define _HDCARSTATUSPANO_
typedef struct {
	struct {
		int   enable;
		float value;  /* car speed (km/h) */
	} speed;
	struct {
		int   enable;
		int   isObstacle;/* whether obstacle */
		float dist;      /* obstacle distance unit: m */
	} radar;
} HDCarStatusPano; /* car status information  */
#endif

/*****************/
#ifndef _HDOBDPANO_
#define _HDOBDPANO_
typedef struct {
	HDCarOperatePano operate; /* car operate information */
	HDCarStatusPano  status;  /* car status information */
} HDObdPano; /* read car information */
#endif

/*****************/
#ifndef _HDSETFRAMEPANO_
#define _HDSETFRAMEPANO_
typedef struct {
	long long         timeStamp;
	IMGYUVC           imgYuv[CAM_NUM];
	HDGpsPano         gps;			/* gps speed */
	HDObdPano         obd;			/* obd para */
} HDSetFramePano;
#endif

/*****************/
#ifndef _HDGETFRAMEPANO_
#define _HDGETFRAMEPANO_
typedef struct {
	long long      timeStamp;
	IMGYUVC        imgPreView;
	void           *testP;	/* algorithm internal test use */
} HDGetFramePano;
#endif
/*****************/
#ifndef _HDCHECKERBOARD_
#define _HDCHECKERBOARD_
typedef struct {
	int  boxRows;		/* the number of rows in the back and white grid */
	int  boxCols;		/* the number of columns in the back and white grid */
	int  boxWidth;		/* back and white grid width (unit: cm) */
	int  boxheight;		/* back and white grid height (unit: cm) */
	int  dist2Rear;		/* the bottom row of the grid to the rear of the car */
	int  carWidth;		/* car width */
	int  carLong;		/* car length */
	int  preViewWidth;
	int  preViewHeight;

	int  frontDist;		/* car front distance (unit: cm) */
	int  rearDist;		/* car rear distance (unit: cm)  */
	int  align;		/* rear direction. -1 or 1*/
} HDCheckerBoard;
#endif

/*****************/
#ifndef _HDCALIBIN_
#define _HDCALIBIN_
typedef struct {
	IMGYUVC        imgYuv;
	HDCheckerBoard checkerBoard;
} HDCalibIn;
#endif
/*****************/
#ifndef _HDCALIBOUT_
#define _HDCALIBOUT_
typedef struct {
	HDCalibParaPano calibPara;
	IMGYUVC         birdBiewImg;
} HDCalibOut;
#endif

/****************************************************************************************/
			/* panoramic algorithm interface */
/****************************************************************************************/
void CreatPano(HDIniSetPano *iniSetPano, HDIniGetPano *iniGetPano, void **dev);
void DeletePano(void);

/* input callback */
typedef void (*SETDATAPANO)(HDSetFramePano *setFramePano, void *dv);

/* get return result callback */
typedef void (*GETDATAPANO)(HDGetFramePano *getFramePano, void *dv);

extern  SETDATAPANO SetDataPano;
extern  GETDATAPANO GetDataPano;

/****************************************************************************************/
			/* calibration interface */
/****************************************************************************************/
/* return 0: fail, 1: may be successful, neet to see preview image */
int CreatCalibrate(HDCalibOut *calibOut, HDCalibIn *calibIn);
void DelteteCalibrate(void);

#endif

