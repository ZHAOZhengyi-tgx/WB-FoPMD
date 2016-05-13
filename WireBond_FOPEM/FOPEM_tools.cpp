//The MIT License (MIT)
//
//Copyright (c) 2016 ZHAOZhengyi-tgx
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
// History
// @3 20130718 Save and load config files
#include "stdafx.h"
//#include "Dmp609Dll.h"//必须包含的DLL文件

#include "EFS_DisplayCfg.h"
#include "math.h"
#include <direct.h>
//extern HANDLE	m_hDevice;

SYS_SETTING_ADC stSysSettingAdc;
SYS_SETTING_ADC stLoadDataAdcSetting;
FOPEM_FORCE_POWER_SETTING stForcePowerSetting;
FOPEM_OFFSET_FORCE_POWER stSysOffsetForcePower;

char cFlagSystemOfflineMode;
void FOPEM_set_system_offline_mode(char iFlag)
{
	cFlagSystemOfflineMode = iFlag;
}
char FOPEM_get_system_offline_mode()
{
	return cFlagSystemOfflineMode;
}

int nLen = 2000;
int nOffsetBondStart = 0;
unsigned int aiData[65536];
unsigned int aiBakData[65536];

ANALYZE_FORCE_POWER_OUTPUT stLoadForcePowerAnalyzeOutput;
ANALYZE_FORCE_POWER_INPUT stLoadForcePowerAnalyzeInput;

double adLoadWaveformEFO[8192];
double adLoadWaveformForce[8192];
double adLoadWaveformVel[8192];
double adLoadWaveformPowerUS[8192];
double adLoadWaveformPowerI[8192];
double adLoadWaveformPowerV[8192];

double adLoadFilterForce[8192];
double adLoadFilterVel[8192];
double adLoadFilterPowerV[8192];

int aiLoadWaveformEFO[8192];
int aiLoadWaveformForce[8192];
int aiLoadWaveformVel[8192];
int aiLoadWaveformPowerUS[8192];
int aiLoadWaveformPowerI[8192];
int aiLoadWaveformPowerV[8192];

extern double dGoldenWaveformImpedance1B;
ANALYZE_FORCE_POWER_OUTPUT stGoldenForcePowerAnalyzeOutput;
ANALYZE_FORCE_POWER_INPUT stGoldenForcePowerAnalyzeInput;

SYS_SETTING_ADC stGoldenWaveformAdcSetting;
double adGoldenWaveformEFO[8192];
double adGoldenWaveformForce[8192];
double adGoldenWaveformVel[8192];
double adGoldenWaveformPowerUS[8192];
double adGoldenWaveformPowerI[8192];
double adGoldenWaveformPowerV[8192];

int aiGoldenWaveformEFO[8192];
int aiGoldenWaveformForce[8192];
int aiGoldenWaveformVel[8192];
int aiGoldenWaveformPowerUS[8192];
int aiGoldenWaveformPowerI[8192];
int aiGoldenWaveformPowerV[8192];
static int nReadLen;

FOPEM_VALUE_REAL_TIME stForcePowerRollingHorizan;
double adRollingPower_mW[FIG_LENGTH_X_ROLLING_HORIZAN];
double adRollingImpedance_ohm[FIG_LENGTH_X_ROLLING_HORIZAN];
double adRollingForce_gram[FIG_LENGTH_X_ROLLING_HORIZAN];

double adSmoothedRollingPower_mW[FIG_LENGTH_X_ROLLING_HORIZAN];
double adSmoothedRollingImpedance_ohm[FIG_LENGTH_X_ROLLING_HORIZAN];
double adSmoothedRollingForce_gram[FIG_LENGTH_X_ROLLING_HORIZAN];

int FOPEM_get_read_data_length()
{
	return nReadLen;
}

double adGoldenFilterVel[8192];
double adGoldenFilterForce[8192];
double adGoldenFilterPowerV[8192];

FOPEM_DETECT_THRESHOLD stFOPEM_CheckingThreshold;
char *pstrCheckingForceThreshold[] = 
{
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16",
	"17",
	"18",
	"19",
	"20",
	"22",
	"25",
	"30"
};
double adCheckingForceThreshold[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 25, 30, 30, 30, 30};

char *pstrCheckingImpedanceThreshold[] = 
{
	"0.5",
	"1.0",
	"1.5",
	"2.0",
	"5.0",
	"8.0",
	"10.0"};
double adCheckingImpedanceThreshold[] = {0.5, 1.0, 1.5, 2.0, 5.0, 8.0, 10.0};

char *pstrCheckingPowerThreshold[] = 
{
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16",
	"17",
	"18",
	"19",
	"20",
	"22",
	"25",
	"30"
};

double adCheckingPowerThreshold[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 25, 30, 30, 30, 30};

char *pstrCaptureDataPromptWizard[] =
{
	"Prepare to capture data: load FPMD Golden Recipe",
	"Point-1: Set (Force, Power) to (25 gram, 50 Dac) in 1st Bond",
	"Point-2: Set (Force, Power) to (100 gram, 50 Dac) in 1st Bond",
	"Point-3: Set (Force, Power) to (200 gram, 50 Dac) in 1st Bond",
	"Point-4: Set (Force, Power) to (100 gram, 100 Dac) in 1st Bond",
	"Point-5: Set (Force, Power) to (100 gram, 150 Dac) in 1st Bond",
	"Complete capturing data, golden waveform saved",
	"Complete capturing data, golden waveform saved",
	"Complete capturing data, golden waveform saved",
	"Complete capturing data, golden waveform saved"
};

char *pstrVerifyDataPromptWizard[] =
{
	"Prepare to capture data: load FPMD Golden Recipe",
	"Please check (Force, Power) to (25 gram,  50 Dac) in 1st Bond, \n\r proceed with auto bonding",
	"Complete verification, please check result",
	"Complete verification, please check result",
	"2nd Point: Set (Force, Power) to (100 gram, 100 Dac) in 1st Bond",
	"Complete verification, please check result",
	"Complete verification, please check result",
	"Complete verification, please check result",
	"Complete verification, please check result",
	"Complete verification, please check result",
	"Complete verification, please check result",
	"Complete verification, please check result"
};

