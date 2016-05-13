// DMP609VCDlg.h : header file
//

#if !defined(AFX_DMP609VCDLG_H__B6DAB033_8CE4_4392_BD8E_FE4F6A1C7227__INCLUDED_)
#define AFX_DMP609VCDLG_H__B6DAB033_8CE4_4392_BD8E_FE4F6A1C7227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDMP609VCDlg dialog

class CDMP609VCDlg : public CDialog
{
// Construction
public:
	CDMP609VCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDMP609VCDlg)
	enum { IDD = IDD_DMP609VC_DIALOG };
	float	m_ad_v;
	double	m_fer;
	int		m_ch1;
	int		m_ch10;
	int		m_ch11;
	int		m_ch13;
	int		m_ch12;
	int		m_ch14;
	int		m_ch15;
	int		m_ch16;
	int		m_ch2;
	int		m_ch3;
	int		m_ch4;
	int		m_ch5;
	int		m_ch6;
	int		m_ch7;
	int		m_ch8;
	int		m_ch9;
	int		m_io1_data;
	int		m_io1_16_data;
	int		m_ad_data;
	//}}AFX_DATA

	void ReadIntegerFromEdit(int nResId, int *iValue);
	void ReadUnsignedIntegerFromEdit(int nResId, unsigned int *iValue);
	void UpdateReadAd();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMP609VCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL


	void SetCheck(UINT idBtn);
    void SetCheck_CH(UINT idBtn_CH);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDMP609VCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAd();
	afx_msg void OnSdAd();
	afx_msg void OnTiAd();
	afx_msg void OnRAd();
	afx_msg void OnScAd();
	afx_msg void OnDa1();
	afx_msg void OnDa2();
	afx_msg void OnDo1();
	afx_msg void OnDo116();
	afx_msg void OnIo1();
	afx_msg void OnIo116();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRa1();
	afx_msg void OnRa2();
	afx_msg void OnRa3();
	afx_msg void OnRa4();
	afx_msg void OnRa5();
	afx_msg void OnRa6();
	afx_msg void OnRa7();
	afx_msg void OnRa8();
	afx_msg void OnRa9();
	afx_msg void OnRa10();
	afx_msg void OnRa11();
	afx_msg void OnRa12();
	afx_msg void OnRa13();
	afx_msg void OnRa14();
	afx_msg void OnRa15();
	afx_msg void OnRa16();
	afx_msg void OnRa17();
	afx_msg void OnRa18();
	afx_msg void OnRa19();
	afx_msg void OnRa21();
	afx_msg void OnRa22();
	afx_msg void OnRa23();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboDa1AnalogWave();
	afx_msg void OnEnKillfocusEditFrequencyAnalogOut();
	afx_msg void OnBnClickedCheckFlagEnableAnalogWave();
	afx_msg void OnEnKillfocusFer();
	afx_msg void OnBnClickedButtonStartForcePowerMonitor();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMP609VCDLG_H__B6DAB033_8CE4_4392_BD8E_FE4F6A1C7227__INCLUDED_)
