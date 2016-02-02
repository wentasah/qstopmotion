/******************************************************************************
 *  Copyright (C) 2010-2016 by                                                *
 *    Ralf Lange (ralf.lange@longsoft.de)                                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 ******************************************************************************/

#ifndef MEDIAFROUNDATION_MFUTIL_H
#define MEDIAFROUNDATION_MFUTIL_H

#include <QString>
#include <mfapi.h>

template <class T>
inline void safeDelete(T& a)
{
    if (a)
        delete a;
    a = 0;
}

template <class T>
inline void safeDeleteArray(T& a)
{
    if (a)
        delete [] a;
    a = 0;
}

template <class T>
inline void safeRenew(T& a, T obj)
{
    if (a)
        delete a;
    a = obj;
}

template <class T>
void SafeRelease(T** ppT)
{
    if (*ppT != NULL) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// The following code enables you to view the contents of a media type while
// debugging.

// void LogUINT32AsUINT64(const PROPVARIANT& var);

// float OffsetToFloat(const MFOffset& offset);

// HRESULT LogVideoArea(const PROPVARIANT& var);

// Handle certain known special cases.
// HRESULT SpecialCaseAttributeValue(GUID guid, const PROPVARIANT& var);

// HRESULT LogAttributeValueByIndex(IMFAttributes *pAttr, DWORD index);

HRESULT LogMediaType(IMFMediaType *pType);

#ifndef IF_EQUAL_RETURN
#define IF_EQUAL_RETURN(param, val) if(val == param) return QString(#val)
#endif

QString GetGUIDNameConst(const GUID& guid);

#endif // MEDIAFROUNDATION_MFUTIL_H