char *pstrCaptureGoldenVerifyWizardConfirmKeyInValue[] =
{
	"Please confirm the parameter and proceed with auto bonding"
};

char *pstrPowerMode[] = 
{
	"Const-I",
	"Const-V",
	"Const-P",
	"Normal"
};

char *pstrPowerLevel[] =
{
	"Ultra Low",
	"Low",
	"High"
};
//double adCapturePower_Dac[DEF_CAPTURE_DATA_COMPLETE * DEF_NUM_WAVEFORMS_PER_POINT];
//double adCaptureForce_CmdGram[DEF_CAPTURE_DATA_COMPLETE * DEF_NUM_WAVEFORMS_PER_POINT];
//double adCapturePower_mW[DEF_CAPTURE_DATA_COMPLETE * DEF_NUM_WAVEFORMS_PER_POINT];
//double adCaptureForce_Gram[DEF_CAPTURE_DATA_COMPLETE * DEF_NUM_WAVEFORMS_PER_POINT];
// @3
extern int iFPMD_ForceDisplayOffset;
extern int iFPMD_PowerDisplayOffset;
void FOPEM_init_offset_setting()
{
	stSysOffsetForcePower.iForceOffset_gram = 0;
	stSysOffsetForcePower.iPowerOffset_mW = 0;

	iFPMD_ForceDisplayOffset = stSysOffsetForcePower.iForceOffset_gram;
	iFPMD_PowerDisplayOffset = stSysOffsetForcePower.iPowerOffset_mW;
}

void FOPEM_init_threshold_setting()
{
	stFOPEM_CheckingThreshold.idxForceThComboSel = 18;
	stFOPEM_CheckingThreshold.dForceTh_gram = adCheckingForceThreshold[stFOPEM_CheckingThreshold.idxForceThComboSel];
	stFOPEM_CheckingThreshold.idxPowerThComboSel = 15;
	stFOPEM_CheckingThreshold.dPowerTh_mW = adCheckingPowerThreshold[stFOPEM_CheckingThreshold.idxPowerThComboSel];
	stFOPEM_CheckingThreshold.idxImpedanceThComboSel = 3;
	stFOPEM_CheckingThreshold.dImpedanceTh_ohm = adCheckingImpedanceThreshold[stFOPEM_CheckingThreshold.idxImpedanceThComboSel];
}

void FOPEM_init_adc_setting()
{
	stSysSettingAdc.nDisplayLen = DEF_DISPLAY_LEN;
	stSysSettingAdc.nSampleLen = DEF_SAMPLE_LEN;
	stSysSettingAdc.tPeriodSample_us = 400;
	stSysSettingAdc.ucSampleVoltMode = 2;

	stGoldenWaveformAdcSetting = stSysSettingAdc;
	stLoadDataAdcSetting = stSysSettingAdc;
}

void WB_analyze_start_by_efo()
{
	nOffsetBondStart = -1;
	int ii;
	for(ii=0; ii< nLen; ii++)
	{
		if(aiData[ii * 6 + SCOPE_OFST_EFO_] > 60000 
			&& aiData[ (ii + 1) * 6 + SCOPE_OFST_EFO_] < 55000)
		{
			nOffsetBondStart = ii;
			break;
		}
	}
}

void WB_analyze_start_by_vel()
{
	nOffsetBondStart = -1;
	int ii;
	for(ii=0; ii< nLen; ii++)
	{
		if(aiData[ii * 6 + SCOPE_OFST_VEL_] > 30000 
			&& aiData[ (ii + 1) * 6 + SCOPE_OFST_VEL_] < 30000
			&& aiData[ (ii + 2) * 6 + SCOPE_OFST_VEL_] < 30000
			&& aiData[ (ii + 3) * 6 + SCOPE_OFST_VEL_] < 30000)
		{
			nOffsetBondStart = ii - 20;
			if(nOffsetBondStart < 0)
			{
				nOffsetBondStart = 0;
			}
			break;
		}
	}
}
#define _DEFAULT_POWER_FACTOR_K_I   (1.0281602e-1)
#define _DEFAULT_POWER_FACTOR_K_V   (2.14842252)
#define _DEFAULT_FORCE_RATIO_DUMMY_GRAM_PER_VOLT    (365.83230)
#define _DEFAULT_FORCE_RATIO_DUMMY_OFFSET_0_FORCE   (-22.94259)
#define _DEFAULT_FORCE_RATIO_ACTUAL_GRAM_PER_VOLT   (739.49467)
#define _DEFAULT_FORCE_RATIO_ACTUAL_OFFSET_0_FORCE  (-46.542725652672281)

//#include "DMP609Dll.h"
SYS_USER_INTERFACE_CFG stSystemUserInterfaceCfg;
char cFlagCaptureTrigger = FALSE;
char cFlagDummyBond = 0;

char FOPEM_get_flag_dummy_bond()
{
	return cFlagDummyBond;
}

void FOPEM_set_combo_by_user_level()
{
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_OPERATOR)
	{
		stSystemUserInterfaceCfg.nFPMD_VerifyMode = WB_MACHINE_FPMD_MODE_VIEW;
		stSystemUserInterfaceCfg.nWbMachineRunningMode = WB_MACHINE_RUNNING_ACTUAL_BOND;
	}
}

