// DMP609VCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DMP609VC.h"
#include "DMP609VCDlg.h"

#include "Dmp609Dll.h"//必须包含的DLL文件

#include "DlgForcePowerMonitor.h"
#include "EFS_DisplayCfg.h"

extern DlgForcePowerMonitor *pDlgForcePowerMonitor;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
///////定义全局变量////////
extern HANDLE	m_hDevice;//定义设备句柄 //  = INVALID_HANDLE_VALUE, in DMP609VC.cpp

unsigned char  ch,g,k;   
int Lx=0,Ly=0,n=0;
unsigned int AD_data[600],AD_data1[16];
unsigned char ucFlagAnalogOutput = 1; // IDC_COMBO_DA1_ANALOG_WAVE
unsigned int uiFreqAnalogOutput = 1000;
unsigned char ucFlagEnableAnalogOutput; // IDC_CHECK_FLAG_ENABLE_ANALOG_WAVE
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

extern SYS_USER_INTERFACE_CFG stSystemUserInterfaceCfg;

BOOL CDMP609VCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// IDM_ABOUTBOX must be in the system command range.
#ifdef __HAS_ABOUT_BOX__
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
//		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
#endif  // __HAS_ABOUT_BOX__
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    m_fer = 50; // 50 us, 20KHz
	ch=1;
	g=2; // +/- 5V
	SetCheck(IDC_RA23);
	SetCheck_CH(IDC_RA1);	

	// 
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_DA1_ANALOG_WAVE);
	pCombo->InsertString(0, _T("Sinusoidal"));
	pCombo->InsertString(1, _T("Triangle"));
	pCombo->InsertString(2, _T("SawTooth"));
	pCombo->SetCurSel(ucFlagAnalogOutput);

	((CButton*) GetDlgItem(IDC_CHECK_FLAG_ENABLE_ANALOG_WAVE))->SetCheck(ucFlagEnableAnalogOutput);

	CString cstrTemp;
	cstrTemp.Format("%d", uiFreqAnalogOutput);
	GetDlgItem(IDC_EDIT_FREQUENCY_ANALOG_OUT)->SetWindowTextA(cstrTemp); // , &uiFreqAnalogOutput

//	FOPEM_sytem_setting_default();
#ifndef __LOAD_TIMER_DMP_FROM_DLG_FPMD__
    gLiDLL=LoadLibrary("Dmp609.dll");//加载DLL
    #include "Dmp609Dll-A.h"
    if(gLiDLL==NULL)
	{
	     MessageBox("          请查找DLL         \n\n ");
	}
	UpdateData(FALSE);	

	m_hDevice = OpenDevice(0); //打开设备
	if (m_hDevice == INVALID_HANDLE_VALUE)
	{
		if(stSystemUserInterfaceCfg.nDisplayLanguage == FPMD_DISPLAY_LANGUAGE_EN)
		{
			int iRet = AfxMessageBox(" Open Device Error, Please check \n\n(1) USB connection, \n(2) Power Supply, \n\n Cancel: to exit, OK: to proceed entering debug mode", MB_OKCANCEL);
			if(iRet == IDOK)
			{
				FOPEM_set_system_offline_mode(TRUE);
				// IDD_DLG_FORCE_POWER_MONITOR 
			}
			else
			{
				exit(0);
			}
		}
		else
		{
			MessageBox("\n\n       打开设备失败,程序将无法正常工作   \n\n\n\n   请检查模块(或板卡)是否已正确连接 \n\n");
		}
	}
	Sleep(100);

	int Tim;
	Tim=SetTimer(1,2000,NULL);
	if(Tim==0)
	{
		MessageBox("         定 时 器 安 装 失 败                          \n \n \n		   程 序 无 法 正 常 工 作!       \n \n \n          请 关 闭 正 在 运 行 的 不 需 要 的 程 序                     "); 
	}

#endif	
#ifdef _DEBUG
//	stSystemUserInterfaceCfg.nUserLevel = USER_LEVEL_DEVELOPER;
#endif

#ifdef _LOAD_FPMD_FROM_DMP609_TEST
	if(pDlgForcePowerMonitor == NULL)
	{
		pDlgForcePowerMonitor = new DlgForcePowerMonitor(this);
		pDlgForcePowerMonitor->Create(IDD_DLG_FORCE_POWER_MONITOR);
	}
	pDlgForcePowerMonitor->ShowWindow(1);
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMP609VCDlg dialog

