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

#pragma once


// DlgForcePowerMonitor dialog
//#include "FPMD_CALC_DEF.h"

class DlgForcePowerMonitor : public CDialog
{
	DECLARE_DYNAMIC(DlgForcePowerMonitor)

public:
	DlgForcePowerMonitor(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgForcePowerMonitor();
	virtual void ShowWindow(int nCmdShow);

	void FOPEM_InitDisplay();
	void FOPEM_InitDisplayFig();
	void UI_UpdateCheckingResult();
	void FOPEM_PlotGoldenWaveform();
	void FOPEM_TimerEventCalculatePlotAfterCapture();
	void FOPEM_TimerEvent();
//	void FOPEM_TimerDisplayStatus();
	void WB_FOPEM_plot_bond_dummy();
	void WB_FOPEM_plot_bond_trig_by_spark();
	char GetFlagShowWindow();
	char GetFlagPauseCapture();
	void FOPEM_CalibrateVerifyThread_CompleteStage_Fitting();

	void FOPEM_plot_rolling_horizan_peak_value();
	void FOPEM_plot_rolling_horizan_golden_line();
	void FOPEM_remove_rolling_horizan_golden_line();
	void FOPEM_plot_rolling_horizan_upp_low_threshold_line();
	void FOPEM_remove_rolling_horizan_upp_low_threshold_line();
	void FOPEM_update_rolling_horizan_upp_low_threshold_value();

	void FOPEM_update_rolling_horizan_upp_low_threshold_force_value();
	void FOPEM_update_rolling_horizan_upp_low_threshold_power_value();
	void FOPEM_update_rolling_horizan_upp_low_threshold_impedance_value();

	void FOPEM_plot_rolling_horizan_upp_low_threshold_line_force();
	void FOPEM_plot_rolling_horizan_upp_low_threshold_line_power();
	void FOPEM_plot_rolling_horizan_upp_low_threshold_line_impedance();

	void FOPEM_plot_rolling_horizan_golden_line_force();
	void FOPEM_plot_rolling_horizan_golden_line_impedance();
	void FOPEM_plot_rolling_horizan_golden_line_power();

	void FOPEM_remove_rolling_horizan_golden_line_force();
	void FOPEM_remove_rolling_horizan_golden_line_power();
	void FOPEM_remove_rolling_horizan_golden_line_impedance();

	void FOPEM_remove_rolling_horizan_upp_low_threshold_line_force();
	void FOPEM_remove_rolling_horizan_upp_low_threshold_line_power();
	void FOPEM_remove_rolling_horizan_upp_low_threshold_line_impedance();

	void FOPEM_plot_rolling_horizan_new_sample();
	void FOPEM_plot_rolling_horizan_smoothed_new_sample();
	void FOPEM_update_rolling_horizan_online();

	void FOPEM_ReplotRollingHorizan();


	void UI_SetComboByUserLevel();
	void UI_EnableButtonComboDuringThread(BOOL bFlag);
	void UI_SetButtonByVerifyMode();
	void UI_InitComboButtonDlgFOPEM();
	void UI_UpdateActualForcePowerAfterCalculate(int idxStart1stBond_RetErr = 0, int idxEnd1stBond_AbnormalFlag = 0); // @4
	void UI_ReusePreviousForcePowerForErrorCalculation();
	void UI_InitializeForcePowerOffsetSlider();

//	void FOPEM_InitDisplayFig2();

	void RunAutoCalibrateVerifyThread();
	UINT StopAutoCalibrateVerifyThread();
	UINT AutoCalibrateVerifyThread();

// Dialog Data
	enum { IDD = IDD_DLG_FORCE_POWER_MONITOR };

protected:
	virtual BOOL OnInitDialog(); // 
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPaint();

	CWinThread* m_pWinThread;
	char m_cStartAutoCalibrateVerifyThread;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPausePfMonitorDlg();
	afx_msg void OnBnClickedButtonSavePfMonitorDlg();
	afx_msg void OnBnClickedButtonLoadPfMonitorDlg();
	afx_msg void OnCbnSelchangeComboUserLevelPfMonitorDlg();
	afx_msg void OnCbnSelchangeCcomboRunningModePfMonitorDlg();
	afx_msg void OnCbnSelchangeCcomboPowerModePfMonitorDlg();
	afx_msg void OnBnClickedBtnExitFpMonitorDlg();
	afx_msg void OnBnClickedCheckFpmdFlagShownGoldenWaveform();
	afx_msg void OnCbnSelchangeComboVerifyModePfMonitorDlg();
	afx_msg void OnCbnSelchangeComboFpmdVerifyThresholdForceGram();
	afx_msg void OnCbnSelchangeComboFpmdVerifyThresholdPowermw();
	afx_msg void OnCbnSelchangeComboFpmdVerifyThresholdImpedanceOhm();
	afx_msg void OnBnClickedButtonFopemDlgAutoCheckCalibrate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMCustomdrawSliderForceDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderPowerDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderForceDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderPowerDisplayOffsetFpmdDlg(NMHDR *pNMHDR, LRESULT *pResult);
};
