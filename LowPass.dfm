object LowPassForm: TLowPassForm
  Left = 353
  Top = 182
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Low Pass Filter'
  ClientHeight = 209
  ClientWidth = 292
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 193
    Height = 169
    BevelOuter = bvLowered
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 16
      Top = 48
      Width = 161
      Height = 105
      Caption = ' Apply Filter to: '
      TabOrder = 0
      object Altitude: TCheckBox
        Left = 16
        Top = 48
        Width = 97
        Height = 17
        Caption = 'Altutude data'
        TabOrder = 0
      end
      object Speed: TCheckBox
        Left = 16
        Top = 72
        Width = 137
        Height = 17
        Caption = 'Speed and Course data'
        TabOrder = 1
      end
      object Coordinates: TCheckBox
        Left = 16
        Top = 24
        Width = 97
        Height = 17
        Caption = 'Coordinates'
        Checked = True
        State = cbChecked
        TabOrder = 2
      end
    end
    object Smoothing: TLabeledEdit
      Left = 136
      Top = 16
      Width = 33
      Height = 21
      EditLabel.Width = 119
      EditLabel.Height = 13
      EditLabel.Caption = 'Smoothing Factor (0 - 1): '
      LabelPosition = lpLeft
      TabOrder = 1
      Text = '0.25'
    end
  end
  object Go: TButton
    Left = 208
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 208
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 2
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 184
    Width = 273
    Height = 16
    TabOrder = 3
  end
end