CDMP609VCDlg::CDMP609VCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMP609VCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDMP609VCDlg)
	m_ad_v = 0.0f;
	m_fer = 0.0;
	m_ch1 = 0;
	m_ch10 = 0;
	m_ch11 = 0;
	m_ch13 = 0;
	m_ch12 = 0;
	m_ch14 = 0;
	m_ch15 = 0;
	m_ch16 = 0;
	m_ch2 = 0;
	m_ch3 = 0;
	m_ch4 = 0;
	m_ch5 = 0;
	m_ch6 = 0;
	m_ch7 = 0;
	m_ch8 = 0;
	m_ch9 = 0;
	m_io1_data = 0;
	m_io1_16_data = 0;
	m_ad_data = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDMP609VCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMP609VCDlg)
	DDX_Text(pDX, IDC_AD_V, m_ad_v);
	DDX_Text(pDX, IDC_FER, m_fer);
	DDX_Text(pDX, IDC_CH1, m_ch1);
	DDX_Text(pDX, IDC_CH10, m_ch10);
	DDX_Text(pDX, IDC_CH11, m_ch11);
	DDX_Text(pDX, IDC_CH13, m_ch13);
	DDX_Text(pDX, IDC_CH12, m_ch12);
	DDX_Text(pDX, IDC_CH14, m_ch14);
	DDX_Text(pDX, IDC_CH15, m_ch15);
	DDX_Text(pDX, IDC_CH16, m_ch16);
	DDX_Text(pDX, IDC_CH2, m_ch2);
	DDX_Text(pDX, IDC_CH3, m_ch3);
	DDX_Text(pDX, IDC_CH4, m_ch4);
	DDX_Text(pDX, IDC_CH5, m_ch5);
	DDX_Text(pDX, IDC_CH6, m_ch6);
	DDX_Text(pDX, IDC_CH7, m_ch7);
	DDX_Text(pDX, IDC_CH8, m_ch8);
	DDX_Text(pDX, IDC_CH9, m_ch9);
	DDX_Text(pDX, IDC_IO1_DATA1, m_io1_data);
	DDX_Text(pDX, IDC_IO1_16_DATA, m_io1_16_data);
	DDX_Text(pDX, IDC_AD_DATA, m_ad_data);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDMP609VCDlg, CDialog)
	//{{AFX_MSG_MAP(CDMP609VCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_AD, OnAd)
	ON_BN_CLICKED(IDC_SD_AD, OnSdAd)
	ON_BN_CLICKED(IDC_TI_AD, OnTiAd)
	ON_BN_CLICKED(IDC_R_AD, OnRAd)
	ON_BN_CLICKED(IDC_SC_AD, OnScAd)
	ON_BN_CLICKED(IDC_DA1, OnDa1)
	ON_BN_CLICKED(IDC_DA2, OnDa2)
	ON_BN_CLICKED(IDC_DO1, OnDo1)
	ON_BN_CLICKED(IDC_DO1_16, OnDo116)
	ON_BN_CLICKED(IDC_IO1, OnIo1)
	ON_BN_CLICKED(IDC_IO1_16, OnIo116)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RA1, OnRa1)
	ON_BN_CLICKED(IDC_RA2, OnRa2)
	ON_BN_CLICKED(IDC_RA3, OnRa3)
	ON_BN_CLICKED(IDC_RA4, OnRa4)
	ON_BN_CLICKED(IDC_RA5, OnRa5)
	ON_BN_CLICKED(IDC_RA6, OnRa6)
	ON_BN_CLICKED(IDC_RA7, OnRa7)
	ON_BN_CLICKED(IDC_RA8, OnRa8)
	ON_BN_CLICKED(IDC_RA9, OnRa9)
	ON_BN_CLICKED(IDC_RA10, OnRa10)
	ON_BN_CLICKED(IDC_RA11, OnRa11)
	ON_BN_CLICKED(IDC_RA12, OnRa12)
	ON_BN_CLICKED(IDC_RA13, OnRa13)
	ON_BN_CLICKED(IDC_RA14, OnRa14)
	ON_BN_CLICKED(IDC_RA15, OnRa15)
	ON_BN_CLICKED(IDC_RA16, OnRa16)
	ON_BN_CLICKED(IDC_RA21, OnRa21)
	ON_BN_CLICKED(IDC_RA22, OnRa22)
	ON_BN_CLICKED(IDC_RA23, OnRa23)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_DA1_ANALOG_WAVE, &CDMP609VCDlg::OnCbnSelchangeComboDa1AnalogWave)
	ON_EN_KILLFOCUS(IDC_EDIT_FREQUENCY_ANALOG_OUT, &CDMP609VCDlg::OnEnKillfocusEditFrequencyAnalogOut)
	ON_BN_CLICKED(IDC_CHECK_FLAG_ENABLE_ANALOG_WAVE, &CDMP609VCDlg::OnBnClickedCheckFlagEnableAnalogWave)
	ON_EN_KILLFOCUS(IDC_FER, &CDMP609VCDlg::OnEnKillfocusFer)
	ON_BN_CLICKED(IDC_BUTTON_START_FORCE_POWER_MONITOR, &CDMP609VCDlg::OnBnClickedButtonStartForcePowerMonitor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMP609VCDlg message handlers

extern int nLen ;
extern int nOffsetBondStart ;
extern unsigned int aiData[];
extern unsigned int aiBakData[];
extern SYS_SETTING_ADC stSysSettingAdc;

static double dPeriodScanAd_Fer_us;
static unsigned char ucVoltageMode;
void FOPEM_jig_start_scan_ad()
{
	dPeriodScanAd_Fer_us = stSysSettingAdc.tPeriodSample_us;
	ucVoltageMode = stSysSettingAdc.ucSampleVoltMode;
	Dmp609_Timer_Scan_Ad(m_hDevice, 1, 6, ucVoltageMode, 0, dPeriodScanAd_Fer_us); // 400.0
//	Dmp609_Timer_Scan_Ad(m_hDevice, 1, 6,
//		stSysSettingAdc.ucSampleVoltMode,
//		0,
//		stSysSettingAdc.tPeriodSample_us);
}

static int nReadAdLen;
void FOPEM_jig_read_ad()
{
	nReadAdLen = stSysSettingAdc.nSampleLen;
	Dmp609_Read_Ad(m_hDevice, nReadAdLen, aiData);  // nLen * 12
}

void FOPEM_jig_end_scan_ad()
{
	Dmp609_Timer_End_Ad(m_hDevice);
}

void FOPEM_close_device()  // @5
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)
	{
		CloseDevice(m_hDevice);
	}
}

