object DPForm: TDPForm
  Left = 413
  Top = 191
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Douglas-Peucker Simplification'
  ClientHeight = 96
  ClientWidth = 288
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
    Width = 185
    Height = 57
    BevelOuter = bvLowered
    TabOrder = 0
    object Epsilon: TLabeledEdit
      Left = 88
      Top = 16
      Width = 41
      Height = 21
      EditLabel.Width = 77
      EditLabel.Height = 13
      EditLabel.Caption = 'Epsilon, meters: '
      LabelPosition = lpLeft
      TabOrder = 0
      Text = '1'
    end
  end
  object Go: TButton
    Left = 200
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 200
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
    Top = 72
    Width = 265
    Height = 16
    TabOrder = 3
  end
end
