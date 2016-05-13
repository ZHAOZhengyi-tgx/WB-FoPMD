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
// DlgForcePowerMonitor.cpp : implementation file
//
// History
// @1  20130709 John  Remove golden-upper-lower lines for impedance
// @2  20130717 Capture Gold, correlation

#include "stdafx.h"
#include "math.h"
#include "DMP609VC.h"
#include "DlgForcePowerMonitor.h"
#include "EFS_DisplayCfg.h"

int iFPMD_ForceDisplayOffset;
int iFPMD_PowerDisplayOffset;

extern HANDLE	m_hDevice;
// DlgForcePowerMonitor dialog
IMPLEMENT_DYNAMIC(DlgForcePowerMonitor, CDialog)

DlgForcePowerMonitor::DlgForcePowerMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(DlgForcePowerMonitor::IDD, pParent)
{
	m_pWinThread = NULL;
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

DlgForcePowerMonitor::~DlgForcePowerMonitor()
{
}

void DlgForcePowerMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgForcePowerMonitor, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE_PF_MONITOR_DLG, &DlgForcePowerMonitor::OnBnClickedButtonPausePfMonitorDlg)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PF_MONITOR_DLG, &DlgForcePowerMonitor::OnBnClickedButtonSavePfMonitorDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PF_MONITOR_DLG, &DlgForcePowerMonitor::OnBnClickedButtonLoadPfMonitorDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_USER_LEVEL_PF_MONITOR_DLG, &DlgForcePowerMonitor::OnCbnSelchangeComboUserLevelPfMonitorDlg)
	ON_CBN_SELCHANGE(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG, &DlgForcePowerMonitor::OnCbnSelchangeCcomboRunningModePfMonitorDlg)
	ON_CBN_SELCHANGE(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG, &DlgForcePowerMonitor::OnCbnSelchangeCcomboPowerModePfMonitorDlg)
	ON_BN_CLICKED(ID_BTN_EXIT_FP_MONITOR_DLG, &DlgForcePowerMonitor::OnBnClickedBtnExitFpMonitorDlg)
	ON_BN_CLICKED(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM, &DlgForcePowerMonitor::OnBnClickedCheckFpmdFlagShownGoldenWaveform)
	ON_CBN_SELCHANGE(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG, &DlgForcePowerMonitor::OnCbnSelchangeComboVerifyModePfMonitorDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM, &DlgForcePowerMonitor::OnCbnSelchangeComboFpmdVerifyThresholdForceGram)
	ON_CBN_SELCHANGE(IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW, &DlgForcePowerMonitor::OnCbnSelchangeComboFpmdVerifyThresholdPowermw)
	ON_CBN_SELCHANGE(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM, &DlgForcePowerMonitor::OnCbnSelchangeComboFpmdVerifyThresholdImpedanceOhm)
	ON_BN_CLICKED(IDC_BUTTON_FOPEM_DLG_AUTO_CHECK_CALIBRATE, &DlgForcePowerMonitor::OnBnClickedButtonFopemDlgAutoCheckCalibrate)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG, &DlgForcePowerMonitor::OnNMCustomdrawSliderForceDisplayOffsetFpmdDlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG, &DlgForcePowerMonitor::OnNMCustomdrawSliderPowerDisplayOffsetFpmdDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG, &DlgForcePowerMonitor::OnNMReleasedcaptureSliderForceDisplayOffsetFpmdDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG, &DlgForcePowerMonitor::OnNMReleasedcaptureSliderPowerDisplayOffsetFpmdDlg)
END_MESSAGE_MAP()

// DlgForcePowerMonitor message handlers
#include "EFS_DisplayCfg.h"

//SYS_SETTING_ADC stSystemSetting;
//void FOPEM_sytem_setting_default()
//{
//	stSystemSetting.nDisplayLen = DEF_DISPLAY_LEN;
//	stSystemSetting.nSampleLen = DEF_SAMPLE_LEN;
//	stSystemSetting.tPeriodSample_us = DEF_SAMPLE_FER_US;
//	stSystemSetting.ucSampleVoltMode = 2;
//}

extern ANALYZE_FORCE_POWER_OUTPUT stGoldenForcePowerAnalyzeOutput;
extern ANALYZE_FORCE_POWER_INPUT stGoldenForcePowerAnalyzeInput;
extern SYS_SETTING_ADC stGoldenWaveformAdcSetting;

extern FOPEM_VALUE_REAL_TIME stForcePowerRollingHorizan;

#include "DMP609VCDlg.h"
CDMP609VCDlg *pDlgTestDMP609;

#define APP_DLG_LEFT 0
#define APP_DLG_TOP  0
#define APP_DLG_WIDTH 1024
#define APP_DLG_HEIGHT 768

unsigned long ulNumLoadedWaveform = 0;
unsigned int idxPrevPointRolling, idxNewPointInRolling;
int iFlagActiveRolling = 0;
int iFlagHasGoldenWaveform = 0;
int iFlagHasAdcCapture = 0;
int iFlagShowGoldenWaveform = 0;
BOOL DlgForcePowerMonitor::OnInitDialog()
{
	FOPEM_init_adc_setting();
	FOPEM_init_offset_setting(); // @3
	FOPEM_init_threshold_setting(); ///////// Checking Threshold
	FOPEM_init_calibrate_checking();///// Initialize the defaul
	FOPEM_init_default_config();

	FOPEM_load_config_from_file();

	UI_InitComboButtonDlgFOPEM();
//	FOPEM_sytem_setting_default();
//	FOPEM_init_adc_setting();
	
	this->SetWindowPos(NULL, APP_DLG_LEFT, APP_DLG_TOP, APP_DLG_WIDTH, APP_DLG_HEIGHT, SWP_NOZORDER);
	CString cstrTitle;
	cstrTitle.Format("RT-FPMD   Ver%d.%d                                                                               MACTRONIX", 
		(int)FPMD_VERSION_MAJOR, (int)FPMD_VERSION_MINOR);
	this->SetWindowTextA(cstrTitle);
	GetDlgItem(IDC_BUTTON_FOPEM_DLG_AUTO_CHECK_CALIBRATE)->SetWindowPos(NULL, 850, 660, 150,30, SWP_NOZORDER);
	GetDlgItem(ID_BTN_EXIT_FP_MONITOR_DLG)->SetWindowPos(NULL, 920, 700, 80, 30,SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_FOPEM_DLG_USER_LEVEL)->SetWindowPos(NULL, 900, 20, 100, 20, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_USER_LEVEL_PF_MONITOR_DLG)->SetWindowPos(NULL, 900, 40, 100, 100, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_DLG_FPMD_MODE)->SetWindowPos(NULL, 900, 70, 100, 20, SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG)->SetWindowPos(NULL, 900, 90, 100, 120, SWP_NOZORDER);

	GetDlgItem(IDC_STATIC_DLG_FPMD_VERIFY_RESULT)->SetWindowPos(NULL, 850, 130, 100, 20, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_DISPLAY_CHECK_RESULT_FOPEM_DLG)->SetWindowPos(NULL, 820, 150, 180, 320, SWP_NOZORDER);

	GetDlgItem(IDC_BUTTON_PAUSE_PF_MONITOR_DLG)->SetWindowPos(NULL, 850, 700, 60, 30, SWP_NOZORDER);
	GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->SetWindowPos(NULL, 10, 700, 60, 30, SWP_NOZORDER);
	GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->SetWindowPos(NULL, 80, 700, 60, 30, SWP_NOZORDER);

	GetDlgItem(IDC_STATIC_FOPEM_DISPLAY_DATE_TIME)->SetWindowPos(NULL, 150, 700, 300, 40, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_CAPTURE_TRIGGER_FLAG_PF_MONITOR_DLG)->SetWindowPos(NULL, 480, 700, 300,40, SWP_NOZORDER); // @1
	GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM)->SetWindowPos(NULL, 750, 800, 50,30, SWP_NOZORDER);       // @1

	GetDlgItem(IDC_STATIC_THRESHOLD_AND_FORCE_)->SetWindowPos(NULL, 8,2, 150, 40,SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM)->SetWindowPos(NULL, 160,2,50,200,SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_POWER_THRESHOLD)->SetWindowPos(NULL, 220,2,100,30,SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW)->SetWindowPos(NULL, 330,2,50,200,SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_BOND_FORCE_MONITOR_1STBOND)->SetWindowPos(NULL, 80,45,130,20,SWP_NOZORDER); // SS_SUNKEN
	GetDlgItem(IDC_STATIC_BOND_POWER_MONITOR_1STBOND)->SetWindowPos(NULL, 220,45,220,60,SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_CHECK_THRESHOLD_IMPEDANCE)->SetWindowPos(NULL, 80,70,75,35,SWP_NOZORDER);
	GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM)->SetWindowPos(NULL, 160,70,50,80, SWP_NOZORDER);

	UI_InitializeForcePowerOffsetSlider();
	GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG)->SetWindowPos(NULL, 140,115,240,20, SWP_NOZORDER);
	GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG)->SetWindowPos(NULL, 140,140,240,20, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_OFFSET_DISPLAY_FPMD_DLG)->SetWindowPos(NULL, 10, 90, 60, 20, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_FORCE_OFFSET_VALUE_FPMD_DLG)->SetWindowPos(NULL, 10, 115, 130, 25, SWP_NOZORDER);
	GetDlgItem(IDC_STATIC_POWER_OFFSET_VALUE_FPMD_DLG)->SetWindowPos(NULL, 10, 140, 130, 25, SWP_NOZORDER);
	cstrTitle.Format("Force(g): %d", iFPMD_ForceDisplayOffset);
	GetDlgItem(IDC_STATIC_FORCE_OFFSET_VALUE_FPMD_DLG)->SetWindowTextA(cstrTitle);
	cstrTitle.Format("Power(mW): %d", iFPMD_PowerDisplayOffset);
	GetDlgItem(IDC_STATIC_POWER_OFFSET_VALUE_FPMD_DLG)->SetWindowTextA(cstrTitle);

	if(FOPEM_load_golden_waveform() == RET_OK_ZERO)
	{
		FOPEM_convert_golden_waveform_adc_to_volt();

		stGoldenForcePowerAnalyzeInput.uiDataLen = FOPEM_get_read_data_length();
		FOPEM_calculate_bond_force_power(&stGoldenForcePowerAnalyzeOutput,
									  &stGoldenForcePowerAnalyzeInput);

		CString cstrGoldenWaveform;
		if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
		{
			cstrGoldenWaveform.Format("1stBond-Golden: Force(g) = %4.0f; Power(mW) = %4.0f; Imp(Ohm) = %4.0f",
				stGoldenForcePowerAnalyzeOutput.f1stBondForceGram, stGoldenForcePowerAnalyzeOutput.f1stBondPmW,
				stGoldenForcePowerAnalyzeOutput.f1stBondImp);
		}

		GetDlgItem(IDC_STATIC_GOLDEN_WAVEFORM_STATISTIC)->SetWindowTextA(cstrGoldenWaveform);

		if(FOPEM_set_golden_state_impedance(stGoldenForcePowerAnalyzeOutput.f1stBondImp)
			!= RET_OK_ZERO)
		{
			cstrGoldenWaveform.Format("Warning: Impedance %4.0f out of range: \n\n[%4.0f, %4.0f] \n\nYes: set to default %4.0f, No: Exit and check golden waveform",
				stGoldenForcePowerAnalyzeOutput.f1stBondImp, 
				DEF_LOWER_LIMIT_GOLD_IMPEDANCE, DEF_UPPER_LIMIT_GOLD_IMPEDANCE, DEF_XDUCER_GOLD_IMPEDANCE);
			AfxMessageBox(cstrGoldenWaveform);
		}

		CString cstrLenView;
		cstrLenView.Format("ms: %d \\ ... ", 
			(int)(stGoldenForcePowerAnalyzeInput.uiDataLen * stGoldenWaveformAdcSetting.tPeriodSample_us/1000));
		GetDlgItem(IDC_STATIC_CAPTURE_TRIGGER_FLAG_PF_MONITOR_DLG)->SetWindowTextA(cstrLenView);
		iFlagHasGoldenWaveform = 1;

		FOPEM_update_peak_value_in_rolling_horizan();
	}

	UI_SetButtonByVerifyMode();
	UI_SetComboByUserLevel();

// Start Timer
// #ifdef __START_FPDM__

#ifdef __LOAD_TIMER_DMP_FROM_DLG_FPMD__
	int iTimFOPEM_Dlg;
	iTimFOPEM_Dlg = SetTimer(1,2000,NULL);
	if(iTimFOPEM_Dlg == 0)
	{
		MessageBox("         定 时 器 安 装 失 败                          \n \n \n		   程 序 无 法 正 常 工 作!       \n \n \n          请 关 闭 正 在 运 行 的 不 需 要 的 程 序                     "); 
	}

#include "Dmp609Dll.h"//必须包含的DLL文件
    gLiDLL=LoadLibrary("Dmp609.dll");//加载DLL
    #include "Dmp609Dll-A.h"
    if(gLiDLL==NULL)
	{
	     AfxMessageBox("          请查找DLL         \n\n ");
	}

	m_hDevice = OpenDevice(0); //打开设备
	if (m_hDevice == INVALID_HANDLE_VALUE)
	{
		CString cstrEnableDebugMode;

		if(stSystemUserInterfaceCfg.nDisplayLanguage == FPMD_DISPLAY_LANGUAGE_EN)
		{

			cstrEnableDebugMode.Format("Open Device Error, Please check \n\n(1) USB connection, \n(2) Power Supply, \n\n Cancel: to exit, OK: to proceed entering debug mode");			
		}
		else
		{
			//MessageBox("\n\n       打开设备失败,程序将无法正常工作   \n\n\n\n   请检查模块(或板卡)是否已正确连接 \n\n");
			cstrEnableDebugMode.Format("\n\n       打开设备失败,程序将无法正常工作 \n\n 请检查模块(或板卡)是否已正确连接 \n\n Cancel: 退出, OK: 进入离线模式");
		}
		int iRet = AfxMessageBox(cstrEnableDebugMode, MB_OKCANCEL);
		if(iRet == IDOK)
		{
			FOPEM_set_system_offline_mode(TRUE);
			stSystemUserInterfaceCfg.nUserLevel = USER_LEVEL_DEVELOPER; // USER_LEVEL_ENGINEER;
		}
		else
		{
			exit(0);
		}
	}
	else
	{	
		FOPEM_set_system_offline_mode(FALSE);
		stSystemUserInterfaceCfg.nUserLevel = USER_LEVEL_OPERATOR;
		Sleep(100);
	}
#else
	if(pDlgTestDMP609 == NULL)
	{
		pDlgTestDMP609 = new CDMP609VCDlg(this);
		pDlgTestDMP609->Create(IDD_DMP609VC_DIALOG);
	}
	pDlgTestDMP609->ShowWindow(0);

#endif // __START_FPDM__
	return CDialog::OnInitDialog(); 
}

void DlgForcePowerMonitor::OnTimer(UINT nIDEvent) 
{
	FOPEM_TimerEvent();
	CDialog::OnTimer(nIDEvent);
}

extern double dGoldenWaveformImpedance;
extern int aiLoadWaveformEFO[8192];
extern int aiLoadWaveformForce[8192];
extern int aiLoadWaveformVel[8192];
extern int aiLoadWaveformPowerUS[8192];
extern int aiLoadWaveformPowerI[8192];
extern int aiLoadWaveformPowerV[8192];
extern ANALYZE_FORCE_POWER_OUTPUT stLoadForcePowerAnalyzeOutput;
extern ANALYZE_FORCE_POWER_INPUT stLoadForcePowerAnalyzeInput;
extern SYS_SETTING_ADC stLoadDataAdcSetting;
extern double adRollingPower_mW[];
extern double adRollingImpedance_ohm[];
extern double adRollingForce_gram[];
extern double adSmoothedRollingPower_mW[];
extern double adSmoothedRollingImpedance_ohm[];
extern double adSmoothedRollingForce_gram[];

extern struct tm *stpTimeGlobalVar;

