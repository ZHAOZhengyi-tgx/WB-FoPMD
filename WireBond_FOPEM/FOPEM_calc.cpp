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
// @4 20130719  Waveform calculation protection
#include "stdafx.h"
//#include "Dmp609Dll.h"//必须包含的DLL文件

#include "EFS_DisplayCfg.h"
#include "math.h"
//extern HANDLE	m_hDevice;

double dGoldenWaveformImpedance;

int FOPEM_set_golden_state_impedance(double dImp)
{
	int iRet = RET_OK_ZERO;

	if( dImp < DEF_UPPER_LIMIT_GOLD_IMPEDANCE && 
		dImp > DEF_LOWER_LIMIT_GOLD_IMPEDANCE)
		dGoldenWaveformImpedance = dImp;
	else
	{
		dGoldenWaveformImpedance = DEF_XDUCER_GOLD_IMPEDANCE;
		iRet = RET_ERROR_GOLD_IMPEDANCE_OUT_RANGE;
	}

	return iRet;
}

double FOPEM_calc_max(int iFrom, int iTo, double *dArray)
{
	double dRetMax;
	dRetMax = dArray[iFrom];
	int ii;
	for(ii = iFrom + 1; ii< iTo; ii++)
	{
		if (dRetMax < dArray[ii]) dRetMax = dArray[ii];
	}

	return dRetMax;
}

double FOPEM_calc_min(int iFrom, int iTo, double *dArray)
{
	double dRetMin;
	dRetMin = dArray[iFrom];
	int ii;
	for(ii = iFrom + 1; ii< iTo; ii++)
	{
		if (dRetMin > dArray[ii]) dRetMin = dArray[ii];
	}

	return dRetMin;
}

extern double adRollingPower_mW[FIG_LENGTH_X_ROLLING_HORIZAN];
extern double adRollingImpedance_ohm[FIG_LENGTH_X_ROLLING_HORIZAN];
extern double adRollingForce_gram[FIG_LENGTH_X_ROLLING_HORIZAN];

extern double adSmoothedRollingPower_mW[FIG_LENGTH_X_ROLLING_HORIZAN];
extern double adSmoothedRollingImpedance_ohm[FIG_LENGTH_X_ROLLING_HORIZAN];
extern double adSmoothedRollingForce_gram[FIG_LENGTH_X_ROLLING_HORIZAN];
extern unsigned int idxPrevPointRolling, idxNewPointInRolling;

void FOPEM_calc_smooth_force_power_impedance(int iMovAveLen)
// Function dependency variables:
// idxNewPointInRolling
{
	int idxFromPoint = idxNewPointInRolling - iMovAveLen + 1;
	if(idxFromPoint < 0) idxFromPoint = 0;
	adSmoothedRollingPower_mW[idxNewPointInRolling] = FOPEM_calc_mean(idxFromPoint, idxNewPointInRolling, adRollingPower_mW);
	adSmoothedRollingForce_gram[idxNewPointInRolling] = FOPEM_calc_mean(idxFromPoint, idxNewPointInRolling, adRollingForce_gram);
	adSmoothedRollingImpedance_ohm[idxNewPointInRolling] = FOPEM_calc_mean(idxFromPoint, idxNewPointInRolling, adRollingImpedance_ohm);
}

double FOPEM_calc_mean(int iFrom, int iTo, double *dArray)
{
	double dRetAve;
	dRetAve = 0;
	int ii;
	for(ii = iFrom; ii<= iTo; ii++)
	{
		dRetAve = dRetAve + dArray[ii];
	}
	dRetAve = dRetAve/(iTo - iFrom + 1.0);

	return dRetAve;
}

double FOPEM_calc_std_with_ave(int iFrom, int iTo, double dAve, double *adArray)
{
	double dRetStd, dSumSq;
	double dSubtract;
	int ii;
	dSumSq = 0;
	dRetStd = 0;
	for(ii = iFrom; ii <= iTo; ii++)
	{
		dSubtract = (adArray[ii] - dAve);
		dSumSq = dSumSq + dSubtract * dSubtract;
	}

	dRetStd = sqrt(dSumSq/(iTo - iFrom)) ;

	return dRetStd;
    
}

void FOPEM_moving_ave(double *dInput, double *dOutput, int nDataLen, int nAveLen)
{
	int ii, nHalfAveLen;
	double dCalcTempAve;
	nHalfAveLen = (int)nAveLen/2;
	dCalcTempAve = 0;

	for(ii = 0; ii<nDataLen; ii++)
	{
		if(ii < nHalfAveLen)
		{
			dOutput[ii] = dInput[ii];
		}
		else if(ii == nHalfAveLen)
		{
			dCalcTempAve = FOPEM_calc_mean(ii - nHalfAveLen, ii + nHalfAveLen, dInput);
			dOutput[ii] = dCalcTempAve;
		}
		else if(ii>= ( nDataLen - nAveLen))
		{
			dOutput[ii] = dInput[ii];
		}
		else
		{
			dCalcTempAve = dCalcTempAve + (dInput[ii + nHalfAveLen] - dInput[ii - nHalfAveLen - 1])/(nHalfAveLen * 2 + 1.0);
			dOutput[ii] = dCalcTempAve;
		}
	}
}

