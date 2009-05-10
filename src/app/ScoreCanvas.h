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

#ifndef _LM_SCORECANVAS_H        //to avoid nested includes
#define _LM_SCORECANVAS_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ScoreCanvas.cpp"
#endif

#include "wx/docview.h"
#include "global.h"
class lmScoreHighlightEvent;
#include "../sound/SoundEvents.h"
#include "toolbox/ToolsBox.h"

class lmScoreView;
class lmDocument;
class lmGMSelection;
class lmToolBoxEvent;
class lmToolPage;
class lmScoreProcessor;

#define lmUNSELECT      false       //remove selection
#define lmSELECT        true        //select objects


//Abstract class. All controllers must derive from it
class lmController : public wxWindow
{
   DECLARE_ABSTRACT_CLASS(lmController)

public:
    lmController(wxWindow *pParent, lmScoreView *pView, lmDocument* pDoc,
				 wxColor colorBg, wxWindowID id = wxID_ANY,
				 const wxPoint& pos = wxDefaultPosition,
				 const wxSize& size = wxDefaultSize, long style = 0);
	virtual ~lmController();

	//commands without Do/Undo support
	virtual void PlayScore(bool fFromCursor=false) {}
    virtual void StopPlaying(bool fWait=false) {}
    virtual void PausePlaying() {}


	// commands with Do/Undo support

        //add/insert commands
	virtual void AddTitle() {}
    virtual void AttachNewText(lmComponentObj* pCO) {}
	virtual void InsertBarline(lmEBarline nType = lm_eBarlineSimple) {}
    virtual void InsertClef(lmEClefType nClefType) {}
	virtual void InsertNote(lmEPitchType nPitchType, int nStep, int nOctave, 
					lmENoteType nNoteType, float rDuration, int nDots,
					lmENoteHeads nNotehead,
                    lmEAccidentals nAcc = lm_eNoAccidentals,
                    int nVoice = 0,
					lmNote* pBaseOfChord = (lmNote*)NULL,
					bool fTiedPrev = false) {}

    virtual void InsertRest(lmENoteType nNoteType, float rDuration, int nDots, int nVoice) {}
    virtual void InsertTimeSignature(int nBeats, int nBeatType, bool fVisible = true) {}    //for type eTS_Normal
    virtual void InsertKeySignature(int nFifths, bool fMajor, bool fVisible = true) {}

        //delete commands
	virtual void DeleteCaretSatffobj() {}
    virtual void DeleteSelection() {}
    virtual void DeleteTie(lmNote* pEndNote) {}
    virtual void DeleteStaffObj(lmStaffObj* pSO) {}

        //change/move commands
    virtual void AddTie(lmNote* pStartNote, lmNote* pEndNote) {}
    virtual void ChangeMidiSettings(lmInstrument* pInstr, int nMidiChannel,
                                    int nMidiInstr) {}
	virtual void ChangeNotePitch(int nSteps) {}
	virtual void ChangeNoteAccidentals(int nAcc) {}
	virtual void ChangeNoteDots(int nDots) {}
    virtual void ChangeTie(lmNote* pStartNote, lmNote* pEndNote) {}
    virtual void AddTuplet() {}
    virtual void DeleteTuplet(lmNoteRest* pStartNote) {}
	virtual void MoveNote(lmGMObject* pGMO, const lmUPoint& uPos, int nSteps) {}
	virtual void MoveObject(lmGMObject* pGMO, const lmUPoint& uPos,
                            bool fUpdateViews = true) {}
    virtual void MoveObjectPoints(lmGMObject* pGMO, lmUPoint uShifts[], int nNumPoints,
                                  bool fUpdateViews = true) {}
    virtual void BreakBeam() {}
    virtual void JoinBeam() {}
    virtual void ChangeText(lmScoreText* pST, wxString sText, lmEHAlign nAlign,
                            lmLocation tPos, lmTextStyle* pStyle, int nHintOptions=0) {}
    virtual void ChangePageMargin(lmGMObject* pGMO, int nIdx, int nPage, lmLUnits uPos) {}
	virtual void ChangeBarline(lmBarline* pBL, lmEBarline nBarlineType, bool fVisible) {}



