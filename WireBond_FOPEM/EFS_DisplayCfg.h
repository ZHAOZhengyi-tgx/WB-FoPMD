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
#ifndef __EFS_DISPLAY_CFG__
#define __EFS_DISPLAY_CFG__

//Version
#define FPMD_VERSION_MAJOR    (1)
#define FPMD_VERSION_MINOR    (1)

//#define __LOAD_TIMER_DMP_FROM_DLG_FPMD__
#define _LOAD_FPMD_FROM_DMP609_TEST

#define FIG_DISPLAY_LENGTH_US  (400000.0)
#define FIG_DISPLAY_LENGTH_X_PIXAL  (170)
#define FIG_DISPLAY_HEIGHT_Y_PIXAL  (120)

#define FIG2_LOCATION_LOW_X  455
#define FIG2_LOCATION_UPP_X  (FIG_DISPLAY_LENGTH_X_PIXAL + FIG2_LOCATION_LOW_X)
#define FIG2_LOCATION_LOW_Y  2
#define FIG2_LOCATION_UPP_Y  (FIG2_LOCATION_LOW_Y + FIG_DISPLAY_HEIGHT_Y_PIXAL)

#define FIG1_LOCATION_LOW_X  (FIG_DISPLAY_LENGTH_X_PIXAL + FIG2_LOCATION_LOW_X + 5)
#define FIG1_LOCATION_UPP_X  (FIG_DISPLAY_LENGTH_X_PIXAL + FIG1_LOCATION_LOW_X)
#define FIG1_LOCATION_LOW_Y  2
#define FIG1_LOCATION_UPP_Y  (FIG1_LOCATION_LOW_Y + FIG_DISPLAY_HEIGHT_Y_PIXAL)

#define FIG_LENGTH_X_ROLLING_HORIZAN_2_COL  395
#define FIG_LENGTH_X_ROLLING_HORIZAN  795
#define FIG_HEIGHT_Y_ROLLING_HORIZAN  195

#define FIG3_ROLLING_HORIZAN_LOW_X  5
#define FIG3_ROLLING_HORIZAN_UPP_X  (FIG_LENGTH_X_ROLLING_HORIZAN + FIG3_ROLLING_HORIZAN_LOW_X)
#define FIG3_ROLLING_HORIZAN_LOW_Y  170
#define FIG3_ROLLING_HORIZAN_UPP_Y  (FIG_HEIGHT_Y_ROLLING_HORIZAN + FIG3_ROLLING_HORIZAN_LOW_Y)

// FIG3_ROLLING_HORIZAN_UPP_X
#define FIG4_ROLLING_HORIZAN_LOW_X  5
#define FIG4_ROLLING_HORIZAN_UPP_X  (FIG_LENGTH_X_ROLLING_HORIZAN + FIG4_ROLLING_HORIZAN_LOW_X)
// 150 @1
#define FIG4_ROLLING_HORIZAN_LOW_Y  (FIG3_ROLLING_HORIZAN_UPP_Y + 5)
#define FIG4_ROLLING_HORIZAN_UPP_Y  (FIG_HEIGHT_Y_ROLLING_HORIZAN + FIG4_ROLLING_HORIZAN_LOW_Y)
// 200
#define FIG5_LENGTH_X_ROLLING_IMPEDANCE_HORIZAN  FIG_LENGTH_X_ROLLING_HORIZAN
#define FIG5_HEIGHT_Y_ROLLING_HORIZAN  80
// + FIG4_ROLLING_HORIZAN_UPP_X
#define FIG5_ROLLING_HORIZAN_LOW_X  (5 )
#define FIG5_ROLLING_HORIZAN_UPP_X  (FIG5_LENGTH_X_ROLLING_IMPEDANCE_HORIZAN + FIG5_ROLLING_HORIZAN_LOW_X)
// 400 @1
#define FIG5_ROLLING_HORIZAN_LOW_Y  FIG4_ROLLING_HORIZAN_UPP_Y + 5
#define FIG5_ROLLING_HORIZAN_UPP_Y  (FIG5_HEIGHT_Y_ROLLING_HORIZAN + FIG5_ROLLING_HORIZAN_LOW_Y)

