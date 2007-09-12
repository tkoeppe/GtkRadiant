/*
BobToolz plugin for GtkRadiant
Copyright (C) 2001 Gordon Biggans

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if !defined(AFX_AUTOCAULKSTARTDIALOG_H__F3DE2E81_E73E_11D4_ACF7_004095A18133__INCLUDED_)
#define AFX_AUTOCAULKSTARTDIALOG_H__F3DE2E81_E73E_11D4_ACF7_004095A18133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCaulkStartDialog.h : header file
//

#define MODE_AC_NORMAL			0
#define MODE_AC_BUILD_MINI_PRT	1
#define MODE_AC_SUPER			2

/////////////////////////////////////////////////////////////////////////////
// CAutoCaulkStartDialog dialog

class CAutoCaulkStartDialog : public CDialog
{
// Construction
public:
	CAutoCaulkStartDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoCaulkStartDialog)
	enum { IDD = IDD_AUTOCAULKSTART_DIALOG };
	BOOL	m_bAllowDestruction;
	CString	m_Warning1;
	int		m_nMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCaulkStartDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoCaulkStartDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCAULKSTARTDIALOG_H__F3DE2E81_E73E_11D4_ACF7_004095A18133__INCLUDED_)