void FOPEM_init_default_config()
{
	stSystemUserInterfaceCfg.nDisplayLanguage = 0;
	stSystemUserInterfaceCfg.nWbMachinePowerMode = WB_MACHINE_POWER_MODE_CONST_I;
	stSystemUserInterfaceCfg.nWbMachineRunningMode = WB_MACHINE_RUNNING_ACTUAL_BOND;
	stSystemUserInterfaceCfg.nFPMD_VerifyMode = WB_MACHINE_FPMD_MODE_VIEW;
	stSystemUserInterfaceCfg.nDisplayLanguage = FPMD_DISPLAY_LANGUAGE_EN;

	// Force & Power
	stForcePowerSetting.stCfgPowerFactor.fFactorK_I = _DEFAULT_POWER_FACTOR_K_I;
	stForcePowerSetting.stCfgPowerFactor.fFactorK_V = _DEFAULT_POWER_FACTOR_K_V;
	stForcePowerSetting.stCfgPowerFactor.fFactorK_R = (_DEFAULT_POWER_FACTOR_K_V/_DEFAULT_POWER_FACTOR_K_I);

	stForcePowerSetting.stCfgDummyBondForceFactor.fFactorForceRatio_GramPerVolt = _DEFAULT_FORCE_RATIO_DUMMY_GRAM_PER_VOLT;
	stForcePowerSetting.stCfgDummyBondForceFactor.fFactorForceRatio_OffsetZeroForce = _DEFAULT_FORCE_RATIO_DUMMY_OFFSET_0_FORCE;
	stForcePowerSetting.stCfgActualBondForceFactor.fFactorForceRatio_GramPerVolt = _DEFAULT_FORCE_RATIO_ACTUAL_GRAM_PER_VOLT;
	stForcePowerSetting.stCfgActualBondForceFactor.fFactorForceRatio_OffsetZeroForce= _DEFAULT_FORCE_RATIO_ACTUAL_OFFSET_0_FORCE;
	
	///// Loaded waveform
	stLoadForcePowerAnalyzeInput.adEFO = &adLoadWaveformEFO[0];
	stLoadForcePowerAnalyzeInput.adVel = &adLoadWaveformVel[0];
	stLoadForcePowerAnalyzeInput.adForce = &adLoadWaveformForce[0];
	stLoadForcePowerAnalyzeInput.adPowerI = &adLoadWaveformPowerI[0];
	stLoadForcePowerAnalyzeInput.adPowerV = &adLoadWaveformPowerV[0];
	stLoadForcePowerAnalyzeInput.adPowerUS = &adLoadWaveformPowerUS[0];
	stLoadForcePowerAnalyzeInput.adFilterForce = &adLoadFilterForce[0];
	stLoadForcePowerAnalyzeInput.adFilterVel = &adLoadFilterVel[0];
	stLoadForcePowerAnalyzeInput.adFilterPowerV = &adLoadFilterPowerV[0];

	stLoadForcePowerAnalyzeInput.fSampleTime_ms = stSysSettingAdc.tPeriodSample_us / 1000;
	stLoadForcePowerAnalyzeInput.uiDataLen = FOPEM_get_read_data_length();
	stLoadForcePowerAnalyzeInput.stpOnlineForcePowerSetting = &stForcePowerSetting;

	/////////////////// Golden 
	stGoldenForcePowerAnalyzeInput.adEFO = &adGoldenWaveformEFO[0];
	stGoldenForcePowerAnalyzeInput.adVel = &adGoldenWaveformVel[0];
	stGoldenForcePowerAnalyzeInput.adForce = &adGoldenWaveformForce[0];
	stGoldenForcePowerAnalyzeInput.adPowerI = &adGoldenWaveformPowerI[0];
	stGoldenForcePowerAnalyzeInput.adPowerV = &adGoldenWaveformPowerV[0];
	stGoldenForcePowerAnalyzeInput.adPowerUS = &adGoldenWaveformPowerUS[0];
	stGoldenForcePowerAnalyzeInput.adFilterForce = &adGoldenFilterForce[0];
	stGoldenForcePowerAnalyzeInput.adFilterVel = &adGoldenFilterVel[0];
	stGoldenForcePowerAnalyzeInput.adFilterPowerV = &adGoldenFilterPowerV[0];

	stGoldenForcePowerAnalyzeInput.fSampleTime_ms = stSysSettingAdc.tPeriodSample_us / 1000;
	stGoldenForcePowerAnalyzeInput.uiDataLen = FOPEM_get_read_data_length();
	stGoldenForcePowerAnalyzeInput.stpOnlineForcePowerSetting = &stForcePowerSetting;

}

void FOPEM_update_peak_value_in_rolling_horizan() 
{
	stForcePowerRollingHorizan.dPeakForce_gram = _MAX_FORCE_DISPLAY_GRAM_DEF_FIG3_;
		// stGoldenForcePowerAnalyzeOutput.f1stBondForceGram + 	2 * stFOPEM_CheckingThreshold.dForceTh_gram;
	stForcePowerRollingHorizan.dPeakImpedance_ohm = _MAX_IMPEDANCE_DISPLAY_OHM_DEF_FIG5_;
		// stGoldenForcePowerAnalyzeOutput.f1stBondImp + 2 * stFOPEM_CheckingThreshold.dImpedanceTh_ohm;
	stForcePowerRollingHorizan.dPeakPower_mW = _MAX_POWER_DISPLAY_MW_DEF_FIG4_;
	    //stGoldenForcePowerAnalyzeOutput.f1stBondPmW + 2 * stFOPEM_CheckingThreshold.dPowerTh_mW;
	
	stForcePowerRollingHorizan.dUpperBoundThForce_gram = stGoldenForcePowerAnalyzeOutput.f1stBondForceGram + 
		stFOPEM_CheckingThreshold.dForceTh_gram;
	stForcePowerRollingHorizan.dUpperBoundThImpedance_ohm = stGoldenForcePowerAnalyzeOutput.f1stBondImp + 
		stFOPEM_CheckingThreshold.dImpedanceTh_ohm;
	stForcePowerRollingHorizan.dUpperBoundThPower_mW = stGoldenForcePowerAnalyzeOutput.f1stBondPmW + 
		stFOPEM_CheckingThreshold.dPowerTh_mW;

	stForcePowerRollingHorizan.dLowerBoundThImpedance_ohm = _DEFAULT_MINIMUM_IMPEDANCE;
	stForcePowerRollingHorizan.dLowerBoundThForce_gram = stGoldenForcePowerAnalyzeOutput.f1stBondForceGram 
		- stFOPEM_CheckingThreshold.dForceTh_gram;
	if(stForcePowerRollingHorizan.dLowerBoundThForce_gram < 0) stForcePowerRollingHorizan.dLowerBoundThForce_gram = 0;

	stForcePowerRollingHorizan.dLowerBoundThPower_mW = stGoldenForcePowerAnalyzeOutput.f1stBondPmW 
		- stFOPEM_CheckingThreshold.dPowerTh_mW;
	if(stForcePowerRollingHorizan.dLowerBoundThPower_mW < 0) stForcePowerRollingHorizan.dLowerBoundThPower_mW = 0;
}