double FOPEM_calc_dot_prod(double *dArray1, int iFrom1, double *dArray2, int iFrom2, int nLen )
{
	double dRetDotProd;
	dRetDotProd = 0;
	int ii;
	for(ii = 0; ii< nLen; ii++)
	{
		dRetDotProd = dRetDotProd + dArray1[ii + iFrom1] * dArray2[ii + iFrom2];
	}

	return dRetDotProd;
}

double FOPEM_calc_sum_sq(double *dArray, int iFrom, int nLen )
{
	double dRetSumSq, dTemp;
	dRetSumSq = 0;
	int ii;
	for(ii = 0; ii< nLen; ii++)
	{
		dTemp = dArray[ii + iFrom];
		dRetSumSq = dRetSumSq + dTemp * dTemp;
	}

	return dRetSumSq;
}

#define DEFAULT_IMP_ASM_WB_XDUCER  (10.0)
#define DEFAULT_PEAK_POWER_ASM_WB_UL  (400.0)
#define DEFAULT_PEAK_POWER_ASM_WB_L   (1600.0)
// @5, ASM using 3200 mW
#define DEFAULT_PEAK_POWER_ASM_WB_H   (3200.0)
#define DEFAULT_PEAK_I_SQ_ASM_WB_UL   (DEFAULT_PEAK_POWER_ASM_WB_UL / DEFAULT_IMP_ASM_WB_XDUCER)
#define DEFAULT_PEAK_I_SQ_ASM_WB_L    (DEFAULT_PEAK_POWER_ASM_WB_L / DEFAULT_IMP_ASM_WB_XDUCER)
#define DEFAULT_PEAK_I_SQ_ASM_WB_H    (DEFAULT_PEAK_POWER_ASM_WB_H / DEFAULT_IMP_ASM_WB_XDUCER)

int FOPEM_calc_dac_by_power_with_imp_WB_A_ultra_low(double dPower_mW, double dImp)
{
	double dDac;
	int iDac;
	dDac = sqrt(dPower_mW / dImp / DEFAULT_PEAK_I_SQ_ASM_WB_UL) * 255;
	iDac = (int)dDac;
	return iDac;
}
int FOPEM_calc_dac_by_power_with_imp_WB_A_low(double dPower_mW, double dImp)
{
	double dDac;
	int iDac;
	dDac = sqrt(dPower_mW / dImp / DEFAULT_PEAK_I_SQ_ASM_WB_L) * 255;
	iDac = (int)dDac;
	return iDac;
}
int FOPEM_calc_dac_by_power_with_imp_WB_A_high(double dPower_mW, double dImp)
{
	double dDac;
	int iDac;
	dDac = sqrt(dPower_mW / dImp / DEFAULT_PEAK_I_SQ_ASM_WB_H) * 255;
	iDac = (int)dDac;
	return iDac;
}

extern int iFPMD_ForceDisplayOffset;
extern int iFPMD_PowerDisplayOffset;

