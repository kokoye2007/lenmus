//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2009 LenMus project
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
//-------------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "Processor.h"
#endif

// for (compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#else
    #include <wx/sizer.h>
    #include <wx/panel.h>
#endif


#include "Processor.h"

// access to main frame
#include "../app/MainFrame.h"
extern lmMainFrame* GetMainFrame();

#include "toolbox/ToolsBox.h"
#include "ScoreDoc.h"
#include "../exercises/auxctrols/UrlAuxCtrol.h"
#include "../score/Score.h"
#include "../score/VStaff.h"
#include "../score/AuxObj.h"        //lmScoreLine
#include "../graphic/ShapeNote.h"

//access to error's logger
#include "../app/Logger.h"
extern lmLogger* g_pLogger;
#include "../auxmusic/ChordManager.h"
//@@ TODO: remove, alredy in *.h #include "../auxmusic/Harmony.h"

//-------------------------------------------------------------------------------------------
// Implementation of class lmScoreProcessor
//-------------------------------------------------------------------------------------------

//IDs for controls
const int lmID_DO_PROCESS = wxNewId();
const int lmID_UNDO_PROCESS = wxNewId();

IMPLEMENT_ABSTRACT_CLASS(lmScoreProcessor, wxEvtHandler)

lmScoreProcessor::lmScoreProcessor()
    : wxEvtHandler()
{
}

lmScoreProcessor::~lmScoreProcessor()
{
}

void lmScoreProcessor::DoProcess()
{
    //get the score
    lmMainFrame* pMainFrame = GetMainFrame();
    lmDocument* pDoc = pMainFrame->GetActiveDoc();
    if (!pDoc) return;
    lmScore* pScore = pDoc->GetScore();
    if (!pScore) return;

    if (ProcessScore(pScore))
    {
        //if changes done in the score, update views
	    pDoc->Modify(true);
        pDoc->UpdateAllViews(true, new lmUpdateHint() );

        //enable undo link
        m_pUndoLink->Enable(true);
    }
}

void lmScoreProcessor::UndoProcess()
{
    //get the score
    lmMainFrame* pMainFrame = GetMainFrame();
    lmDocument* pDoc = pMainFrame->GetActiveDoc();
    if (!pDoc) return;
    lmScore* pScore = pDoc->GetScore();
    if (!pScore) return;

    if (UndoChanges(pScore))
    {
        //if changes done in the score, update views
	    pDoc->Modify(true);
        pDoc->UpdateAllViews(true, new lmUpdateHint() );
    }

    //disable undo link
    m_pUndoLink->Enable(false);
}