void FOPEM_convert_load_data_to_adc_capture()
{
	int ii, nDataLen, dd;
	nDataLen = FOPEM_get_read_data_length();
	dd = 0;
	for(ii = 0; ii< nDataLen; ii++)
	{
		aiData[dd++] = aiLoadWaveformForce[ii];
		aiData[dd++] = aiLoadWaveformVel[ii];
		aiData[dd++] = aiLoadWaveformPowerUS[ii];
		aiData[dd++] = aiLoadWaveformPowerV[ii];
		aiData[dd++] = aiLoadWaveformPowerI[ii];
		aiData[dd++] = aiLoadWaveformEFO[ii];
	}

	memset(&aiData[dd], 0, sizeof(int) * (65535 - dd));
}
// 2013May6
// from nOffsetBondStart to nLen
//		aiBakData[ii * 6 + SCOPE_OFST_EFO_], aiBakData[ii * 6 + SCOPE_OFST_FORCE_], aiBakData[ii * 6 + SCOPE_OFST_VEL_], 
//		aiBakData[ii * 6 + SCOPE_OFST_POWER_1_],aiBakData[ii * 6 + SCOPE_OFST_POWER_2_],aiBakData[ii * 6 + SCOPE_OFST_POWER_US_]);

void FOPEM_convert_adc_capture_to_load_data()
{
	int ii, dd;
static int idxChannel, idxAllData;
//	nDataLen = FOPEM_get_read_data_length();
	dd = nOffsetBondStart * 6;
	for(ii = nOffsetBondStart; ii< nLen; ii++)
	{
		idxChannel = ii - nOffsetBondStart;
		idxAllData = ii * 6;
		aiLoadWaveformForce[idxChannel] = aiData[idxAllData + SCOPE_OFST_FORCE_];
		aiLoadWaveformVel[idxChannel]   = aiData[idxAllData + SCOPE_OFST_VEL_];
		aiLoadWaveformPowerUS[idxChannel] = aiData[idxAllData + SCOPE_OFST_POWER_1_];
		aiLoadWaveformPowerV[idxChannel]  = aiData[idxAllData + SCOPE_OFST_POWER_2_];
		aiLoadWaveformPowerI[idxChannel]  = aiData[idxAllData + SCOPE_OFST_POWER_US_];
		aiLoadWaveformEFO[idxChannel] = aiData[idxAllData + SCOPE_OFST_EFO_];
	}

	// set nReadLen
	nReadLen = nLen - nOffsetBondStart;

}

#define _FPMD_FILE_FULL_NAME_  ("D:\\FPMD_Data\\FPMD_Config.ini")
#define _FPMD_STRING_BUFFER_SIZE   (1024)
static char strTempReadFile[_FPMD_STRING_BUFFER_SIZE];
static char *strStopString;

int _FOPEM_load_power_config_from_file(FILE *fptr, FOPEM_CONFIG_POWER_FACTOR *stpPowerConfig)
{
	int iRet = RET_OK_ZERO;

	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	stpPowerConfig->fFactorK_I = strtod(strTempReadFile, &strStopString);

	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	stpPowerConfig->fFactorK_V= strtod(strTempReadFile, &strStopString);

	return iRet;
}

int _FOPEM_load_force_config_from_file(FILE *fptr,  FOPEM_CONFIG_FORCE_FACTOR *stpForceConfig)
{
	int iRet = RET_OK_ZERO;

	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	stpForceConfig->fFactorForceRatio_GramPerVolt = strtod(strTempReadFile, &strStopString);

	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	stpForceConfig->fFactorForceRatio_OffsetZeroForce = strtod(strTempReadFile, &strStopString);

	return iRet;
}
// @3
int _FOPEM_load_offset_force_power_from_file(FILE *fptr, FOPEM_OFFSET_FORCE_POWER *stpSysOffsetForcePower)
{
int iRet = RET_OK_ZERO;
int iReadTemp = 0;
	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	sscanf_s(strTempReadFile, "%d", &iReadTemp);
	if(iReadTemp > MAXIMUM_FORCE_OFFSET_GRAM)
	{
		stpSysOffsetForcePower->iForceOffset_gram = MAXIMUM_FORCE_OFFSET_GRAM;
	}
	else if(iReadTemp < MINIMUM_FORCE_OFFSET_GRAM)
	{
		stpSysOffsetForcePower->iForceOffset_gram = MINIMUM_FORCE_OFFSET_GRAM;
	}
	else
	{
		stpSysOffsetForcePower->iForceOffset_gram = iReadTemp;
	}

	iReadTemp = 0;
	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	sscanf_s(strTempReadFile, "%d", &iReadTemp);
	if(iReadTemp > MAXIMUM_POWER_OFFSET_MW)
	{
		stpSysOffsetForcePower->iPowerOffset_mW = MAXIMUM_POWER_OFFSET_MW;
	}
	else if(iReadTemp < MINIMUM_POWER_OFFSET_MW)
	{
		stpSysOffsetForcePower->iPowerOffset_mW = MINIMUM_POWER_OFFSET_MW;
	}
	else
	{
		stpSysOffsetForcePower->iPowerOffset_mW = iReadTemp;
	}
	iFPMD_ForceDisplayOffset = stSysOffsetForcePower.iForceOffset_gram;
	iFPMD_PowerDisplayOffset = stSysOffsetForcePower.iPowerOffset_mW;

	return iRet;
}