char WB_fast_analyze_start_by_vel()
{

	Dmp609_Read_Ad(m_hDevice, __FAST_ANA_LEN__ * 6, aiData);

	char cReturn = FALSE;
	nOffsetBondStart = -1;
	int ii;
	for(ii=0; ii< __FAST_ANA_LEN__; ii++)
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
			cReturn = TRUE;
			break;
		}
	}

	return cReturn;
}

void CDMP609VCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDMP609VCDlg::OnPaint() 
{
    CPaintDC dc(this);
    CBrush br;
    CRect re;
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}

   br.CreateSolidBrush(RGB(0,0,0));
    CBrush*pOldBrush=dc.SelectObject(&br);

    dc.Rectangle(30,250,545,650);//pDC->
   
    CPen pen(PS_SOLID,4,RGB(0,0,225));   //画曲线的外框
    CPen*pOldPen=dc.SelectObject(&pen);
    dc.MoveTo(30,250);
    dc.LineTo(545,250);	
    dc.MoveTo(30,250);
    dc.LineTo(30,650);
    CPen pen3(PS_SOLID,6,RGB(255,255,200));   //
    dc.SelectObject(&pen3);
    dc.MoveTo(30,652);
    dc.LineTo(545,652);
    dc.MoveTo(548,250);
    dc.LineTo(548,650);	
    dc.SelectObject(pOldPen);

CPen pen1(PS_SOLID,1,RGB(50,50,50));    //画曲线坐标线---横线PS_DOT
CPen*pOldPen1=dc.SelectObject(&pen1);
dc.MoveTo(30,290);
dc.LineTo(545,290);	
dc.MoveTo(30,330);
dc.LineTo(545,330);	
dc.MoveTo(30,370);
dc.LineTo(545,370);	
dc.MoveTo(30,410);
dc.LineTo(545,410);////
dc.MoveTo(30,450);
dc.LineTo(545,450);	
dc.MoveTo(30,490);
dc.LineTo(545,490);
dc.MoveTo(31,530);
dc.LineTo(545,530);	
dc.MoveTo(30,570);//525
dc.LineTo(545,570);
dc.MoveTo(30,610);//588
dc.LineTo(545,610);//588	

