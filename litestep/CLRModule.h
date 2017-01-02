//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This is a part of the Litestep Shell source code.
//
// Copyright (C) 1997-2015  LiteStep Development Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#if !defined(CLRMODULE_H)
#define CLRMODULE_H

#include "Module.h"
#include <string>

#include <metahost.h>

class CLRModule : public Module
{
private:
	ICorRuntimeHost *m_pCorRuntimeHost;
	ICLRMetaHost	*m_pMetaHost;
	ICLRRuntimeInfo *m_pRuntimeInfo;

public:
	CLRModule(const std::wstring& sLocation, DWORD dwFlags);
	~CLRModule();

	bool Init(HWND hMainWindow, const std::wstring& sAppPath);
	void Quit();
	HINSTANCE  GetInstance() const;
	
private:
	bool _Initialize();
	void _Dispose();
	HRESULT _InvokeMethod(LPCWSTR szMethodName, SAFEARRAY * pArgs);
};

#endif