void DlgForcePowerMonitor::FOPEM_ReplotRollingHorizan()
{
	int iBakPrevIndex = idxPrevPointRolling;
	int iBakNewIndex = idxNewPointInRolling;
	int ii;
	for(ii = 1; ii<= iBakNewIndex; ii++)
	{
		idxPrevPointRolling = ii - 1;
		idxNewPointInRolling = ii;

		//FOPEM_plot_rolling_horizan_new_sample();
		FOPEM_plot_rolling_horizan_smoothed_new_sample();
	}

	idxPrevPointRolling = iBakPrevIndex;
	idxNewPointInRolling = iBakNewIndex;

}

void DlgForcePowerMonitor::OnPaint()
{
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		if(iFlagHasAdcCapture == 1 
			&& 	stSystemUserInterfaceCfg.nFPMD_VerifyMode != WB_MACHINE_FPMD_MODE_CAPTURE)  // @2
		{
			WB_FOPEM_plot_bond_trig_by_spark();
		}
		if(iFlagHasGoldenWaveform == 1 && iFlagShowGoldenWaveform== 1)
		{
			FOPEM_PlotGoldenWaveform();
		}
	}
	FOPEM_InitDisplay();
	FOPEM_ReplotRollingHorizan();
	FOPEM_plot_rolling_horizan_peak_value();
}

char cFlagShowWindow = 1;
char DlgForcePowerMonitor::GetFlagShowWindow()
{
	return cFlagShowWindow;
}
char cFlagPauseCapture = 0;
char DlgForcePowerMonitor::GetFlagPauseCapture()
{
	return cFlagPauseCapture;
}

void DlgForcePowerMonitor::ShowWindow(int nCmdShow)
{
	cFlagShowWindow = 1;
	CDialog::ShowWindow(nCmdShow);
}

void DlgForcePowerMonitor::FOPEM_InitDisplay()
{

	FOPEM_InitDisplayFig();
	FOPEM_plot_rolling_horizan_peak_value();
	FOPEM_plot_rolling_horizan_golden_line();
	FOPEM_update_rolling_horizan_upp_low_threshold_value();

	if(stSystemUserInterfaceCfg.nUserLevel > USER_LEVEL_OPERATOR)
	{
		FOPEM_plot_rolling_horizan_upp_low_threshold_line();
	}
//	FOPEM_InitDisplayFig1();
}

extern unsigned int aiData[65536];
extern int nLen;
extern int nOffsetBondStart;
extern unsigned int aiBakData[65536];

extern char WB_fast_analyze_start_by_vel();
extern void FOPEM_jig_start_scan_ad();
extern void FOPEM_jig_end_scan_ad();
extern void FOPEM_jig_read_ad();
extern SYS_SETTING_ADC stSysSettingAdc;
extern void WB_analyze_start_by_vel();

extern char cFlagDummyBond;
extern void WB_analyze_start_by_vel();
extern void WB_analyze_start_by_efo();

UINT AutoCalibrateVerifyThreadProc( LPVOID pParam )
{
    DlgForcePowerMonitor* pObject = (DlgForcePowerMonitor *)pParam;
	return pObject->AutoCalibrateVerifyThread(); 	
}

int nSleepFastCapture, nLongWaitSampling;

char FOPEM_get_flag_capture()
{
	return cFlagCaptureTrigger;
}
void FOPEM_clear_flag_capture_trigger()
{
	cFlagCaptureTrigger = FALSE;
}

extern char *pstrCaptureDataPromptWizard[];
extern char *pstrVerifyDataPromptWizard[];
extern char *pstrCaptureGoldenVerifyWizardConfirmKeyInValue[];

extern FOPEM_FORCE_POWER_VALUE_SAVE_FILE astForcePowerValueCalibrate[DEF_CAPTURE_DATA_STAGE_5];

char cFlag_FOPEM_capture_data_stage = DEF_CAPTURE_DATA_COMPLETE;
char FOPEM_dlg_get_capture_data_stage()
{
	return cFlag_FOPEM_capture_data_stage;
}

unsigned long ulPrevLoadedWaveform; // only for WaveformCapturedPerPointForCalibration
unsigned int nNumWaveformCapturedPerPointForCalibration;
double adCaptureForceCmd_gram[MAX_NUM_SAMPLE_CALIBRATION];
double adCaptureForceFb_volt[MAX_NUM_SAMPLE_CALIBRATION];
double adCapturePowerI_amp[MAX_NUM_SAMPLE_CALIBRATION];
double adCapturePowerV_voltUS[MAX_NUM_SAMPLE_CALIBRATION];
double adCapturePowerI_volt[MAX_NUM_SAMPLE_CALIBRATION];
double adCapturePowerV_volt[MAX_NUM_SAMPLE_CALIBRATION];
double adCapturePower_mW[MAX_NUM_SAMPLE_CALIBRATION];
double adCapturePower_Dac[MAX_NUM_SAMPLE_CALIBRATION];
double adCaptureImpedance_ohm[MAX_NUM_SAMPLE_CALIBRATION]; // @5
double adRoundsAverageCaptureGoldenForce[DEF_CAPTURE_DATA_COMPLETE];
double adRoundsAverageCaptureGoldenPower[DEF_CAPTURE_DATA_COMPLETE];

void DlgForcePowerMonitor::RunAutoCalibrateVerifyThread()
{
	cFlag_FOPEM_capture_data_stage = 0;
	int iRetMessageBox = AfxMessageBox(_T(pstrCaptureDataPromptWizard[cFlag_FOPEM_capture_data_stage]), MB_OKCANCEL);
	if(iRetMessageBox = IDOK)
	{
		cFlag_FOPEM_capture_data_stage = cFlag_FOPEM_capture_data_stage + 1;
		if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
		{
			iRetMessageBox = AfxMessageBox(_T(pstrCaptureDataPromptWizard[cFlag_FOPEM_capture_data_stage]), MB_OKCANCEL);
		}
		else  // WB_MACHINE_FPMD_MODE_VERIFY with only one point
		{
			iRetMessageBox = AfxMessageBox(_T(pstrVerifyDataPromptWizard[cFlag_FOPEM_capture_data_stage]), MB_OKCANCEL);			
		}
		if(iRetMessageBox != IDOK)
		{
			cFlag_FOPEM_capture_data_stage = DEF_CAPTURE_DATA_COMPLETE;
		}
		else
		{
			if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
				iRetMessageBox = AfxMessageBox(_T(pstrCaptureGoldenVerifyWizardConfirmKeyInValue[0]));
		}
	}
	else
	{
		cFlag_FOPEM_capture_data_stage = DEF_CAPTURE_DATA_COMPLETE;
	}

	FOPEM_reset_calibrate_checking(); // @2
	ulPrevLoadedWaveform = ulNumLoadedWaveform; // 
	nNumWaveformCapturedPerPointForCalibration = 0; // DEF_NUM_WAVEFORMS_PER_POINT;
	m_cStartAutoCalibrateVerifyThread = TRUE;
	m_pWinThread = AfxBeginThread(AutoCalibrateVerifyThreadProc, this);
	SetPriorityClass(m_pWinThread->m_hThread, THREAD_PRIORITY_TIME_CRITICAL); // @2 REALTIME_PRIORITY_CLASS

	m_pWinThread->m_bAutoDelete = FALSE;
}

UINT DlgForcePowerMonitor::StopAutoCalibrateVerifyThread()
{
	if (m_pWinThread)
	{
		m_cStartAutoCalibrateVerifyThread = FALSE;
		WaitForSingleObject(m_pWinThread->m_hThread, 1000);
		m_pWinThread = NULL;
	}

	return 0;
}

int FOPEM_dlg_get_curr_num_file_capture_calibrate()
{
	return nNumWaveformCapturedPerPointForCalibration;
}
char cFlagSaveFileCaptureVerify = 0;
char FOPEM_dlg_get_save_capture_verify_file_flag()
{
	return cFlagSaveFileCaptureVerify;
}
#include <direct.h>