int _FOPEM_load_threshold_force_power_from_file(FILE *fptr, FOPEM_DETECT_THRESHOLD *stpCheckingThreshold)
{
	int iRet = RET_OK_ZERO;
	double dTempRead;
	int ii;

	//// Force Checking Threshold
	dTempRead = stpCheckingThreshold->dForceTh_gram;
	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	dTempRead = strtod(strTempReadFile, &strStopString);
	if(dTempRead < adCheckingForceThreshold[0])
	{
		stpCheckingThreshold->dForceTh_gram = adCheckingForceThreshold[0];
		stpCheckingThreshold->idxForceThComboSel = 0;
		FOPEM_set_flag_config_setting_changed();
	}
	else if(dTempRead > adCheckingForceThreshold[TOTAL_LEVEL_DETECTION_FORCE_TH - 1])
	{
		dTempRead = adCheckingForceThreshold[TOTAL_LEVEL_DETECTION_FORCE_TH - 1];
		stpCheckingThreshold->dForceTh_gram = adCheckingForceThreshold[TOTAL_LEVEL_DETECTION_FORCE_TH - 1];
		stpCheckingThreshold->idxForceThComboSel = TOTAL_LEVEL_DETECTION_FORCE_TH - 1;
		FOPEM_set_flag_config_setting_changed();
	}
	else
	{
		for(ii = TOTAL_LEVEL_DETECTION_FORCE_TH - 1; ii>0; ii--)
		{
			if( dTempRead <= adCheckingForceThreshold[ii] && dTempRead > adCheckingForceThreshold[ii - 1])
			{
				dTempRead = adCheckingForceThreshold[ii];
				stpCheckingThreshold->dForceTh_gram = adCheckingForceThreshold[ii];
				stpCheckingThreshold->idxForceThComboSel = ii;
				break;
			}
		}
	}

	//// Power checking threshold
	dTempRead = stpCheckingThreshold->dPowerTh_mW;
	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	dTempRead = strtod(strTempReadFile, &strStopString);
	if(dTempRead < adCheckingPowerThreshold[0])
	{
		stpCheckingThreshold->dPowerTh_mW = adCheckingPowerThreshold[0];
		stpCheckingThreshold->idxPowerThComboSel = 0;
		FOPEM_set_flag_config_setting_changed();
	}
	else if(dTempRead > adCheckingPowerThreshold[TOTAL_LEVEL_DETECTION_POWER_TH - 1])
	{
		dTempRead = adCheckingPowerThreshold[TOTAL_LEVEL_DETECTION_POWER_TH - 1];
		stpCheckingThreshold->dPowerTh_mW = adCheckingPowerThreshold[TOTAL_LEVEL_DETECTION_POWER_TH - 1];
		stpCheckingThreshold->idxPowerThComboSel = TOTAL_LEVEL_DETECTION_POWER_TH - 1;
		FOPEM_set_flag_config_setting_changed();
	}
	else
	{
		for(ii = TOTAL_LEVEL_DETECTION_POWER_TH - 1; ii>0; ii--)
		{
			if( dTempRead <= adCheckingPowerThreshold[ii] && dTempRead > adCheckingPowerThreshold[ii - 1])
			{
				dTempRead = adCheckingPowerThreshold[ii];
				stpCheckingThreshold->dPowerTh_mW = adCheckingPowerThreshold[ii];
				stpCheckingThreshold->idxPowerThComboSel = ii;
				break;
			}
		}
	}

	//// Impedance checking threshold
	return iRet;
}

int _FOPEM_load_adc_config_from_file(FILE *fptr, SYS_SETTING_ADC *stpAdcConfig)
{
int iRet = RET_OK_ZERO;
int iReadTemp;
	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	sscanf_s(strTempReadFile, "%d", &iReadTemp);
	if(iReadTemp < 200)	{ iReadTemp = 200; }
	stpAdcConfig->tPeriodSample_us = (double)iReadTemp;

	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	sscanf_s(strTempReadFile, "%d", &(stpAdcConfig->ucSampleVoltMode));

	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	sscanf_s(strTempReadFile, "%d", &(stpAdcConfig->nSampleLen));

	fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
	sscanf_s(strTempReadFile, "%d", &(stpAdcConfig->nDisplayLen));
	return iRet;
}

int FOPEM_load_config_from_file()
{
	int iRet = RET_OK_ZERO;

//	int iBlkId;
char strTemp[256];
	FILE *fptr;
	sprintf_s(strTemp, 256, "%s", _FPMD_FILE_FULL_NAME_);
	fopen_s(&fptr, strTemp, "r"); // stpServoAxis_ACS->strAxisServoParaFilename, "r");

	if(fptr != NULL)
	{
		while(!feof(fptr))
		{
			fgets(strTempReadFile, _FPMD_STRING_BUFFER_SIZE, fptr); 
			if(strstr(strTempReadFile, "[POWER_CFG]") != NULL)
			{
				_FOPEM_load_power_config_from_file(fptr, &stForcePowerSetting.stCfgPowerFactor);
			}
			else if(strstr(strTempReadFile, "[DUMMY_BOND_FORCE_CFG]") != NULL)
			{
				_FOPEM_load_force_config_from_file(fptr, &stForcePowerSetting.stCfgDummyBondForceFactor);
			}
			else if(strstr(strTempReadFile, "[AUTO_BOND_FORCE_CFG]") != NULL)
			{
				_FOPEM_load_force_config_from_file(fptr, &stForcePowerSetting.stCfgActualBondForceFactor);
			}
			else if(strstr( strTempReadFile, "[ADC_CONFIG]") != NULL)
			{
				_FOPEM_load_adc_config_from_file(fptr, &stSysSettingAdc);
			}
			else if(strstr( strTempReadFile, "[OFFSET_F_P]") != NULL)
			{
				_FOPEM_load_offset_force_power_from_file(fptr, &stSysOffsetForcePower);
			}
			else if(strstr( strTempReadFile, "[THRESHOLD_F_P_I]") != NULL)
			{
				_FOPEM_load_threshold_force_power_from_file(fptr, &stFOPEM_CheckingThreshold);
			}
			else
			{
			}
		}

		fclose(fptr);
	}
	return iRet;
}

