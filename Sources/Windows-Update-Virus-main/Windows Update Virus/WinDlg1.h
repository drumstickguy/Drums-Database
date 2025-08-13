#if !defined(AFX_WINDLG1_H__C241DD11_A925_40F5_9320_931B160EEF4A__INCLUDED_)
#define AFX_WINDLG1_H__C241DD11_A925_40F5_9320_931B160EEF4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WinDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WinDlg dialog

class WinDlg : public CDialog
{
// Construction
public:
	WinDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(WinDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(WinDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDLG1_H__C241DD11_A925_40F5_9320_931B160EEF4A__INCLUDED_)