bool lmScoreProcessor::CreateToolsPanel(wxString sTitle, wxString sDoLink,
                                       wxString sUndoLink)
{
    lmMainFrame* pMainFrame = GetMainFrame();
    lmToolBox* pToolbox = pMainFrame->GetActiveToolBox();
    if (!pToolbox) return false;        //false -> failure

    m_pToolsPanel = new wxPanel(pToolbox, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

	m_pMainSizer = new wxBoxSizer( wxVERTICAL );

    AddStandardLinks(m_pMainSizer, sDoLink, sUndoLink);

    return true;
}
void lmScoreProcessor::AddStandardLinks(wxBoxSizer* pSizer, wxString sDoLink,
                                        wxString sUndoLink)
{
    // 'Do process' link
    if (sDoLink != wxEmptyString)
    {
        m_pDoLink = new lmUrlAuxCtrol(m_pToolsPanel, lmID_DO_PROCESS, 1.0, sDoLink);
        pSizer->Add(m_pDoLink, 0, wxALL|wxEXPAND, 5);
        m_pToolsPanel->Connect(lmID_DO_PROCESS, lmEVT_URL_CLICK,
                               wxObjectEventFunction(&lmScoreProcessor::DoProcess),
                               NULL, this);
    }

    // 'Do process' link
    if (sDoLink != wxEmptyString)
    {
        m_pUndoLink = new lmUrlAuxCtrol(m_pToolsPanel, lmID_UNDO_PROCESS, 1.0, sUndoLink);
        pSizer->Add(m_pUndoLink, 0, wxALL|wxEXPAND, 5);
        m_pToolsPanel->Connect(lmID_UNDO_PROCESS, lmEVT_URL_CLICK,
                               wxObjectEventFunction(&lmScoreProcessor::UndoProcess),
                               NULL, this);
    }
}

void lmScoreProcessor::RealizePanel()
{
    m_pToolsPanel->SetSizer( m_pMainSizer );
	m_pToolsPanel->Layout();
	m_pMainSizer->Fit( m_pToolsPanel );

    lmMainFrame* pMainFrame = GetMainFrame();
    lmToolBox* pToolbox = pMainFrame->GetActiveToolBox();
    if (pToolbox)
        pToolbox->AddSpecialTools(m_pToolsPanel, this);

    //disable undo link
    m_pUndoLink->Enable(false);
}


/*------------ TODO: TEST ONLY; REMOVE!
//-------------------------------------------------------------------------------------------
// Implementation of class lmTestProcessor
//-------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(lmTestProcessor, lmScoreProcessor)

lmTestProcessor::lmTestProcessor()
    : lmScoreProcessor()
{
}

lmTestProcessor::~lmTestProcessor()
{
}

bool lmTestProcessor::SetTools()
{
    //Create a panel with the exercise buttons, and place it on the ToolBox
    //Returns false if failure.

    return true;
}

bool lmTestProcessor::ProcessScore(lmScore* pScore)
{
    //This method checks the score and show errors
    //Returns true if it has done any change in the score

	bool fScoreModified = false;

	//Get the instrument
	lmInstrument* pInstr = pScore->GetFirstInstrument();
	lmVStaff* pVStaff = pInstr->GetVStaff();

    //notes to join with arrows
	int nNote = 0;
    lmNote* pNote1;
    lmNote* pNote2;
    lmNote* pNote3;
    lmNote* pNote4;
    lmNote* pNote5;
    lmNote* pNote6;

	// Loop to process notes/rests in first staff of first instrument
	lmSOIterator* pIter = pVStaff->CreateIterator();
	while(!pIter->EndOfCollection())
	{
	  	lmStaffObj* pSO = pIter->GetCurrent();
	  	if (pSO->IsNoteRest() && ((lmNoteRest*)pSO)->IsNote())
	  	{
            // It is a note. Count it
			++nNote;
            if (nNote == 2)
                pNote1 = (lmNote*)pSO;
            else if (nNote == 6)
                pNote2 = (lmNote*)pSO;
            else if (nNote == 3)
                pNote3 = (lmNote*)pSO;
            else if (nNote == 7)
                pNote4 = (lmNote*)pSO;
            else if (nNote == 5)
                pNote5 = (lmNote*)pSO;
            else if (nNote == 12)
                pNote6 = (lmNote*)pSO;

		}
		pIter->MoveNext();
	}
	delete pIter;       //Do not forget this. We are not using smart pointers!

    //Add line between pairs of notes

    if (pNote1 && pNote2)
    {
        DrawArrow(pNote1, pNote2, *wxRED);
	    fScoreModified = true;
    }
    if (pNote3 && pNote4)
    {
        DrawArrow(pNote3, pNote4, wxColour(0, 170, 0) );
	    fScoreModified = true;
    }
    if (pNote5 && pNote6)
    {
        DrawArrow(pNote5, pNote6, wxColour(0, 0, 200) );
	    fScoreModified = true;
    }

	return fScoreModified;      //true -> score modified
}

bool lmTestProcessor::UndoChanges(lmScore* pScore)
{
    //This method removes all error markup from the score

    return true;
}

void lmTestProcessor::DrawArrow(lmNote* pNote1, lmNote* pNote2, wxColour color)
{
    //get VStaff
    lmVStaff* pVStaff = pNote1->GetVStaff();

    //get note heads positions
    lmURect uBounds1 = pNote1->GetNoteheadShape()->GetBounds();
    lmURect uBounds2 = pNote2->GetNoteheadShape()->GetBounds();

    //start point
    lmUPoint uStart( uBounds1.GetWidth(), 0);
    uStart.y = pNote1->GetShiftToNotehead();        //center of notehead

    //end point
    lmUPoint uEnd(uBounds2.GetRightTop() - uBounds1.GetRightTop());
    uEnd.y += uStart.y;

    //convert to tenths
    lmTenths xtStart = pVStaff->LogicalToTenths(uStart.x) + 8.0;
    lmTenths ytStart = pVStaff->LogicalToTenths(uStart.y);
    lmTenths xtEnd = pVStaff->LogicalToTenths(uEnd.x) - 8.0;
    lmTenths ytEnd = pVStaff->LogicalToTenths(uEnd.y);

    //create arrow
    lmScoreLine* pLine = new lmScoreLine(xtStart, ytStart, xtEnd, ytEnd, 2, lm_eLineCap_None,
                                         lm_eLineCap_Arrowhead, lm_eLine_Solid,
                                         color);
    pNote1->SetColour(color);
    pNote2->SetColour(color);
	pNote1->AttachAuxObj(pLine);
}
-------------------*/


//-------------------------------------------------------------------------------------------
// Implementation of class lmHarmonyProcessor
//-------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(lmHarmonyProcessor, lmScoreProcessor)

lmHarmonyProcessor::lmHarmonyProcessor()
    : lmScoreProcessor()
{
}

lmHarmonyProcessor::~lmHarmonyProcessor()
{
    //delete all markup pairs
    std::list<lmMarkup*>::iterator it = m_markup.begin();
    while (it != m_markup.end())
    {
        lmMarkup* pError = *it;
        m_markup.erase(it++);
        delete pError;
    }

    assert(nNumChords<lmMAX_NUM_CHORDS);
    for (int i = 0; i <nNumChords; i++)
        delete tChordDescriptor[i].pChord;
}

bool lmHarmonyProcessor::SetTools()
{
    //Create a panel with the exercise buttons, and place it on the ToolBox
    //Returns false if failure.

    if (!CreateToolsPanel(_("Harmony exercise"),
                          _("Check harmony"),
                          _("Clear errors")) )
        return false;

    //No more tools to add. Show panel in toolbox
    RealizePanel();
    return true;
}


// TODO: ESTO ES SOLO PROVISIONAL!!!!!!!!!!!!!!!!!!!!!!
static const int ntDisXstart = 0;
static const int ntDisXend = -200;
static const int ntDisYstart = 40;
static const int ntDisYend = -120;
void  lmHarmonyProcessor::DisplayChordInfo(lmScore* pScore, lmChordDescriptor*  pChordDsct
                                           , wxColour colour, wxString &sText, bool reset)
{
    int nNumChordNotes  = pChordDsct->nNumChordNotes;
    // Remember: all 'y' positions are relative to top line (5th line of
    //   first staff). 'x' positions are relative to current object position.
    lmTenths ntxStart = ntDisXstart;  // fijo; relativo al usuario
    lmTenths ntxEnd = ntDisXend; // fijo
    static lmTenths ntyStart = ntDisYstart;  // relativo a top line; positivo: abajo
    static lmTenths ntyEnd = ntDisYend;  // negativo: arriba. Se baja en cada uso

	lmTenths nTxPos = ntxEnd + 10;
    lmTenths nTyPos = ntyEnd + 10;
    if ( reset )
    {
        // only reset
        ntyStart = ntDisYstart;  
        ntyEnd = ntDisYend; 
        wxLogMessage(_T("DisplayChordInfo: reset Y coord to  %d %d")
            , ntDisYstart, ntDisYend);
        return;
    }

    //define the font to use for texts
    lmFontInfo tFont = {_("Comic Sans MS"), 6, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL};
    lmTextStyle* pStyle = pScore->GetStyleName(tFont);

    // Display chord info in score with a line and text
    assert(nNumChordNotes > 0);
    assert(nNumChordNotes < 20);
    assert(pChordDsct != NULL);

    for (int i = 0; i<nNumChordNotes; i++)
    {
        assert(pChordDsct->pChordNotes[i] != NULL);
        pChordDsct->pChordNotes[i]->SetColour(colour);
    }

    // Green line pointing to the chord
    // Remember: all 'y' positions are relative to top line (5th line of
    //   first staff). 'x' positions are relative to current object position.
    lmStaffObj* cpSO =pChordDsct->pChordNotes[nNumChordNotes-1];
//not good for linux!    lmAuxObj* pTxtBox = cpSO->AttachTextBox(lmTPoint(nTxPos, nTyPos), lmTPoint(ntxStart, ntyStart),
//                                            sText, pStyle,	wxSize(400, 60), colour);
    lmTPoint lmTP1(nTxPos, nTyPos);
    lmTPoint lmTP2(ntxStart, ntyStart);
    wxSize size(400, 60);
    lmAuxObj* pTxtBox = cpSO->AttachTextBox(lmTP1, lmTP2,
                                            sText, pStyle,	size, colour);

	lmMarkup* pError = new lmMarkup(cpSO, pTxtBox);
    m_markup.push_back(pError);

    ntyEnd +=50; // y positions are NOT relative; change each time

}


// All chord processing:
//  analysis of chord notes and intervals
//  chord creation
//  results: display messages...
//  TODO: consider to improve return: status/reason
bool lmHarmonyProcessor::ProccessChord(lmScore* pScore, lmChordDescriptor* ptChordDescriptorArray
                                       , int* pNumChords, wxString &sStatusStr)
{
    bool fOk = false;
    bool fCanBeCreated = false;
    lmChordDescriptor* ptChordDescriptor = &ptChordDescriptorArray[*pNumChords]; //2@@@ TODO MEJORAR...
    int nNumChordNotes = ptChordDescriptor->nNumChordNotes;

    if (nNumChordNotes < 3)
       return false;

    wxLogMessage(_T("ProccessChord %d, num CHORD NOTES: %d")
    , *pNumChords, nNumChordNotes);

    lmChordInfo tChordInfo;
    tChordInfo.Initalize();

    assert(ptChordDescriptor != NULL);
    assert(ptChordDescriptor->pChordNotes != NULL);

    // Create Chord
    fCanBeCreated = TryChordCreation(nNumChordNotes, ptChordDescriptor->pChordNotes, &tChordInfo,  sStatusStr);
    
    wxColour colour;

    if (fCanBeCreated)
    {
        lmNote* pChordBaseNote = ptChordDescriptor->pChordNotes[0]; //TODO @@@ (confirmar que aun con inversiones la primera nota es la fundamental)
        ptChordDescriptor->pChord = new lmChordManager(pChordBaseNote, tChordInfo);
        sStatusStr = ptChordDescriptor->ToString();
        (*pNumChords)++;
// todo: set definitive colour       colour = *wxGREEN;
        colour = wxColour(10,255,0,128); // R, G, B, Transparency
        // no necessary to display:  DisplayChordInfo(pScore, ptChordDescriptor, colour, sStatusStr);
        fOk = true;
    }
    else
    {
       // Even with errors, the chord is created and used for analysis of progression  (TODO: confirm this)
        lmNote* pChordBaseNote = ptChordDescriptor->pChordNotes[0];
        ptChordDescriptor->pChord = new lmChordManager(pChordBaseNote, tChordInfo);
        sStatusStr = ptChordDescriptor->ToString();
        (*pNumChords)++;
// todo: set definitive colour       colour = *wxRED;
        colour = wxColour(255,0,0,128); // R, G, B, Transparency
        DisplayChordInfo(pScore, ptChordDescriptor, colour, sStatusStr);
        fOk = false;
    }

    wxLogMessage(sStatusStr);



    return fOk;

}

bool lmHarmonyProcessor::ProcessScore(lmScore* pScore)
{
    //This method checks the score and show errors
    //Returns true if it has done any change in the score

    //As an example, I will put red and a green lines pointing to fourth and
    //sixth notes, respectively, and add some texts
    bool fScoreModified = false;
    nNumChords = 0;

    float rAbsTime = 0.0f;
    float rTimeAtStartOfMeasure = 0.0f;

    //Get the instrument
    lmInstrument* pInstr = pScore->GetFirstInstrument();
    lmVStaff* pVStaff = pInstr->GetVStaff();

	lmNote* pCurrentNote;
    float rCurrentNoteAbsTime = -2.0f;
    float rRelativeTime = -2.0f;
    wxString sStatusStr;

    // TODO: MEJORAR ; PROVISIONAL
    //  resetear el control de las indicaciones (reiniciar el espaciado)....
    DisplayChordInfo(pScore, &tChordDescriptor[0], *wxGREEN, sStatusStr, true);

    /*---

      Algorithm of chord detection, keeping a list of "active notes"

       When NEW NOTE
	      if NEW TIME (i.e. higher) then
             update current list of active notes: remove notes with end-time < TIME
             analize possible chord in previous state: with current list of active notes
             set new time
          add new note to the list of active notes

    ---*/

    // loop to process notes/rests in first staff of first instrument
    int nNote = 0;
    lmSOIterator* pIter = pVStaff->CreateIterator();
    while(!pIter->EndOfCollection())
    {
        // process only notes, rests and barlines
        lmStaffObj* pSO = pIter->GetCurrent();
        if (pSO->IsBarline())
        {
            // new measure starts. Update current time
            rTimeAtStartOfMeasure += pSO->GetTimePos();
            rAbsTime = rTimeAtStartOfMeasure;
        }
        else if (pSO->IsNoteRest())
        {
            // we continue in previous measure. Update current time if necessary
            rRelativeTime = pSO->GetTimePos();
            rAbsTime = rTimeAtStartOfMeasure + rRelativeTime;

            // process notes
            if (((lmNoteRest*)pSO)->IsNote())
            {
                // It is a note. Count it
                ++nNote;
			    pCurrentNote  = (lmNote*) pSO;

                // calculate note's absolute time
                rCurrentNoteAbsTime = rTimeAtStartOfMeasure + rRelativeTime;

                // check new starting time (to analyze previous chord candidate) 
                if (  IsHigherTime(rCurrentNoteAbsTime, ActiveNotesList.GetTime())  )
                {
                    /*-----
                      if NEW-TIME (i.e. higher) then
                         update current list of active notes: remove notes with end-time < CURRENT-TIME
                         analize possible chord in previous state: with current list of active notes
                         set new time
                      add new note to the list of active notes
                    ---*/

               
                    // analyze possible chord with current list of active notes
                    ActiveNotesList.GetChordDescriptor(&tChordDescriptor[nNumChords]);

                    bool fChordOk = ProccessChord(pScore, tChordDescriptor, &nNumChords, sStatusStr);

                    // set new time and recalculate list of active notes
                    ActiveNotesList.SetTime( rCurrentNoteAbsTime );

                    fScoreModified = true; // repaint
                }
//                else  wxLogMessage(_T("    not higher ") );

                // add new note to the list of active notes
                ActiveNotesList.AddNote(pCurrentNote, rCurrentNoteAbsTime + pCurrentNote->GetDuration());

            } //  if (((lmNoteRest*)pSO)->IsNote())

        } // [else if (pSO->IsNoteRest()) ] ignore other staff objects

        pIter->MoveNext();

    } // while
    delete pIter;       //Do not forget this. We are not using smart pointers!

    // Analyze the remaining notes
    // 
    ActiveNotesList.RecalculateActiveNotes( );
    ActiveNotesList.GetChordDescriptor(&tChordDescriptor[nNumChords]);
    bool fChordOk = ProccessChord(pScore, tChordDescriptor, &nNumChords, sStatusStr);

    int nNumErrors = AnalyzeChordsLinks(&tChordDescriptor[0], nNumChords);

    wxLogMessage(_T("ANALYSIS RESULT of %d chords:  ok: %d, Num Errors:%d ")
        ,nNumChords, fChordOk, nNumErrors);

    if (fChordOk && nNumErrors == 0)
    {
       wxString sOkMsg = _T(" OK!");
       wxLogMessage( sOkMsg );
       DisplayChordInfo(pScore, &tChordDescriptor[nNumChords-1], *wxGREEN, sOkMsg );
       fScoreModified = true; // repaint
    }

    return fScoreModified;      //true -> score modified
}

// return: total number of errors
int lmHarmonyProcessor::AnalyzeChordsLinks(lmChordDescriptor* pChordDescriptor, int nNCH)
{
    wxLogMessage(_T("AnalyzeChordsLinks N:%d "), nNCH);

    int nNumChordError[lmMAX_NUM_CHORDS]; // number of errors in each chord

    lmRuleList tRules(pChordDescriptor, nNCH);

    wxString sStr;
    sStr.clear();

    int nNumErros = 0; // TODO: @@decidir: num de errores o num de acordes con error
    lmRule* pRule;
    // TODO: crear metodo de la lista que evalue TODAS las reglas??
    for (int nR = lmCVR_FirstChordValidationRule; nR<lmCVR_LastChordValidationRule; nR++)
    {
        pRule = tRules.GetRule(nR);
        if ( pRule == NULL)
        {
//            wxLogMessage(_T(" Rule %d is NULL !!!"), nR);
        }
        else if (pRule->IsEnabled())
        {
            wxLogMessage(_T("Evaluating rule %d, description: %s")
                , pRule->GetRuleId(), pRule->GetDescription().c_str());
            nNumErros += pRule->Evaluate(sStr, &nNumChordError[0]);
            wxLogMessage(sStr);
            wxLogMessage(_T("   RESULT of Rule %d: %d errors"), pRule->GetRuleId(), nNumErros   );
            if (nNumErros > 0)
            {

/*-- TODO: dejar una forma definitiva de mostrar los mensajes
  esto fue un intento de mostrarlos despues de aplicar cada regla,
  pero parece mejor al momento de detectar cada error en la regla (puede haer varios)
                //@@@ provisional TODO: mostrar correctamente los errores:
                //     en una ventana o apuntando al acorde
	            wxColour colour( 100, 150, 200 );
                // wxColour colour = *wxRED;
                for (int nE=0; nE<nNumChords; nE++)
                {
                    if ( nNumChordError[nE] > 0 )
                    {
                        DisplayChordInfo(GetMainFrame()->GetActiveDoc()->GetScore()
                          , &pChordDescriptor[nE], colour, sStr);
                    }
                }
----*/
            }
        }
    }

    return nNumErros;

}


bool lmHarmonyProcessor::UndoChanges(lmScore* pScore)
{
    //This method removes all error markup from the score

    //TODO:
    //The base class should provide an standard functionality for doing this.
    //For example, it should maintain a list of changes and it will undo all these
    //changes. This way, derived classes have nothing to implement, unless an
    //special behaviour is desired.

    //This code is just an example to undo what we did at ProcessScore()

    WXUNUSED(pScore);
    std::list<lmMarkup*>::iterator it = m_markup.begin();
    while (it != m_markup.end())
    {
        //undo this markup
        lmMarkup* pError = *it;
        lmStaffObj* pParent = pError->first;
        lmAuxObj* pAttachment = pError->second;
	    pParent->DetachAuxObj(pAttachment);
	    pParent->SetColour(*wxBLACK);
        delete pAttachment;

        //remove used element and move to next item in list
        m_markup.erase(it++);
        delete pError;
    }
    return true;
}