int FOPEM_save_config_to_file() // @3
{
	int iRet = RET_OK_ZERO;

//	int iBlkId;
char strTemp[256];
	FILE *fptr;
	sprintf_s(strTemp, 256, "%s", _FPMD_FILE_FULL_NAME_);
	fopen_s(&fptr, strTemp, "w"); 

	if(fptr != NULL)
	{
		////// 1.
		fprintf_s(fptr, "[POWER_CFG]\n");
		fprintf_s(fptr, "%12.8f\n", stForcePowerSetting.stCfgPowerFactor.fFactorK_I);
		fprintf_s(fptr, "%12.8f\n", stForcePowerSetting.stCfgPowerFactor.fFactorK_V);
		fprintf_s(fptr, "\n");

		////// 2.
		fprintf_s(fptr, "[DUMMY_BOND_FORCE_CFG]\n");
		fprintf_s(fptr, "%12.8f\n", stForcePowerSetting.stCfgDummyBondForceFactor.fFactorForceRatio_GramPerVolt);
		fprintf_s(fptr, "%12.8f\n", stForcePowerSetting.stCfgDummyBondForceFactor.fFactorForceRatio_OffsetZeroForce);
		fprintf_s(fptr, "\n");

		////// 3.
		fprintf_s(fptr, "[AUTO_BOND_FORCE_CFG]\n");
		fprintf_s(fptr, "%12.8f\n", stForcePowerSetting.stCfgActualBondForceFactor.fFactorForceRatio_GramPerVolt);
		fprintf_s(fptr, "%12.8f\n", stForcePowerSetting.stCfgActualBondForceFactor.fFactorForceRatio_OffsetZeroForce);
		fprintf_s(fptr, "\n");

		////// 4.
		fprintf_s(fptr, "[ADC_CONFIG]\n");
		fprintf_s(fptr, "%5.0f\n", stSysSettingAdc.tPeriodSample_us); // @3 bug fix
		fprintf_s(fptr, "%d\n", stSysSettingAdc.ucSampleVoltMode);
		fprintf_s(fptr, "%d\n", stSysSettingAdc.nSampleLen);
		fprintf_s(fptr, "%d\n", stSysSettingAdc.nDisplayLen);
		fprintf_s(fptr, "\n");

		////// 5.
		fprintf_s(fptr, "[OFFSET_F_P]\n");
		fprintf_s(fptr, "%d\n", stSysOffsetForcePower.iForceOffset_gram);
		fprintf_s(fptr, "%d\n", stSysOffsetForcePower.iPowerOffset_mW);
		fprintf_s(fptr, "\n");

		////// 6.
		fprintf_s(fptr, "[THRESHOLD_F_P_I]\n");
		fprintf_s(fptr, "%4.1f\n", stFOPEM_CheckingThreshold.dForceTh_gram);
		fprintf_s(fptr, "%4.1f\n", stFOPEM_CheckingThreshold.dPowerTh_mW);
		fprintf_s(fptr, "%4.1f\n", stFOPEM_CheckingThreshold.dImpedanceTh_ohm);
		fprintf_s(fptr, "\n");

		fclose(fptr);
	}
	else
	{
		iRet = RET_ERROR_OPEN_FILE;
	}
	return iRet;
}

char cFlagConfigSettingChanged = RET_ZERO_CONFIG_SETTING_NOT_CHANGE;
char FOPEM_get_flag_config_setting_changed()
{
	return cFlagConfigSettingChanged;
}
void FOPEM_set_flag_config_setting_changed()
{
	cFlagConfigSettingChanged = RET_CONFIG_SETTING_CHANGED;
}

extern FOPEM_FORCE_POWER_VALUE_SAVE_FILE astForcePowerValueCalibrate[DEF_CAPTURE_DATA_STAGE_5];
char strFoldernameCaptureDataCalibration[512];
char strFileFullnameCaptureDataCalibration[1024];
void FOPEM_save_calibration_capture_waveform()
{
	FILE *fptr;
//	int ii;
	int iPointCalibrate = FOPEM_dlg_get_capture_data_stage() - 1;

	if(iPointCalibrate >= DEF_CAPTURE_DATA_STAGE_5) return;  // @2 protection not to overflow

	sprintf_s(strFileFullnameCaptureDataCalibration, 1024, "D:\\FPMD_Calibrate\\%s\\FPMD_Calibrate_Point%d_Sample%d.txt",
		strFoldernameCaptureDataCalibration,
		iPointCalibrate,
		FOPEM_dlg_get_curr_num_file_capture_calibrate());

	fopen_s(&fptr, strFileFullnameCaptureDataCalibration, "w");

	if(cFlagSystemOfflineMode == TRUE)
	{
		extern double dGoldenWaveformImpedance;
		extern unsigned long ulNumLoadedWaveform;

//		astForcePowerValueCalibrate[iPointCalibrate].dForceCmd_gram = stLoadForcePowerAnalyzeOutput.f1stBondForceGram;
//		astForcePowerValueCalibrate[iPointCalibrate].dPowerCmd_Dac = FOPEM_calc_dac_by_power_with_imp_WB_A_ultra_low(stLoadForcePowerAnalyzeOutput.f1stBondPmW, 
//				dGoldenWaveformImpedance);
		ulNumLoadedWaveform = ulNumLoadedWaveform + 1;
	}
	else
	{
	}

	FOPEM_save_waveform(fptr, &astForcePowerValueCalibrate[iPointCalibrate]);
}


extern FOPEM_FORCE_POWER_VALUE_SAVE_FILE stForcePowerValueChecking;
char strFoldernameCheckingMachineDriftFOPEM[512];
char strFileFullnameCheckingMachineDriftFOPEM[1024];
void FOPEM_save_checking_waveform()
{
	FILE *fptr;
//	int ii; // FPMD_CheckDrift

	sprintf_s(strFileFullnameCheckingMachineDriftFOPEM, 1024, "D:\\FPMD_CheckDrift\\%s\\FPMD_Checking_sample_%d.txt",
		strFoldernameCheckingMachineDriftFOPEM, FOPEM_dlg_get_curr_num_file_capture_calibrate());
	fopen_s(&fptr, strFileFullnameCheckingMachineDriftFOPEM, "w");

	if(cFlagSystemOfflineMode == TRUE)
	{
		extern double dGoldenWaveformImpedance;
		extern unsigned long ulNumLoadedWaveform;
		stForcePowerValueChecking.dForceCmd_gram = stLoadForcePowerAnalyzeOutput.f1stBondForceGram;
		stForcePowerValueChecking.dPowerCmd_Dac = FOPEM_calc_dac_by_power_with_imp_WB_A_ultra_low(stLoadForcePowerAnalyzeOutput.f1stBondPmW, 
				dGoldenWaveformImpedance);
		ulNumLoadedWaveform = ulNumLoadedWaveform + 1;
	}
	else
	{
	}

	FOPEM_save_waveform(fptr, &stForcePowerValueChecking);
}

