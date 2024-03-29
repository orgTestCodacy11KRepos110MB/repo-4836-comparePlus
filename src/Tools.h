/*
 * This file is part of ComparePlus plugin for Notepad++
 * Copyright (C) 2016-2022 Pavel Nedev (pg.nedev@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#include <map>
#include <windows.h>
#include <tchar.h>


/**
 *  \struct
 *  \brief
 */
template <typename T>
struct ScopedIncrementer
{
	ScopedIncrementer(T& useCount) : _useCount(useCount)
	{
		++_useCount;
	}

	~ScopedIncrementer()
	{
		--_useCount;
	}

	ScopedIncrementer& operator=(const ScopedIncrementer&) = delete;

private:
	T&	_useCount;
};


using ScopedIncrementerInt = ScopedIncrementer<int>;


/**
 *  \class
 *  \brief
 */
class DelayedWork
{
public:
	bool post(UINT delay_ms);
	void cancel();

	bool isPending() const
	{
		return (_timerId != 0);
	}

	explicit operator bool() const
	{
		return (_timerId != 0);
	}

	bool operator!() const
	{
		return (_timerId == 0);
	}

protected:
	DelayedWork() : _timerId(0) {}
	DelayedWork(const DelayedWork&) = delete;
	DelayedWork& operator=(const DelayedWork&) = delete;

	virtual ~DelayedWork()
	{
		cancel();
	}

	virtual void operator()() = 0;

	UINT_PTR _timerId;

private:
	static VOID CALLBACK timerCB(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	static std::map<UINT_PTR, DelayedWork*> workMap;
};


inline void flushMsgQueue()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
}


inline bool fileExists(const TCHAR* filePath)
{
	if (filePath == nullptr)
		return false;

	DWORD dwAttrib = ::GetFileAttributes(filePath);
	return (bool)(dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
