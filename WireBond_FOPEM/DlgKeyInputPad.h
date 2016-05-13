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

#include "resource.h"
// CDlgKeyInputPad dialog

class CDlgKeyInputPad : public CDialog
{
	DECLARE_DYNAMIC(CDlgKeyInputPad)

public:
	CDlgKeyInputPad(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgKeyInputPad();
	virtual BOOL OnInitDialog();

	BOOL bFlagShowNumberKeyPad;
	int iInputNumber;
	int iReturnNumber;

	void SetInputNumber(int iNumber);
	int GetReturnNumber();
	void SetFlagShowNumberOnKeyPad(BOOL bShow);
	void ReadIntegerFromEdit(int nResId, int *iValue);

// Dialog Data
	enum { IDD = IDD_DIALOG_KEY_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDlgKey1();
	afx_msg void OnBnClickedButtonDlgKey2();
	afx_msg void OnBnClickedButtonDlgKey3();
	afx_msg void OnBnClickedButtonDlgKey4();
	afx_msg void OnBnClickedButtonDlgKey5();
	afx_msg void OnBnClickedButtonDlgKey6();
	afx_msg void OnBnClickedButtonDlgKey7();
	afx_msg void OnBnClickedButtonDlgKey8();
	afx_msg void OnBnClickedButtonDlgKey9();
	afx_msg void OnBnClickedButtonDlgKey0();
	afx_msg void OnBnClickedCheckDlgShowNumber();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDlgKeyPadClear();
	afx_msg void OnEnChangeEditDlgKeyInputShowText();
	afx_msg void OnBnClickedButtonDlgKeyPadOk();
};
