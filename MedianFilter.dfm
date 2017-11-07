object MedianForm: TMedianForm
  Left = 404
  Top = 186
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Median Filter'
  ClientHeight = 232
  ClientWidth = 382
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
    Width = 273
    Height = 193
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 16
      Width = 68
      Height = 13
      Caption = 'Filter Aperture:'
    end
    object Aperture: TCSpinEdit
      Left = 96
      Top = 14
      Width = 49
      Height = 22
      Hint = 'Higher values provide more smoothing'
      Increment = 2
      MaxValue = 9999999
      MinValue = 3
      TabOrder = 0
      Value = 3
    end
    object UseRank: TCheckBox
      Left = 16
      Top = 48
      Width = 193
      Height = 17
      Caption = 'Use Rank Statistics with coeffisient:'
      TabOrder = 1
      OnClick = UseRankClick
    end
    object RankCoeff: TEdit
      Left = 208
      Top = 46
      Width = 49
      Height = 21
      Enabled = False
      TabOrder = 2
      Text = '0.65'
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 80
      Width = 241
      Height = 97
      Caption = ' Apply filter to '
      TabOrder = 3
      object Altitude: TCheckBox
        Left = 16
        Top = 48
        Width = 97
        Height = 17
        Caption = 'Altitude'
        TabOrder = 0
      end
      object Speed: TCheckBox
        Left = 16
        Top = 72
        Width = 113
        Height = 17
        Caption = 'Speed and Course'
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
  end
  object Go: TButton
    Left = 296
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 296
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
    Top = 208
    Width = 361
    Height = 16
    TabOrder = 3
  end
end
