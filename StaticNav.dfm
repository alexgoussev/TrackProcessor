object SNEForm: TSNEForm
  Left = 330
  Top = 168
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Position Pinning'
  ClientHeight = 194
  ClientWidth = 389
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 289
    Height = 145
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 16
      Width = 156
      Height = 13
      Caption = 'Pinning Speed Threshold (km/h):'
    end
    object Label2: TLabel
      Left = 16
      Top = 48
      Width = 140
      Height = 13
      Caption = 'Pinning Time Threshold (sec):'
    end
    object Label3: TLabel
      Left = 16
      Top = 84
      Width = 169
      Height = 13
      Caption = 'Unpinning Speed Threshold (km/h):'
    end
    object Label4: TLabel
      Left = 16
      Top = 114
      Width = 153
      Height = 13
      Caption = 'Unpinning Time Threshold (sec):'
    end
    object PinSpeedTh: TEdit
      Left = 232
      Top = 12
      Width = 33
      Height = 21
      TabOrder = 0
      Text = '2.5'
    end
    object PinTimeTh: TEdit
      Left = 232
      Top = 46
      Width = 33
      Height = 21
      TabOrder = 1
      Text = '3'
    end
    object UnpinSpeedTh: TEdit
      Left = 232
      Top = 80
      Width = 33
      Height = 21
      TabOrder = 2
      Text = '3'
    end
    object UnpinTimeTh: TEdit
      Left = 232
      Top = 112
      Width = 33
      Height = 21
      TabOrder = 3
      Text = '3'
    end
  end
  object Go: TButton
    Left = 304
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 304
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 2
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 168
    Width = 369
    Height = 16
    TabOrder = 3
  end
end