int FOPEM_calculate_bond_force_power(ANALYZE_FORCE_POWER_OUTPUT *stpForcePowerAnalyzeOutput,
									  ANALYZE_FORCE_POWER_INPUT *stpForcePowerAnalyzeInput)
{
//nLen = length(aEFO);
int iRet = RET_OK_ZERO;

double fSampleTime_ms = stpForcePowerAnalyzeInput->fSampleTime_ms;
unsigned int uiDataLen = stpForcePowerAnalyzeInput->uiDataLen;
unsigned int ii;
FOPEM_BOND_CYCLE_TIME_INDEX *stpTimeIndex;
//iPlotFlag = stOnlineForcePowerSetting.iPlotFlag;

	stpForcePowerAnalyzeOutput->fMean1stPowerI = 0; 
	stpForcePowerAnalyzeOutput->fMean1stPowerV = 0; 
	stpForcePowerAnalyzeOutput->fMean2ndPowerI = 0; 
	stpForcePowerAnalyzeOutput->fMean2ndPowerV = 0; 
	stpForcePowerAnalyzeOutput->f1stBondImp = 0;
	stpForcePowerAnalyzeOutput->f2ndBondImp = 0;
	stpForcePowerAnalyzeOutput->f1stBondPmW = 0;
	stpForcePowerAnalyzeOutput->f2ndBondPmW = 0;
	stpForcePowerAnalyzeOutput->f1stBondForce = 0;
	stpForcePowerAnalyzeOutput->f2ndBondForce = 0;
	stpForcePowerAnalyzeOutput->f1stBondForceGram = 0;
	stpForcePowerAnalyzeOutput->f2ndBondForceGram = 0;
	stpForcePowerAnalyzeOutput->fMeanZeroForce =0;
	stpTimeIndex = &(stpForcePowerAnalyzeOutput->stBondCycleTimeIndex);
	stpForcePowerAnalyzeOutput->iFlagAbnormalWaveform = 0;

static double *aEFO, *aVel, *aPowerV, *aForce, *aPowerI, *aPowerUS;
static double *aFilterVel, *aFilterForce;

	aEFO = stpForcePowerAnalyzeInput->adEFO;
	aVel = stpForcePowerAnalyzeInput->adVel;
	aPowerUS = stpForcePowerAnalyzeInput->adPowerUS;
	aForce = stpForcePowerAnalyzeInput->adForce;
	aPowerI = stpForcePowerAnalyzeInput->adPowerI;
	aPowerV = stpForcePowerAnalyzeInput->adPowerV;
	aFilterVel = stpForcePowerAnalyzeInput->adFilterVel;
	aFilterForce = stpForcePowerAnalyzeInput->adFilterForce;

unsigned int idxStartEFO = -1;
    for( ii = 0; ii < 100; ii++)
	{
		if((aEFO[ii]>= 3) && aEFO[ii + 1] < 3 
			&& aEFO[ii + 2] < 3 )
		{
			idxStartEFO = ii; break;
		}
	}
unsigned int idxEndEFO = -1;
    if(idxStartEFO == -1)
	{
		for( ii = 5; ii < uiDataLen - 4; ii ++)
		{
			if((aEFO[ii]<= 3) && aEFO[ii + 1] > 3 
				&& aEFO[ii + 2] > 3 )
			{
				idxEndEFO = ii; break;
			}
		}
	}
	else
	{
		idxEndEFO = idxStartEFO + 3;
	}

int iFlagDummyBond = 0; // stpForcePowerAnalyzeInput->iFlagDummyBond;
double fStdNoiseVel, fStdNoisePowerV, fStdNoiseForce, fStdNoisePowerI;
double fAveNoiseVel, fAveNoisePowerV, fAveNoiseForce, fAveNoisePowerI;
int idxStartCalc = 0;
	if(idxEndEFO == -1 && FOPEM_get_flag_dummy_bond() == 0)
	{
		idxEndEFO = 10;
		// std(aVel(1:idxEndEFO));
		fAveNoiseVel = FOPEM_calc_mean(0, idxEndEFO, aVel);
		fStdNoiseVel = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoiseVel, aVel) * 2; 

		// std(aPowerV(1:idxEndEFO)) * 3;
		fAveNoisePowerV = FOPEM_calc_mean(0, idxEndEFO, aPowerV);
		fStdNoisePowerV = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoisePowerV, aPowerV) * 2; // @4

		// std(aForce(1:idxEndEFO)) * 3;
		fAveNoiseForce = FOPEM_calc_mean(0, idxEndEFO, aForce);
		fStdNoiseForce = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoiseForce, aForce) * 3; // @4

		// std(aPowerI(1:idxEndEFO)) * 3;
		fAveNoisePowerI = FOPEM_calc_mean(0, idxEndEFO, aPowerI);
		fStdNoisePowerI = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoisePowerI, aPowerI) * 8; // @4

		iRet = RET_ABNORMAL_WAVEFORM_CANNOT_FIND_EFO_TRIGGER;  //// @4
		return iRet;

	}
	//    return;
	else if( idxEndEFO <= 20)
	{
		// std(aVel(1:idxEndEFO));
		fAveNoiseVel = FOPEM_calc_mean(0, idxEndEFO, aVel);
		fStdNoiseVel = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoiseVel, aVel) * 2;  

		// std(aPowerV(1:idxEndEFO)) * 3;
		fAveNoisePowerV = FOPEM_calc_mean(0, idxEndEFO, aPowerV);
		fStdNoisePowerV = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoisePowerV, aPowerV) * 2; // @4

		// std(aForce(1:idxEndEFO)) * 3;
		fAveNoiseForce = FOPEM_calc_mean(0, idxEndEFO, aForce);
		fStdNoiseForce = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoiseForce, aForce) * 3; // @4

		// std(aPowerI(1:idxEndEFO)) * 3;
		fAveNoisePowerI = FOPEM_calc_mean(0, idxEndEFO, aPowerI);
		fStdNoisePowerI = FOPEM_calc_std_with_ave(0, idxEndEFO, fAveNoisePowerI, aPowerI) * 8; // @4
	}
	else
	{
		idxStartCalc = idxEndEFO - 20;
		//fStdNoiseVel = std(aVel((idxEndEFO - 20):idxEndEFO)) * 3;
		fAveNoiseVel = FOPEM_calc_mean(idxStartCalc, idxEndEFO, aVel);
		fStdNoiseVel = FOPEM_calc_std_with_ave(idxStartCalc, idxEndEFO, fAveNoiseVel, aVel); 

		// fStdNoisePowerV = std(aPowerV((idxEndEFO - 20):idxEndEFO)) * 3;
		fAveNoisePowerV = FOPEM_calc_mean(idxStartCalc, idxEndEFO, aPowerV);
		fStdNoisePowerV = FOPEM_calc_std_with_ave(idxStartCalc, idxEndEFO, fAveNoisePowerV, aPowerV) * 2; 

		// fStdNoiseForce = std(aForce((idxEndEFO - 20):idxEndEFO)) * 3;
		fAveNoiseForce = FOPEM_calc_mean(idxStartCalc, idxEndEFO, aForce);
		fStdNoiseForce = FOPEM_calc_std_with_ave(idxStartCalc, idxEndEFO, fAveNoiseForce, aForce) * 3; 

		// fStdNoisePowerI = std(aPowerI((idxEndEFO - 20):idxEndEFO)) * 3;
		fAveNoisePowerI = FOPEM_calc_mean(idxStartCalc, idxEndEFO, aPowerI);
		fStdNoisePowerI = FOPEM_calc_std_with_ave(idxStartCalc, idxEndEFO, fAveNoisePowerI, aPowerI) * 8;

	}
	stpTimeIndex->idxEndEFO = idxEndEFO;


	//nMovingAveLen = 4;
	//aFilterFirB = ones(1,nMovingAveLen)/nMovingAveLen;
	//aFilterVel = filter(aFilterFirB, 1, aVel);
	FOPEM_moving_ave(aVel, aFilterVel, uiDataLen, 4);

	//aFilterForce = filter(aFilterFirB, 1, aForce);
	FOPEM_moving_ave(aForce, aFilterForce, uiDataLen, 4);