UINT DlgForcePowerMonitor::AutoCalibrateVerifyThread()
{
int iRetMessageBox;
static int idxSampleInLinearFit;

	while(m_cStartAutoCalibrateVerifyThread == TRUE &&
		m_pWinThread != NULL)
	{
		cFlagSaveFileCaptureVerify = TRUE;
		if(ulNumLoadedWaveform >= ulPrevLoadedWaveform + 1 ||
			(ulNumLoadedWaveform == 0 && ulPrevLoadedWaveform == MAXULONG64)) // Simulation mode
		{
			nNumWaveformCapturedPerPointForCalibration ++;
			ulPrevLoadedWaveform = ulNumLoadedWaveform;
		}
		// @2
		idxSampleInLinearFit = (FOPEM_dlg_get_capture_data_stage() - 1) * DEF_NUM_WAVEFORMS_PER_POINT + 
			FOPEM_dlg_get_curr_num_file_capture_calibrate();

		if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
		{
			adCaptureForceCmd_gram[idxSampleInLinearFit] = astForcePowerValueCalibrate[cFlag_FOPEM_capture_data_stage - 1].dForceCmd_gram;
			adCapturePower_mW[idxSampleInLinearFit] = astForcePowerValueCalibrate[cFlag_FOPEM_capture_data_stage - 1].dPowerCmd_Dac; // @5
		}
		else
		{
			adCaptureForceCmd_gram[idxSampleInLinearFit] = stLoadForcePowerAnalyzeOutput.f1stBondForceGram;
			adCapturePower_mW[idxSampleInLinearFit] = stLoadForcePowerAnalyzeOutput.f1stBondPmW;  // @5
		}
		adCaptureForceFb_volt[idxSampleInLinearFit] = stLoadForcePowerAnalyzeOutput.f1stBondForce;
		adCaptureImpedance_ohm[idxSampleInLinearFit] = stLoadForcePowerAnalyzeOutput.f1stBondImp; // @5

		if(nNumWaveformCapturedPerPointForCalibration == DEF_NUM_WAVEFORMS_PER_POINT)
		{
			cFlagSaveFileCaptureVerify = FALSE;
			nNumWaveformCapturedPerPointForCalibration = 0;
			if(cFlag_FOPEM_capture_data_stage < DEF_CAPTURE_DATA_COMPLETE - 1 &&
				stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
			{
				iRetMessageBox = AfxMessageBox(_T(pstrCaptureDataPromptWizard[cFlag_FOPEM_capture_data_stage + 1]), MB_OKCANCEL);
				if(iRetMessageBox = IDOK)
				{
					cFlag_FOPEM_capture_data_stage = cFlag_FOPEM_capture_data_stage + 1;
					if(cFlag_FOPEM_capture_data_stage <= DEF_CAPTURE_DATA_COMPLETE)
						iRetMessageBox = AfxMessageBox(_T(pstrCaptureGoldenVerifyWizardConfirmKeyInValue[0]));
				}
				else
				{
					cFlag_FOPEM_capture_data_stage = DEF_CAPTURE_DATA_COMPLETE; m_cStartAutoCalibrateVerifyThread = FALSE; // @2
				}
			}
			else if(cFlag_FOPEM_capture_data_stage <= DEF_VERIFY_DATA_COMPLETE - 1&&
				stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
			{
				iRetMessageBox = AfxMessageBox(_T(pstrVerifyDataPromptWizard[cFlag_FOPEM_capture_data_stage + 1]), MB_OKCANCEL);
				if(iRetMessageBox = IDOK)
				{
					cFlag_FOPEM_capture_data_stage = cFlag_FOPEM_capture_data_stage + 1;
					if(cFlag_FOPEM_capture_data_stage <= DEF_VERIFY_DATA_COMPLETE)
						iRetMessageBox = AfxMessageBox(_T(pstrCaptureGoldenVerifyWizardConfirmKeyInValue[0]));
				}
				else
				{
					cFlag_FOPEM_capture_data_stage = DEF_CAPTURE_DATA_COMPLETE; m_cStartAutoCalibrateVerifyThread = FALSE; // @2
				}
			}
			else  // case DEF_CAPTURE_DATA_COMPLETE:
			{				
				cFlag_FOPEM_capture_data_stage = DEF_CAPTURE_DATA_COMPLETE; // @2
				m_cStartAutoCalibrateVerifyThread = FALSE;                  // @2, must put at the VERY beginning of this BLOCK

				FOPEM_CalibrateVerifyThread_CompleteStage_Fitting(); // @2
			}
		}
	}

	UI_EnableButtonComboDuringThread(TRUE);
	UI_SetComboByUserLevel();

	return 0;
}


//CString cstrDisplay;
//void DlgForcePowerMonitor::FOPEM_TimerDisplayStatus()
//{
//	cstrDisplay.Format("%d", cFlagCaptureTrigger);
//	GetDlgItem(IDC_STATIC_CAPTURE_TRIGGER_FLAG_PF_MONITOR_DLG)->SetWindowTextA(cstrDisplay);
//}

extern char strFoldernameCaptureDataCalibration[];
extern char strFoldernameCheckingMachineDriftFOPEM[];

void DlgForcePowerMonitor::FOPEM_CalibrateVerifyThread_CompleteStage_Fitting()
{
static FILE *fptrFileLinearFit, *fptrFileCheckResult;
char strCheckingResultFilename[1024];

int ii;
static FOPEM_CONFIG_FORCE_FACTOR stNewForceFactor;
double dAveForceGram, dStdForceGram, dMaxForceGram, dMinForceGram;
double dAvePower_mW, dStdPower_mW, dMaxPower_mW, dMinPower_mW;
double dAveImpedance_ohm, dStdImpedance_ohm, dMaxImpedance_ohm, dMinImpedance_ohm;
CString cstrCheckResult;

    fptrFileLinearFit = NULL;
	if(stSystemUserInterfaceCfg.nUserLevel >= USER_LEVEL_ENGINEER)
	{
		if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
		{
			if(_chdir("D:\\FPMD_Calibrate"))
			{
				_mkdir("D:\\FPMD_Calibrate");
			}
			fopen_s(&fptrFileLinearFit, "D:\\FPMD_Calibrate\\CalibrateLinear.fit.m", "w"); // @2
			if(fptrFileLinearFit != NULL)
			{
				fprintf_s(fptrFileLinearFit, "%% CmdForceGram Volt\n");
	            
				fprintf_s(fptrFileLinearFit, "PowerCalibrate = [%6.4f, %6.4f;\n",  //@2, Actually it is force, use "PowerCalibrate" to match with matlab code
					adCaptureForceCmd_gram[0], adCaptureForceFb_volt[0]);

				for(ii = 1; ii<MAX_NUM_SAMPLE_CALIBRATION - 1; ii++)
				{
					fprintf_s(fptrFileLinearFit, "%6.4f, %6.4f;\n",
						adCaptureForceCmd_gram[ii], adCaptureForceFb_volt[ii]);
				}
				fprintf_s(fptrFileLinearFit, "%6.4f, %6.4f",
						adCaptureForceCmd_gram[MAX_NUM_SAMPLE_CALIBRATION - 1], adCaptureForceFb_volt[MAX_NUM_SAMPLE_CALIBRATION - 1]);
				fprintf_s(fptrFileLinearFit, "];\n");
			}
		}
		else if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
		{
			dAveForceGram = FOPEM_calc_mean(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCaptureForceCmd_gram);
			dStdForceGram = FOPEM_calc_std_with_ave(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, dAveForceGram, adCaptureForceCmd_gram);
			dMaxForceGram = FOPEM_calc_max(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCaptureForceCmd_gram);
			dMinForceGram = FOPEM_calc_min(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCaptureForceCmd_gram);

			dAvePower_mW = FOPEM_calc_mean(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCapturePower_mW);
			dStdPower_mW = FOPEM_calc_std_with_ave(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, dAvePower_mW, adCapturePower_mW);
			dMaxPower_mW = FOPEM_calc_max(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCapturePower_mW);
			dMinPower_mW = FOPEM_calc_min(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCapturePower_mW);

			dAveImpedance_ohm = FOPEM_calc_mean(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCaptureImpedance_ohm);
			dStdImpedance_ohm = FOPEM_calc_std_with_ave(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, dAveImpedance_ohm, adCaptureImpedance_ohm);
			dMaxImpedance_ohm = FOPEM_calc_max(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCaptureImpedance_ohm);
			dMinImpedance_ohm = FOPEM_calc_min(0, DEF_NUM_WAVEFORMS_PER_POINT - 1, adCaptureImpedance_ohm);

			cstrCheckResult.Format("Statistic over %d sample: \n ----: [Ave, Std, Max, Min] \n Force: [%4.1f, %3.1f, %4.1f, %4.1f]\n Power: [%5.1f, %5.1f, %5.1f, %5.1f] \n Imp: [%4.1f, %4.1f, %4.1f, %4.1f]",
				DEF_NUM_WAVEFORMS_PER_POINT, 
				dAveForceGram, dStdForceGram, dMaxForceGram, dMinForceGram,
				dAvePower_mW, dStdPower_mW, dMaxPower_mW, dMinPower_mW,
				dAveImpedance_ohm, dStdImpedance_ohm, dMaxImpedance_ohm, dMinImpedance_ohm);

			GetDlgItem(IDC_STATIC_DISPLAY_CHECK_RESULT_FOPEM_DLG)->SetWindowTextA(cstrCheckResult);

			if(_chdir("D:\\FPMD_CheckDrift")) // @5
			{
				_mkdir("D:\\FPMD_CheckDrift");
			}
			sprintf_s(strCheckingResultFilename, 1024, "D:\\FPMD_CheckDrift\\%s\\CheckingResult.txt", 
				strFoldernameCheckingMachineDriftFOPEM);

			fopen_s(&fptrFileCheckResult, strCheckingResultFilename, "w"); // @2
			if(fptrFileCheckResult != NULL)
			{
				fprintf_s(fptrFileCheckResult, "%s", cstrCheckResult.GetString());
				fclose(fptrFileCheckResult);
			} // @5
		}
	}

	if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE) // @5
	{
		FOPEM_calibrate_linear_fit_force(&stNewForceFactor, fptrFileLinearFit);
	}

	if(fptrFileLinearFit != NULL)
	{
		fclose(fptrFileLinearFit);
	}

	CString cstrMessageBox;
	int iRetMessageBox;
	if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
	{
		AfxMessageBox(_T(pstrVerifyDataPromptWizard[DEF_VERIFY_DATA_COMPLETE + 1]));
	}
	else
	{
		cstrMessageBox.Format("%s\n Correlation Result for force: [%6.4f, %6.4f] \nOK: Update real-time factors \nCancel: proceed with old factor-[%6.4f, %6.4f]", 
			pstrCaptureDataPromptWizard[DEF_CAPTURE_DATA_COMPLETE],
			stNewForceFactor.fFactorForceRatio_GramPerVolt, stNewForceFactor.fFactorForceRatio_OffsetZeroForce,
			stLoadForcePowerAnalyzeInput.stpOnlineForcePowerSetting->stCfgActualBondForceFactor.fFactorForceRatio_GramPerVolt, stLoadForcePowerAnalyzeInput.stpOnlineForcePowerSetting->stCfgActualBondForceFactor.fFactorForceRatio_OffsetZeroForce
			);
		iRetMessageBox = AfxMessageBox(cstrMessageBox, MB_OKCANCEL);
		if(iRetMessageBox == IDOK)
		{
			stLoadForcePowerAnalyzeInput.stpOnlineForcePowerSetting->stCfgActualBondForceFactor = stNewForceFactor;
			stLoadForcePowerAnalyzeInput.stpOnlineForcePowerSetting->stCfgDummyBondForceFactor = stNewForceFactor;

			FOPEM_set_flag_config_setting_changed(); // @3
		}
	}

}

#include <time.h>
struct tm stTime;
struct tm *stpTimeGlobalVar = &stTime;
static __time64_t stLongTime;
static CString cstrDisplayDateTime;
static	char strTimeDate[128];
void DlgForcePowerMonitor::FOPEM_TimerEvent()
{
static unsigned short nValidDataLen;

	if(m_hDevice!=INVALID_HANDLE_VALUE)
	{
		FOPEM_UpdateReadAd();
		if(nOffsetBondStart > 0)
		{
			nValidDataLen = (nLen - nOffsetBondStart) * 12 * sizeof(int);
			memcpy(&aiBakData[0], &aiData[6*nOffsetBondStart], nValidDataLen);
			memset(&aiBakData[nValidDataLen], 0, (65536 - nValidDataLen));
			FOPEM_TimerEventCalculatePlotAfterCapture();
		}
	}

	//// Update clock
	_time64(&stLongTime);
	_localtime64_s(stpTimeGlobalVar, &stLongTime);

	CString cstrTemp;
	asctime_s(strTimeDate, 128, stpTimeGlobalVar);
	int iDisplayWidthX = GetSystemMetrics(SM_CXSCREEN); // SM_XVIRTUALSCREEN
	int iDisplayHeightY = GetSystemMetrics(SM_CYSCREEN);

	cstrDisplayDateTime.Format("%d x %d, %s ", iDisplayWidthX, iDisplayHeightY, strTimeDate);
	GetDlgItem(IDC_STATIC_FOPEM_DISPLAY_DATE_TIME)->SetWindowTextA(cstrDisplayDateTime);
}
char cFlagPlotFigure = FALSE;

// IDC_BUTTON_PAUSE_PF_MONITOR_DLG
void DlgForcePowerMonitor::OnBnClickedButtonPausePfMonitorDlg()
{
	cFlagPauseCapture = 1 - cFlagPauseCapture;
	if(cFlagPauseCapture==0)
	{
		GetDlgItem(IDC_BUTTON_PAUSE_PF_MONITOR_DLG)->SetWindowTextA(_T("Pause"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PAUSE_PF_MONITOR_DLG)->SetWindowTextA(_T("Restart"));
	}
}

static FILE *fptrSaveSampleFile;
static char strDebugSampleFilePrefix[256];

// IDC_BUTTON_SAVE_PF_MONITOR_DLG
void DlgForcePowerMonitor::OnBnClickedButtonSavePfMonitorDlg()
{
static char strFPDM_SampleDataFilename[512];

	if(FOPEM_get_system_offline_mode() == TRUE)  // @4, only for off-line mode
	{	
			idxNewPointInRolling = ulNumLoadedWaveform % FIG_LENGTH_X_ROLLING_HORIZAN;
			adRollingPower_mW[idxNewPointInRolling] = stLoadForcePowerAnalyzeOutput.f1stBondPmW;
			adRollingImpedance_ohm[idxNewPointInRolling] = stLoadForcePowerAnalyzeOutput.f1stBondImp;
			adRollingForce_gram[idxNewPointInRolling] = stLoadForcePowerAnalyzeOutput.f1stBondForceGram;
			FOPEM_update_rolling_horizan_online();

		if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
		{
			FOPEM_save_checking_waveform();
		}
		else if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
		{
			FOPEM_save_calibration_capture_waveform();
		}
	}
	else 
		// @4, In online-mode, there are 2 cases: 
		// 1. normal case, when the "SAVE" button is clicked, 
		// 2. abnormal case, when 
	{
		FOPEM_FORCE_POWER_VALUE_SAVE_FILE stForcePowerSaveFileLoaded;
		stForcePowerSaveFileLoaded.dForceCmd_gram = stLoadForcePowerAnalyzeOutput.f1stBondForceGram;
		stForcePowerSaveFileLoaded.dPowerCmd_Dac = 
			FOPEM_calc_dac_by_power_with_imp_WB_A_ultra_low(stLoadForcePowerAnalyzeOutput.f1stBondPmW, 
			dGoldenWaveformImpedance);
		stForcePowerSaveFileLoaded.dSampleTime_Fer_us = stSysSettingAdc.tPeriodSample_us;
		stForcePowerSaveFileLoaded.iPowerLevel = WB_MACHINE_POWER_LEVEL_ULTRA_LOW;
		stForcePowerSaveFileLoaded.iPowerMode = WB_MACHINE_POWER_MODE_CONST_I;

		if(_chdir("D:\\FPMD_CheckDrift"))
		{
			_mkdir("D:\\FPMD_CheckDrift");
		}
		if(strDebugSampleFilePrefix[0] != 0)
		{
			sprintf_s(strFPDM_SampleDataFilename, 512, "D:\\FPMD_CheckDrift\\FPMD_SampleData_%s_Y%dM%dD%d_H%dM%d.txt",
				strDebugSampleFilePrefix,
				stpTimeGlobalVar->tm_year +1900, stpTimeGlobalVar->tm_mon +1, stpTimeGlobalVar->tm_mday, stpTimeGlobalVar->tm_hour, stpTimeGlobalVar->tm_min);
		}
		else
		{
			sprintf_s(strFPDM_SampleDataFilename, 512, "D:\\FPMD_CheckDrift\\FPMD_SampleData_Y%dM%dD%d_H%dM%d.txt",
				stpTimeGlobalVar->tm_year +1900, stpTimeGlobalVar->tm_mon +1, stpTimeGlobalVar->tm_mday, stpTimeGlobalVar->tm_hour, stpTimeGlobalVar->tm_min);
		}
		fopen_s(&fptrSaveSampleFile, strFPDM_SampleDataFilename, "w");

		if(fptrSaveSampleFile != NULL)
		{
			FOPEM_save_waveform(fptrSaveSampleFile, &stForcePowerSaveFileLoaded);
		}
	}
}

void DlgForcePowerMonitor::FOPEM_TimerEventCalculatePlotAfterCapture()
{

	// Convert to 6 arrays
	FOPEM_convert_adc_capture_to_load_data();
	// Convert to voltage 
	FOPEM_convert_load_waveform_adc_to_volt();
	// Calculation
	stLoadForcePowerAnalyzeInput.uiDataLen = FOPEM_get_read_data_length();

ANALYZE_FORCE_POWER_OUTPUT stTempLoadForcePowerAnalyzeOutput;
int iRetCalc = RET_ERROR_NO_BONDING_INFO;  // @4
	stTempLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform = 1; // @4

	iRetCalc = FOPEM_calculate_bond_force_power(&stTempLoadForcePowerAnalyzeOutput, // @2 stLoadForcePowerAnalyzeOutput
								  &stLoadForcePowerAnalyzeInput);

	if((iRetCalc == RET_OK_ZERO 
		&& stTempLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform == 0)
		|| (iRetCalc == RET_ERROR_NO_BONDING_INFO 
		&& stTempLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform == RET_ABNORMAL_WAVEFORM_CANNOT_FIND_2ND_B) )// @2 stLoadForcePowerAnalyzeOutput
	{
		stLoadForcePowerAnalyzeOutput = stTempLoadForcePowerAnalyzeOutput; // @2

		if(m_cStartAutoCalibrateVerifyThread == TRUE && FOPEM_dlg_get_save_capture_verify_file_flag() == TRUE)
		{
			if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
			{
				FOPEM_save_checking_waveform();
			}
			else if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
			{
				FOPEM_save_calibration_capture_waveform();
			}
		}
		UI_UpdateActualForcePowerAfterCalculate(
			stLoadForcePowerAnalyzeOutput.stBondCycleTimeIndex.idxStart1stBond,
			stLoadForcePowerAnalyzeOutput.stBondCycleTimeIndex.idxEnd1stBond); // @4
		ulNumLoadedWaveform = ulNumLoadedWaveform + 1; // @4, should be put after saving the files
	}
	else
	{
		UI_ReusePreviousForcePowerForErrorCalculation();
		sprintf_s(strDebugSampleFilePrefix, 256, "CalcErr_%d_%d", 
			iRetCalc, stTempLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform); // @4 record the error code
		if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER) // @4
		{
			OnBnClickedButtonSavePfMonitorDlg();
		}
		strDebugSampleFilePrefix[0] = 0; strDebugSampleFilePrefix[1] = 0; strDebugSampleFilePrefix[2] = 0;
		UI_UpdateActualForcePowerAfterCalculate(
			iRetCalc*1000,	stTempLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform*1000); // @4
	}

	// @ already plotted in UI_UpdateActualForcePowerAfterCalculate
	//// Plot the waveform only for developer
	//if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	//{
	//	cFlagPlotFigure = TRUE;
	//	WB_FOPEM_plot_bond_trig_by_spark();
	//	cFlagPlotFigure = FALSE;
	//}

}

// IDC_BUTTON_LOAD_PF_MONITOR_DLG
void DlgForcePowerMonitor::OnBnClickedButtonLoadPfMonitorDlg()
{
CFileDialog mcFileDlg(TRUE);
CString cstrFilename;
static FOPEM_WAVEFORM_INT stLoadWaveformInt;
char strFileFullname[512];

	if(mcFileDlg.DoModal() == IDOK)
	{
		sprintf_s(strFileFullname, 512, "%s", mcFileDlg.GetPathName());

		stLoadWaveformInt.piEFO =	&aiLoadWaveformEFO[0];
		stLoadWaveformInt.piForce = &aiLoadWaveformForce[0];
		stLoadWaveformInt.piVel = &aiLoadWaveformVel[0];
		stLoadWaveformInt.piPowerUS = &aiLoadWaveformPowerUS[0];
		stLoadWaveformInt.piPowerV = &aiLoadWaveformPowerV[0];
		stLoadWaveformInt.piPowerI = &aiLoadWaveformPowerI[0];

		if(FOPEM_load_waveform(strFileFullname, &stLoadWaveformInt) == RET_OK_ZERO)
		{
			stLoadDataAdcSetting.tPeriodSample_us = stLoadWaveformInt.tSampleFer_us;

			FOPEM_convert_load_waveform_adc_to_volt();
			stLoadForcePowerAnalyzeInput.uiDataLen = FOPEM_get_read_data_length();
			int iCalcRet = RET_ERROR_NO_BONDING_INFO;
			iCalcRet = FOPEM_calculate_bond_force_power(&stLoadForcePowerAnalyzeOutput,
										  &stLoadForcePowerAnalyzeInput);

			nOffsetBondStart = stLoadForcePowerAnalyzeOutput.stBondCycleTimeIndex.idxEndEFO;
			FOPEM_convert_load_data_to_adc_capture();
			iFlagHasAdcCapture = 1;
			nLen = stLoadForcePowerAnalyzeInput.uiDataLen;
			int iCopyLen = (nLen - nOffsetBondStart)*6 * sizeof(int);
			memcpy(aiBakData, &aiData[6*nOffsetBondStart], iCopyLen);
			memset(&aiBakData[iCopyLen], 0, (65536 - iCopyLen));

			if((iCalcRet == RET_OK_ZERO
				&& stLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform == 0)
				|| (iCalcRet == RET_ERROR_NO_BONDING_INFO 
		&& stLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform == RET_ABNORMAL_WAVEFORM_CANNOT_FIND_2ND_B) )// @4
			{

				UI_UpdateActualForcePowerAfterCalculate(
					stLoadForcePowerAnalyzeOutput.stBondCycleTimeIndex.idxStart1stBond,
					stLoadForcePowerAnalyzeOutput.stBondCycleTimeIndex.idxEnd1stBond);
			}
			else
			{
				UI_UpdateActualForcePowerAfterCalculate(
					iCalcRet *1000,
					stLoadForcePowerAnalyzeOutput.iFlagAbnormalWaveform *1000);
			} // @4
		}
	}
}
// IDC_STATIC_FOPEM_DLG_USER_LEVEL
// IDC_STATIC_DLG_FPMD_MODE

void DlgForcePowerMonitor::UI_UpdateActualForcePowerAfterCalculate(int idxStart1stBond_RetErr, int idxEnd1stBond_AbnormalFlag)
{
static int iDacH, iDacL, iDacUL;

//			ulNumLoadedWaveform ++;
	idxNewPointInRolling = ulNumLoadedWaveform % FIG_LENGTH_X_ROLLING_HORIZAN;
	adRollingPower_mW[idxNewPointInRolling] = stLoadForcePowerAnalyzeOutput.f1stBondPmW;
	adRollingImpedance_ohm[idxNewPointInRolling] = stLoadForcePowerAnalyzeOutput.f1stBondImp;
	adRollingForce_gram[idxNewPointInRolling] = stLoadForcePowerAnalyzeOutput.f1stBondForceGram;

	FOPEM_update_rolling_horizan_online();


double dSmoothedPower_mW, dSmoothedForce_gram, dSmoothedImpedance_ohm;  // @5
	dSmoothedPower_mW = adSmoothedRollingPower_mW[idxNewPointInRolling];
	dSmoothedForce_gram = adSmoothedRollingForce_gram[idxNewPointInRolling];
	dSmoothedImpedance_ohm = adSmoothedRollingImpedance_ohm[idxNewPointInRolling]; // @5

	//// @5
	iDacH = FOPEM_calc_dac_by_power_with_imp_WB_A_high(dSmoothedPower_mW, dGoldenWaveformImpedance); // stLoadForcePowerAnalyzeOutput.f1stBondPmW
	iDacL = FOPEM_calc_dac_by_power_with_imp_WB_A_low(dSmoothedPower_mW, dGoldenWaveformImpedance);
	iDacUL = FOPEM_calc_dac_by_power_with_imp_WB_A_ultra_low(dSmoothedPower_mW, dGoldenWaveformImpedance);

			CString cstrDisplay;
			if(stSystemUserInterfaceCfg.nUserLevel >= USER_LEVEL_ENGINEER // USER_LEVEL_DEVELOPER
				)
			{
				cstrDisplay.Format("Power(mW)= %4.0f, Dac: %dH,%dL,%dUL; Imp= %4.1f", 
					dSmoothedPower_mW, iDacH, iDacL, iDacUL,
					dSmoothedImpedance_ohm); // stLoadForcePowerAnalyzeOutput.f1stBondPmW, stLoadForcePowerAnalyzeOutput.f1stBondImp
			}
			else // 				cstrDisplay.Format("Power(mW) = %4.0f, Imp(Ohm)= %4.1f", stLoadForcePowerAnalyzeOutput.f1stBondPmW, stLoadForcePowerAnalyzeOutput.f1stBondImp);
			{
				cstrDisplay.Format("Power(mW) = %4.0f, Dac: %dH,%dL,%dUL;", dSmoothedPower_mW,
					iDacH, iDacL, iDacUL); // stLoadForcePowerAnalyzeOutput.f1stBondPmW
			}

			// , Imp(Ohm)]= [% %4.1f, stLoadForcePowerAnalyzeOutput.f1stBondImp
			GetDlgItem(IDC_STATIC_BOND_POWER_MONITOR_1STBOND)->SetWindowTextA(cstrDisplay);

			cstrDisplay.Format("Force(g)= %4.0f", dSmoothedForce_gram); // stLoadForcePowerAnalyzeOutput.f1stBondForceGram
			GetDlgItem(IDC_STATIC_BOND_FORCE_MONITOR_1STBOND)->SetWindowTextA(cstrDisplay);

			cstrDisplay.Format("ms: %d\\ %d. %d/%d, [%d, %d]", 
				(int)(stGoldenForcePowerAnalyzeInput.uiDataLen * stGoldenWaveformAdcSetting.tPeriodSample_us/1000),
				(int)(stLoadForcePowerAnalyzeInput.uiDataLen * stLoadDataAdcSetting.tPeriodSample_us/1000),
				nNumWaveformCapturedPerPointForCalibration,
				ulNumLoadedWaveform, 
				(int)(idxStart1stBond_RetErr * stLoadDataAdcSetting.tPeriodSample_us/1000),
				(int)(idxEnd1stBond_AbnormalFlag * stLoadDataAdcSetting.tPeriodSample_us/1000)
				);
			GetDlgItem(IDC_STATIC_CAPTURE_TRIGGER_FLAG_PF_MONITOR_DLG)->SetWindowTextA(cstrDisplay);

			if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
			{
				UI_UpdateCheckingResult();
			}

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		WB_FOPEM_plot_bond_trig_by_spark();
	}

}