#define _MAX_POWER_DISPLAY_MW_DEF_FIG4_      (150.0)
#define _MAX_FORCE_DISPLAY_GRAM_DEF_FIG3_    (200.0)
#define _MAX_IMPEDANCE_DISPLAY_OHM_DEF_FIG5_    (20.0)

#define SCOPE_OFST_FORCE_   0
#define SCOPE_OFST_VEL_     1
#define SCOPE_OFST_POWER_1_   3
#define SCOPE_OFST_POWER_2_   4
#define SCOPE_OFST_POWER_US_  2
#define SCOPE_OFST_EFO_   5


#include "FPMD_CONFIG.h"

#define __FAST_ANA_LEN__ (30)
#include "FPMD_CALC_DEF.h"

void FOPEM_set_combo_by_user_level();

int FOPEM_calculate_bond_force_power(ANALYZE_FORCE_POWER_OUTPUT *stpForcePowerAnalyzeOutput,
									  ANALYZE_FORCE_POWER_INPUT *stpForcePowerAnalyzeInput);
extern void FOPEM_jig_end_scan_ad();

typedef struct
{
	int *piEFO;
	int *piForce;
	int *piVel;
	int *piPowerUS;
	int *piPowerV;
	int *piPowerI;
	double tSampleFer_us;
}FOPEM_WAVEFORM_INT;

int FOPEM_load_waveform(char *strFileFullName, FOPEM_WAVEFORM_INT *stpWaveformInt);

extern int FOPEM_load_golden_waveform();

typedef struct
{
	double *pdEFO;
	double *pdForce;
	double *pdVel;
	double *pdPowerUS;
	double *pdPowerV;
	double *pdPowerI;

	int *piEFO;
	int *piForce;
	int *piVel;
	int *piPowerUS;
	int *piPowerV;
	int *piPowerI;

	int nDataLen;
}CONVERT_FROM_ADC_TO_VOLT;
extern void FOPEM_convert_read_adc_to_volt(CONVERT_FROM_ADC_TO_VOLT *stpConvert);
extern void FOPEM_convert_golden_waveform_adc_to_volt();

extern void FOPEM_jig_read_ad();
extern void FOPEM_jig_end_scan_ad();
extern void FOPEM_jig_start_scan_ad();
extern void FOPEM_UpdateReadAd();
extern char WB_fast_analyze_start_by_vel();
extern void WB_analyze_start_by_vel();
extern void WB_analyze_start_by_efo();
extern void FOPEM_init_adc_setting();
extern void FOPEM_init_offset_setting(); // @3
extern void FOPEM_init_threshold_setting(); // @3
extern void FOPEM_init_calibrate_checking();
extern void FOPEM_reset_calibrate_checking();
extern void FOPEM_init_default_config();
extern int FOPEM_calibrate_linear_fit_force(FOPEM_CONFIG_FORCE_FACTOR *stpForceFactor, FILE * fpData);
extern void FOPEM_close_device(); // @5
extern int FOPEM_load_config_from_file();
extern int FOPEM_get_read_data_length();
//// Save config file
extern char FOPEM_get_flag_config_setting_changed();
extern void FOPEM_set_flag_config_setting_changed();
extern int FOPEM_save_config_to_file();

////
extern char FOPEM_dlg_get_capture_data_stage();
extern int FOPEM_dlg_get_curr_num_file_capture_calibrate();
extern char FOPEM_dlg_get_save_capture_verify_file_flag();

int FOPEM_load_waveform(char *strFileFullName, FOPEM_WAVEFORM_INT *stpWaveformInt);
void FOPEM_convert_load_waveform_adc_to_volt();
void FOPEM_convert_golden_waveform_adc_to_volt();