void FOPEM_save_waveform(FILE *fptr, FOPEM_FORCE_POWER_VALUE_SAVE_FILE *stpForcePowerSaveFile)
{
	int ii;
	if(fptr != NULL)
	{
		fprintf_s(fptr, "Force_gram = %5.0f;\n", stpForcePowerSaveFile->dForceCmd_gram);
		fprintf_s(fptr, "strPowerLevel = %s;\n", pstrPowerLevel[stpForcePowerSaveFile->iPowerLevel]);
		fprintf_s(fptr, "strPowerMode = %s;\n", pstrPowerMode[stpForcePowerSaveFile->iPowerMode]);
		fprintf_s(fptr, "Power_Dac = %5.0f;\n", stpForcePowerSaveFile->dPowerCmd_Dac);
		fprintf_s(fptr, "tSampleFer_us = %5.0f;\n\n", stpForcePowerSaveFile->dSampleTime_Fer_us);
		fprintf_s(fptr, "aWbDataOFPM = [");
		for(ii=0; ii<nLen - nOffsetBondStart; ii++)
		{
			fprintf_s(fptr, "%d, %d, %d, %d, %d, %d; \n", 
				aiBakData[ii * 6 + SCOPE_OFST_EFO_], aiBakData[ii * 6 + SCOPE_OFST_FORCE_], aiBakData[ii * 6 + SCOPE_OFST_VEL_], 
				aiBakData[ii * 6 + SCOPE_OFST_POWER_1_],aiBakData[ii * 6 + SCOPE_OFST_POWER_2_],aiBakData[ii * 6 + SCOPE_OFST_POWER_US_]);

		}
		fprintf_s(fptr, "];\n");
		fclose(fptr);
	}
}

// based on moving_average
void FOPEM_calculate_online_force()
{

}

void FOPEM_calculate_online_power()
{

}

extern char FOPEM_get_flag_capture();
extern void FOPEM_clear_flag_capture_trigger();

char strReadLine[512];
#define _FILE_FOPEM_GOLDEN_WAVEFORM_  "D:\\FPMD_Data\\E60_FOPEM_GoldenWaveform.txt"
int FOPEM_load_golden_waveform()
{
	
static FOPEM_WAVEFORM_INT stGoldenWaveformInt;

	stGoldenWaveformInt.piEFO =	&aiGoldenWaveformEFO[0];
	stGoldenWaveformInt.piForce = &aiGoldenWaveformForce[0];
	stGoldenWaveformInt.piVel = &aiGoldenWaveformVel[0];
	stGoldenWaveformInt.piPowerUS = &aiGoldenWaveformPowerUS[0];
	stGoldenWaveformInt.piPowerV = &aiGoldenWaveformPowerV[0];
	stGoldenWaveformInt.piPowerI = &aiGoldenWaveformPowerI[0];

	int iRet = FOPEM_load_waveform(_FILE_FOPEM_GOLDEN_WAVEFORM_, &stGoldenWaveformInt);

	stGoldenWaveformAdcSetting.tPeriodSample_us = stGoldenWaveformInt.tSampleFer_us;

	return iRet;
}

int FOPEM_load_waveform(char *strFileFullName, FOPEM_WAVEFORM_INT *stpWaveformInt)
{
	FILE *fptrGoldenWaveform;
	int iRet;
	char *pRetStr = NULL;
	char *pStrVarName = "aWbDataOFPM";
	char *pStrVarSampleFer = "tSampleFer_us";
	char *pStrMatrixEnd = "];";
	int iSampleFer_us = (int)(DEF_SAMPLE_FER_US);
//// aWbDataOFPM = [
static int *aiEFO, *aiVel, *aiForce, *aiPowerUS, *aiPowerI, *aiPowerV;
	aiEFO = stpWaveformInt->piEFO;
	aiVel = stpWaveformInt->piVel;
	aiForce = stpWaveformInt->piForce;
	aiPowerUS = stpWaveformInt->piPowerUS;
	aiPowerI = stpWaveformInt->piPowerI;
	aiPowerV = stpWaveformInt->piPowerV;

	iRet = RET_OK_ZERO;
	nReadLen = 0;
	fopen_s(&fptrGoldenWaveform, strFileFullName, "r");
	if(fptrGoldenWaveform != NULL)
	{
		fgets(strReadLine, 512, fptrGoldenWaveform);
		pRetStr = strstr(strReadLine, pStrVarName);
		while(pRetStr == NULL && feof(fptrGoldenWaveform) == FALSE)
		{
			fgets(strReadLine, 512, fptrGoldenWaveform);
			if(strstr(strReadLine, pStrVarSampleFer) > 0)
			{
				sscanf_s(strReadLine, "tSampleFer_us = %d", &iSampleFer_us);
				if(iSampleFer_us > 0)	stpWaveformInt->tSampleFer_us = iSampleFer_us;
			}
			pRetStr = strstr(strReadLine, pStrVarName);
		}

		if(feof(fptrGoldenWaveform) == TRUE)
		{
			fclose(fptrGoldenWaveform);
			iRet = RET_ERROR_FIND_VALID_DATA;
		}
		else
		{
			sscanf_s(strReadLine, "aWbDataOFPM = [%d, %d, %d, %d, %d, %d;", 
				&aiEFO[nReadLen],
				&aiForce[nReadLen], &aiVel[nReadLen],
				&aiPowerUS[nReadLen], &aiPowerV[nReadLen], &aiPowerI[nReadLen]);
			nReadLen ++;

			fgets(strReadLine, 512, fptrGoldenWaveform);
			pRetStr = strstr(strReadLine, pStrMatrixEnd);
			while(pRetStr == NULL && feof(fptrGoldenWaveform) == FALSE)
			{
				sscanf_s(strReadLine, "%d, %d, %d, %d, %d, %d;", 
					&aiEFO[nReadLen],
					&aiForce[nReadLen], &aiVel[nReadLen],
					&aiPowerUS[nReadLen], &aiPowerV[nReadLen], &aiPowerI[nReadLen]);
				nReadLen ++;
				fgets(strReadLine, 512, fptrGoldenWaveform);
				pRetStr = strstr(strReadLine, pStrMatrixEnd);
			}
			fclose(fptrGoldenWaveform);

			if(nReadLen == 0) iRet = RET_ERROR_FIND_VALID_DATA;
		}
	}
	else
	{
		iRet = RET_ERROR_OPEN_FILE;
	}
	return iRet;
}