void DlgForcePowerMonitor::UI_ReusePreviousForcePowerForErrorCalculation()
{
	idxNewPointInRolling = ulNumLoadedWaveform % FIG_LENGTH_X_ROLLING_HORIZAN;
int idxPrevPoint = (ulNumLoadedWaveform - 1) % FIG_LENGTH_X_ROLLING_HORIZAN;
	adRollingPower_mW[idxNewPointInRolling] = adRollingPower_mW[idxPrevPoint]; // stLoadForcePowerAnalyzeOutput.f1stBondPmW;
	adRollingImpedance_ohm[idxNewPointInRolling] = adRollingImpedance_ohm[idxPrevPoint]; // stLoadForcePowerAnalyzeOutput.f1stBondImp;
	adRollingForce_gram[idxNewPointInRolling] = adRollingForce_gram[idxPrevPoint]; // stLoadForcePowerAnalyzeOutput.f1stBondForceGram;
}
unsigned int FPMD_get_password_mm_dd()
{
	unsigned int uiRet = 5588;

	return uiRet;
}

#include "DlgKeyInputPad.h"
char FPMD_CheckPasswordByUserLevel(unsigned int uiPass)
{
char cRet = RET_OK_ZERO;
	static CDlgKeyInputPad cDlgInputPad;
	cDlgInputPad.SetFlagShowNumberOnKeyPad(0);
	cDlgInputPad.SetInputNumber(0);

//	switch
	if(cDlgInputPad.DoModal() == IDOK) //  20120906
	{
		if(cDlgInputPad.GetReturnNumber()== FPMD_get_password_mm_dd())
		{

		}
	}

	return cRet;
}

#define FPMD_USER_LEVEL_SERVICE_PASSWORD   (666888)
#define FPMD_USER_LEVEL_ENGINEER_PASSWORD  (12500)
#define FPMD_USER_LEVEL_DEVELOPER_PASSWORD (9668)
static CDlgKeyInputPad cDlgInputPad;

// IDC_COMBO_USER_LEVEL_PF_MONITOR_DLG
void DlgForcePowerMonitor::OnCbnSelchangeComboUserLevelPfMonitorDlg()
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_USER_LEVEL_PF_MONITOR_DLG);
	unsigned int nTempUserLevel = pCombo->GetCurSel();

	if(nTempUserLevel > stSystemUserInterfaceCfg.nUserLevel)
	{
		cDlgInputPad.SetFlagShowNumberOnKeyPad(0);
		cDlgInputPad.SetInputNumber(0);
		if(cDlgInputPad.DoModal() == IDOK)
		{
			if((cDlgInputPad.GetReturnNumber()== FPMD_USER_LEVEL_SERVICE_PASSWORD &&
				nTempUserLevel == USER_LEVEL_SERVICE)
			    || 
				(cDlgInputPad.GetReturnNumber()== FPMD_USER_LEVEL_ENGINEER_PASSWORD &&
				nTempUserLevel == USER_LEVEL_ENGINEER)
				|| 
				(cDlgInputPad.GetReturnNumber()== FPMD_USER_LEVEL_DEVELOPER_PASSWORD &&
				nTempUserLevel == USER_LEVEL_DEVELOPER)	
				)
			{
				stSystemUserInterfaceCfg.nUserLevel = nTempUserLevel;
			}
			else
			{
				pCombo->SetCurSel(stSystemUserInterfaceCfg.nUserLevel);
			}
		}
		else
		{
			pCombo->SetCurSel(stSystemUserInterfaceCfg.nUserLevel);
		}
	}
	else
	{
		stSystemUserInterfaceCfg.nUserLevel = nTempUserLevel;
	}
	FOPEM_set_combo_by_user_level();
	UI_SetComboByUserLevel();
	FOPEM_InitDisplayFig();
}
// IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG
void DlgForcePowerMonitor::OnCbnSelchangeCcomboRunningModePfMonitorDlg()
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG);
	stSystemUserInterfaceCfg.nWbMachineRunningMode = pCombo->GetCurSel();
}
// IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG
void DlgForcePowerMonitor::OnCbnSelchangeCcomboPowerModePfMonitorDlg()
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG);
	stSystemUserInterfaceCfg.nWbMachinePowerMode = pCombo->GetCurSel();
}

// ID_BTN_EXIT_FP_MONITOR_DLG
void DlgForcePowerMonitor::OnBnClickedBtnExitFpMonitorDlg()
{
	cFlagShowWindow = 0;
	StopAutoCalibrateVerifyThread();

	CString cstrMessageBox;
	int iRetMessageBox;
	// 
	if(FOPEM_get_flag_config_setting_changed() != RET_ZERO_CONFIG_SETTING_NOT_CHANGE)
	{
		cstrMessageBox.Format("Config changed \nOK: Save config file \nCancel: Discard changes and keep original config file");
		iRetMessageBox = AfxMessageBox(cstrMessageBox, MB_OKCANCEL);
		if(iRetMessageBox == IDOK)
		{
			FOPEM_save_config_to_file();
		}
	}

	OnOK();

	Sleep(500);
	FOPEM_close_device();
	exit(0);
}
// IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM
void DlgForcePowerMonitor::OnBnClickedCheckFpmdFlagShownGoldenWaveform()
{
	iFlagShowGoldenWaveform = ((CButton*)GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM))->GetCheck();
	if(iFlagHasGoldenWaveform == 1 && iFlagShowGoldenWaveform== 1 &&
		stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		FOPEM_PlotGoldenWaveform();
	}
}

// IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG
void DlgForcePowerMonitor::OnCbnSelchangeComboVerifyModePfMonitorDlg()
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG);
	stSystemUserInterfaceCfg.nFPMD_VerifyMode = pCombo->GetCurSel();

	UI_SetButtonByVerifyMode();
}

void DlgForcePowerMonitor::UI_SetButtonByVerifyMode()
{
	if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE)
	{
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BUTTON_FOPEM_DLG_AUTO_CHECK_CALIBRATE)->SetWindowTextA(_T("Start"));
	}
	//else
	//{
	//	GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->EnableWindow(FALSE);
	//}

	if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
	{
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG)->EnableWindow(TRUE); // @3
		GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG)->EnableWindow(TRUE); // @3
		GetDlgItem(IDC_STATIC_DISPLAY_CHECK_RESULT_FOPEM_DLG)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_DLG_FPMD_VERIFY_RESULT)->ShowWindow(TRUE);
		if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
		{
			GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG)->EnableWindow(FALSE); // @3
		GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG)->EnableWindow(FALSE); // @3
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_DISPLAY_CHECK_RESULT_FOPEM_DLG)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DLG_FPMD_VERIFY_RESULT)->ShowWindow(FALSE);
	}

	if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE ||
		stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
	{
		GetDlgItem(IDC_BUTTON_FOPEM_DLG_AUTO_CHECK_CALIBRATE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_FOPEM_DLG_AUTO_CHECK_CALIBRATE)->EnableWindow(FALSE);
	}

}
void DlgForcePowerMonitor::UI_EnableButtonComboDuringThread(BOOL bFlag)
{
	GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->EnableWindow(bFlag);
	GetDlgItem(ID_BTN_EXIT_FP_MONITOR_DLG)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_PAUSE_PF_MONITOR_DLG)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_USER_LEVEL_PF_MONITOR_DLG)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_FOPEM_DLG_AUTO_CHECK_CALIBRATE)->EnableWindow(bFlag);
}

void DlgForcePowerMonitor::UI_SetComboByUserLevel()
{
		// IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG);
	pCombo->SetCurSel(stSystemUserInterfaceCfg.nWbMachineRunningMode);

		// IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG
	pCombo = (CComboBox *)GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG);
	pCombo->SetCurSel(stSystemUserInterfaceCfg.nWbMachinePowerMode);

	   // IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG
	pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG);
	pCombo->SetCurSel(stSystemUserInterfaceCfg.nFPMD_VerifyMode);

	if(stSystemUserInterfaceCfg.nUserLevel >= USER_LEVEL_ENGINEER)
	{
		GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->EnableWindow(TRUE);
	}
	else if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_SERVICE)
	{
		GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->EnableWindow(TRUE);
	}
	else // USER_LEVEL_OPERATOR
	{
		GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG)->EnableWindow(FALSE); // @3
		GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG)->EnableWindow(FALSE); // @3
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_DISPLAY_CHECK_RESULT_FOPEM_DLG)->ShowWindow(FALSE);

		FOPEM_remove_rolling_horizan_upp_low_threshold_line();
	}


	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		GetDlgItem(IDC_STATIC_BOND_POWER_MONITOR_1STBOND)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_IMPEDANCE)->ShowWindow(1);
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_CHECK_THRESHOLD_IMPEDANCE)->ShowWindow(1);

		GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG)->ShowWindow(0);
		GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_BOND_FORCE_MONITOR_1STBOND)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_GOLDEN_WAVEFORM_STATISTIC)->ShowWindow(1);
		GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->ShowWindow(1);
	}
	else if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_ENGINEER)
	{
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->ShowWindow(1);
	}
	else
	{
//		GetDlgItem(IDC_STATIC_BOND_POWER_MONITOR_1STBOND)->ShowWindow(0);
//		GetDlgItem(IDC_STATIC_BOND_FORCE_MONITOR_1STBOND)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_IMPEDANCE)->ShowWindow(0);
		GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_CHECK_THRESHOLD_IMPEDANCE)->ShowWindow(0);

		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->ShowWindow(0);
		GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->ShowWindow(0);
		GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG)->ShowWindow(0);
		GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_GOLDEN_WAVEFORM_STATISTIC)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM)->ShowWindow(0);

//		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_IMPEDANCE)->ShowWindow(0);
		FOPEM_remove_rolling_horizan_golden_line_impedance(); // @1
		FOPEM_remove_rolling_horizan_upp_low_threshold_line_impedance();  // @1
	}
}

// IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM
void DlgForcePowerMonitor::OnCbnSelchangeComboFpmdVerifyThresholdForceGram()
{
	stFOPEM_CheckingThreshold.idxForceThComboSel = ((CComboBox *)GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM))->GetCurSel();
	stFOPEM_CheckingThreshold.dForceTh_gram = adCheckingForceThreshold[stFOPEM_CheckingThreshold.idxForceThComboSel];

	FOPEM_remove_rolling_horizan_golden_line_force();
	FOPEM_remove_rolling_horizan_upp_low_threshold_line_force();

	FOPEM_update_peak_value_in_rolling_horizan();
	FOPEM_plot_rolling_horizan_peak_value();

	FOPEM_plot_rolling_horizan_golden_line_force();
	FOPEM_plot_rolling_horizan_upp_low_threshold_line_force();

	FOPEM_set_flag_config_setting_changed(); // @3
}
// IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW
void DlgForcePowerMonitor::OnCbnSelchangeComboFpmdVerifyThresholdPowermw()
{
	stFOPEM_CheckingThreshold.idxPowerThComboSel = ((CComboBox *)GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW))->GetCurSel();
	stFOPEM_CheckingThreshold.dPowerTh_mW = adCheckingPowerThreshold[stFOPEM_CheckingThreshold.idxPowerThComboSel];

	FOPEM_remove_rolling_horizan_golden_line_power();
	FOPEM_remove_rolling_horizan_upp_low_threshold_line_power();

	FOPEM_update_peak_value_in_rolling_horizan();
	FOPEM_plot_rolling_horizan_peak_value();

	FOPEM_plot_rolling_horizan_golden_line_power();
	FOPEM_plot_rolling_horizan_upp_low_threshold_line_power();

	FOPEM_set_flag_config_setting_changed(); // @3
}
// IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM
void DlgForcePowerMonitor::OnCbnSelchangeComboFpmdVerifyThresholdImpedanceOhm()
{
	stFOPEM_CheckingThreshold.idxImpedanceThComboSel = ((CComboBox *)GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM))->GetCurSel();
	stFOPEM_CheckingThreshold.dImpedanceTh_ohm = adCheckingImpedanceThreshold[stFOPEM_CheckingThreshold.idxImpedanceThComboSel];

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		FOPEM_remove_rolling_horizan_golden_line_impedance();
		FOPEM_remove_rolling_horizan_upp_low_threshold_line_impedance();
	}
	FOPEM_update_peak_value_in_rolling_horizan();
	FOPEM_plot_rolling_horizan_peak_value();

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		FOPEM_plot_rolling_horizan_golden_line_impedance();
		FOPEM_plot_rolling_horizan_upp_low_threshold_line_impedance();
	}
	FOPEM_set_flag_config_setting_changed(); // @3
}