int idxEndMove1stSrchHt = -1;
	for( ii = idxEndEFO; ii <= uiDataLen - 5; ii++)
	{
		if(( aFilterVel[ii] < -fStdNoiseVel 
				&& fabs(aFilterVel[ii + 1]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 2]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 4]) <= fStdNoiseVel)
			||
			( aFilterVel[ii] < -fStdNoiseVel 
				&& fabs(aFilterVel[ii + 1]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 2]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 3]) <= fStdNoiseVel)
			||
			( aFilterVel[ii] < -fStdNoiseVel 
				&& fabs(aFilterVel[ii + 4]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 2]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 3]) <= fStdNoiseVel)
			||
			( aFilterVel[ii] < -fStdNoiseVel 
				&& fabs(aFilterVel[ii + 1]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 4]) <= fStdNoiseVel 
				&& fabs(aFilterVel[ii + 3]) <= fStdNoiseVel)
				)
		{
			idxEndMove1stSrchHt = ii; break;
		}
	}
	if(idxEndMove1stSrchHt == -1)
	{
		iRet = RET_ABNORMAL_WAVEFORM_CANNOT_1ST_SEARCH_MOTION_END;  ///// @4
		return iRet;
	}
	stpTimeIndex->idxEndMove1stSrchHt = idxEndMove1stSrchHt;

