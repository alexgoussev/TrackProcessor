object HDOPForm: THDOPForm
  Left = 357
  Top = 130
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'HDOP Filter'
  ClientHeight = 549
  ClientWidth = 683
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  DesignSize = (
    683
    549)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 577
    Height = 513
    Anchors = [akLeft, akTop, akRight]
    BevelOuter = bvLowered
    TabOrder = 0
    DesignSize = (
      577
      513)
    object Label1: TLabel
      Left = 16
      Top = 112
      Width = 89
      Height = 13
      Caption = 'HDOP Distribution:'
    end
    object Label2: TLabel
      Left = 408
      Top = 388
      Width = 79
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'Upper Threshold'
    end
    object Panel2: TPanel
      Left = 16
      Top = 128
      Width = 545
      Height = 249
      Anchors = [akLeft, akTop, akRight]
      BevelInner = bvLowered
      BevelOuter = bvNone
      TabOrder = 0
      object HDOPDistribution: TImage
        Left = 1
        Top = 1
        Width = 543
        Height = 247
        Align = alClient
      end
    end
    object UpperSpin: TCSpinButton
      Left = 545
      Top = 384
      Width = 15
      Height = 21
      Anchors = [akTop, akRight]
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
      OnDownClick = UpperSpinDownClick
      OnUpClick = UpperSpinUpClick
    end
    object UpperTh: TEdit
      Left = 504
      Top = 384
      Width = 41
      Height = 21
      Anchors = [akTop, akRight]
      TabOrder = 2
      Text = '1.5'
      OnExit = UpperThExit
      OnKeyPress = UpperThKeyPress
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 8
      Width = 545
      Height = 97
      Anchors = [akLeft, akTop, akRight]
      Caption = ' HDOP Statistics '
      TabOrder = 3
      DesignSize = (
        545
        97)
      object Label3: TLabel
        Left = 8
        Top = 22
        Width = 47
        Height = 13
        Caption = 'Maximum:'
      end
      object Label4: TLabel
        Left = 8
        Top = 44
        Width = 43
        Height = 13
        Caption = 'Average:'
      end
      object Label5: TLabel
        Left = 8
        Top = 68
        Width = 110
        Height = 13
        Caption = 'Standardt Deviation, S:'
      end
      object m_pMax: TEdit
        Left = 496
        Top = 16
        Width = 41
        Height = 21
        Anchors = [akTop, akRight]
        ReadOnly = True
        TabOrder = 0
      end
      object m_pAvg: TEdit
        Left = 496
        Top = 40
        Width = 41
        Height = 21
        Anchors = [akTop, akRight]
        ReadOnly = True
        TabOrder = 1
      end
      object m_pS: TEdit
        Left = 496
        Top = 64
        Width = 41
        Height = 21
        Anchors = [akTop, akRight]
        ReadOnly = True
        TabOrder = 2
      end
    end
    object GroupBox2: TGroupBox
      Left = 16
      Top = 408
      Width = 545
      Height = 89
      Caption = ' Set Upper Threshold '
      TabOrder = 4
      object Label6: TLabel
        Left = 8
        Top = 24
        Width = 63
        Height = 13
        Caption = 'According to '
      end
      object Label7: TLabel
        Left = 104
        Top = 24
        Width = 5
        Height = 13
        Caption = '/'
      end
      object Label8: TLabel
        Left = 144
        Top = 24
        Width = 17
        Height = 13
        Caption = 'rule'
      end
      object Label9: TLabel
        Left = 8
        Top = 56
        Width = 63
        Height = 13
        Caption = 'According to '
      end
      object Label10: TLabel
        Left = 112
        Top = 56
        Width = 8
        Height = 13
        Caption = '%'
      end
      object m_p2S: TButton
        Left = 72
        Top = 23
        Width = 30
        Height = 16
        Caption = '2S'
        TabOrder = 0
        OnClick = m_p2SClick
      end
      object m_p3S: TButton
        Left = 112
        Top = 23
        Width = 30
        Height = 16
        Caption = '3S'
        TabOrder = 1
        OnClick = m_p3SClick
      end
      object m_pCIPers: TCSpinEdit
        Left = 72
        Top = 53
        Width = 41
        Height = 22
        MaxValue = 99
        MinValue = 1
        TabOrder = 2
        Value = 95
      end
      object m_pCI: TButton
        Left = 128
        Top = 56
        Width = 130
        Height = 16
        Caption = 'Confidence Interval'
        TabOrder = 3
        OnClick = m_pCIClick
      end
    end
  end
  object Go: TButton
    Left = 600
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 600
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 2
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 527
    Width = 665
    Height = 16
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 3
  end
end