dc.MoveTo(94,250);//画曲线坐标线---竖线
dc.LineTo(94,650);
dc.MoveTo(158,250);
dc.LineTo(158,650);
dc.MoveTo(222,250);
dc.LineTo(222,650);		
dc.MoveTo(286,250);
dc.LineTo(286,650);	
dc.MoveTo(350,250);
dc.LineTo(350,650);	
dc.MoveTo(414,250);
dc.LineTo(414,650);	
dc.MoveTo(478,250);
dc.LineTo(478,650);	//

//pDC->MoveTo(630,250);
//pDC->LineTo(630,650);	
//pDC->MoveTo(705,250);
//pDC->LineTo(705,650);	

dc.SelectObject(pOldPen);

	/*else
	{
		CDialog::OnPaint();
	}*/
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDMP609VCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDMP609VCDlg::OnAd() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{
        k=0;
		m_ad_data=Dmp609_Ad(m_hDevice,ch,g);

		if(g==0)
		{
           m_ad_v=(float)(m_ad_data*2.5/65535);
		}
		if(g==1)
		{
           m_ad_v=(float)(m_ad_data*5.0/65535);
		}
		if(g==2)
		{
           m_ad_v=(float)((m_ad_data-32767)*5.0/32767);
		}

        UpdateData(FALSE);
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");		
}

void CDMP609VCDlg::OnSdAd() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{	  	
        k=1;     
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");	
}

void CDMP609VCDlg::OnTiAd() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{
        k=0;
        UpdateData(TRUE);

		Dmp609_Timer_Ad(m_hDevice,ch,g,0,m_fer);
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");	
}

#include "EFS_DisplayCfg.h"

extern char cFlagDummyBond;

void FOPEM_UpdateReadAd()
{
    FOPEM_jig_end_scan_ad();
	FOPEM_jig_read_ad();
    FOPEM_jig_start_scan_ad();

    //if(cFlagDummyBond == 1)
    //{
    //    WB_analyze_start_by_vel();
    //}
    //else
    //{
        WB_analyze_start_by_efo();
     //}
}

void CDMP609VCDlg::UpdateReadAd()
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)
	{
		FOPEM_UpdateReadAd();
	}
}

void CDMP609VCDlg::OnRAd() 
{

	int n1;
    CClientDC dc(this);
    CPen pen(PS_SOLID,1,RGB(255,0,0));   
    CPen*pOldPen=dc.SelectObject(&pen);

    CPen penBlack(PS_SOLID,1,RGB(0,0,0));   

    CBrush br;
	br.CreateSolidBrush(RGB(0,0,0));
    CBrush*pOldBrush=dc.SelectObject(&br);

    Dmp609_Read_Ad(m_hDevice,512,AD_data);//读取已采样的496点数据组。
	Lx=0;
	Ly=0;
	n=0;	
    ////////////////////画曲线///////////////////////
	for(n1=0;n1<512;n1++)
	{
		n++;

		pOldPen=dc.SelectObject(&penBlack);
        dc.MoveTo((30+n),250);
        dc.LineTo((30+n),650);//

		if(n1 >= 1)
		{
			pOldPen=dc.SelectObject(&pen);
			dc.MoveTo((30+Lx),(650-Ly));
			dc.LineTo((30+n), (int)(650-(AD_data[n1]*2.5*160/65535)));//
		}
	    Lx=n;
	    Ly= (int)(AD_data[n1]*2.5*160/65535);//
				//n1++;
	}
    ///////////////////////////////////////////////////////////
	dc.SelectObject(pOldPen);	
}

void CDMP609VCDlg::OnScAd() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{
	   UpdateData(TRUE);
       Dmp609_Scan_Ad(m_hDevice,1,16,g,AD_data1);//m_s,m_red

       m_ch1=(int)AD_data1[0];
       m_ch2=AD_data1[1];
       m_ch3=AD_data1[2];
       m_ch4=AD_data1[3];
       m_ch5=AD_data1[4];
       m_ch6=AD_data1[5];
       m_ch7=AD_data1[6];
       m_ch8=AD_data1[7];
       m_ch9=AD_data1[8];
       m_ch10=AD_data1[9];
       m_ch11=AD_data1[10];
       m_ch12=AD_data1[11];
       m_ch13=AD_data1[12];
       m_ch14=AD_data1[13];
       m_ch15=AD_data1[14];
       m_ch16=AD_data1[15];
       UpdateData(FALSE);
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");	
}