int idxStart1stBond = -1;
	for( ii = (int)( (idxEndEFO + idxEndMove1stSrchHt)/2); ii <(uiDataLen - 5); ii++) // @4 Prevent abnormal in standby power
	{
		if(
			(aPowerV[ii] < fStdNoisePowerV 
			&& aPowerV[ii+1] >= fStdNoisePowerV 
			&& aPowerV[ii+2] >= fStdNoisePowerV 
			&& aPowerV[ii+3] >= fStdNoisePowerV)
			||
			(aPowerV[ii-1] < fStdNoisePowerV 
			&& aPowerV[ii+1] >= fStdNoisePowerV 
			&& aPowerV[ii+2] >= fStdNoisePowerV 
			&& aPowerV[ii+3] >= fStdNoisePowerV)
			||
			(aPowerV[ii] < fStdNoisePowerV 
			&& aPowerV[ii+1] >= fStdNoisePowerV 
			&& aPowerV[ii+3] >= fStdNoisePowerV 
			&& aPowerV[ii+4] >= fStdNoisePowerV)
			||			
			(aPowerV[ii-1] < fStdNoisePowerV 
			&& aPowerV[ii+1] >= fStdNoisePowerV 
			&& aPowerV[ii+3] >= fStdNoisePowerV 
			&& aPowerV[ii+4] >= fStdNoisePowerV)
			||			
			(aPowerV[ii] < fStdNoisePowerV 
			&& aPowerV[ii+2] >= fStdNoisePowerV 
			&& aPowerV[ii+3] >= fStdNoisePowerV 
			&& aPowerV[ii+4] >= fStdNoisePowerV)
			||			
			(aPowerV[ii-1] < fStdNoisePowerV 
			&& aPowerV[ii+2] >= fStdNoisePowerV 
			&& aPowerV[ii+3] >= fStdNoisePowerV 
			&& aPowerV[ii+4] >= fStdNoisePowerV)
			||			
			(aPowerV[ii] < fStdNoisePowerV 
			&& aPowerV[ii+1] >= fStdNoisePowerV 
			&& aPowerV[ii+2] >= fStdNoisePowerV 
			&& aPowerV[ii+4] >= fStdNoisePowerV)
			||			
			(aPowerI[ii-1] < fStdNoisePowerI 
			&& aPowerI[ii+1] >= fStdNoisePowerI 
			&& aPowerI[ii+2] >= fStdNoisePowerI 
			&& aPowerI[ii+4] >= fStdNoisePowerI)
			||			
			(aPowerI[ii] < fStdNoisePowerI 
			&& aPowerI[ii+1] >= fStdNoisePowerI 
			&& aPowerI[ii+2] >= fStdNoisePowerI 
			&& aPowerI[ii+4] >= fStdNoisePowerI)
			||			
			(aPowerI[ii] < fStdNoisePowerI 
			&& aPowerI[ii+1] >= fStdNoisePowerI 
			&& aPowerI[ii+3] >= fStdNoisePowerI 
			&& aPowerI[ii+4] >= fStdNoisePowerI)
			||			
			(aPowerI[ii] < fStdNoisePowerI 
			&& aPowerI[ii+1] >= fStdNoisePowerI 
			&& aPowerI[ii+2] >= fStdNoisePowerI 
			&& aPowerI[ii+3] >= fStdNoisePowerI)
			||			
			(aPowerI[ii] < fStdNoisePowerI 
			&& aPowerI[ii+3] >= fStdNoisePowerI 
			&& aPowerI[ii+2] >= fStdNoisePowerI 
			&& aPowerI[ii+4] >= fStdNoisePowerI)
			||			
			(aPowerI[ii-1] < fStdNoisePowerI 
			&& aPowerI[ii+1] >= fStdNoisePowerI 
			&& aPowerI[ii+2] >= fStdNoisePowerI 
			&& aPowerI[ii+4] >= fStdNoisePowerI)
			||			
			(aPowerI[ii-1] < fStdNoisePowerI 
			&& aPowerI[ii+1] >= fStdNoisePowerI 
			&& aPowerI[ii+3] >= fStdNoisePowerI 
			&& aPowerI[ii+4] >= fStdNoisePowerI)
			||			
			(aPowerI[ii-1] < fStdNoisePowerI 
			&& aPowerI[ii+1] >= fStdNoisePowerI 
			&& aPowerI[ii+2] >= fStdNoisePowerI 
			&& aPowerI[ii+3] >= fStdNoisePowerI)
			||			
			(aPowerI[ii-1] < fStdNoisePowerI 
			&& aPowerI[ii+3] >= fStdNoisePowerI 
			&& aPowerI[ii+2] >= fStdNoisePowerI 
			&& aPowerI[ii+4] >= fStdNoisePowerI)
			)
		{
			idxStart1stBond = ii; break;
		}
	}
	if(idxStart1stBond == -1)
	{
		iRet = RET_ABNORMAL_WAVEFORM_CANNOT_FIND_1ST_B; //// @4
		return iRet;
	}
	stpTimeIndex->idxStart1stBond = idxStart1stBond;

////// Zero Force
int nLenFromEndMoveSrchHtTo1stBond = (int)((idxStart1stBond - idxEndMove1stSrchHt)/6);
int idxMiddleSearch = (int)(((idxStart1stBond + idxEndMove1stSrchHt)/2) + 0.5);
int idxGetZeroForceFrom = idxMiddleSearch - nLenFromEndMoveSrchHtTo1stBond;
int idxGetZeroForceTo = idxMiddleSearch + nLenFromEndMoveSrchHtTo1stBond;
// int aZeroBondForce = aFilterForce((idxGetZeroForceFrom):(idxGetZeroForceTo));
double fMeanZeroForce = FOPEM_calc_mean(idxGetZeroForceFrom, idxGetZeroForceTo, aFilterForce); 

	stpTimeIndex->idxGetZeroForceFrom = idxGetZeroForceFrom;
	stpTimeIndex->idxGetZeroForceTo   = idxGetZeroForceTo;