void DlgForcePowerMonitor::UI_InitComboButtonDlgFOPEM()
{

	if(stSystemUserInterfaceCfg.nUserLevel >= USER_LEVEL_ENGINEER) // @2 USER_LEVEL_DEVELOPER
	{
		// IDC_STATIC_BOND_FORCE_MONITOR_1STBOND
		GetDlgItem(IDC_STATIC_BOND_FORCE_MONITOR_1STBOND)->SetWindowTextA(_T("Force: "));
		// IDC_STATIC_BOND_POWER_MONITOR_1STBOND

		GetDlgItem(IDC_STATIC_BOND_POWER_MONITOR_1STBOND)->SetWindowTextA(_T("[Power(mW), Imp(Ohm)]: "));
	}

	// Initialize the combos
		// IDC_COMBO_USER_LEVEL_PF_MONITOR_DLG
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_USER_LEVEL_PF_MONITOR_DLG);
	pCombo->InsertString(USER_LEVEL_OPERATOR, _T("Operator"));
	pCombo->InsertString(USER_LEVEL_SERVICE, _T("Process"));
	pCombo->InsertString(USER_LEVEL_ENGINEER, _T("Engineer"));
	pCombo->InsertString(USER_LEVEL_DEVELOPER, _T("Developer"));
	pCombo->SetCurSel(stSystemUserInterfaceCfg.nUserLevel);

		// IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG
	pCombo = (CComboBox *)GetDlgItem(IDC_CCOMBO_RUNNING_MODE_PF_MONITOR_DLG);
	pCombo->InsertString(WB_MACHINE_RUNNING_ACTUAL_BOND, _T("AutoBond"));
	pCombo->InsertString(WB_MACHINE_RUNNING_DUMMY_BOND, _T("DummyBond"));
	pCombo->SetCurSel(stSystemUserInterfaceCfg.nWbMachineRunningMode);

		// IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG
	pCombo = (CComboBox *)GetDlgItem(IDC_CCOMBO_POWER_MODE_PF_MONITOR_DLG);
	pCombo->InsertString(WB_MACHINE_POWER_MODE_CONST_I, _T("ConstI"));
	pCombo->InsertString(WB_MACHINE_POWER_MODE_CONST_V, _T("ConstV"));
	pCombo->InsertString(WB_MACHINE_POWER_MODE_CONST_P, _T("ConstP"));
	pCombo->InsertString(WB_MACHINE_POWER_MODE_NORMAL, _T("Normal"));
	pCombo->SetCurSel(stSystemUserInterfaceCfg.nWbMachinePowerMode);

	   // IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG
	pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_VERIFY_MODE_PF_MONITOR_DLG);
	pCombo->InsertString(WB_MACHINE_POWER_MODE_CONST_I, _T("VIEW"));
	pCombo->InsertString(WB_MACHINE_POWER_MODE_CONST_V, _T("VERIFY"));
	pCombo->InsertString(WB_MACHINE_POWER_MODE_CONST_P, _T("CAPTURE GOLD"));
	pCombo->SetCurSel(stSystemUserInterfaceCfg.nFPMD_VerifyMode);

	int ii;
	//IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM
	pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_FORCE_GRAM);
	for(ii = 0; ii<TOTAL_LEVEL_DETECTION_FORCE_TH; ii++)
	{
		pCombo->InsertString(ii, pstrCheckingForceThreshold[ii]);
	}
	pCombo->SetCurSel(stFOPEM_CheckingThreshold.idxForceThComboSel);

	//IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW
	pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_POWER_mW);
	for(ii = 0; ii<TOTAL_LEVEL_DETECTION_POWER_TH; ii++)
	{
		pCombo->InsertString(ii, pstrCheckingPowerThreshold[ii]);
	}
	pCombo->SetCurSel(stFOPEM_CheckingThreshold.idxPowerThComboSel);

	//IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM
	pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_FPMD_VERIFY_THRESHOLD_IMPEDANCE_OHM);
	for(ii = 0; ii<TOTAL_LEVEL_DETECTION_IMPEDANCE_TH; ii++)
	{
		pCombo->InsertString(ii, pstrCheckingImpedanceThreshold[ii]);
	}
	pCombo->SetCurSel(stFOPEM_CheckingThreshold.idxImpedanceThComboSel);

	cFlagCaptureTrigger = FALSE;
	((CButton*)GetDlgItem(IDC_CHECK_FPMD_FLAG_SHOWN_GOLDEN_WAVEFORM))->SetCheck(iFlagShowGoldenWaveform);
}
extern FOPEM_VALUE_REAL_TIME stForcePowerRollingHorizan;


void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_smoothed_new_sample()
// Function dependency variables:
// idxPrevPointRolling, idxNewPointInRolling
{
CPen *pOldPen;
CClientDC dc(this);
int iPrevPosnY, iNewPosnY;
int ppNewIdx, ppNextIdx;
int nNumEraseSample;

	///// Threshold with solid Red line
CPen penGreenActualLine(PS_SOLID,4,RGB(0, 255, 0));  // @5
	pOldPen = dc.SelectObject(&penGreenActualLine);

	CPen penBlackClearLine(PS_SOLID,4,RGB(0, 0, 0)); // @5

	// Force in Fig.3
	iPrevPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adSmoothedRollingForce_gram[idxPrevPointRolling]/stForcePowerRollingHorizan.dPeakForce_gram);  // adRollingForce_gram
	iNewPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adSmoothedRollingForce_gram[idxNewPointInRolling]/stForcePowerRollingHorizan.dPeakForce_gram); // adRollingForce_gram
	dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X + idxPrevPointRolling, iPrevPosnY);
	dc.LineTo(FIG3_ROLLING_HORIZAN_LOW_X + idxNewPointInRolling, iNewPosnY);

	// Power in Fig.4
	iPrevPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adSmoothedRollingPower_mW[idxPrevPointRolling]/stForcePowerRollingHorizan.dPeakPower_mW);  // adRollingPower_mW
	iNewPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adSmoothedRollingPower_mW[idxNewPointInRolling]/stForcePowerRollingHorizan.dPeakPower_mW); // adRollingPower_mW
	dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X + idxPrevPointRolling, iPrevPosnY);
	dc.LineTo(FIG4_ROLLING_HORIZAN_LOW_X + idxNewPointInRolling, iNewPosnY);

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		// Impedance in Fig.5
		iPrevPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
				* adSmoothedRollingImpedance_ohm[idxPrevPointRolling]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
		iNewPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
				* adSmoothedRollingImpedance_ohm[idxNewPointInRolling]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
		dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X + idxPrevPointRolling, iPrevPosnY);
		dc.LineTo(FIG5_ROLLING_HORIZAN_LOW_X + idxNewPointInRolling, iNewPosnY);
	}
	if(idxNewPointInRolling == FIG_LENGTH_X_ROLLING_HORIZAN - 1)
	{
		idxPrevPointRolling = 0;
	}
	else
	{
		idxPrevPointRolling = idxNewPointInRolling;
	}
	if(iFlagActiveRolling == 0 && 
		(idxNewPointInRolling >= FIG_LENGTH_X_ROLLING_HORIZAN - 5))
	{
		iFlagActiveRolling = 1;
	}
	else if(iFlagActiveRolling == 1)
	{
		dc.SelectObject(&penBlackClearLine);
		ppNewIdx = idxNewPointInRolling;
		nNumEraseSample = FIG_LENGTH_X_ROLLING_HORIZAN/20; // @5

		for(int ii = 0; ii<nNumEraseSample; ii++)  // @5
		{
			if(ppNewIdx == FIG_LENGTH_X_ROLLING_HORIZAN - 1) ppNewIdx = 0;
			ppNextIdx = ppNewIdx + 1;

			// Force in Fig.3
			iPrevPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adSmoothedRollingForce_gram[ppNewIdx]/stForcePowerRollingHorizan.dPeakForce_gram);
			iNewPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adSmoothedRollingForce_gram[ppNextIdx]/stForcePowerRollingHorizan.dPeakForce_gram);
			dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X + ppNewIdx, iPrevPosnY);
			dc.LineTo(FIG3_ROLLING_HORIZAN_LOW_X + ppNextIdx, iNewPosnY);

			// Power in Fig.4
			iPrevPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adSmoothedRollingPower_mW[ppNewIdx]/stForcePowerRollingHorizan.dPeakPower_mW);
			iNewPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adSmoothedRollingPower_mW[ppNextIdx]/stForcePowerRollingHorizan.dPeakPower_mW);
			dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X + ppNewIdx, iPrevPosnY);
			dc.LineTo(FIG4_ROLLING_HORIZAN_LOW_X + ppNextIdx, iNewPosnY);

			// Impedance in Fig.5
			if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
			{
				iPrevPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
						* adSmoothedRollingImpedance_ohm[ppNewIdx]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
				iNewPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
						* adSmoothedRollingImpedance_ohm[ppNextIdx]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
				dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X + ppNewIdx, iPrevPosnY);
				dc.LineTo(FIG5_ROLLING_HORIZAN_LOW_X + ppNextIdx, iNewPosnY);
			}
			ppNewIdx ++;
		}
	}

	dc.SelectObject(pOldPen);
}

//
void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_new_sample()
// Function dependency variables:
// idxPrevPointRolling, idxNewPointInRolling
{
CPen *pOldPen;
CClientDC dc(this);
int iPrevPosnY, iNewPosnY;
int ppNewIdx, ppNextIdx;
	///// Threshold with solid Red line
CPen penGreenActualLine(PS_SOLID,2,RGB(0, 255, 0));  
	pOldPen = dc.SelectObject(&penGreenActualLine);

	CPen penBlackClearLine(PS_SOLID,2,RGB(0, 0, 0));

	// Force in Fig.3
	iPrevPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adRollingForce_gram[idxPrevPointRolling]/stForcePowerRollingHorizan.dPeakForce_gram);  // adRollingForce_gram
	iNewPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adRollingForce_gram[idxNewPointInRolling]/stForcePowerRollingHorizan.dPeakForce_gram); // adRollingForce_gram
	dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X + idxPrevPointRolling, iPrevPosnY);
	dc.LineTo(FIG3_ROLLING_HORIZAN_LOW_X + idxNewPointInRolling, iNewPosnY);

	// Power in Fig.4
	iPrevPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adRollingPower_mW[idxPrevPointRolling]/stForcePowerRollingHorizan.dPeakPower_mW);  // adRollingPower_mW
	iNewPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* adRollingPower_mW[idxNewPointInRolling]/stForcePowerRollingHorizan.dPeakPower_mW); // adRollingPower_mW
	dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X + idxPrevPointRolling, iPrevPosnY);
	dc.LineTo(FIG4_ROLLING_HORIZAN_LOW_X + idxNewPointInRolling, iNewPosnY);

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		// Impedance in Fig.5
		iPrevPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
				* adRollingImpedance_ohm[idxPrevPointRolling]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
		iNewPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
				* adRollingImpedance_ohm[idxNewPointInRolling]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
		dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X + idxPrevPointRolling, iPrevPosnY);
		dc.LineTo(FIG5_ROLLING_HORIZAN_LOW_X + idxNewPointInRolling, iNewPosnY);
	}
	if(idxNewPointInRolling == FIG_LENGTH_X_ROLLING_HORIZAN - 1)
	{
		idxPrevPointRolling = 0;
	}
	else
	{
		idxPrevPointRolling = idxNewPointInRolling;
	}
	if(iFlagActiveRolling == 0 && 
		(idxNewPointInRolling >= FIG_LENGTH_X_ROLLING_HORIZAN - 5))
	{
		iFlagActiveRolling = 1;
	}
	else if(iFlagActiveRolling == 1)
	{
		dc.SelectObject(&penBlackClearLine);
		ppNewIdx = idxNewPointInRolling;
		for(int ii = 0; ii<6; ii++)
		{
			if(ppNewIdx == FIG_LENGTH_X_ROLLING_HORIZAN - 1) ppNewIdx = 0;
			ppNextIdx = ppNewIdx + 1;

			// Force in Fig.3
			iPrevPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adRollingForce_gram[ppNewIdx]/stForcePowerRollingHorizan.dPeakForce_gram);
			iNewPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adRollingForce_gram[ppNextIdx]/stForcePowerRollingHorizan.dPeakForce_gram);
			dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X + ppNewIdx, iPrevPosnY);
			dc.LineTo(FIG3_ROLLING_HORIZAN_LOW_X + ppNextIdx, iNewPosnY);

			// Power in Fig.4
			iPrevPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adRollingPower_mW[ppNewIdx]/stForcePowerRollingHorizan.dPeakPower_mW);
			iNewPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
					* adRollingPower_mW[ppNextIdx]/stForcePowerRollingHorizan.dPeakPower_mW);
			dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X + ppNewIdx, iPrevPosnY);
			dc.LineTo(FIG4_ROLLING_HORIZAN_LOW_X + ppNextIdx, iNewPosnY);

			// Impedance in Fig.5
			if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
			{
				iPrevPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
						* adRollingImpedance_ohm[ppNewIdx]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
				iNewPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
						* adRollingImpedance_ohm[ppNextIdx]/stForcePowerRollingHorizan.dPeakImpedance_ohm);
				dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X + ppNewIdx, iPrevPosnY);
				dc.LineTo(FIG5_ROLLING_HORIZAN_LOW_X + ppNextIdx, iNewPosnY);
			}
			ppNewIdx ++;
		}
	}

	dc.SelectObject(pOldPen);
}

// Fix power display 400mW
void DlgForcePowerMonitor::FOPEM_update_rolling_horizan_online()
{
	// calculate the peak value
	if(adRollingPower_mW[idxNewPointInRolling] > stForcePowerRollingHorizan.dPeakPower_mW)
	{
//		FOPEM_remove_rolling_horizan_golden_line_power();
//		FOPEM_remove_rolling_horizan_upp_low_threshold_line_power();

		stForcePowerRollingHorizan.dPeakPower_mW = _MAX_POWER_DISPLAY_MW_DEF_FIG4_; // adRollingPower_mW[idxNewPointInRolling];
		adRollingPower_mW[idxNewPointInRolling] = _MAX_POWER_DISPLAY_MW_DEF_FIG4_;
//		FOPEM_plot_rolling_horizan_peak_value();

//		FOPEM_plot_rolling_horizan_golden_line_power();
//		FOPEM_remove_rolling_horizan_golden_line_power();
	}

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		if(adRollingImpedance_ohm[idxNewPointInRolling] > stForcePowerRollingHorizan.dPeakImpedance_ohm)
		{
//			FOPEM_remove_rolling_horizan_golden_line_impedance();
//			FOPEM_remove_rolling_horizan_upp_low_threshold_line_impedance();

			stForcePowerRollingHorizan.dPeakImpedance_ohm = _MAX_IMPEDANCE_DISPLAY_OHM_DEF_FIG5_; // adRollingImpedance_ohm[idxNewPointInRolling];
			adRollingImpedance_ohm[idxNewPointInRolling] = _MAX_IMPEDANCE_DISPLAY_OHM_DEF_FIG5_;
//			FOPEM_plot_rolling_horizan_peak_value();

//			FOPEM_plot_rolling_horizan_golden_line_impedance();
//			FOPEM_remove_rolling_horizan_golden_line_impedance();
		}
	}
	if(adRollingForce_gram[idxNewPointInRolling] > stForcePowerRollingHorizan.dPeakForce_gram)
	{
//		FOPEM_remove_rolling_horizan_golden_line_force();
//		FOPEM_remove_rolling_horizan_upp_low_threshold_line_force();

		stForcePowerRollingHorizan.dPeakForce_gram = _MAX_FORCE_DISPLAY_GRAM_DEF_FIG3_; // adRollingForce_gram[idxNewPointInRolling];
		adRollingForce_gram[idxNewPointInRolling] = _MAX_FORCE_DISPLAY_GRAM_DEF_FIG3_;
		FOPEM_plot_rolling_horizan_peak_value();

//		FOPEM_plot_rolling_horizan_golden_line_force();
//		FOPEM_remove_rolling_horizan_golden_line_force();
	}

	// if > current peak
	//    remove the goldline
	//    update the peak
	//    re-plot the actual line
	// plot new sample of actual line

