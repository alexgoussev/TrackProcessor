object DriftForm: TDriftForm
  Left = 387
  Top = 202
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Drift Filter'
  ClientHeight = 152
  ClientWidth = 430
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
  object Action: TLabel
    Left = 8
    Top = 104
    Width = 409
    Height = 13
    AutoSize = False
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 321
    Height = 89
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 10
      Top = 50
      Width = 66
      Height = 13
      Caption = 'Filter strength:'
    end
    object Aperture: TLabeledEdit
      Left = 96
      Top = 16
      Width = 33
      Height = 21
      EditLabel.Width = 83
      EditLabel.Height = 13
      EditLabel.Caption = 'Aperture, meters: '
      LabelPosition = lpLeft
      TabOrder = 0
      Text = '50'
    end
    object Thereshould: TLabeledEdit
      Left = 272
      Top = 16
      Width = 33
      Height = 21
      EditLabel.Width = 118
      EditLabel.Height = 13
      EditLabel.Caption = 'Speed Threshold, km/h: '
      LabelPosition = lpLeft
      TabOrder = 1
      Text = '1.0'
    end
    object Strength: TComboBox
      Left = 88
      Top = 48
      Width = 89
      Height = 21
      Style = csDropDownList
      ItemIndex = 2
      TabOrder = 2
      Text = 'Average'
      Items.Strings = (
        'Extremely'
        'High'
        'Average'
        'Low')
    end
  end
  object Go: TButton
    Left = 344
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 344
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 2
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 124
    Width = 417
    Height = 16
    TabOrder = 3
  end
end
