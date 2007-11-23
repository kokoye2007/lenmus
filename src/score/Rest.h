//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2007 Cecilio Salmeron
//
//    This program is free software; you can redistribute it and/or modify it under the 
//    terms of the GNU General Public License as published by the Free Software Foundation;
//    either version 2 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY 
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this 
//    program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, 
//    Fifth Floor, Boston, MA  02110-1301, USA.
//
//    For any comment, suggestion or feature request, please contact the manager of 
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#ifndef __REST_H__        //to avoid nested includes
#define __REST_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "Rest.cpp"
#endif

#include "Glyph.h"


class lmRest: public lmNoteRest
{
public:
    lmRest(lmVStaff* pVStaff, 
        ENoteType nNoteType, float rDuration,
        bool fDotted, bool fDoubleDotted,
        int nStaff, bool fVisible,
        lmContext* pContext, 
        bool fBeamed, lmTBeamInfo BeamInfo[]);

    ~lmRest();

    //implementation of virtual methods of base classes
        // lmStaffObj
    void LayoutObject(lmBox* pBox, lmPaper* pPaper, wxColour colorC);

    wxString    Dump();
    wxString    SourceLDP(int nIndent);
    wxString    SourceXML(int nIndent);

        //lmStaffObj
    lmScoreObj* FindSelectableObject(lmUPoint& pt);

        //lmNoteRest
    bool IsInChord() { return false; }        

    //lmRest methods
    void DoVerticalShift(lmTenths yShift);



private:
    // access to glyph data to define character to use and selection rectangle 
    lmEGlyphIndex GetGlyphIndex();

};


#endif    // __REST_H__