#define DEF_POWER_FORCE_ROLLING_HORIZAN_MOV_AVE_LEN 16
	FOPEM_calc_smooth_force_power_impedance(DEF_POWER_FORCE_ROLLING_HORIZAN_MOV_AVE_LEN); // Moving Average
//	FOPEM_plot_rolling_horizan_new_sample();
	FOPEM_plot_rolling_horizan_smoothed_new_sample();
}

CString cstrDisplayMaxValue;
void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_peak_value()
{
	// IDC_STATIC_ROLLING_DISPLAY_MAX_FORCE
	cstrDisplayMaxValue.Format("%3.0f g", stForcePowerRollingHorizan.dUpperBoundThForce_gram);
	GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_FORCE)->SetWindowTextA(cstrDisplayMaxValue);

	cstrDisplayMaxValue.Format("%3.0f g", (stForcePowerRollingHorizan.dLowerBoundThForce_gram + stForcePowerRollingHorizan.dUpperBoundThForce_gram)/2.0);
	GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_GOLD_FORCE)->SetWindowTextA(cstrDisplayMaxValue);

	cstrDisplayMaxValue.Format("%3.0f g", stForcePowerRollingHorizan.dLowerBoundThForce_gram);
	GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_LOW_THRESHOLD_FORCE)->SetWindowTextA(cstrDisplayMaxValue);

	// IDC_STATIC_ROLLING_DISPLAY_MAX_POWER_mW
	cstrDisplayMaxValue.Format("%4.0f mW", stForcePowerRollingHorizan.dUpperBoundThPower_mW);
	GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_POWER_mW)->SetWindowTextA(cstrDisplayMaxValue);


	cstrDisplayMaxValue.Format("%4.0f mW", (stForcePowerRollingHorizan.dUpperBoundThPower_mW + stForcePowerRollingHorizan.dLowerBoundThPower_mW)/2.0);
	GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_GOLD_POWER_mW)->SetWindowTextA(cstrDisplayMaxValue);

	
	cstrDisplayMaxValue.Format("%4.0f mW", stForcePowerRollingHorizan.dLowerBoundThPower_mW);
	GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MIN_POWER_mW)->SetWindowTextA(cstrDisplayMaxValue);


	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		// IDC_STATIC_ROLLING_DISPLAY_MAX_IMPEDANCE
		cstrDisplayMaxValue.Format("%5.0f ohm", stForcePowerRollingHorizan.dUpperBoundThImpedance_ohm); // dPeakImpedance_ohm
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_IMPEDANCE)->SetWindowTextA(cstrDisplayMaxValue);
	}
}

void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_golden_line_force()
{
	// Bright Califonia Gold 
    CClientDC dc(this);
	CPen penBrightCaliforniaGold(PS_DOT,1,RGB(254, 157, 4));    
	CPen*pOldPen=dc.SelectObject(&penBrightCaliforniaGold);
	int iPosnY;
	if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
	{
		iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - 
			FIG_HEIGHT_Y_ROLLING_HORIZAN * stGoldenForcePowerAnalyzeOutput.f1stBondForceGram/stForcePowerRollingHorizan.dPeakForce_gram);
		if( iPosnY < FIG3_ROLLING_HORIZAN_LOW_Y ) 
			iPosnY = FIG3_ROLLING_HORIZAN_LOW_Y;
		else if( iPosnY > FIG3_ROLLING_HORIZAN_UPP_Y )
			iPosnY = FIG3_ROLLING_HORIZAN_UPP_Y;
		dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG3_ROLLING_HORIZAN_UPP_X, iPosnY);	
	}
	dc.SelectObject(pOldPen);
}
void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_golden_line_power()
{
	// Bright Califonia Gold 
    CClientDC dc(this);
	CPen penBrightCaliforniaGold(PS_DOT,1,RGB(254, 157, 4));    
	CPen*pOldPen=dc.SelectObject(&penBrightCaliforniaGold);
	int iPosnY;
	if(stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
	{
		iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - 
			FIG_HEIGHT_Y_ROLLING_HORIZAN * stGoldenForcePowerAnalyzeOutput.f1stBondPmW/stForcePowerRollingHorizan.dPeakPower_mW);
		if( iPosnY < FIG4_ROLLING_HORIZAN_LOW_Y ) 
			iPosnY = FIG4_ROLLING_HORIZAN_LOW_Y + 1;
		else if( iPosnY > FIG4_ROLLING_HORIZAN_UPP_Y )
			iPosnY = FIG4_ROLLING_HORIZAN_UPP_Y;
		dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG4_ROLLING_HORIZAN_UPP_X, iPosnY);	
	}

	dc.SelectObject(pOldPen);
}
void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_golden_line_impedance()
{
	// Bright Califonia Gold 
    CClientDC dc(this);
	CPen penBrightCaliforniaGold(PS_DOT,1,RGB(254, 157, 4));    
	CPen*pOldPen=dc.SelectObject(&penBrightCaliforniaGold);
	int iPosnY;

	iPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - 
		FIG5_HEIGHT_Y_ROLLING_HORIZAN * stGoldenForcePowerAnalyzeOutput.f1stBondImp/stForcePowerRollingHorizan.dPeakImpedance_ohm);
	if( iPosnY < FIG5_ROLLING_HORIZAN_LOW_Y ) 
			iPosnY = FIG5_ROLLING_HORIZAN_LOW_Y + 1;
	else if( iPosnY > FIG5_ROLLING_HORIZAN_UPP_Y )
			iPosnY = FIG5_ROLLING_HORIZAN_UPP_Y;
	dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG5_ROLLING_HORIZAN_UPP_X, iPosnY);	

	dc.SelectObject(pOldPen);
}

void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_golden_line()
{
	FOPEM_plot_rolling_horizan_golden_line_force();
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		FOPEM_plot_rolling_horizan_golden_line_impedance();
	}
	FOPEM_plot_rolling_horizan_golden_line_power();
}

void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_golden_line_force()
{
    CClientDC dc(this);
	CPen penBlackRemove(PS_DOT,1,RGB(0, 0, 0));    
	CPen*pOldPen=dc.SelectObject(&penBlackRemove);
	int iPosnY;

	iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - 
			FIG_HEIGHT_Y_ROLLING_HORIZAN * stGoldenForcePowerAnalyzeOutput.f1stBondForceGram/stForcePowerRollingHorizan.dPeakForce_gram);
	dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG3_ROLLING_HORIZAN_UPP_X, iPosnY);	

	dc.SelectObject(pOldPen);
}
void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_golden_line_power()
{
    CClientDC dc(this);
	CPen penBlackRemove(PS_DOT,1,RGB(0, 0, 0));    
	CPen*pOldPen=dc.SelectObject(&penBlackRemove);
	int iPosnY;

	iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - 
			FIG_HEIGHT_Y_ROLLING_HORIZAN * stGoldenForcePowerAnalyzeOutput.f1stBondPmW/stForcePowerRollingHorizan.dPeakPower_mW);
	dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG4_ROLLING_HORIZAN_UPP_X, iPosnY);	

	dc.SelectObject(pOldPen);
}
void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_golden_line_impedance()
{
    CClientDC dc(this);
	CPen penBlackRemove(PS_DOT,1,RGB(240, 240, 240));    // @1
	CPen*pOldPen=dc.SelectObject(&penBlackRemove);
	int iPosnY;

	iPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - 
		FIG5_HEIGHT_Y_ROLLING_HORIZAN * stGoldenForcePowerAnalyzeOutput.f1stBondImp/stForcePowerRollingHorizan.dPeakImpedance_ohm);
	dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG5_ROLLING_HORIZAN_UPP_X, iPosnY);	

	dc.SelectObject(pOldPen);
}

void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_golden_line()
{
	FOPEM_remove_rolling_horizan_golden_line_force();
	FOPEM_remove_rolling_horizan_golden_line_power();
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		FOPEM_remove_rolling_horizan_golden_line_impedance();
	}
}

void DlgForcePowerMonitor::FOPEM_update_rolling_horizan_upp_low_threshold_force_value()
{
int iPosnY;

		iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dUpperBoundThForce_gram/stForcePowerRollingHorizan.dPeakForce_gram);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_FORCE)->MoveWindow(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY, 50, 20, TRUE);

		iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* (stForcePowerRollingHorizan.dLowerBoundThForce_gram + stForcePowerRollingHorizan.dUpperBoundThForce_gram)/2.0/stForcePowerRollingHorizan.dPeakForce_gram);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_GOLD_FORCE)->MoveWindow(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY, 50, 20, TRUE);

		iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* (stForcePowerRollingHorizan.dLowerBoundThForce_gram )/stForcePowerRollingHorizan.dPeakForce_gram);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_LOW_THRESHOLD_FORCE)->MoveWindow(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY, 50, 20, TRUE);
}

void DlgForcePowerMonitor::FOPEM_update_rolling_horizan_upp_low_threshold_power_value()
{
int iPosnY;
		iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dUpperBoundThPower_mW/stForcePowerRollingHorizan.dPeakPower_mW);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_POWER_mW)->MoveWindow(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY, 70, 20, TRUE);

		iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* (stForcePowerRollingHorizan.dLowerBoundThPower_mW + stForcePowerRollingHorizan.dUpperBoundThPower_mW)/2.0 /stForcePowerRollingHorizan.dPeakPower_mW);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_GOLD_POWER_mW)->MoveWindow(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY, 70, 20, TRUE);


		iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dLowerBoundThPower_mW/stForcePowerRollingHorizan.dPeakPower_mW);
		GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MIN_POWER_mW)->MoveWindow(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY, 70, 20, TRUE);
}



void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_upp_low_threshold_line_force()
{
CClientDC dc(this);
int iPosnY;
	///// Threshold with solid Red line
CPen penRedThreshold(PS_SOLID,2,RGB(250, 0, 0));  
CPen *pOldPen;
pOldPen = dc.SelectObject(&penRedThreshold);


		iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dUpperBoundThForce_gram/stForcePowerRollingHorizan.dPeakForce_gram);
		dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG3_ROLLING_HORIZAN_UPP_X, iPosnY);			

		iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dLowerBoundThForce_gram/stForcePowerRollingHorizan.dPeakForce_gram);
		dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG3_ROLLING_HORIZAN_UPP_X, iPosnY);	

	dc.SelectObject(pOldPen);	
}
void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_upp_low_threshold_line_power()
{
    CClientDC dc(this);
int iPosnY;
	///// Threshold with solid Red line
CPen penRedThreshold(PS_SOLID,2,RGB(250, 0, 0));  
CPen *pOldPen;
pOldPen = dc.SelectObject(&penRedThreshold);

		iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dUpperBoundThPower_mW/stForcePowerRollingHorizan.dPeakPower_mW);
		dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG4_ROLLING_HORIZAN_UPP_X, iPosnY);	


		iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dLowerBoundThPower_mW/stForcePowerRollingHorizan.dPeakPower_mW);
		dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG4_ROLLING_HORIZAN_UPP_X, iPosnY);	


	dc.SelectObject(pOldPen);	
}

#define __IMPEDANCE_TEXT_HEIGHT__   20
void DlgForcePowerMonitor::FOPEM_update_rolling_horizan_upp_low_threshold_impedance_value()
{
int		iPosnY = (int)(
					   FIG5_ROLLING_HORIZAN_LOW_Y 
					   + (FIG5_HEIGHT_Y_ROLLING_HORIZAN * stForcePowerRollingHorizan.dUpperBoundThImpedance_ohm
					      /stForcePowerRollingHorizan.dPeakImpedance_ohm)
					   - __IMPEDANCE_TEXT_HEIGHT__
			           );

    // FIG5_ROLLING_HORIZAN_LOW_Y
GetDlgItem(IDC_STATIC_ROLLING_DISPLAY_MAX_IMPEDANCE)->SetWindowPos(
	NULL, FIG5_ROLLING_HORIZAN_LOW_X, iPosnY, 40, __IMPEDANCE_TEXT_HEIGHT__, SWP_NOZORDER);
}

void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_upp_low_threshold_line_impedance()
{
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		CClientDC dc(this);
	int iPosnY;
		///// Threshold with solid Red line
	CPen penRedThreshold(PS_SOLID,2,RGB(250, 0, 0));  
	CPen *pOldPen;
	pOldPen = dc.SelectObject(&penRedThreshold);

		iPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dUpperBoundThImpedance_ohm/stForcePowerRollingHorizan.dPeakImpedance_ohm);
		dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG5_ROLLING_HORIZAN_UPP_X, iPosnY);			

		iPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dLowerBoundThImpedance_ohm/stForcePowerRollingHorizan.dPeakImpedance_ohm);
		dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG5_ROLLING_HORIZAN_UPP_X, iPosnY);	

		dc.SelectObject(pOldPen);	
	}
}

void DlgForcePowerMonitor::FOPEM_plot_rolling_horizan_upp_low_threshold_line()
{
	FOPEM_plot_rolling_horizan_upp_low_threshold_line_force();
	FOPEM_plot_rolling_horizan_upp_low_threshold_line_power();
//	FOPEM_plot_rolling_horizan_upp_low_threshold_line_impedance();
}

void DlgForcePowerMonitor::FOPEM_update_rolling_horizan_upp_low_threshold_value()
{
	FOPEM_update_rolling_horizan_upp_low_threshold_force_value();
	FOPEM_update_rolling_horizan_upp_low_threshold_power_value();
	FOPEM_update_rolling_horizan_upp_low_threshold_impedance_value();
}
void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_upp_low_threshold_line_force()
{
CClientDC dc(this);
int iPosnY;
CPen *pOldPen;
	///// Threshold with solid Red line
CPen penRedThreshold(PS_SOLID,2,RGB(0, 0, 0));  
	pOldPen = dc.SelectObject(&penRedThreshold);

	iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dUpperBoundThForce_gram/stForcePowerRollingHorizan.dPeakForce_gram);
	dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG3_ROLLING_HORIZAN_UPP_X, iPosnY);	

	iPosnY = (int)(FIG3_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dLowerBoundThForce_gram/stForcePowerRollingHorizan.dPeakForce_gram);
	dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG3_ROLLING_HORIZAN_UPP_X, iPosnY);	
	dc.SelectObject(pOldPen);	
}
void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_upp_low_threshold_line_power()
{
CClientDC dc(this);
int iPosnY;
	///// Threshold with solid Red line
CPen *pOldPen;
CPen penRedThreshold(PS_SOLID,2,RGB(0, 0, 0));  
	pOldPen = dc.SelectObject(&penRedThreshold);

	iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dUpperBoundThPower_mW/stForcePowerRollingHorizan.dPeakPower_mW);
	dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG4_ROLLING_HORIZAN_UPP_X, iPosnY);	

	iPosnY = (int)(FIG4_ROLLING_HORIZAN_UPP_Y - FIG_HEIGHT_Y_ROLLING_HORIZAN 
			* stForcePowerRollingHorizan.dLowerBoundThPower_mW/stForcePowerRollingHorizan.dPeakPower_mW);
	dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG4_ROLLING_HORIZAN_UPP_X, iPosnY);	

	dc.SelectObject(pOldPen);	
}
void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_upp_low_threshold_line_impedance()
{
CClientDC dc(this);
int iPosnY;
	///// Threshold with solid Red line
CPen penGrayBackground(PS_SOLID,2,RGB(240, 240, 240));  // @1
CPen *pOldPen;
	pOldPen = dc.SelectObject(&penGrayBackground);

	iPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
		* stForcePowerRollingHorizan.dUpperBoundThImpedance_ohm/stForcePowerRollingHorizan.dPeakImpedance_ohm);
	dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG5_ROLLING_HORIZAN_UPP_X, iPosnY);	

	iPosnY = (int)(FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_HEIGHT_Y_ROLLING_HORIZAN 
		* stForcePowerRollingHorizan.dLowerBoundThImpedance_ohm/stForcePowerRollingHorizan.dPeakImpedance_ohm);
	dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X, iPosnY);
	dc.LineTo(FIG5_ROLLING_HORIZAN_UPP_X, iPosnY);	

	dc.SelectObject(pOldPen);	
}

