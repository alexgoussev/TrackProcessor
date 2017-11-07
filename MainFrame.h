//---------------------------------------------------------------------------

#ifndef MainFrameH
#define MainFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

#include "GPXFile.h"
#include "OSMTrackView.h"
//---------------------------------------------------------------------------

#define TPM_LOADFILE      (WM_APP + 850)

struct VisibleTrack {
  PGPXTrack pTrack;
  int ColorNo;
};

struct tagTrackColor {
  TColor Color;
  int nCount;
};

typedef VisibleTrack* PVisibleTrack;
typedef tagTrackColor* PTrackColor;

class TreeItem
{
  public:
    AnsiString asText;
    PVisibleTrack pTrack;
    int nState;
    TList *pChilds;
    __fastcall TreeItem(TTreeNode *pNode);
    __fastcall ~TreeItem();
    void __fastcall ConnectChildsTo(TTreeView *pTree, TTreeNode *pNode);
};

class TMainForm : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *MainMenu1;
  TMenuItem *File;
  TMenuItem *Open1;
  TMenuItem *N1;
  TMenuItem *Exit1;
  TActionList *MainActionList;
  TAction *OpenAction;
  TAction *ExitAction;
  TMenuItem *Save1;
  TAction *SaveCheckedAction;
  TOpenDialog *OpenDialog;
  TSaveDialog *SaveDialog;
  TMenuItem *Action1;
  TAction *KalmanAction;
  TStatusBar *StatusBar;
  TAction *MedianAction;
  TAction *DPAction;
  TMenuItem *DouglasPeuckerSimplification1;
  TAction *HDOPAction;
  TAction *VelocityAction;
  TAction *SRTMAction;
  TMenuItem *Filters1;
  TMenuItem *KalmanFilter2;
  TMenuItem *MedianFilter2;
  TMenuItem *VelocityFilter2;
  TAction *DriftAction;
  TMenuItem *DriftFilter1;
  TAction *LowPassAction;
  TMenuItem *LowPassFilter1;
  TMenuItem *N2;
  TMenuItem *HDOPFilter1;
  TMenuItem *SetSRTMAltitude1;
  TAction *TimeShiftAction;
  TAction *StatisticsAction;
  TAction *ProfilesAction;
  TAction *PointsAction;
  TMenuItem *ShiftTime1;
  TMenuItem *N5;
  TAction *SplitAction;
  TAction *MergeAction;
  TMenuItem *N7;
  TMenuItem *MergeTracks1;
  TMenuItem *SplitTrack1;
  TAction *WAvgAction;
  TMenuItem *WeightedAverage1;
  TMenuItem *View1;
  TMenuItem *Points1;
  TMenuItem *Profiles2;
  TMenuItem *Statistics2;
  TAction *StaticAction;
  TMenuItem *StaticNavigation1;
  TPanel *Panel1;
  TSplitter *VSplitter;
  TPanel *Panel2;
  TTreeView *TracksTree;
  TImageList *TreeViewCheckBox;
  TAction *DESAction;
  TMenuItem *DES1;
  TMenuItem *Google1;
  TMenuItem *MaptypeItem;
  TMenuItem *Saveselected1;
  TAction *SaveSelectedAction;
  TMenuItem *N8;
  TMenuItem *LandscapeCycleMapItem;
  TMenuItem *OpenCycleMapItem;
  TMenuItem *OpenStreetMapMapItem;
  TMenuItem *OutdoorsCycleMapItem;
  TPopupMenu *TrackListPopupMenu;
  TMenuItem *Saveselected2;
  TMenuItem *N9;
  TAction *DeleteSelectedAction;
  TAction *DeleteCheckedAction;
  TMenuItem *DeleteChecked1;
  TMenuItem *DeleteSelected1;
  TMenuItem *N11;
  TAction *CollapseNodeAction;
  TAction *ExpandNodeAction;
  TAction *CollapseAllAction;
  TAction *ExpandAllAction;
  TAction *CheckAllAction;
  TAction *UncheckAllAction;
  TMenuItem *CheckAll1;
  TMenuItem *UncheckAll1;
  TMenuItem *N12;
  TMenuItem *CollapseAll1;
  TMenuItem *CollapseNode1;
  TMenuItem *ExpandAll1;
  TMenuItem *ExpandNode1;
  TMenuItem *N13;
  TMenuItem *Action2;
  TMenuItem *SplitTrack3;
  TMenuItem *MergeTracks3;
  TMenuItem *Savechecked1;
  TMenuItem *DouglasPeuckerSimplification2;
  TMenuItem *SetSRTMAltitude2;
  TMenuItem *ShiftTime2;
  TMenuItem *Filter1;
  TMenuItem *HDOP1;
  TMenuItem *Speed1;
  TMenuItem *Median1;
  TMenuItem *LowPass1;
  TMenuItem *DES2;
  TMenuItem *AveragingSmooth1;
  TMenuItem *Kalman1;
  TMenuItem *Drift1;
  TMenuItem *PositionPinning1;
  TAction *MetadataAction;
  TMenuItem *N14;
  TMenuItem *EditMetadata1;
  TMenuItem *EditMetadata2;
  TPaintBox *TrackViewBox;
  TPopupMenu *MapPopupMenu;
  TMenuItem *MapTypePopupItem;
  TMenuItem *LandscapeCycleMap1;
  TMenuItem *OpenCycleMap1;
  TMenuItem *OpenStreetMap1;
  TMenuItem *OutdoorsCycleMap1;
  TAction *ShowAllAction;
  TMenuItem *FitAllTracks1;
  TMenuItem *ZoomtoFitAllTracks1;
  TAction *SmoothSplineAction;
  TMenuItem *SplineSmooth1;
  TMenuItem *SplineSmooth2;
  TImageList *TrackColors;
  TAction *APLAAction;
  TMenuItem *APLA1;
  TMenuItem *APLA2;
  TMenuItem *N3;
  TMenuItem *N4;
  TMenuItem *N6;
  TMenuItem *N10;
  TMenuItem *N15;
  TMenuItem *N16;
  TAction *ConfidenceAction;
  TMenuItem *ConfidenceInterval1;
  TMenuItem *ConfidenceInterval2;
  TAction *CellEqAction;
  TMenuItem *CellEqualizer1;
  TMenuItem *CellEqualizer2;
  TMenuItem *Settings1;
  TMenuItem *OpenTopoMapItem;
  TMenuItem *OpenTopoMapItem1;
  TMenuItem *NoMapItem;
  TMenuItem *NoMapPopupItem;
  TAction *NoMapAction;
  void __fastcall OpenActionExecute(TObject *Sender);
  void __fastcall ExitActionExecute(TObject *Sender);
  void __fastcall SaveCheckedActionExecute(TObject *Sender);
  void __fastcall KalmanActionExecute(TObject *Sender);
  void __fastcall MedianActionExecute(TObject *Sender);
  void __fastcall DPActionExecute(TObject *Sender);
  void __fastcall HDOPActionExecute(TObject *Sender);
  void __fastcall HDOPActionUpdate(TObject *Sender);
  void __fastcall VelocityActionExecute(TObject *Sender);
  void __fastcall SRTMActionExecute(TObject *Sender);
  void __fastcall DriftActionExecute(TObject *Sender);
  void __fastcall LowPassActionExecute(TObject *Sender);
  void __fastcall KalmanActionUpdate(TObject *Sender);
  void __fastcall MedianActionUpdate(TObject *Sender);
  void __fastcall DPActionUpdate(TObject *Sender);
  void __fastcall VelocityActionUpdate(TObject *Sender);
  void __fastcall DriftActionUpdate(TObject *Sender);
  void __fastcall SRTMActionUpdate(TObject *Sender);
  void __fastcall LowPassActionUpdate(TObject *Sender);
  void __fastcall TimeShiftActionExecute(TObject *Sender);
  void __fastcall TimeShiftActionUpdate(TObject *Sender);
  void __fastcall StatisticsActionExecute(TObject *Sender);
  void __fastcall StatisticsActionUpdate(TObject *Sender);
  void __fastcall ProfilesActionExecute(TObject *Sender);
  void __fastcall ProfilesActionUpdate(TObject *Sender);
  void __fastcall PointsActionExecute(TObject *Sender);
  void __fastcall PointsActionUpdate(TObject *Sender);
  void __fastcall SplitActionExecute(TObject *Sender);
  void __fastcall SplitActionUpdate(TObject *Sender);
  void __fastcall MergeActionExecute(TObject *Sender);
  void __fastcall MergeActionUpdate(TObject *Sender);
  void __fastcall WAvgActionExecute(TObject *Sender);
  void __fastcall WAvgActionUpdate(TObject *Sender);
  void __fastcall StaticActionExecute(TObject *Sender);
  void __fastcall StaticActionUpdate(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall TracksTreeClick(TObject *Sender);
  void __fastcall TracksTreeChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange);
  void __fastcall TracksTreeChange(TObject *Sender, TTreeNode *Node);
  void __fastcall SaveCheckedActionUpdate(TObject *Sender);
  void __fastcall DESActionExecute(TObject *Sender);
  void __fastcall DESActionUpdate(TObject *Sender);
  void __fastcall SaveSelectedActionExecute(TObject *Sender);
  void __fastcall SaveSelectedActionUpdate(TObject *Sender);
  void __fastcall MapTypeItemClick(TObject *Sender);
  void __fastcall DeleteSelectedActionExecute(TObject *Sender);
  void __fastcall DeleteCheckedActionExecute(TObject *Sender);
  void __fastcall CollapseNodeActionExecute(TObject *Sender);
  void __fastcall ExpandNodeActionExecute(TObject *Sender);
  void __fastcall CollapseAllActionExecute(TObject *Sender);
  void __fastcall CollapseAllActionUpdate(TObject *Sender);
  void __fastcall ExpandAllActionExecute(TObject *Sender);
  void __fastcall CheckAllActionExecute(TObject *Sender);
  void __fastcall CheckAllActionUpdate(TObject *Sender);
  void __fastcall UncheckAllActionExecute(TObject *Sender);
  void __fastcall UncheckAllActionUpdate(TObject *Sender);
  void __fastcall MetadataActionExecute(TObject *Sender);
  void __fastcall MetadataActionUpdate(TObject *Sender);
  void __fastcall TrackViewBoxPaint(TObject *Sender);
  void __fastcall TrackViewBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall TrackViewBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall TrackViewBoxMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall TrackViewBoxDblClick(TObject *Sender);
  void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
  void __fastcall ShowAllActionUpdate(TObject *Sender);
  void __fastcall ShowAllActionExecute(TObject *Sender);
  void __fastcall SmoothSplineActionExecute(TObject *Sender);
  void __fastcall SmoothSplineActionUpdate(TObject *Sender);
  void __fastcall APLAActionExecute(TObject *Sender);
  void __fastcall APLAActionUpdate(TObject *Sender);
  void __fastcall TracksTreeEdited(TObject *Sender, TTreeNode *Node, String &S);
  void __fastcall ConfidenceActionExecute(TObject *Sender);
  void __fastcall ConfidenceActionUpdate(TObject *Sender);
  void __fastcall CellEqActionExecute(TObject *Sender);
  void __fastcall CellEqActionUpdate(TObject *Sender);
  void __fastcall Settings1Click(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall NoMapActionExecute(TObject *Sender);
private:
  //TColor __fastcall GetCurrentColor();	// User declarations
  void __fastcall IdleHandler(TObject *Sender, bool &Done);
public:		// User declarations
  __fastcall TMainForm(TComponent* Owner);
  void OnNewTrack(TMessage & msg);
  void OnNewPoint(TMessage & msg);
  __fastcall ~TMainForm();
  void OnSaveTrack(TMessage & msg);
  void OnSavePoint(TMessage & msg);
  void __fastcall OnManualTrackChange(TMessage & msg);
  //__property TColor CurrentColor  = { read=GetCurrentColor };
protected:
  PTrackColor m_pTrackColors;
  bool m_bFirstShow;
  bool m_bInternalSelect;
  int m_nSelectedItems;
  TTreeNode * m_pLastSelected;
  int m_nCheckedItems;
  TOSMTrackView * m_pTrackView;
  bool m_bDblClicked;
  bool m_bMousePressed;
  //int m_nMaxColor;
  int m_nCurrentColor;
  eMapType m_eMapType;
  bool m_bNoDrag;
  int m_nSelectedPoint;
  TCursorSet * m_pCursors;
  AnsiString m_asCmdLineFile;
  void __fastcall AddProcessedTrack(TTreeNode * pNode, PGPXTrack pNewTrack, AnsiString asAddition);
  void __fastcall SetNodeState(TTreeNode * pNode, int nState);
  void __fastcall DeleteTreeNode(TTreeNode * pNode);
  void __fastcall LoadGPXFile(AnsiString asFileName);
  void __fastcall LoadCSVFile(AnsiString asFileName);
  void __fastcall LoadNMEAFile(AnsiString asFileName);
  bool __fastcall LoadLastCoordinates(double & dMinLat, double & dMinLon, double & dMaxLat, double & dMaxLon, int & nZoom);
  void __fastcall SaveLastCoordinates();
  void __fastcall LoadPVTFile(AnsiString asFileName);
  void __fastcall OnTilesLeft(TMessage & msg);
  void __fastcall OnTileURL(TMessage & msg);
  void __fastcall OnTileCache(TMessage & msg);
  void OnShowPoint(TMessage & msg);
  void __fastcall ShowPointHint(int n);
  void __fastcall LoadFile(AnsiString asFileName, int nFileType, bool bFirstItem);
  int __fastcall SelectTrackColor();
  void __fastcall OnLoadFile(TMessage & msg);
  void __fastcall OnTileMemory(TMessage & msg);
  void __fastcall OnCanvasUpdate(TMessage & msg);
  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(GPXFILE_MSG_NEWTRACK, TMessage, OnNewTrack)
    VCL_MESSAGE_HANDLER(GPXFILE_MSG_NEWPOINT, TMessage, OnNewPoint)
    VCL_MESSAGE_HANDLER(GPXFILE_MSG_SAVETRACK, TMessage, OnSaveTrack)
    VCL_MESSAGE_HANDLER(GPXFILE_MSG_SAVEPOINT, TMessage, OnSavePoint)
    VCL_MESSAGE_HANDLER(MANUAL_TRACK_CHANGE, TMessage, OnManualTrackChange)
    VCL_MESSAGE_HANDLER(OSMMAP_MSG_TILESLEFT, TMessage, OnTilesLeft)
    VCL_MESSAGE_HANDLER(OSMMAP_MSG_TILEURL, TMessage, OnTileURL)
    VCL_MESSAGE_HANDLER(OSMMAP_MSG_TILECACHE, TMessage, OnTileCache)
    VCL_MESSAGE_HANDLER(SHOW_POINT, TMessage, OnShowPoint)
    VCL_MESSAGE_HANDLER(TPM_LOADFILE, TMessage, OnLoadFile)
    VCL_MESSAGE_HANDLER(OSMMAP_MSG_TILEMEMORY, TMessage, OnTileMemory)
    VCL_MESSAGE_HANDLER(OSMMAP_MSG_CANVASUPDATED, TMessage, OnCanvasUpdate)
  END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
