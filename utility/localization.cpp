/*
This is a part of the LiteStep Shell Source code.

Copyright (C) 1997-2002 The LiteStep Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 
/****************************************************************************
****************************************************************************/ 
/*
	Provides routines useful for localizing code
*/
#include "localization.h"
#include "stdio.h"
#include "safestr.h"

#define CHINESESIMPLIFIED_CHARSET 134

//
// void GetResStr(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText)
//
void GetResStr(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText)
{
	if (IsValidStringPtr(pszText, cchText))
	{
		if (LoadString(hInstance, uIDText, pszText, cchText) == 0)
		{
			StringCchCopy(pszText, cchText, pszDefText);
		}
	}
}


//
// GetResStrEx(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText, ...)
//
void GetResStrEx(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText, ...)
{
	SAFE_CHAR(szFormat, MAX_LINE_LENGTH)
	va_list vargs;

	if (IsValidStringPtr(pszText, cchText))
	{
		GetResStr(hInstance, uIDText, szFormat, MAX_LINE_LENGTH, pszDefText);

		va_start(vargs, pszDefText);
		StringCchVPrintf(pszText, cchText, szFormat, vargs);
		va_end(vargs);
	}
}

HFONT LocalCreateFont(int nHeight,
                      int nWidth,
                      int nEscapement,
                      int nOrientation,
                      int fnWeight,
                      DWORD fdwItalic,
                      DWORD fdwUnderline,
                      DWORD fdwStrikeOut,
                      DWORD fdwCharSet,
                      DWORD fdwOutputPrecision,
                      DWORD fdwClipPrecision,
                      DWORD fdwQuality,
                      DWORD fdwPitchAndFamily,
                      LPCTSTR lpszFace)
{
	DWORD dwCharSet = fdwCharSet;

	switch (GetUserDefaultLCID())
	{
		case 0x404:
		{
			dwCharSet = CHINESEBIG5_CHARSET;
		}
		break;

		case 0x411:
		{
			dwCharSet = SHIFTJIS_CHARSET;
		}
		break;

		case 0x412:
		{
			dwCharSet = HANGEUL_CHARSET;
		}
		break;

		case 0x804:
		{
			dwCharSet = CHINESESIMPLIFIED_CHARSET;
		}
		break;
	}

	return CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight,
	                  fdwItalic, fdwUnderline, fdwStrikeOut, dwCharSet,
	                  fdwOutputPrecision, fdwClipPrecision, fdwQuality,
	                  fdwPitchAndFamily, lpszFace);
}

HFONT LocalCreateFontIndirect(LOGFONT *lplf)
{
	switch (GetUserDefaultLCID())
	{
		case 0x404:
		{
			lplf->lfCharSet = CHINESEBIG5_CHARSET;
		}
		break;

		case 0x411:
		{
			lplf->lfCharSet = SHIFTJIS_CHARSET;
		}
		break;

		case 0x412:
		{
			lplf->lfCharSet = HANGEUL_CHARSET;
		}
		break;

		case 0x804:
		{
			lplf->lfCharSet = CHINESESIMPLIFIED_CHARSET;
		}
		break;
	}

	return CreateFontIndirect(lplf);
}


Localization::Localization()
{
	m_hRes = NULL;
	m_wLanguageID = -1;
	
	LANGID wLanguageID;
	
	LANGID (__stdcall * GetUserDefaultUILanguage)(VOID) = NULL;
	GetUserDefaultUILanguage = (LANGID (__stdcall *)(VOID))GetProcAddress(GetModuleHandle(_T("KERNEL32.DLL")), "GetUserDefaultUILanguage");
	if (GetUserDefaultUILanguage)
	{
		wLanguageID = GetUserDefaultUILanguage();
	}
	else
	{
		wLanguageID = GetUserDefaultLangID();
	}
	
	LoadLanguage(wLanguageID);
}


Localization::~Localization()
{
	if (m_hRes != GetModuleHandle(NULL))
	{
		FreeLibrary(m_hRes);
	}
}


void Localization::LoadLanguage(LANGID wLanguageID)
{
	if (wLanguageID != m_wLanguageID)
	{
		TCHAR tzResourceDll[MAX_PATH];
		HINSTANCE hRes;
		
		StringCchPrintf(tzResourceDll, MAX_PATH, _T("res%04x.dll"), wLanguageID);
		
		hRes = LoadLibrary(tzResourceDll);
		if (hRes)
		{
			m_hRes = hRes;
			m_wLanguageID = wLanguageID;
		}
		else
		{
			m_hRes = GetModuleHandle(NULL);
			m_wLanguageID = 0x0404; // English (United States)
		}
	}
}


void Localization::LoadString(UINT uID, LPTSTR ptzBuffer, size_t cchMax)
{
	if (IsValidStringPtr(ptzBuffer, cchMax))
	{
		if (::LoadString(m_hRes, uID, ptzBuffer, cchMax) == 0)
		{
			if (::LoadString(GetModuleHandle(NULL), uID, ptzBuffer, cchMax) == 0)
		 	{
		 		ptzBuffer[0] = _T('\0');
		 	}
		}
	}
}


int Localization::MessageBox(HWND hWnd, UINT uText, UINT uCaption, UINT uType)
{
	int nReturn = 0;
	TCHAR tzCaption[MAX_LINE_LENGTH];
	TCHAR tzText[MAX_LINE_LENGTH];
	
	LoadString(uText, tzText, MAX_LINE_LENGTH);
	LoadString(uCaption, tzCaption, MAX_LINE_LENGTH);
	
	nReturn = ::MessageBox(hWnd, tzText, tzCaption, uType);
	
	return nReturn;
}