    // event handlers
    virtual void OnKeyPress(wxKeyEvent& event) { event.Skip(); }
    virtual void OnKeyDown(wxKeyEvent& event) { event.Skip(); }
	virtual void OnEraseBackground(wxEraseEvent& event);

	//contextual menus
	void ShowContextualMenu(lmScoreObj* pOwner, lmGMObject* pGMO, wxMenu* pMenu, int x, int y);
	virtual wxMenu* GetContextualMenu(bool fInitialize = true);

	//event handlers for contextual menus
	virtual void OnCut(wxCommandEvent& event) {}
    virtual void OnCopy(wxCommandEvent& event) {}
    virtual void OnPaste(wxCommandEvent& event) {}
    virtual void OnColor(wxCommandEvent& event) {}
    virtual void OnProperties(wxCommandEvent& event) {}
    virtual void OnDeleteTiePrev(wxCommandEvent& event) {}
    virtual void OnAttachText(wxCommandEvent& event) {}
	virtual void OnScoreTitles(wxCommandEvent& event) {}
	virtual void OnViewPageMargins(wxCommandEvent& event) {}
#ifdef __WXDEBUG__
	virtual void OnDumpShape(wxCommandEvent& event) {}
#endif

    //call backs
    virtual void SynchronizeToolBox() {}
    virtual void RestoreToolBoxSelections() {}



protected:
	wxMenu*			m_pMenu;			//contextual menu
	lmScoreObj*		m_pMenuOwner;		//contextual menu owner
	lmGMObject*		m_pMenuGMO;			//graphic object who displayed the contextual menu
	lmToolBox*      m_pToolBox;         //the associated toolbox, if any
    int             m_nOctave;          //current octave for note insertion
	int				m_nVoice;			//current voice for note insertion

private:

	DECLARE_EVENT_TABLE()
};



class lmScoreCanvas : public lmController
{
	DECLARE_DYNAMIC_CLASS(lmScoreCanvas)

public:

    // constructors and destructor
    lmScoreCanvas(lmScoreView *pView, wxWindow *pParent, lmDocument* pDoc, 
                  const wxPoint& pos, const wxSize& size, long style, wxColor colorBg);
    ~lmScoreCanvas();

