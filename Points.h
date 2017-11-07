//---------------------------------------------------------------------------

#ifndef PointsH
#define PointsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>

#include "GPXData.h"
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

#define MANUAL_TRACK_CHANGE       (WM_APP + 801)
#define SHOW_POINT                (WM_APP + 802)
//---------------------------------------------------------------------------
enum eMouseWnd {mwMap, mwPoints};

class TPointsForm : public TForm
{
__published:	// IDE-managed Components
  TPopupMenu *PopupMenu1;
  TMenuItem *Settings;
  TMenuItem *DeletePoints;
  TMenuItem *N1;
  TMenuItem *SaveText;
  TSaveDialog *SaveDialog;
  TListView *PointsList;
  TActionList *PointsActionList;
  TAction *DelCheckedAction;
  TAction *UncheckAllAction;
  TMenuItem *UncheckAll1;
  TAction *CheckToBegin;
  TAction *CheckToEnd;
  TAction *UncheckToBegin;
  TAction *UncheckToEnd;
  TMenuItem *N2;
  TMenuItem *ChecktoStart1;
  TMenuItem *ChecktoFinish1;
  TMenuItem *UnchecktoStart1;
  TMenuItem *UnchecktoFinish1;
  TImageList *ImageList;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall SettingsClick(TObject *Sender);
  void __fastcall SaveTextClick(TObject *Sender);
  void __fastcall PointsListSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
  void __fastcall DelCheckedActionExecute(TObject *Sender);
  void __fastcall DelCheckedActionUpdate(TObject *Sender);
  void __fastcall UncheckAllActionExecute(TObject *Sender);
  void __fastcall UncheckAllActionUpdate(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall PointsListMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall CheckToBeginExecute(TObject *Sender);
  void __fastcall CheckToBeginUpdate(TObject *Sender);
  void __fastcall CheckToEndExecute(TObject *Sender);
  void __fastcall CheckToEndUpdate(TObject *Sender);
  void __fastcall UncheckToBeginExecute(TObject *Sender);
  void __fastcall UncheckToBeginUpdate(TObject *Sender);
  void __fastcall UncheckToEndExecute(TObject *Sender);
  void __fastcall UncheckToEndUpdate(TObject *Sender);
  void __fastcall PointsListData(TObject *Sender, TListItem *Item);
  void __fastcall PointsListClick(TObject *Sender);
private:
  eMouseWnd m_eMouseWnd;
  int m_nSelectedPoint;
  void __fastcall SetCurrentTrack(PGPXTrack value);
  PGPXTrack __fastcall GetCurrentTrack();
  void __fastcall SetMouseWnd(eMouseWnd value);
  eMouseWnd __fastcall GetMouseWnd();
  void __fastcall SetSelectedPoint(int value);
  int __fastcall GetSelectedPoint();	// User declarations
public:		// User declarations
  __fastcall TPointsForm(TComponent* Owner);
  __fastcall ~TPointsForm();
  __property PGPXTrack CurrentTrack  = { read=GetCurrentTrack, write=SetCurrentTrack };
  __property eMouseWnd MouseWnd  = { read=GetMouseWnd, write=SetMouseWnd };
  __property int SelectedPoint  = { read=GetSelectedPoint, write=SetSelectedPoint };
protected:
  PGPXTrack m_pCurrentTrack;
  bool m_bFirstShow;
  bool m_bLocalTime;
  bool m_bDMSCoord;
  int m_nChecked;
  int m_nX;
  int m_nY;
  bool * m_pChecked;
};
//---------------------------------------------------------------------------
extern PACKAGE TPointsForm *PointsForm;
//---------------------------------------------------------------------------
#endif