void FOPEM_convert_load_data_to_adc_capture();
void FOPEM_convert_adc_capture_to_load_data();

void FOPEM_update_peak_value_in_rolling_horizan();

#define MAX_NUM_SAMPLE_CALIBRATION   (DEF_CAPTURE_DATA_COMPLETE * DEF_NUM_WAVEFORMS_PER_POINT)

double FOPEM_calc_dot_prod(double *dArray1, int iFrom1, double *dArray2, int iFrom2, int nLen );
double FOPEM_calc_sum_sq(double *dArray, int iFrom, int nLen );
double FOPEM_calc_mean(int iFrom, int iTo, double *dArray);
double FOPEM_calc_max(int iFrom, int iTo, double *dArray);
double FOPEM_calc_min(int iFrom, int iTo, double *dArray);

double FOPEM_calc_std_with_ave(int iFrom, int iTo, double dAve, double *adArray);
void FOPEM_moving_ave(double *dInput, double *dOutput, int nDataLen, int nAveLen);

void FOPEM_calc_smooth_force_power_impedance(int iMovAveLen);

// ASM - USG-Xducer Calibration
#define DEF_UPPER_LIMIT_GOLD_IMPEDANCE  18
#define DEF_LOWER_LIMIT_GOLD_IMPEDANCE  8
#define DEF_XDUCER_GOLD_IMPEDANCE       13

int FOPEM_calc_dac_by_power_with_imp_WB_A_ultra_low(double dPower_mW, double dImp);
int FOPEM_calc_dac_by_power_with_imp_WB_A_low(double dPower_mW, double dImp);
int FOPEM_calc_dac_by_power_with_imp_WB_A_high(double dPower_mW, double dImp);
int FOPEM_set_golden_state_impedance(double dImp);

// based on moving_average
extern void FOPEM_calculate_online_force();
extern void FOPEM_calculate_online_power();
extern SYS_USER_INTERFACE_CFG stSystemUserInterfaceCfg;
extern char cFlagCaptureTrigger;

#define RET_OK_ZERO   0
#define RET_ERROR_OPEN_FILE        1
#define RET_ERROR_FIND_VALID_DATA  2
#define RET_ERROR_NO_BONDING_INFO  3
#define RET_ERROR_GOLD_IMPEDANCE_OUT_RANGE  4
#define RET_ERROR_IMPEDANCE_ERROR  5
#define RET_ERROR_CALCULATION      6

#define RET_ABNORMAL_WAVEFORM_CANNOT_FIND_1ST_B  1
#define RET_ABNORMAL_WAVEFORM_CANNOT_FIND_2ND_B  2
#define RET_ABNORMAL_WAVEFORM_CANNOT_1ST_SEARCH_MOTION_END 3
#define RET_ABNORMAL_WAVEFORM_CANNOT_FIND_EFO_TRIGGER 4

#define RET_ZERO_CONFIG_SETTING_NOT_CHANGE  0
#define RET_CONFIG_SETTING_CHANGED  1

#define FOPEM_CONVERT_ADC_TO_2d5VOLT(x)     ((double)((x)*2.5/65535))
#define FOPEM_CONVERT_ADC_TO_5VOLT(x)     ((double)((x)*5.0/65535))
#define FOPEM_CONVERT_ADC_TO_PN5VOLT(x)     ((double)(((x) - 32767.0)*5.0/32767.0))

void FOPEM_save_waveform(FILE *fptr, FOPEM_FORCE_POWER_VALUE_SAVE_FILE *stpForcePowerSaveFile);
void FOPEM_save_checking_waveform();
void FOPEM_save_calibration_capture_waveform();

void FOPEM_set_system_offline_mode(char cFlag);
char FOPEM_get_system_offline_mode();

char FOPEM_get_flag_dummy_bond();

#endif  // __EFS_DISPLAY_CFG__