//mean(aZeroBondForce);
//stTimeIdx.idxGetZeroForceFrom = idxGetZeroForceFrom;
//stTimeIdx.idxGetZeroForceTo = idxGetZeroForceTo;
unsigned int idxEnd1stBond;
	idxEnd1stBond = -1; // @4
	for(ii = idxStart1stBond + 25; ii<uiDataLen-5; ii++) // @4
	{
		if(( aPowerV[ii] > fStdNoisePowerV 
			&& aPowerV[ii-1] > fStdNoisePowerV 
			&& aPowerV[ii-2] > fStdNoisePowerV 
			&& aPowerV[ii+1] <= fStdNoisePowerV 
			&& aPowerV[ii+2] <= fStdNoisePowerV 
			&& aPowerV[ii+3] <= fStdNoisePowerV 
			)
			||
			( aPowerV[ii] > fStdNoisePowerV 
			&& aPowerV[ii-1] > fStdNoisePowerV 
			&& aPowerV[ii-2] > fStdNoisePowerV 
			&& aPowerV[ii+2] <= fStdNoisePowerV 
			&& aPowerV[ii+3] <= fStdNoisePowerV 
			&& aPowerV[ii+4] <= fStdNoisePowerV)
			||
			( aPowerV[ii] > fStdNoisePowerV 
			&& aPowerV[ii-1] > fStdNoisePowerV 
			&& aPowerV[ii-2] > fStdNoisePowerV 
			&& aPowerV[ii+1] <= fStdNoisePowerV 
			&& aPowerV[ii+4] <= fStdNoisePowerV 
			&& aPowerV[ii+3] <= fStdNoisePowerV)
			||
			( aPowerV[ii] > fStdNoisePowerV 
			&& aPowerV[ii-1] > fStdNoisePowerV 
			&& aPowerV[ii-2] > fStdNoisePowerV 
			&& aPowerV[ii+2] <= fStdNoisePowerV 
			&& aPowerV[ii+3] <= fStdNoisePowerV 
			&& aPowerV[ii+4] <= fStdNoisePowerV)
			||
			( aPowerI[ii] > fStdNoisePowerI 
			&& aPowerI[ii-1] > fStdNoisePowerI 
			&& aPowerI[ii-2] > fStdNoisePowerI 
			&& aPowerI[ii+1] <= fStdNoisePowerI )
			||
			( aPowerI[ii] > fStdNoisePowerI 
			&& aPowerI[ii-1] > fStdNoisePowerI 
			&& aPowerI[ii-2] > fStdNoisePowerI 
			&& aPowerI[ii+2] <= fStdNoisePowerI )
			||
			( aPowerI[ii] > fStdNoisePowerI 
			&& aPowerI[ii-1] > fStdNoisePowerI 
			&& aPowerI[ii-2] > fStdNoisePowerI 
			&& aPowerI[ii+3] <= fStdNoisePowerI )
			)   // @4
		{
			idxEnd1stBond = ii; break;
		}
	}
	if(idxEnd1stBond == -1) // @4
	{
		stpForcePowerAnalyzeOutput->iFlagAbnormalWaveform = RET_ABNORMAL_WAVEFORM_CANNOT_FIND_1ST_B;  // @4
		iRet = RET_ERROR_NO_BONDING_INFO;
		return iRet;
	} // @4

	stpTimeIndex->idxEnd1stBond = idxEnd1stBond;
	unsigned int idxStartNextWire = uiDataLen;
	for( ii = idxEnd1stBond; ii< (uiDataLen - 5); ii++)
	{
		if(aEFO[ii] >= 3 
			&& aEFO[ii+1] < 3 
			&& aEFO[ii+2] < 3 
			&& aEFO[ii+3] < 3)
		{
			idxStartNextWire = ii; break;
		}
	}
	stpTimeIndex->idxStartNextWire = idxStartNextWire;

	unsigned int idxEnd2ndBond = -1;
	int idxStart2ndBond;
	for(ii = idxStartNextWire; ii>=idxEnd1stBond; ii--)
	{
		if(aPowerV[ii] <= fStdNoisePowerV  && 
				aPowerV[ii - 1] > fStdNoisePowerV && 
				aPowerV[ii - 2] > fStdNoisePowerV && 
				aPowerV[ii - 3] > fStdNoisePowerV && 
				aFilterVel[ii-1] <= fStdNoiseVel * 2 && 
				aFilterVel[ii-2] <= fStdNoiseVel * 2 && 
				aFilterVel[ii-3] <= fStdNoiseVel * 2)
	//            aFilterForce(ii
		{
			idxEnd2ndBond = ii; break;
		}
	}
	if(idxEnd2ndBond == -1)
	{
		idxEnd2ndBond = idxEnd1stBond;
		idxStart2ndBond = idxStart1stBond;
		//return;
	}
	stpTimeIndex->idxEnd2ndBond = idxEnd2ndBond;

	idxStart2ndBond = -1;
	for(ii = idxEnd2ndBond; ii >= idxEnd1stBond; ii--)
	{
		if(aPowerI[ii] > fStdNoisePowerI  && 
				aPowerI[ii - 1] < fStdNoisePowerI && 
				aPowerI[ii - 2] < fStdNoisePowerI && 
				aPowerI[ii - 3] < fStdNoisePowerI && 
		   aVel[ii-1] <= fStdNoiseVel*2 && 
				aVel[ii-2] <= fStdNoiseVel*2 && 
				aVel[ii-3] <= fStdNoiseVel*2
				)
		{           
			idxStart2ndBond = ii; break;
		}
	}
	if( idxStart2ndBond == -1)
	{
		idxStart2ndBond = idxStart1stBond;
	//    return;
	}
	stpTimeIndex->idxStart2ndBond = idxStart2ndBond;


