object DESForm: TDESForm
  Left = 351
  Top = 124
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Double Exponential Smoothing'
  ClientHeight = 176
  ClientWidth = 278
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
  object SmoothFactor: TLabeledEdit
    Left = 128
    Top = 8
    Width = 33
    Height = 21
    EditLabel.Width = 113
    EditLabel.Height = 13
    EditLabel.Caption = 'Smoothing Factor (0-1): '
    LabelPosition = lpLeft
    TabOrder = 0
    Text = '0.65'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 40
    Width = 169
    Height = 105
    Caption = ' Apply filter to: '
    TabOrder = 1
    object Coordinates: TCheckBox
      Left = 16
      Top = 24
      Width = 97
      Height = 17
      Caption = 'Coordinates'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object Altitude: TCheckBox
      Left = 16
      Top = 48
      Width = 97
      Height = 17
      Caption = 'Altitude data'
      TabOrder = 1
    end
    object Speed: TCheckBox
      Left = 16
      Top = 72
      Width = 145
      Height = 17
      Caption = 'Speed and Cource data'
      TabOrder = 2
    end
  end
  object Go: TButton
    Left = 192
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 2
    OnClick = GoClick
  end
  object Button2: TButton
    Left = 192
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 3
  end
  object Progress: TProgressBar
    Left = 8
    Top = 152
    Width = 257
    Height = 16
    TabOrder = 4
  end
end