void CDMP609VCDlg::OnDa1() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{	  	
	   Dmp609_Da(m_hDevice,1,2047);//
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");		
}

void CDMP609VCDlg::OnDa2() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{	  	
	   Dmp609_Da(m_hDevice,2,4095);//
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");	
}

void CDMP609VCDlg::OnDo1() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{
        Dmp609_Set_Do(m_hDevice,1,1);
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");	
}

void CDMP609VCDlg::OnDo116() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{
	  	UpdateData(TRUE);
        Dmp609_Set_Do(m_hDevice,18,65535);
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");	
}

void CDMP609VCDlg::OnIo1() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{	  	
        m_io1_data=Dmp609_Get_Di(m_hDevice,1);
        UpdateData(FALSE);
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");		
}

void CDMP609VCDlg::OnIo116() 
{
	if(m_hDevice!=INVALID_HANDLE_VALUE)//必须测试设备句柄是否存在
	{	  	
        m_io1_16_data=Dmp609_Get_Di(m_hDevice,18);
        UpdateData(FALSE);
	}
	else
	   MessageBox("          未打开设备或设备连接不正确         \n\n ");	
}

void CDMP609VCDlg::OnTimer(UINT nIDEvent) 
{
	if(k==1)//必须测试设备句柄是否存在
	{
		m_ad_data = Dmp609_Ad(m_hDevice,ch,g);

		if(g==0)
		{
           m_ad_v=(float)(m_ad_data*2.5/65535);
		}
		if(g==1)
		{
           m_ad_v=(float)(m_ad_data*5.0/65535);
		}
		if(g==2)
		{
           m_ad_v=(float)((m_ad_data-32767)*5.0/32767);
		}
        UpdateData(FALSE);

	}	
static unsigned short nValidDataLen;
		if(pDlgForcePowerMonitor->GetFlagShowWindow() >= 1
			&& pDlgForcePowerMonitor->GetFlagPauseCapture() == 0)
		{
//			UpdateReadAd();
			pDlgForcePowerMonitor->FOPEM_TimerEvent();

			//if(nOffsetBondStart > 0)
			//{
			//	nValidDataLen = (nLen - nOffsetBondStart) * 12 * sizeof(int);
			//	memcpy(aiBakData, &aiData[6*nOffsetBondStart], nValidDataLen);
			//	memset(aiBakData, 0, (65536 - nValidDataLen));
			//	pDlgForcePowerMonitor->FOPEM_TimerEventCalculatePlotAfterCapture();
			//}
		}

	//if(pDlgForcePowerMonitor->GetFlagShowWindow() >= 1)
	//{
	//	pDlgForcePowerMonitor->FOPEM_TimerEvent();
	//}
	CDialog::OnTimer(nIDEvent);
}

void CDMP609VCDlg::OnRa1() 
{
   ch=1;	
}

void CDMP609VCDlg::OnRa2() 
{
   ch=2;	
}

void CDMP609VCDlg::OnRa3() 
{
   ch=3;	
}

void CDMP609VCDlg::OnRa4() 
{
   ch=4;	
}

void CDMP609VCDlg::OnRa5() 
{
   ch=5;	
}

void CDMP609VCDlg::OnRa6() 
{
   ch=6;	
}

void CDMP609VCDlg::OnRa7() 
{
   ch=7;	
}

void CDMP609VCDlg::OnRa8() 
{
   ch=8;	
}

void CDMP609VCDlg::OnRa9() 
{
   ch=9;	
}

void CDMP609VCDlg::OnRa10() 
{
   ch=10;	
}

void CDMP609VCDlg::OnRa11() 
{
   ch=11;	
}

void CDMP609VCDlg::OnRa12() 
{
   ch=12;	
}

void CDMP609VCDlg::OnRa13() 
{
   ch=13;	
}

void CDMP609VCDlg::OnRa14() 
{
   ch=14;	
}

void CDMP609VCDlg::OnRa15() 
{
   ch=15;	
}

void CDMP609VCDlg::OnRa16() 
{
   ch=16;	
}

void CDMP609VCDlg::OnRa21() 
{
   g=0;		
}