/// 1st Bond
int nLen1stBondPowerStable = (int)((idxEnd1stBond - idxStart1stBond)/6);
int idxMean1stBondTime = (int)((idxEnd1stBond + idxStart1stBond)/2 + 0.5);
int idxCalcPower1stBondFrom = idxMean1stBondTime - nLen1stBondPowerStable;
int idxCalcPower1stBondTo = idxMean1stBondTime + nLen1stBondPowerStable;
int nLenCalcPower1stBond = nLen1stBondPowerStable * 2 + 1;

	stpTimeIndex->idxCalcPower1stBondFrom = idxCalcPower1stBondFrom;
	stpTimeIndex->idxCalcPower1stBondTo = idxCalcPower1stBondTo;
	stpTimeIndex->nLenCalcPower1stBond = nLenCalcPower1stBond;
//aStable1stBondPowerI = aPowerI((idxMean1stBondTime - nLen1stBondPowerStable):(idxMean1stBondTime + nLen1stBondPowerStable));
//aStable1stBondPowerV = aPowerV((idxMean1stBondTime - nLen1stBondPowerStable):(idxMean1stBondTime + nLen1stBondPowerStable)); 
double fMean1stPowerI = FOPEM_calc_mean(idxCalcPower1stBondFrom, idxCalcPower1stBondTo,
										aPowerI); // mean(aStable1stBondPowerI);
double fMean1stPowerV = FOPEM_calc_mean(idxCalcPower1stBondFrom, idxCalcPower1stBondTo,
										aPowerV); // mean(aStable1stBondPowerV);
double dDotProdVI = FOPEM_calc_dot_prod(aPowerV, idxCalcPower1stBondFrom,
										aPowerI, idxCalcPower1stBondFrom,
										nLenCalcPower1stBond);
double dSumSqI = FOPEM_calc_sum_sq(aPowerI, idxCalcPower1stBondFrom,
										nLenCalcPower1stBond);
double f1stBondImp = dDotProdVI/dSumSqI; //aStable1stBondPowerV'/aStable1stBondPowerI'; //fMean1stPowerV/fMean1stPowerI;
/// 2nd Bond
int nLen2ndBondPowerStable = (int)((idxEnd2ndBond - idxStart2ndBond)/6);
int idxMean2ndBondTime = (int)((idxEnd2ndBond + idxStart2ndBond)/2 + 0.5);
int idxCalcPower2ndBondFrom = idxMean2ndBondTime - nLen2ndBondPowerStable;
int idxCalcPower2ndBondTo = idxMean2ndBondTime + nLen2ndBondPowerStable;
int nLenCalcPower2ndBond = nLen2ndBondPowerStable * 2 + 1;
	stpTimeIndex->idxCalcPower2ndBondFrom = idxCalcPower2ndBondFrom;
	stpTimeIndex->idxCalcPower2ndBondTo = idxCalcPower2ndBondTo;
	stpTimeIndex->nLenCalcPower2ndBond = nLenCalcPower2ndBond;

// aStable2ndBondPowerI = aPowerI((idxMean2ndBondTime - nLen2ndBondPowerStable):(idxMean2ndBondTime + nLen2ndBondPowerStable));
// aStable2ndBondPowerV = aPowerV((idxMean2ndBondTime - nLen2ndBondPowerStable):(idxMean2ndBondTime + nLen2ndBondPowerStable));
double fMean2ndPowerI = FOPEM_calc_mean(idxCalcPower2ndBondFrom, idxCalcPower2ndBondTo,
										aPowerI); // mean(aStable2ndBondPowerI);
double fMean2ndPowerV = FOPEM_calc_mean(idxCalcPower2ndBondFrom, idxCalcPower2ndBondTo,
										aPowerV); // mean(aStable2ndBondPowerV);
	dDotProdVI = FOPEM_calc_dot_prod(aPowerV, idxCalcPower2ndBondFrom,
											aPowerI, idxCalcPower2ndBondFrom,
											nLenCalcPower2ndBond);
	dSumSqI = FOPEM_calc_sum_sq(aPowerI, idxCalcPower2ndBondFrom,
										nLenCalcPower2ndBond);
double f2ndBondImp = dDotProdVI/dSumSqI;  // aStable2ndBondPowerV'/aStable2ndBondPowerI'; //fMean2ndPowerV/fMean2ndPowerI;


//aStable1stBondForce = aFilterForce((idxMean1stBondTime - nLen1stBondPowerStable):(idxMean1stBondTime + nLen1stBondPowerStable));
//aStable2ndBondForce = aFilterForce((idxMean2ndBondTime - nLen2ndBondPowerStable):(idxMean2ndBondTime + nLen2ndBondPowerStable));

// size2ndV = size(aStable2ndBondPowerV)
// size2ndI = size(aStable2ndBondPowerI)
// size1stV = size(aStable1stBondPowerV)
// size1stV = size(aStable1stBondPowerI)