void DlgForcePowerMonitor::FOPEM_remove_rolling_horizan_upp_low_threshold_line()
{
	FOPEM_remove_rolling_horizan_upp_low_threshold_line_force();
	FOPEM_remove_rolling_horizan_upp_low_threshold_line_power();
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		FOPEM_remove_rolling_horizan_upp_low_threshold_line_impedance();
	}
}

void DlgForcePowerMonitor::WB_FOPEM_plot_bond_trig_by_spark()
{
	int n1, n, Lx, Ly, nX;

	CPen *pOldPen;

	n = 0;n1=0;Lx=0;Ly=0;
		//Analysis read Data
		//Display read data
    CClientDC dc(this);
    CPen penRed(PS_SOLID,2,RGB(255,0,0));   
    pOldPen=dc.SelectObject(&penRed);

    CPen penBlack(PS_SOLID,2,RGB(0,0,0));   

    CBrush br;
	br.CreateSolidBrush(RGB(0,0,0));
    CBrush*pOldBrush=dc.SelectObject(&br);


	int nHeightY = (FIG2_LOCATION_UPP_Y - FIG2_LOCATION_LOW_Y);
	double tSampleFer_us = stLoadDataAdcSetting.tPeriodSample_us;
    //////////////////// Trace - Ain-1 ///////////////////////
	for(n1=nOffsetBondStart;n1<nLen + nOffsetBondStart;n1++)
	{
		n++;
		nX = (int) (n * tSampleFer_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);

		if(nX < FIG_DISPLAY_LENGTH_X_PIXAL)
		{
			dc.SelectObject(&penBlack);
			dc.MoveTo((FIG2_LOCATION_LOW_X + nX),FIG2_LOCATION_LOW_Y);
			dc.LineTo((FIG2_LOCATION_LOW_X + nX),FIG2_LOCATION_UPP_Y);//
		}
		if(n1 >= 1)
		{
			dc.SelectObject(&penRed);
			dc.MoveTo((FIG2_LOCATION_LOW_X + Lx),(FIG2_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG2_LOCATION_LOW_X + nX),(FIG2_LOCATION_UPP_Y - (aiData[n1 * 6 + SCOPE_OFST_FORCE_]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiData[n1 * 6 + SCOPE_OFST_FORCE_]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)	break;
	}
    ////////////////////// Trace - Ain-2 /////////////////////////////////////
	n = 0;Lx=0; Ly=0;
    CPen penGreen(PS_SOLID,2,RGB(0,255,0));   
    CPen penBlue(PS_SOLID,2,RGB(0,100,127));   
    CPen penGreenBlue(PS_SOLID,2,RGB(0,127,200));   
	for(n1=nOffsetBondStart;n1<nLen + nOffsetBondStart;n1++)
	{
		n++;
		nX = (int) (n * tSampleFer_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);
		if(n1 >= 1)
		{
			dc.SelectObject(&penGreen);
			dc.MoveTo((FIG2_LOCATION_LOW_X + Lx),(FIG2_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG2_LOCATION_LOW_X + nX),(FIG2_LOCATION_UPP_Y - (aiData[n1 * 6 + SCOPE_OFST_VEL_]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiData[n1 * 6 + SCOPE_OFST_VEL_]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)	break;
	}

    ////////////////////// Trace - Ain-6 /////////////////////////////////////
	n = 0;Lx=0; Ly=0;
    //penGreen(PS_SOLID,1,RGB(0,255,0));   
	for(n1=nOffsetBondStart;n1<nLen + nOffsetBondStart;n1++)
	{
		n++;
		nX = (int) (n * tSampleFer_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US); 
		if(n1 >= 1)
		{
			dc.SelectObject(&penBlue);
			dc.MoveTo((FIG2_LOCATION_LOW_X + Lx),(FIG2_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG2_LOCATION_LOW_X + nX),(FIG2_LOCATION_UPP_Y - (aiData[n1 * 6 + SCOPE_OFST_EFO_]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiData[n1 * 6 + SCOPE_OFST_EFO_]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)	break;
	}

	/////////////////// Plot -Trace-{3,4,5} in Fig.1
    ////////////////////// Trace - Ain-5 /////////////////////////////////////
#ifdef __PLOT_POWER_US_FREQ__
	n = 0;Lx=0; Ly=0;
	for(n1=nOffsetBondStart;n1<nLen + nOffsetBondStart;n1++)
	{
		n++;
		nX = (int) (n * tSampleFer_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);
		if(nX<FIG_DISPLAY_LENGTH_X_PIXAL)
		{
			dc.SelectObject(&penBlack);
			dc.MoveTo((FIG1_LOCATION_LOW_X +nX),FIG1_LOCATION_LOW_Y);
			dc.LineTo((FIG1_LOCATION_LOW_X +nX),FIG1_LOCATION_UPP_Y);//
		}
		if(n1 >= 1)
		{
			dc.SelectObject(&penBlue);
			dc.MoveTo((FIG1_LOCATION_LOW_X + Lx),(FIG1_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG1_LOCATION_LOW_X + nX),(FIG1_LOCATION_UPP_Y - (aiData[n1 * 6 + SCOPE_OFST_POWER_US_]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiData[n1 * 6 + SCOPE_OFST_POWER_US_]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)	break;
	}
#endif // __PLOT_POWER_US_FREQ__ @2
    //////////////////// Trace - Ain-3 ///////////////////////
	n = 0;Lx=0; Ly=0;
	for(n1=nOffsetBondStart;n1<nLen + nOffsetBondStart;n1++)
	{
		n++;
		nX = (int) (n * tSampleFer_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US); 
		if(nX<FIG_DISPLAY_LENGTH_X_PIXAL)
		{
			dc.SelectObject(&penBlack);
			dc.MoveTo((FIG1_LOCATION_LOW_X +nX),FIG1_LOCATION_LOW_Y);
			dc.LineTo((FIG1_LOCATION_LOW_X +nX),FIG1_LOCATION_UPP_Y);//
		}

		if(n1 >= 1)
		{
			dc.SelectObject(&penRed);
			dc.MoveTo((FIG1_LOCATION_LOW_X + Lx),(FIG1_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG1_LOCATION_LOW_X + nX),(FIG1_LOCATION_UPP_Y - (aiData[n1 * 6 + SCOPE_OFST_POWER_1_]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiData[n1 * 6 + SCOPE_OFST_POWER_1_]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)	break;
	}
    ////////////////////// Trace - Ain-4 /////////////////////////////////////
	n = 0;Lx=0; Ly=0;
	for(n1=nOffsetBondStart;n1<nLen + nOffsetBondStart;n1++)
	{
		n++;
		nX = (int) (n * tSampleFer_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);
		if(n1 >= 1)
		{
			dc.SelectObject(&penGreen);
			dc.MoveTo((FIG1_LOCATION_LOW_X + Lx),(FIG1_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG1_LOCATION_LOW_X + nX),(FIG1_LOCATION_UPP_Y - (aiData[n1 * 6 + SCOPE_OFST_POWER_2_]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiData[n1 * 6 + SCOPE_OFST_POWER_2_]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)	break;
	}

	dc.SelectObject(pOldPen);	
}

extern int aiGoldenWaveformEFO[8192];
extern int aiGoldenWaveformForce[8192];
extern int aiGoldenWaveformVel[8192];
extern int aiGoldenWaveformPowerUS[8192];
extern int aiGoldenWaveformPowerI[8192];
extern int aiGoldenWaveformPowerV[8192];
extern double adGoldenFilterVel[8192];
extern double adGoldenFilterForce[8192];
extern SYS_SETTING_ADC stGoldenWaveformAdcSetting;

void DlgForcePowerMonitor::FOPEM_PlotGoldenWaveform()
{
	int n1= 0, n= 0, Lx= 0, Ly= 0;

	int idxStartEndEFO = stGoldenForcePowerAnalyzeOutput.stBondCycleTimeIndex.idxEndEFO;
	int nPlotLen = stGoldenForcePowerAnalyzeInput.uiDataLen - idxStartEndEFO;

    CClientDC dc(this);
    CPen penRed(PS_DOT,1,RGB(255,127,0));   
    CPen penGreen(PS_DOT,1,RGB(127,255,0));   
    CPen penBlue(PS_DOT,1,RGB(0,127,255));   

    CPen*pOldPen=dc.SelectObject(&penRed);

    CBrush br;
	br.CreateSolidBrush(RGB(0,0,0));
    CBrush*pOldBrush=dc.SelectObject(&br);

	int nHeightY = (FIG2_LOCATION_UPP_Y - FIG2_LOCATION_LOW_Y);
	int nX;
    //////////////////// Trace - Ain-1 Force///////////////////////
	for(n1=idxStartEndEFO;n1<nPlotLen;n1++)
	{
		n++;
		nX = (int) (n * stGoldenWaveformAdcSetting.tPeriodSample_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);

		if(n1 >= 1)
		{
			dc.SelectObject(&penRed);
//			dc.MoveTo((FIG2_LOCATION_LOW_X + Lx),(FIG2_LOCATION_UPP_Y - Ly));
			dc.MoveTo((int)(FIG2_LOCATION_LOW_X + Lx), (int)((FIG2_LOCATION_UPP_Y + FIG2_LOCATION_LOW_Y)/2 - Ly));
//			dc.LineTo((FIG2_LOCATION_LOW_X + nX),(FIG2_LOCATION_UPP_Y - (aiGoldenWaveformForce[n1]* nHeightY/65535)));//
			dc.LineTo((int)(FIG2_LOCATION_LOW_X + nX), (int)((FIG2_LOCATION_UPP_Y + FIG2_LOCATION_LOW_Y)/2 - (adGoldenFilterForce[n1] * nHeightY/10.0)));
		}
	    Lx=nX;
//	    Ly=(aiGoldenWaveformForce[n1]* nHeightY/65535);//
	    Ly=(int)(adGoldenFilterForce[n1]/5.0 * nHeightY/2);//
				//n1++;
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)			break;

	}
    ////////////////////// Trace - Ain-2 /////////////////////////////////////
	n = 0;Lx=0; Ly=0;
	for(n1=idxStartEndEFO;n1<nPlotLen;n1++)
	{
		n++;
		nX = (int) (n * stGoldenWaveformAdcSetting.tPeriodSample_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US); 
		if(n1 >= 1)
		{
			dc.SelectObject(&penGreen);
//			dc.MoveTo((FIG2_LOCATION_LOW_X + Lx),(FIG2_LOCATION_UPP_Y - Ly));
			dc.MoveTo((int)(FIG2_LOCATION_LOW_X + Lx), (int)((FIG2_LOCATION_UPP_Y + FIG2_LOCATION_LOW_Y)/2 - Ly));
//			dc.LineTo((FIG2_LOCATION_LOW_X + nX),(FIG2_LOCATION_UPP_Y - (aiGoldenWaveformVel[n1]* nHeightY/65535)));//
			dc.LineTo((int)(FIG2_LOCATION_LOW_X + nX), (int)((FIG2_LOCATION_UPP_Y + FIG2_LOCATION_LOW_Y)/2 - (adGoldenFilterVel[n1]* nHeightY/10.0)));//
		}
	    Lx=nX;
//	    Ly=(aiGoldenWaveformVel[n1]* nHeightY/65535);//
		Ly=(int) (adGoldenFilterVel[n1]* nHeightY/10.0);
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)			break;
	}

    ////////////////////// Trace - Ain-6 /////////////////////////////////////
	n = 0;Lx=0; Ly=0;
    //penGreen(PS_SOLID,1,RGB(0,255,0));   
	for(n1=idxStartEndEFO;n1<nPlotLen;n1++)
	{
		n++;
		nX = (int) (n * stGoldenWaveformAdcSetting.tPeriodSample_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);
		if(n1 >= 1)
		{
			dc.SelectObject(&penBlue);
			dc.MoveTo((FIG2_LOCATION_LOW_X + Lx),(FIG2_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG2_LOCATION_LOW_X + nX),(FIG2_LOCATION_UPP_Y - (aiGoldenWaveformEFO[n1]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiGoldenWaveformEFO[n1]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)			break;
	}

	/////////////////// Plot -Trace-{3,4,5} in Fig.1
    //////////////////// Trace - Ain-3 ///////////////////////
#ifdef __PLOT_POWER_US_FREQ__
	n = 0;Lx=0; Ly=0;
	for(n1=idxStartEndEFO;n1<nPlotLen;n1++)
	{
		n++;
		nX = (int) (n * stGoldenWaveformAdcSetting.tPeriodSample_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);

		if(n1 >= 1)
		{
			dc.SelectObject(&penRed);
			dc.MoveTo((FIG1_LOCATION_LOW_X + Lx),(FIG1_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG1_LOCATION_LOW_X + nX),(FIG1_LOCATION_UPP_Y - (aiGoldenWaveformPowerUS[n1]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiGoldenWaveformPowerUS[n1]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)			break;
	}
#endif // __PLOT_POWER_US_FREQ__ @2
    ////////////////////// Trace - Ain-4 /////////////////////////////////////
	n = 0;Lx=0; Ly=0;
	for(n1=idxStartEndEFO;n1<nPlotLen;n1++)
	{
		n++;
		nX = (int) (n * stGoldenWaveformAdcSetting.tPeriodSample_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US);
		if(n1 >= 1)
		{
			dc.SelectObject(&penGreen);
			dc.MoveTo((FIG1_LOCATION_LOW_X + Lx),(FIG1_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG1_LOCATION_LOW_X + nX),(FIG1_LOCATION_UPP_Y - (aiGoldenWaveformPowerV[n1]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiGoldenWaveformPowerV[n1]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)			break;
	}

    ////////////////////// Trace - Ain-5 /////////////////////////////////////
	n = 0;Lx=0; Ly=0;
	for(n1=idxStartEndEFO;n1<nPlotLen;n1++)
	{
		n++;
		nX = (int)(n * stGoldenWaveformAdcSetting.tPeriodSample_us * FIG_DISPLAY_LENGTH_X_PIXAL/FIG_DISPLAY_LENGTH_US); 
		if(n1 >= 1)
		{
			dc.SelectObject(&penBlue);
			dc.MoveTo((FIG1_LOCATION_LOW_X + Lx),(FIG1_LOCATION_UPP_Y - Ly));
			dc.LineTo((FIG1_LOCATION_LOW_X + nX),(FIG1_LOCATION_UPP_Y - (aiGoldenWaveformPowerI[n1]* nHeightY/65535)));//
		}
	    Lx=nX;
	    Ly=(aiGoldenWaveformPowerI[n1]* nHeightY/65535);//
		if(nX >= FIG_DISPLAY_LENGTH_X_PIXAL)			break;
	}

//	int nX_line, nY_Posn;
//    CPen penGreenBlue(PS_DASHDOT,1,RGB(0,127,200));   

	//int ii;
	//for(ii = 0; ii<10; ii++)
	//{
	//	nY_Posn = (int)(FIG1_LOCATION_UPP_Y * 1.0*ii/9.0) + (int)(FIG1_LOCATION_LOW_Y * (9.0 - ii)/9.0);
	//	dc.SelectObject(&penGreenBlue);
	//	dc.MoveTo(FIG1_LOCATION_LOW_X,nY_Posn);
	//	dc.LineTo(FIG1_LOCATION_UPP_X,nY_Posn);	
	//}

	dc.SelectObject(pOldPen);	
}
int iFlagInitOnlyOnce = 0;
void DlgForcePowerMonitor::FOPEM_InitDisplayFig()
{
CPen *pOldPen;
    CPaintDC dc(this);
    CBrush brBlack;
    CRect re;

    brBlack.CreateSolidBrush(RGB(0,0,0));
    CBrush*pOldBrush=dc.SelectObject(&brBlack);

CBrush brGrayBackground;   // @2
	brGrayBackground.CreateSolidBrush(RGB(240, 240, 240));  // @2
CPen penGray(PS_SOLID,2,RGB(240, 240, 240));   //
    pOldPen = dc.SelectObject(&penGray);

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		dc.SelectObject(&brBlack);
		dc.Rectangle(FIG2_LOCATION_LOW_X,FIG2_LOCATION_LOW_Y,
			FIG2_LOCATION_UPP_X, FIG2_LOCATION_UPP_Y);//pDC->
		dc.Rectangle(FIG1_LOCATION_LOW_X,FIG1_LOCATION_LOW_Y,
			FIG1_LOCATION_UPP_X, FIG1_LOCATION_UPP_Y);//pDC->
	}
	else // @2
	{
		dc.SelectObject(&brGrayBackground);
		dc.SelectObject(&penGray);
		dc.Rectangle(FIG2_LOCATION_LOW_X,FIG2_LOCATION_LOW_Y,
			FIG2_LOCATION_UPP_X, FIG2_LOCATION_UPP_Y);//pDC->
		dc.Rectangle(FIG1_LOCATION_LOW_X,FIG1_LOCATION_LOW_Y,
			FIG1_LOCATION_UPP_X, FIG1_LOCATION_UPP_Y);//pDC->
	}// @2
		// 20130430
	dc.SelectObject(&brBlack);
	dc.Rectangle(FIG3_ROLLING_HORIZAN_LOW_X,FIG3_ROLLING_HORIZAN_LOW_Y,
			FIG3_ROLLING_HORIZAN_UPP_X, FIG3_ROLLING_HORIZAN_UPP_Y);
	dc.Rectangle(FIG4_ROLLING_HORIZAN_LOW_X,FIG4_ROLLING_HORIZAN_LOW_Y,
			FIG4_ROLLING_HORIZAN_UPP_X, FIG4_ROLLING_HORIZAN_UPP_Y);
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		dc.SelectObject(&brBlack);
		dc.Rectangle(FIG5_ROLLING_HORIZAN_LOW_X,FIG5_ROLLING_HORIZAN_LOW_Y,
			FIG5_ROLLING_HORIZAN_UPP_X, FIG5_ROLLING_HORIZAN_UPP_Y);
	}
	else // @2
	{
		dc.SelectObject(&brGrayBackground);
		dc.SelectObject(&penGray);
		dc.Rectangle(FIG5_ROLLING_HORIZAN_LOW_X,FIG5_ROLLING_HORIZAN_LOW_Y,
			FIG5_ROLLING_HORIZAN_UPP_X, FIG5_ROLLING_HORIZAN_UPP_Y);
	}// @2

    CPen pen3(PS_SOLID,2,RGB(255,255,200));   //
    pOldPen = dc.SelectObject(&pen3);
	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		dc.MoveTo(FIG2_LOCATION_LOW_X,FIG2_LOCATION_UPP_Y);
		dc.LineTo(FIG2_LOCATION_UPP_X,FIG2_LOCATION_UPP_Y);
		dc.MoveTo(FIG1_LOCATION_LOW_X,FIG1_LOCATION_UPP_Y);
		dc.LineTo(FIG1_LOCATION_UPP_X,FIG1_LOCATION_UPP_Y);
	}

	CPen pen1(PS_DOT,1,RGB(50,50,50));    //画曲线坐标线---横线PS_DOT
	CPen*pOldPen1=dc.SelectObject(&pen1);
	int nSectorY = 10;
	int nDeltaY = (FIG2_LOCATION_UPP_Y - FIG2_LOCATION_LOW_Y)/nSectorY;
	int iPosnY, ii;

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		for( ii = 1; ii<nSectorY; ii++)
		{
			iPosnY = FIG2_LOCATION_LOW_Y + nDeltaY * ii;
			dc.MoveTo(FIG2_LOCATION_LOW_X, iPosnY);
			dc.LineTo(FIG2_LOCATION_UPP_X, iPosnY);	
		}
	}

	int nSectorX = 8;
	int nDeltaX = (FIG2_LOCATION_UPP_X - FIG2_LOCATION_LOW_X)/nSectorX;
	int iPosnX;

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		for( ii = 1; ii<nSectorX; ii++)
		{
			iPosnX = FIG2_LOCATION_LOW_X + ii * nDeltaX;
			dc.MoveTo(iPosnX,FIG2_LOCATION_LOW_Y);//画曲线坐标线---竖线
			dc.LineTo(iPosnX,FIG2_LOCATION_UPP_Y);
		}
	}

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		//// 2nd Fig
		nSectorY = 10;
		nDeltaY = (FIG1_LOCATION_UPP_Y - FIG1_LOCATION_LOW_Y)/nSectorY;
		for( ii = 1; ii<nSectorY; ii++)
		{
			iPosnY = FIG1_LOCATION_LOW_Y + nDeltaY * ii;
			dc.MoveTo(FIG1_LOCATION_LOW_X, iPosnY);
			dc.LineTo(FIG1_LOCATION_UPP_X, iPosnY);	
		}

		nSectorX = 8;
		nDeltaX = (FIG1_LOCATION_UPP_X - FIG1_LOCATION_LOW_X)/nSectorX;
		for( ii = 1; ii<nSectorX; ii++)
		{
			iPosnX = FIG1_LOCATION_LOW_X + ii * nDeltaX;
			dc.MoveTo(iPosnX,FIG1_LOCATION_LOW_Y);//画曲线坐标线---竖线
			dc.LineTo(iPosnX,FIG1_LOCATION_UPP_Y);
		}
	}

	//Figure 3, 4, 5
	nSectorY = 5;
	nDeltaY = (FIG3_ROLLING_HORIZAN_UPP_Y - FIG3_ROLLING_HORIZAN_LOW_Y)/nSectorY;
	for( ii = 1; ii<nSectorY; ii++)
	{
		iPosnY = FIG3_ROLLING_HORIZAN_LOW_Y + nDeltaY * ii;
		dc.MoveTo(FIG3_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG3_ROLLING_HORIZAN_UPP_X, iPosnY);	
	}

	nSectorY = 5;
	nDeltaY = (FIG4_ROLLING_HORIZAN_UPP_Y - FIG4_ROLLING_HORIZAN_LOW_Y)/nSectorY;
	for( ii = 1; ii<nSectorY; ii++)
	{
		iPosnY = FIG4_ROLLING_HORIZAN_LOW_Y + nDeltaY * ii;
		dc.MoveTo(FIG4_ROLLING_HORIZAN_LOW_X, iPosnY);
		dc.LineTo(FIG4_ROLLING_HORIZAN_UPP_X, iPosnY);	
	}

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_DEVELOPER)
	{
		nSectorY = 5;
		nDeltaY = (FIG5_ROLLING_HORIZAN_UPP_Y - FIG5_ROLLING_HORIZAN_LOW_Y)/nSectorY;
		for( ii = 1; ii<nSectorY; ii++)
		{
			iPosnY = FIG5_ROLLING_HORIZAN_LOW_Y + nDeltaY * ii;
			dc.MoveTo(FIG5_ROLLING_HORIZAN_LOW_X, iPosnY);
			dc.LineTo(FIG5_ROLLING_HORIZAN_UPP_X, iPosnY);	
		}
	}
	dc.SelectObject(pOldPen);
}

char *pstrPlusMinusThanGold[] =
{
	"+",
	" "
};

char *pstrCheckingForcePowerMoreLess[] =
{
	"compared with Gold",  // more than
	"compared with Gold"   // less than
};

char *pstrCheckingLabelForcePower[] =
{
	"Force",
	"Power",
	"Impedance"
};

void DlgForcePowerMonitor::UI_UpdateCheckingResult()
{
double dSmoothedPower_mW, dSmoothedForce_gram, dSmoothedImpedance_ohm;  // @5

int idxSmoothedNewSample = ulNumLoadedWaveform % FIG_LENGTH_X_ROLLING_HORIZAN;
	dSmoothedPower_mW = adSmoothedRollingPower_mW[idxSmoothedNewSample];
	dSmoothedForce_gram = adSmoothedRollingForce_gram[idxSmoothedNewSample];
	dSmoothedImpedance_ohm = adSmoothedRollingImpedance_ohm[idxSmoothedNewSample]; // @5

	CString cstrDisplay;
	// stLoadForcePowerAnalyzeOutput.f1stBondForceGram @5
	double dDifferenceInForceGram = dSmoothedForce_gram - stGoldenForcePowerAnalyzeOutput.f1stBondForceGram;
	char strDifferenceCheckingResult[512];
	char cFlagSignMoreLess = 0;
	
	if(dDifferenceInForceGram >= 0)
	{
		cFlagSignMoreLess = 0;
	}
	else
	{
		cFlagSignMoreLess = 1;
	}
	sprintf_s(strDifferenceCheckingResult, 512, "%s%4.0f gram %s %s", 
		pstrPlusMinusThanGold[cFlagSignMoreLess],
		dDifferenceInForceGram, 
		pstrCheckingForcePowerMoreLess[cFlagSignMoreLess],
		pstrCheckingLabelForcePower[0]);

	if(fabs(dDifferenceInForceGram) < stFOPEM_CheckingThreshold.dForceTh_gram)
	{
		cstrDisplay.Format("Force Check: PASS \r\n %s \r\n", strDifferenceCheckingResult);
	}
	else
	{
		cstrDisplay.Format("Force Check: FAIL \r\n %s \r\n", strDifferenceCheckingResult);
	}

	// @5  stLoadForcePowerAnalyzeOutput.f1stBondPmW
	double dDifferenceInPower_mW = dSmoothedPower_mW - stGoldenForcePowerAnalyzeOutput.f1stBondPmW;
	if(dDifferenceInPower_mW >= 0)
	{
		cFlagSignMoreLess = 0;
	}
	else
	{
		cFlagSignMoreLess = 1;
	}
	sprintf_s(strDifferenceCheckingResult, 512, "%s%4.0f mW %s %s", 
		pstrPlusMinusThanGold[cFlagSignMoreLess],
		dDifferenceInPower_mW, 
		pstrCheckingForcePowerMoreLess[cFlagSignMoreLess],
		pstrCheckingLabelForcePower[1]);

	if(fabs(dDifferenceInPower_mW) < stFOPEM_CheckingThreshold.dPowerTh_mW)
	{
		cstrDisplay.AppendFormat("Power Check: PASS \r\n %s \r\n", strDifferenceCheckingResult);
	}
	else
	{
		cstrDisplay.AppendFormat("Power Check: FAIL \r\n %s \r\n", strDifferenceCheckingResult);
	}

	if(stSystemUserInterfaceCfg.nUserLevel == USER_LEVEL_ENGINEER) // USER_LEVEL_DEVELOPER
	{
		// @5 stLoadForcePowerAnalyzeOutput.f1stBondImp, stLoadForcePowerAnalyzeOutput.f1stBondImp
		if(dSmoothedImpedance_ohm - stGoldenForcePowerAnalyzeOutput.f1stBondImp < stFOPEM_CheckingThreshold.dImpedanceTh_ohm &&
				dSmoothedImpedance_ohm > _DEFAULT_MINIMUM_IMPEDANCE)
		{
			cstrDisplay.AppendFormat("Impedance: PASS \r\n");
		}
		else
		{
			cstrDisplay.AppendFormat("Impedance: FAIL \r\n");
		}
	}
	GetDlgItem(IDC_STATIC_DISPLAY_CHECK_RESULT_FOPEM_DLG)->SetWindowTextA(cstrDisplay);

}


void DlgForcePowerMonitor::OnBnClickedButtonFopemDlgAutoCheckCalibrate()
{
	UI_EnableButtonComboDuringThread(FALSE);
	if( stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_CAPTURE )
	{
		if(_chdir("D:\\FPMD_Calibrate"))
		{
			_mkdir("D:\\FPMD_Calibrate");
		}
		sprintf_s(strFoldernameCaptureDataCalibration, 512, "FPMD_Calibrate_Y%dM%dD%d_H%dM%d",
			stpTimeGlobalVar->tm_year +1900, stpTimeGlobalVar->tm_mon +1, stpTimeGlobalVar->tm_mday, stpTimeGlobalVar->tm_hour, stpTimeGlobalVar->tm_min);
		_mkdir(strFoldernameCaptureDataCalibration);
	}
	else if( stSystemUserInterfaceCfg.nFPMD_VerifyMode == WB_MACHINE_FPMD_MODE_VERIFY)
	{
		if(_chdir("D:\\FPMD_CheckDrift"))
		{
			_mkdir("D:\\FPMD_CheckDrift");
		}
		sprintf_s(strFoldernameCheckingMachineDriftFOPEM, 512, "FPMD_CheckDrift_Y%dM%dD%d_H%dM%d",
			stpTimeGlobalVar->tm_year +1900, stpTimeGlobalVar->tm_mon +1, stpTimeGlobalVar->tm_mday, stpTimeGlobalVar->tm_hour, stpTimeGlobalVar->tm_min);
		_mkdir(strFoldernameCheckingMachineDriftFOPEM);

	}
	if(FOPEM_get_system_offline_mode() == TRUE)
	{
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON_SAVE_PF_MONITOR_DLG)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON_LOAD_PF_MONITOR_DLG)->EnableWindow(1);
	}
	RunAutoCalibrateVerifyThread();
}

void DlgForcePowerMonitor::UI_InitializeForcePowerOffsetSlider()
{
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG))->
		SetRangeMax(abs(MAXIMUM_FORCE_OFFSET_GRAM - MINIMUM_FORCE_OFFSET_GRAM));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG))->
		SetRange(MINIMUM_FORCE_OFFSET_GRAM, MAXIMUM_FORCE_OFFSET_GRAM);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG))->SetPos(iFPMD_ForceDisplayOffset);


	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG))->
		SetRangeMax(abs(MAXIMUM_POWER_OFFSET_MW - MINIMUM_POWER_OFFSET_MW));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG))->
		SetRange(MINIMUM_POWER_OFFSET_MW, MAXIMUM_POWER_OFFSET_MW);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG))->SetPos(iFPMD_PowerDisplayOffset);
}

extern FOPEM_OFFSET_FORCE_POWER stSysOffsetForcePower;
// IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG
void DlgForcePowerMonitor::OnNMCustomdrawSliderForceDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}
// IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG
void DlgForcePowerMonitor::OnNMCustomdrawSliderPowerDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}

// IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG
void DlgForcePowerMonitor::OnNMReleasedcaptureSliderForceDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString cstrTemp;
	iFPMD_ForceDisplayOffset = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_FORCE_DISPLAY_OFFSET_FPMD_DLG))->GetPos();
	cstrTemp.Format("Force(g): %d", iFPMD_ForceDisplayOffset);
	GetDlgItem(IDC_STATIC_FORCE_OFFSET_VALUE_FPMD_DLG)->SetWindowTextA(cstrTemp);

	FOPEM_set_flag_config_setting_changed(); // @3
	stSysOffsetForcePower.iForceOffset_gram = iFPMD_ForceDisplayOffset;

	*pResult = 0;
}

// IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG
void DlgForcePowerMonitor::OnNMReleasedcaptureSliderPowerDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString cstrTemp;
	iFPMD_PowerDisplayOffset = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_POWER_DISPLAY_OFFSET_FPMD_DLG))->GetPos();
	cstrTemp.Format("Power(mW): %d", iFPMD_PowerDisplayOffset);
	GetDlgItem(IDC_STATIC_POWER_OFFSET_VALUE_FPMD_DLG)->SetWindowTextA(cstrTemp);

	FOPEM_set_flag_config_setting_changed(); // @3
	stSysOffsetForcePower.iPowerOffset_mW = iFPMD_PowerDisplayOffset;
	*pResult = 0;
}
