// WinDlg.h : header file
//

#include "PictureEx.h"


#if !defined(AFX_WINDLG_H__77E16C91_CC04_4A7F_828D_EB669EDB91FC__INCLUDED_)
#define AFX_WINDLG_H__77E16C91_CC04_4A7F_828D_EB669EDB91FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWinDlg dialog

class CWinDlg : public CDialog
{
// Construction
public:
	CWinDlg(CWnd* pParent = NULL);	// standard constructor

	int PictureNumber;

// Dialog Data
	//{{AFX_DATA(CWinDlg)
	enum { IDD = IDD_WIN_DIALOG };
	CStatic	m_Taskbar;
	CStatic	m_StartMenu;
	CStatic	m_LoadPicture;
	CPictureEx m_Loading;
	CPictureEx m_GifContent;
	CStatic	m_Content;
	CStatic	Picture;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartBUTTON();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDoubleclickedDesktopIcon1();
	afx_msg void OnDoubleclickedDesktopIcon2();
	afx_msg void OnDoubleclickedDesktopIcon3();
	afx_msg void OnDoubleclickedDesktopIcon4();
	afx_msg void OnDoubleclickedDesktopIcon5();
	afx_msg void OnDoubleclickedDesktopIcon6();
	afx_msg void OnClose();
	afx_msg void OnClose2();
	afx_msg void DesktopElements(bool Flag);
	afx_msg void FlashTerror();
	afx_msg void RunTerror();
	afx_msg void PlaySwitch(bool Flag);
	afx_msg void DesktopEnabled(bool Flag);
	afx_msg void RefreshDesktopIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDLG_H__77E16C91_CC04_4A7F_828D_EB669EDB91FC__INCLUDED_)
