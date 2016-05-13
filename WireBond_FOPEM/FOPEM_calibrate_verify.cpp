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

#include "stdafx.h"
//#include "Dmp609Dll.h"//必须包含的DLL文件

#include "EFS_DisplayCfg.h"

FOPEM_FORCE_POWER_VALUE_SAVE_FILE astForcePowerValueCalibrate[DEF_CAPTURE_DATA_STAGE_5];
FOPEM_FORCE_POWER_VALUE_SAVE_FILE stForcePowerValueChecking;
extern SYS_SETTING_ADC stSysSettingAdc;
extern double adCaptureForceCmd_gram[];
extern double adCaptureForceFb_volt[];
extern double adCapturePowerI_amp[];
extern double adCapturePowerV_voltUS[];
extern double adCapturePowerI_volt[];
extern double adCapturePowerV_volt[];
extern double adCapturePower_mW[];
extern double adCapturePower_Dac[];

void FOPEM_init_calibrate_checking()
{
	astForcePowerValueCalibrate[0].dForceCmd_gram = 25;
	astForcePowerValueCalibrate[0].dPowerCmd_Dac = 50;

	astForcePowerValueCalibrate[1].dForceCmd_gram = 100;
	astForcePowerValueCalibrate[1].dPowerCmd_Dac = 50;

	astForcePowerValueCalibrate[2].dForceCmd_gram = 200;
	astForcePowerValueCalibrate[2].dPowerCmd_Dac = 50;

	astForcePowerValueCalibrate[3].dForceCmd_gram = 100;
	astForcePowerValueCalibrate[3].dPowerCmd_Dac = 100;

	astForcePowerValueCalibrate[4].dForceCmd_gram = 100;
	astForcePowerValueCalibrate[4].dPowerCmd_Dac = 150;

	int ii;
	for(ii = 0; ii<DEF_CAPTURE_DATA_STAGE_5; ii++)
	{
		astForcePowerValueCalibrate[ii].dSampleTime_Fer_us = stSysSettingAdc.tPeriodSample_us;
		astForcePowerValueCalibrate[ii].iPowerMode = WB_MACHINE_POWER_MODE_CONST_I;
		astForcePowerValueCalibrate[ii].iPowerLevel = WB_MACHINE_POWER_LEVEL_ULTRA_LOW;
	}

	stForcePowerValueChecking.dForceCmd_gram = 100;
	stForcePowerValueChecking.dPowerCmd_Dac = 100;
	stForcePowerValueChecking.dSampleTime_Fer_us = stSysSettingAdc.tPeriodSample_us;
	stForcePowerValueChecking.iPowerLevel = WB_MACHINE_POWER_LEVEL_ULTRA_LOW;
	stForcePowerValueChecking.iPowerMode = WB_MACHINE_POWER_MODE_CONST_I;

	FOPEM_reset_calibrate_checking();
}

void FOPEM_reset_calibrate_checking()
{
	int ii;
	for(ii = 0; ii<MAX_NUM_SAMPLE_CALIBRATION; ii++)
	{
		adCaptureForceCmd_gram[ii] = 0;
		adCaptureForceFb_volt[ii] = 0;
	}
}

extern int brinv(double a[],int n);
extern void brmul(double *a, double *b, int m, int n, int k, double *c);

#define MATRIX_COL_2       2
#define MATRIX_ROW_2       2
#define MATRIX_LEN         (MATRIX_COL_2 * MATRIX_ROW_2)

int FOPEM_calibrate_linear_fit_force(FOPEM_CONFIG_FORCE_FACTOR *stpForceFactor, FILE * fpData)
{
int iRet = RET_OK_ZERO;
double *dMatrixSq; // 2 by 2 matrix
double *dRHS, *dResult;      // 2
int ii;

	dMatrixSq = (double*)calloc(MATRIX_LEN, sizeof(double));
	dRHS = (double*)calloc(MATRIX_COL_2, sizeof(double));
	dResult = (double*)calloc(MATRIX_COL_2, sizeof(double));

	for(ii = 0; ii<MAX_NUM_SAMPLE_CALIBRATION; ii++)
	{
		dMatrixSq[0 * MATRIX_COL_2 + 0] += adCaptureForceFb_volt[ii] * adCaptureForceFb_volt[ii];
		dMatrixSq[0 * MATRIX_COL_2 + 1] += adCaptureForceFb_volt[ii];
		dRHS[0] += adCaptureForceFb_volt[ii] * adCaptureForceCmd_gram[ii];
		dRHS[1] += adCaptureForceCmd_gram[ii];
	}
	dMatrixSq[1 * MATRIX_COL_2 + 0] = dMatrixSq[0 * MATRIX_COL_2 + 1];
	dMatrixSq[1 * MATRIX_COL_2 + 1] = MAX_NUM_SAMPLE_CALIBRATION;
	if(brinv(dMatrixSq, MATRIX_COL_2) == 0)
	{
		dResult[0] = 0;
		dResult[1] = 0;
		iRet = RET_ERROR_CALCULATION;
	}
	else
	{
		brmul(dMatrixSq, dRHS, MATRIX_COL_2, MATRIX_ROW_2, 1, dResult);
		stpForceFactor->fFactorForceRatio_GramPerVolt = dResult[0];
		stpForceFactor->fFactorForceRatio_OffsetZeroForce = dResult[1];

		if(fpData != NULL)
		{
			fprintf(fpData, "\n%% Calculation By C Library\n");
			fprintf(fpData, "ForceFactorByC = %10.6f\n", dResult[0]);
			fprintf(fpData, "OffsetByC = %10.6f\n", dResult[1]);
		}

		iRet = RET_OK_ZERO;
	}

	free(dMatrixSq);
	free(dRHS);
	free(dResult);


	return iRet;
}