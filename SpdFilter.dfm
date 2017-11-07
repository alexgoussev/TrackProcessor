object SpeedFilter: TSpeedFilter
  Left = 308
  Top = 137
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Speed Filter'
  ClientHeight = 359
  ClientWidth = 675
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
    Width = 577
    Height = 321
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 8
      Width = 87
      Height = 13
      Caption = 'Speed distribution:'
    end
    object Label2: TLabel
      Left = 76
      Top = 284
      Width = 79
      Height = 13
      Caption = 'Lower Threshold'
    end
    object Label3: TLabel
      Left = 408
      Top = 284
      Width = 79
      Height = 13
      Caption = 'Upper Threshold'
    end
    object Panel2: TPanel
      Left = 16
      Top = 24
      Width = 545
      Height = 249
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object SpeedDistribution: TImage
        Left = 1
        Top = 1
        Width = 543
        Height = 247
        Align = alClient
      end
    end
    object LowerSpin: TCSpinButton
      Left = 57
      Top = 280
      Width = 15
      Height = 21
      DownGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000008080000080800000808000000000000080800000808000008080000080
        8000008080000080800000808000000000000000000000000000008080000080
        8000008080000080800000808000000000000000000000000000000000000000
        0000008080000080800000808000000000000000000000000000000000000000
        0000000000000000000000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      TabOrder = 1
      UpGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000000000000000000000000000000000000000000000000000000000000080
        8000008080000080800000000000000000000000000000000000000000000080
        8000008080000080800000808000008080000000000000000000000000000080
        8000008080000080800000808000008080000080800000808000000000000080
        8000008080000080800000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      OnDownClick = LowerSpinDownClick
      OnUpClick = LowerSpinUpClick
    end
    object UpperSpin: TCSpinButton
      Left = 545
      Top = 280
      Width = 15
      Height = 21
      DownGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000008080000080800000808000000000000080800000808000008080000080
        8000008080000080800000808000000000000000000000000000008080000080
        8000008080000080800000808000000000000000000000000000000000000000
        0000008080000080800000808000000000000000000000000000000000000000
        0000000000000000000000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      FocusControl = UpperTh
      TabOrder = 2
      UpGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000000000000000000000000000000000000000000000000000000000000080
        8000008080000080800000000000000000000000000000000000000000000080
        8000008080000080800000808000008080000000000000000000000000000080
        8000008080000080800000808000008080000080800000808000000000000080
        8000008080000080800000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      OnDownClick = UpperSpinDownClick
      OnUpClick = UpperSpinUpClick
    end
    object LowerTh: TEdit
      Left = 16
      Top = 280
      Width = 41
      Height = 21
      TabOrder = 3
      Text = '0.0'
      OnExit = LowerThExit
      OnKeyPress = ThKeyPress
    end
    object UpperTh: TEdit
      Left = 504
      Top = 280
      Width = 41
      Height = 21
      TabOrder = 4
      Text = '100.0'
      OnExit = UpperThExit
      OnKeyPress = ThKeyPress
    end
  end
  object Go: TButton
    Left = 592
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 592
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
    Top = 336
    Width = 659
    Height = 16
    TabOrder = 3
  end
end