//stAnalyzeOneWireForcePower.stTimeIdx = stTimeIdx;

	stpForcePowerAnalyzeOutput->fMean1stPowerI = 
		fMean1stPowerI * stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgPowerFactor.fFactorK_I; 
	stpForcePowerAnalyzeOutput->fMean1stPowerV = 
		fMean1stPowerV * stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgPowerFactor.fFactorK_V; 
	stpForcePowerAnalyzeOutput->fMean2ndPowerI = 
		fMean2ndPowerI * stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgPowerFactor.fFactorK_I; 
	stpForcePowerAnalyzeOutput->fMean2ndPowerV = 
		fMean2ndPowerV * stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgPowerFactor.fFactorK_V; 

	stpForcePowerAnalyzeOutput->f1stBondImp = 
		f1stBondImp * stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgPowerFactor.fFactorK_R;
	stpForcePowerAnalyzeOutput->f2ndBondImp = 
		f2ndBondImp * stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgPowerFactor.fFactorK_R;

	stpForcePowerAnalyzeOutput->f1stBondPmW = 
		1000 * stpForcePowerAnalyzeOutput->fMean1stPowerI * stpForcePowerAnalyzeOutput->fMean1stPowerV
		+ iFPMD_PowerDisplayOffset; // @1
	if(stpForcePowerAnalyzeOutput->f1stBondPmW < 0.5 ) 
	{
		stpForcePowerAnalyzeOutput->f1stBondPmW = 0.5;
	} // @1
	stpForcePowerAnalyzeOutput->f2ndBondPmW = 
		1000 * stpForcePowerAnalyzeOutput->fMean2ndPowerI * stpForcePowerAnalyzeOutput->fMean2ndPowerV;

double fFactorForceRatio_GramPerVolt, fFactorForceRatio_OffsetZeroForce;
////// Force
	if( iFlagDummyBond == 1)
	{
		fFactorForceRatio_GramPerVolt = 
			stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgDummyBondForceFactor.fFactorForceRatio_GramPerVolt;
		fFactorForceRatio_OffsetZeroForce = 
			stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgDummyBondForceFactor.fFactorForceRatio_OffsetZeroForce;
	}
	else
	{
		fFactorForceRatio_GramPerVolt = 
			stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgActualBondForceFactor.fFactorForceRatio_GramPerVolt;
		fFactorForceRatio_OffsetZeroForce = 
			stpForcePowerAnalyzeInput->stpOnlineForcePowerSetting->stCfgActualBondForceFactor.fFactorForceRatio_OffsetZeroForce;
	}
	stpForcePowerAnalyzeOutput->f1stBondForce = FOPEM_calc_mean(idxCalcPower1stBondFrom, idxCalcPower1stBondTo,
											aFilterForce); // mean(aStable1stBondForce); //  - fMeanZeroForce
	stpForcePowerAnalyzeOutput->f2ndBondForce = FOPEM_calc_mean(idxCalcPower2ndBondFrom, idxCalcPower2ndBondTo,
											aFilterForce); // mean(aStable2ndBondForce); //  - fMeanZeroForce
	stpForcePowerAnalyzeOutput->f1stBondForceGram = 
		stpForcePowerAnalyzeOutput->f1stBondForce * fFactorForceRatio_GramPerVolt + fFactorForceRatio_OffsetZeroForce
		+ (double)iFPMD_ForceDisplayOffset; // @1
;
	stpForcePowerAnalyzeOutput->f2ndBondForceGram = 
		stpForcePowerAnalyzeOutput->f2ndBondForce * fFactorForceRatio_GramPerVolt + fFactorForceRatio_OffsetZeroForce;

#ifdef __TESTED__

#endif // __TESTED__
	if(stpForcePowerAnalyzeOutput->f1stBondForceGram < 0)
	{
		stpForcePowerAnalyzeOutput->f1stBondForceGram = 0;
	}
	if( stpForcePowerAnalyzeOutput->f2ndBondForceGram < 0)
	{
		stpForcePowerAnalyzeOutput->f2ndBondForceGram = 0;
	}
	stpForcePowerAnalyzeOutput->fMeanZeroForce = fMeanZeroForce * fFactorForceRatio_GramPerVolt + fFactorForceRatio_OffsetZeroForce;
	if(	stpForcePowerAnalyzeOutput->f1stBondImp <= 6 
		|| (stpForcePowerAnalyzeOutput->f1stBondPmW <= 1.0 
			&& stpForcePowerAnalyzeOutput->f1stBondForceGram <= 1.0)
			)
	{
		iRet = RET_ERROR_IMPEDANCE_ERROR;
	}

////// abnormal case
	if(stpTimeIndex->idxStart1stBond == stpTimeIndex->idxStart2ndBond)
	{
		stpForcePowerAnalyzeOutput->iFlagAbnormalWaveform = RET_ABNORMAL_WAVEFORM_CANNOT_FIND_2ND_B;  // @4
		iRet = RET_ERROR_NO_BONDING_INFO;
		return iRet;
	}

return iRet;
} 