    // event handlers
    void OnPaint(wxPaintEvent &WXUNUSED(event));
    void OnMouseEvent(wxMouseEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnVisualHighlight(lmScoreHighlightEvent& event);
	void OnKeyPress(wxKeyEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnToolBoxEvent(lmToolBoxEvent& event);

	//commands without Do/Undo support
    void PlayScore(bool fFromCursor=false);
    void StopPlaying(bool fWait=false);
    void PausePlaying();

	// commands with Do/Undo support

        //add/insert commands
	void AddTitle();
    void AttachNewText(lmComponentObj* pCO);
	void InsertBarline(lmEBarline nType = lm_eBarlineSimple);
    void InsertClef(lmEClefType nClefType);
	void InsertNote(lmEPitchType nPitchType, int nStep, int nOctave, 
					lmENoteType nNoteType, float rDuration, int nDots,
					lmENoteHeads nNotehead,
                    lmEAccidentals nAcc = lm_eNoAccidentals,
                    int nVoice = 0,
					lmNote* pBaseOfChord = (lmNote*)NULL,
					bool fTiedPrev = false);

    void InsertRest(lmENoteType nNoteType, float rDuration, int nDots, int nVoice);
    void InsertTimeSignature(int nBeats, int nBeatType, bool fVisible = true);    //for type eTS_Normal
    void InsertKeySignature(int nFifths, bool fMajor, bool fVisible = true);

        //delete commands
	void DeleteCaretSatffobj();
    void DeleteSelection();
    void DeleteTie(lmNote* pEndNote);
    void DeleteStaffObj(lmStaffObj* pSO);

        //change/move commands
    void AddTie(lmNote* pStartNote, lmNote* pEndNote);
    void ChangeMidiSettings(lmInstrument* pInstr, int nMidiChannel, int nMidiInstr);
	void ChangeNotePitch(int nSteps);
	void ChangeNoteAccidentals(int nAcc);
	void ChangeNoteDots(int nDots);
    void ChangeTie(lmNote* pStartNote, lmNote* pEndNote);
    void AddTuplet();
    void DeleteTuplet(lmNoteRest* pStartNR);
	void MoveNote(lmGMObject* pGMO, const lmUPoint& uPos, int nSteps);
	void MoveObject(lmGMObject* pGMO, const lmUPoint& uPos, bool fUpdateViews = true);
    void MoveObjectPoints(lmGMObject* pGMO, lmUPoint uShifts[], int nNumPoints,
                          bool fUpdateViews = true);
    void BreakBeam();
    void JoinBeam();
    void ChangeText(lmScoreText* pST, wxString sText, lmEHAlign nAlign,
                    lmLocation tPos, lmTextStyle* pStyle, int nHintOptions=0);
    void ChangePageMargin(lmGMObject* pGMO, int nIdx, int nPage, lmLUnits uPos);
	void ChangeBarline(lmBarline* pBL, lmEBarline nBarlineType, bool fVisible);



    //call backs
    void SynchronizeToolBox();
    void RestoreToolBoxSelections();

	//contextual menus
	wxMenu* GetContextualMenu(bool fInitialize = true);

	//event handlers for contextual menus
	void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnColor(wxCommandEvent& event);
    void OnProperties(wxCommandEvent& event);
    void OnDeleteTiePrev(wxCommandEvent& event);
    void OnAttachText(wxCommandEvent& event);
    void OnScoreTitles(wxCommandEvent& event);
    void OnViewPageMargins(wxCommandEvent& event);
#ifdef __WXDEBUG__
	void OnDumpShape(wxCommandEvent& event);
#endif

private:
    void DeleteCaretOrSelected();
    void SynchronizeToolBoxWithSelection(bool fEnable = true);
    void SynchronizeToolBoxWithCaret(bool fEnable = true);

    bool TileBitmap(const wxRect& rect, wxDC& dc, wxBitmap& bitmap);
    void LogKeyEvent(wxString name, wxKeyEvent& event, int nTool=-1);
    wxString KeyCodeToName(int nKeyCode);
    void ProcessKey(wxKeyEvent& event);

    //tools' selection
    void SelectNoteDuration(int iButton);
    void SelectNoteAccidentals(bool fNext);
    void SelectNoteDots(bool fNext);
    void SelectOctave(bool fUp);
    void SelectOctave(int nOctave);
    void SelectVoice(bool fUp);
    void SelectVoice(int nVoice);

    //managing selections and cursor pointed objects
    bool IsSelectionValidForTies(lmNote** ppStartNote = NULL, lmNote** ppEndNote = NULL);
    lmNoteRest* IsSelectionValidForTuplet();
    bool IsCursorValidToCutBeam();
    bool IsSelectionValidToJoinBeam();




    lmScoreView*        m_pView;        //owner view
    wxWindow*           m_pOwner;       //parent window
    lmDocument*    m_pDoc;         //the document rendered by the view

    wxColour        m_colorBg;			//colour for background

	//to control octave when inserting several consecutive notes
	bool			m_fInsertionSequence;
	int				m_nLastOctave;

    //to sore information of key down event in order to deal properly with key codes
    int             m_nKeyDownCode;     //key code when KEY_DOWN event
    bool            m_fCmd;             //Ctrol pressed
    bool            m_fAlt;             //Alt pressed
    bool            m_fShift;           //Shift pressed

    //buffer for commands requiring several keystrokes
    wxString        m_sCmd;


    //to save options selected by user in ToolBox
    bool            m_fToolBoxSavedOptions;
    int             m_nTbAcc;
    int             m_nTbDots;
    int             m_nTbDuration;


    DECLARE_EVENT_TABLE()
};


//---------------------------------------------------------------------------
// Helper class to define edition modes
//---------------------------------------------------------------------------

class lmEditorMode
{
public:
    lmEditorMode(wxClassInfo* pControllerInfo, wxClassInfo* pScoreProcInfo);
    ~lmEditorMode();

    void CustomizeToolBoxPages(lmToolBox* pToolBox);
    lmScoreProcessor* CreateScoreProcessor();

    //ToolBox reated
    void ChangeToolPage(int nPageID, wxClassInfo* pToolPageInfo);

protected:

    wxClassInfo*            m_pControllerInfo;
    wxClassInfo*            m_pScoreProcInfo;
    wxClassInfo*            m_ToolPagesInfo[lmPAGE_MAX];
    //lmToolBoxConfiguration  m_config;

};




#endif  // _LM_SCORECANVAS_H
