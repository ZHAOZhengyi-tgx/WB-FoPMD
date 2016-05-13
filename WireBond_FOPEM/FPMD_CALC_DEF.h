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

#ifndef __FPMD_CALC_DEF__
#define __FPMD_CALC_DEF__


#define MINIMUM_FORCE_OFFSET_GRAM  -30
#define MAXIMUM_FORCE_OFFSET_GRAM  30
#define MINIMUM_POWER_OFFSET_MW  -30
#define MAXIMUM_POWER_OFFSET_MW  30

typedef struct
{
	int idxEndEFO;
	int idxEndMove1stSrchHt;
	int idxStart1stBond;
	int idxGetZeroForceFrom;
	int idxGetZeroForceTo;
	int idxEnd1stBond;
	int idxStartNextWire;
	int idxEnd2ndBond;
	int idxStart2ndBond;
	int idxCalcPower1stBondFrom;
	int idxCalcPower1stBondTo;
	int idxCalcPower2ndBondFrom;
	int idxCalcPower2ndBondTo;
	int nLenCalcPower1stBond;
	int nLenCalcPower2ndBond;
}FOPEM_BOND_CYCLE_TIME_INDEX;

typedef struct
{
    double fMean1stPowerI; 
    double fMean1stPowerV; 
    double fMean2ndPowerI; 
    double fMean2ndPowerV; 
    double f1stBondImp;
    double f2ndBondImp;
    double f1stBondPmW;
    double f2ndBondPmW;
    double f1stBondForce;
    double f2ndBondForce;
    double f1stBondForceGram;
    double f2ndBondForceGram;
    double fMeanZeroForce;
	int iEstimateDac1stB_UL;
	int iEstimateDac1stB_L;
	int iEstimateDac1stB_H;
	int iEstimateDac2ndB_UL;
	int iEstimateDac2ndB_L;
	int iEstimateDac2ndB_H;
	int iFlagAbnormalWaveform;
	FOPEM_BOND_CYCLE_TIME_INDEX stBondCycleTimeIndex;
}ANALYZE_FORCE_POWER_OUTPUT;
//void FOPEM_sytem_setting_default();

// typedef UINT unsigned int;

typedef struct
{
	double fFactorK_I; 
	double fFactorK_V;
	double fFactorK_R;
}FOPEM_CONFIG_POWER_FACTOR;

typedef struct
{
	double fFactorForceRatio_GramPerVolt;
	double fFactorForceRatio_OffsetZeroForce;
}FOPEM_CONFIG_FORCE_FACTOR;

typedef struct
{
	FOPEM_CONFIG_POWER_FACTOR stCfgPowerFactor;
	FOPEM_CONFIG_FORCE_FACTOR stCfgDummyBondForceFactor;
	FOPEM_CONFIG_FORCE_FACTOR stCfgActualBondForceFactor;

}FOPEM_FORCE_POWER_SETTING;

typedef struct
{
	FOPEM_FORCE_POWER_SETTING *stpOnlineForcePowerSetting;
	double fSampleTime_ms;
	unsigned int uiDataLen;
	int iFlagDummyBond;
	double *adEFO;
	double *adVel;
	double *adForce;
	double *adPowerUS;
	double *adPowerI;
	double *adPowerV;
	double *adFilterVel;
	double *adFilterForce;
	double *adFilterPowerV;
}ANALYZE_FORCE_POWER_INPUT;

#endif // __FPMD_CALC_DEF__