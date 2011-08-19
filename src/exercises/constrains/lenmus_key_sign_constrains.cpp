//---------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2011 LenMus project
//
//    This program is free software; you can redistribute it and/or modify it under the
//    terms of the GNU General Public License as published by the Free Software Foundation,
//    either version 3 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this
//    program. If not, see <http://www.gnu.org/licenses/>.
//
//    For any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//---------------------------------------------------------------------------------------

////lenmus
//#include "lenmus_constrains.h"
//
//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma implementation "TheoKeySignConstrains.h"
//#endif
//
//// For compilers that support precompilation, includes <wx.h>.
//#include <wx/wxprec.h>
//
//#ifdef __BORLANDC__
//#pragma hdrstop
//#endif
//
//#include "TheoKeySignConstrains.h"
//
//lmTheoKeySignConstrains::lmTheoKeySignConstrains(wxString sSection)
//    : lmExerciseOptions(sSection)
//{
//    //
//    // default values
//    //
//
//    // all key signatures allowed
//    int i;
//    for (i=lmMIN_KEY; i <= lmMAX_KEY; i++) {
//        m_oValidKeys.SetValid((lmEKeySignatures)i, true);
//    }
//
//    // only G clef allowed
//    for (i = lmMIN_CLEF; i <= lmMAX_CLEF; i++) {
//        m_oClefs.SetValid((lmEClefType) i, false);
//    }
//    m_oClefs.SetValid(lmE_Sol, true);
//
//    // other settings
//    m_nProblemType = eBothKeySignProblems;      // both problems allowed
//    m_nMaxAccidentals = 5;                      // max.: 5 accidentals
//    m_nMode = eMayorAndMinorModes;              // both, major and minor, allowed
//
//}
//