void FOPEM_convert_read_adc_to_volt(CONVERT_FROM_ADC_TO_VOLT *stpConvert)
{
	int ii, nDataLen;
static double *adEFO, *adVel, *adForce, *adPowerUS, *adPowerI, *adPowerV;
static int *aiEFO, *aiVel, *aiForce, *aiPowerUS, *aiPowerI, *aiPowerV;

	adEFO = stpConvert->pdEFO;
	adVel = stpConvert->pdVel;
	adForce = stpConvert->pdForce;
	adPowerUS = stpConvert->pdPowerUS;
	adPowerI = stpConvert->pdPowerI;
	adPowerV = stpConvert->pdPowerV;

	nDataLen = stpConvert->nDataLen;

	aiEFO = stpConvert->piEFO;
	aiVel = stpConvert->piVel;
	aiForce = stpConvert->piForce;
	aiPowerUS = stpConvert->piPowerUS;
	aiPowerI = stpConvert->piPowerI;
	aiPowerV = stpConvert->piPowerV;

	for(ii = 0; ii<nDataLen; ii++)
	{
		if(stSysSettingAdc.ucSampleVoltMode == 2)
		{
			adEFO[ii] = FOPEM_CONVERT_ADC_TO_PN5VOLT(aiEFO[ii]);
			adForce[ii] = FOPEM_CONVERT_ADC_TO_PN5VOLT(aiForce[ii]);
			adVel[ii] = FOPEM_CONVERT_ADC_TO_PN5VOLT(aiVel[ii]);
			adPowerUS[ii] = FOPEM_CONVERT_ADC_TO_PN5VOLT(aiPowerUS[ii]);
			adPowerI[ii] = FOPEM_CONVERT_ADC_TO_PN5VOLT(aiPowerI[ii]);
			adPowerV[ii] = FOPEM_CONVERT_ADC_TO_PN5VOLT(aiPowerV[ii]);
			
		}
		else if(stSysSettingAdc.ucSampleVoltMode == 1)
		{
			adEFO[ii] = FOPEM_CONVERT_ADC_TO_5VOLT(aiEFO[ii]);
			adForce[ii] = FOPEM_CONVERT_ADC_TO_5VOLT(aiForce[ii]);
			adVel[ii] = FOPEM_CONVERT_ADC_TO_5VOLT(aiVel[ii]);
			adPowerUS[ii] = FOPEM_CONVERT_ADC_TO_5VOLT(aiPowerUS[ii]);
			adPowerI[ii] = FOPEM_CONVERT_ADC_TO_5VOLT(aiPowerI[ii]);
			adPowerV[ii] = FOPEM_CONVERT_ADC_TO_5VOLT(aiPowerV[ii]);
		}
		else //if(stSysSettingAdc.ucSampleVoltMode == 0)
		{
			adEFO[ii] = FOPEM_CONVERT_ADC_TO_2d5VOLT(aiEFO[ii]);
			adForce[ii] = FOPEM_CONVERT_ADC_TO_2d5VOLT(aiForce[ii]);
			adVel[ii] = FOPEM_CONVERT_ADC_TO_2d5VOLT(aiVel[ii]);
			adPowerUS[ii] = FOPEM_CONVERT_ADC_TO_2d5VOLT(aiPowerUS[ii]);
			adPowerI[ii] = FOPEM_CONVERT_ADC_TO_2d5VOLT(aiPowerI[ii]);
			adPowerV[ii] = FOPEM_CONVERT_ADC_TO_2d5VOLT(aiPowerV[ii]);
		}
	}
}

void FOPEM_convert_golden_waveform_adc_to_volt()
{
static CONVERT_FROM_ADC_TO_VOLT stConvertGoldenWaveform;
	stConvertGoldenWaveform.nDataLen = FOPEM_get_read_data_length();
	stConvertGoldenWaveform.pdEFO = adGoldenWaveformEFO;
	stConvertGoldenWaveform.pdForce = adGoldenWaveformForce;
	stConvertGoldenWaveform.pdPowerI = adGoldenWaveformPowerI;
	stConvertGoldenWaveform.pdPowerUS = adGoldenWaveformPowerUS;
	stConvertGoldenWaveform.pdPowerV = adGoldenWaveformPowerV;
	stConvertGoldenWaveform.pdVel = adGoldenWaveformVel;

	stConvertGoldenWaveform.piEFO = aiGoldenWaveformEFO;
	stConvertGoldenWaveform.piForce = aiGoldenWaveformForce;
	stConvertGoldenWaveform.piPowerI = aiGoldenWaveformPowerI;
	stConvertGoldenWaveform.piPowerUS = aiGoldenWaveformPowerUS;
	stConvertGoldenWaveform.piPowerV = aiGoldenWaveformPowerV;
	stConvertGoldenWaveform.piVel = aiGoldenWaveformVel;

	FOPEM_convert_read_adc_to_volt(&stConvertGoldenWaveform);
}

void FOPEM_convert_load_waveform_adc_to_volt()
{
static CONVERT_FROM_ADC_TO_VOLT stConvertLoadWaveform;
	stConvertLoadWaveform.nDataLen = FOPEM_get_read_data_length();
	stConvertLoadWaveform.pdEFO = adLoadWaveformEFO;
	stConvertLoadWaveform.pdForce = adLoadWaveformForce;
	stConvertLoadWaveform.pdPowerI = adLoadWaveformPowerI;
	stConvertLoadWaveform.pdPowerUS = adLoadWaveformPowerUS;
	stConvertLoadWaveform.pdPowerV = adLoadWaveformPowerV;
	stConvertLoadWaveform.pdVel = adLoadWaveformVel;

	stConvertLoadWaveform.piEFO = aiLoadWaveformEFO;
	stConvertLoadWaveform.piForce = aiLoadWaveformForce;
	stConvertLoadWaveform.piPowerI = aiLoadWaveformPowerI;
	stConvertLoadWaveform.piPowerUS = aiLoadWaveformPowerUS;
	stConvertLoadWaveform.piPowerV = aiLoadWaveformPowerV;
	stConvertLoadWaveform.piVel = aiLoadWaveformVel;

	FOPEM_convert_read_adc_to_volt(&stConvertLoadWaveform);
}

