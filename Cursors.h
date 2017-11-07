//---------------------------------------------------------------------------

#ifndef CursorsH
#define CursorsH
//---------------------------------------------------------------------------

class TCursorSet {

public:
  __fastcall TCursorSet();
  __fastcall ~TCursorSet();
  __property bool Wait  = { write=SetWait };
  __property bool Drag  = { write=SetDrag };
  __property bool Finger  = { write=SetFinger };
  __property TCursor WaitCursor  = { read=GetWaitCursor };
  __property TCursor DragCursor  = { read=GetDragCursor };
  __property TCursor FingerCursor  = { read=GetFingerCursor };
protected:
  TCursor m_cWaitSavedCursor;
  TCursor m_cDragSavedCursor;
  TCursor m_cFingerSavedCursor;
  bool m_bWait;
  bool m_bDrag;
  bool m_bFinger;
private:
  void __fastcall SetWait(bool value);
  void __fastcall SetDrag(bool value);
  void __fastcall SetFinger(bool value);
  TCursor __fastcall GetWaitCursor();
  TCursor __fastcall GetDragCursor();
  TCursor __fastcall GetFingerCursor();
};

#endif
