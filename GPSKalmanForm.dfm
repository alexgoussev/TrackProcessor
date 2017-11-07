object GPSKalmanFilter: TGPSKalmanFilter
  Left = 433
  Top = 243
  BorderStyle = bsDialog
  Caption = 'Kalman Filter'
  ClientHeight = 140
  ClientWidth = 343
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  DesignSize = (
    343
    140)
  PixelsPerInch = 96
  TextHeight = 13
  object ProgressBar: TProgressBar
    Left = 8
    Top = 114
    Width = 329
    Height = 16
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object m_Ok: TButton
    Left = 260
    Top = 12
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = m_OkClick
  end
  object m_Cancel: TButton
    Left = 260
    Top = 41
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 241
    Height = 97
    BevelOuter = bvLowered
    TabOrder = 3
    object Label3: TLabel
      Left = 8
      Top = 18
      Width = 125
      Height = 13
      Caption = 'Process noise covariance:'
    end
    object RValue: TEdit
      Left = 176
      Top = 14
      Width = 46
      Height = 21
      BiDiMode = bdLeftToRight
      ParentBiDiMode = False
      TabOrder = 0
      Text = '15'
      OnKeyPress = ValueKeyPress
    end
    object SaveSpeed: TCheckBox
      Left = 71
      Top = 72
      Width = 153
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Filter Speed and Course'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object SaveAlt: TCheckBox
      Left = 71
      Top = 48
      Width = 153
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Filter Altitude'
      TabOrder = 2
    end
  end
end