void CDMP609VCDlg::OnRa22() 
{
   g=1;	
}
void CDMP609VCDlg::OnRa23() 
{
   g=2;	
}
void CDMP609VCDlg::SetCheck(UINT idBtn)
{
	static UINT buttons [3]={IDC_RA21,IDC_RA22,IDC_RA23};
	CButton *pBtn=NULL;
	for(int i=0;i<3;i++)
	{
		pBtn = (CButton*)GetDlgItem(buttons[i]);
		if(!pBtn) continue;

		if(buttons[i]==idBtn)//1
			pBtn->SetCheck(1);
		else
			pBtn->SetCheck(0);
	}
}

void CDMP609VCDlg::SetCheck_CH(UINT idBtn_CH)
{
	static UINT buttons [16]={IDC_RA1,IDC_RA2,IDC_RA3,IDC_RA4,IDC_RA5,IDC_RA6,IDC_RA7,IDC_RA8,
		                    IDC_RA9,IDC_RA10,IDC_RA11,IDC_RA12,IDC_RA13,IDC_RA14,IDC_RA15,IDC_RA16};
	CButton *pBtn_CH=NULL;
	for(int i=0;i<16;i++)
	{
		pBtn_CH = (CButton*)GetDlgItem(buttons[i]);
		if(!pBtn_CH) continue;

		if(buttons[i]==idBtn_CH)//1
			pBtn_CH->SetCheck(1);
		else
			pBtn_CH->SetCheck(0);
	}
}

void CDMP609VCDlg::ReadIntegerFromEdit(int nResId, int *iValue)
{
	static char tempChar[32];
	GetDlgItem(nResId)->GetWindowTextA(&tempChar[0], 32);
	sscanf_s(tempChar, "%d", iValue);
}

void CDMP609VCDlg::ReadUnsignedIntegerFromEdit(int nResId, unsigned int *iValue)
{
	static char tempChar[32];
	GetDlgItem(nResId)->GetWindowTextA(&tempChar[0], 32);
	sscanf_s(tempChar, "%d", iValue);
}

// IDC_COMBO_DA1_ANALOG_WAVE
void CDMP609VCDlg::OnCbnSelchangeComboDa1AnalogWave()
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_DA1_ANALOG_WAVE);
	ucFlagAnalogOutput = pCombo->GetCurSel();
	if(ucFlagEnableAnalogOutput > 0)
	{
		Dmp609_AnalogWave(m_hDevice, ucFlagAnalogOutput, ucFlagEnableAnalogOutput, uiFreqAnalogOutput);
	}
}

// IDC_EDIT_FREQUENCY_ANALOG_OUT
void CDMP609VCDlg::OnEnKillfocusEditFrequencyAnalogOut()
{
	//GetDlgItem(IDC_EDIT_FREQUENCY_ANALOG_OUT)
	ReadUnsignedIntegerFromEdit(IDC_EDIT_FREQUENCY_ANALOG_OUT, &uiFreqAnalogOutput);
	if(uiFreqAnalogOutput < 10)
	{
		uiFreqAnalogOutput = 10;
	}
	else if(uiFreqAnalogOutput > 10000)
	{
		uiFreqAnalogOutput = 10000;
	}
	if(ucFlagEnableAnalogOutput > 0)
	{
		Dmp609_AnalogWave(m_hDevice, ucFlagAnalogOutput, ucFlagEnableAnalogOutput, uiFreqAnalogOutput);
	}
}
// IDC_CHECK_FLAG_ENABLE_ANALOG_WAVE
void CDMP609VCDlg::OnBnClickedCheckFlagEnableAnalogWave()
{
	ucFlagEnableAnalogOutput = ((CButton*) GetDlgItem(IDC_CHECK_FLAG_ENABLE_ANALOG_WAVE))->GetCheck();
	Dmp609_AnalogWave(m_hDevice, ucFlagAnalogOutput, ucFlagEnableAnalogOutput, uiFreqAnalogOutput);
}

void CDMP609VCDlg::OnEnKillfocusFer()
{
	// TODO: Add your control notification handler code here
}

void CDMP609VCDlg::OnBnClickedButtonStartForcePowerMonitor()
{
	// IDD_DLG_FORCE_POWER_MONITOR 
	if(pDlgForcePowerMonitor == NULL)
	{
		pDlgForcePowerMonitor = new DlgForcePowerMonitor(this);
		pDlgForcePowerMonitor->Create(IDD_DLG_FORCE_POWER_MONITOR);

	}

	pDlgForcePowerMonitor->ShowWindow(1);
}
