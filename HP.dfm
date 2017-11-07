object HPForm: THPForm
  Left = 465
  Top = 227
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Hodrick-Prescott Filter'
  ClientHeight = 197
  ClientWidth = 314
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
    Width = 217
    Height = 161
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 69
      Height = 13
      Caption = 'Smooth factor:'
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 48
      Width = 201
      Height = 97
      Caption = ' Apply Filter to: '
      TabOrder = 0
      object m_pCoordinates: TCheckBox
        Left = 8
        Top = 24
        Width = 97
        Height = 17
        Caption = 'Coordinates'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object m_pAltitude: TCheckBox
        Left = 8
        Top = 48
        Width = 97
        Height = 17
        Caption = 'Altitude'
        TabOrder = 1
      end
      object m_pSpeed: TCheckBox
        Left = 8
        Top = 72
        Width = 121
        Height = 17
        Caption = 'Speed'
        TabOrder = 2
      end
    end
    object m_pLambda: TEdit
      Left = 144
      Top = 14
      Width = 65
      Height = 21
      TabOrder = 1
      Text = '500'
      OnKeyPress = OnLambdaKeyPress
    end
  end
  object Button1: TButton
    Left = 232
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 232
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 176
    Width = 297
    Height = 16
    TabOrder = 3
  end
end
