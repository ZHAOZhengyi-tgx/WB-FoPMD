
#ifndef __FPMD_CFG__
#define __FPMD_CFG__

typedef struct
{
	unsigned int nUserLevel;
	unsigned int nWbMachineRunningMode;
	unsigned int nWbMachinePowerMode;
	unsigned int nFPMD_VerifyMode;
	unsigned int nDisplayLanguage;

}SYS_USER_INTERFACE_CFG;

#define USER_LEVEL_DEVELOPER  3
#define USER_LEVEL_ENGINEER   2
#define USER_LEVEL_SERVICE    1
#define USER_LEVEL_OPERATOR   0

#define WB_MACHINE_RUNNING_ACTUAL_BOND 0
#define WB_MACHINE_RUNNING_DUMMY_BOND  1
#define WB_MACHINE_RUNNING_CALIBRATION 2

#define WB_MACHINE_POWER_LEVEL_ULTRA_LOW  0
#define WB_MACHINE_POWER_LEVEL_LOW        1
#define WB_MACHINE_POWER_LEVEL_HIGH       2

#define WB_MACHINE_POWER_MODE_CONST_I  0
#define WB_MACHINE_POWER_MODE_CONST_V  1
#define WB_MACHINE_POWER_MODE_CONST_P  2
#define WB_MACHINE_POWER_MODE_NORMAL   3

#define WB_MACHINE_FPMD_MODE_VIEW    0
#define WB_MACHINE_FPMD_MODE_VERIFY  1
#define WB_MACHINE_FPMD_MODE_CAPTURE 2

#define FPMD_DISPLAY_LANGUAGE_EN    0
#define FPMD_DISPLAY_LANGUAGE_CN    0

#define DEF_NUM_WAVEFORMS_PER_POINT        10
#define DEF_CAPTURE_DATA_STAGE_PREPRATION  0
#define DEF_CAPTURE_DATA_STAGE_1           1
#define DEF_CAPTURE_DATA_STAGE_2           2
#define DEF_CAPTURE_DATA_STAGE_3           3
#define DEF_CAPTURE_DATA_STAGE_4           4
#define DEF_CAPTURE_DATA_STAGE_5           5
#define DEF_CAPTURE_DATA_COMPLETE          6

#define DEF_VERIFY_DATA_COMPLETE          1

typedef struct
{
	double dForceCmd_gram;
	double dPowerCmd_Dac;
	int iPowerLevel;
	int iPowerMode;
	double dSampleTime_Fer_us;
}FOPEM_FORCE_POWER_VALUE_SAVE_FILE;

typedef struct
{
	unsigned int nSampleLen;
	unsigned int nDisplayLen;
	double tPeriodSample_us;
	unsigned char ucSampleVoltMode;

}SYS_SETTING_ADC;

// @3
typedef struct
{
	int iForceOffset_gram;
	int iPowerOffset_mW;
}FOPEM_OFFSET_FORCE_POWER;

#define DEF_DISPLAY_LEN   1024
#define DEF_SAMPLE_LEN    (DEF_DISPLAY_LEN * 6 * 2)
#define DEF_SAMPLE_FER_US  (400.0)

typedef struct
{
	double dPowerTh_mW;
	double dImpedanceTh_ohm;
	double dForceTh_gram;
	int idxPowerThComboSel;
	int idxImpedanceThComboSel;
	int idxForceThComboSel;
}FOPEM_DETECT_THRESHOLD;

typedef struct
{
	double dPeakPower_mW;
	double dPeakImpedance_ohm;
	double dPeakForce_gram;
	double dUpperBoundThPower_mW;
	double dUpperBoundThImpedance_ohm;
	double dUpperBoundThForce_gram;
	double dLowerBoundThPower_mW;
	double dLowerBoundThImpedance_ohm;
	double dLowerBoundThForce_gram;
}FOPEM_VALUE_REAL_TIME;

extern FOPEM_DETECT_THRESHOLD stFOPEM_CheckingThreshold;
#define _DEFAULT_MINIMUM_IMPEDANCE (7.0)
#define TOTAL_LEVEL_DETECTION_FORCE_TH   22
#define TOTAL_LEVEL_DETECTION_IMPEDANCE_TH   7
#define TOTAL_LEVEL_DETECTION_POWER_TH   19

extern char *pstrCheckingForceThreshold[];
extern double adCheckingForceThreshold[];

extern char *pstrCheckingImpedanceThreshold[];
extern double adCheckingImpedanceThreshold[];

extern char *pstrCheckingPowerThreshold[];
extern double adCheckingPowerThreshold[];

#endif // __FPMD_CFG__