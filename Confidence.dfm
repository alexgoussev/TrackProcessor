object ConfidenceForm: TConfidenceForm
  Left = 430
  Top = 51
  BorderStyle = bsDialog
  Caption = 'Confidence Interval'
  ClientHeight = 603
  ClientWidth = 567
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  DesignSize = (
    567
    603)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 457
    Height = 569
    BevelOuter = bvLowered
    TabOrder = 0
    DesignSize = (
      457
      569)
    object Label1: TLabel
      Left = 17
      Top = 11
      Width = 58
      Height = 13
      Caption = 'Base Track:'
    end
    object Label2: TLabel
      Left = 17
      Top = 35
      Width = 62
      Height = 13
      Caption = 'Trend Track:'
    end
    object m_pExchange: TSpeedButton
      Left = 417
      Top = 19
      Width = 24
      Height = 24
      Anchors = [akTop, akRight]
      Glyph.Data = {
        C6050000424DC605000000000000360400002800000014000000140000000100
        0800000000009001000000000000000000000001000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
        A6000020400000206000002080000020A0000020C0000020E000004000000040
        20000040400000406000004080000040A0000040C0000040E000006000000060
        20000060400000606000006080000060A0000060C0000060E000008000000080
        20000080400000806000008080000080A0000080C0000080E00000A0000000A0
        200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
        200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
        200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
        20004000400040006000400080004000A0004000C0004000E000402000004020
        20004020400040206000402080004020A0004020C0004020E000404000004040
        20004040400040406000404080004040A0004040C0004040E000406000004060
        20004060400040606000406080004060A0004060C0004060E000408000004080
        20004080400040806000408080004080A0004080C0004080E00040A0000040A0
        200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
        200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
        200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
        20008000400080006000800080008000A0008000C0008000E000802000008020
        20008020400080206000802080008020A0008020C0008020E000804000008040
        20008040400080406000804080008040A0008040C0008040E000806000008060
        20008060400080606000806080008060A0008060C0008060E000808000008080
        20008080400080806000808080008080A0008080C0008080E00080A0000080A0
        200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
        200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
        200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
        2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
        2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
        2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
        2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
        2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
        2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
        2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F7F7F7F7F7F7
        F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F704F7F7F9F9F9F7F7F7F7
        F7F7F7F7F7F7F7F7F70404F7F7F9F9F9F7F7F7F7F7F7F7F7F7F7F7F7040404F7
        F7F9F9F9F7F7F7F7F7F7F7F7F7F7F704040404F7F7F9F9F9F7F7F7F7F7F7F7F7
        F7F70404040404F7F7F9F9F9F7F7F7F7F7F7F7F7F7040404040404F7F7F9F9F9
        F7F7F7F7F7F7F7F704040404040404F7F7F9F9F9F7F7F7F7F7F7F7F7F7F7F7F7
        040404F7F7F9F9F9F7F7F7F7F7F7F7F7F7F7F7F7040404F7F7F9F9F9F7F7F7F7
        F7F7F7F7F7F7F7F7040404F7F7F9F9F9F7F7F7F7F7F7F7F7F7F7F7F7040404F7
        F7F9F9F9F7F7F7F7F7F7F7F7F7F7F7F7040404F7F7F9F9F9F9F9F9F9F7F7F7F7
        F7F7F7F7040404F7F7F9F9F9F9F9F9F7F7F7F7F7F7F7F7F7040404F7F7F9F9F9
        F9F9F7F7F7F7F7F7F7F7F7F7040404F7F7F9F9F9F9F7F7F7F7F7F7F7F7F7F7F7
        040404F7F7F9F9F9F7F7F7F7F7F7F7F7F7F7F7F7040404F7F7F9F9F7F7F7F7F7
        F7F7F7F7F7F7F7F7040404F7F7F9F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7
        F7F7F7F7F7F7F7F7F7F7}
      OnClick = ExchangeClick
    end
    object Label3: TLabel
      Left = 16
      Top = 176
      Width = 106
      Height = 13
      Caption = 'Deviations distribution:'
    end
    object Label7: TLabel
      Left = 248
      Top = 436
      Width = 99
      Height = 13
      Caption = 'Upper Threshold (m):'
    end
    object Panel2: TPanel
      Left = 16
      Top = 192
      Width = 425
      Height = 233
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvLowered
      TabOrder = 0
      object m_pResDistr: TImage
        Left = 1
        Top = 1
        Width = 423
        Height = 231
        Align = alClient
      end
    end
    object m_pBase: TEdit
      Left = 96
      Top = 9
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      ReadOnly = True
      TabOrder = 1
    end
    object m_pTrend: TEdit
      Left = 96
      Top = 33
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      ReadOnly = True
      TabOrder = 2
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 64
      Width = 425
      Height = 105
      Caption = ' Deviations statistics '
      TabOrder = 3
      object Label4: TLabel
        Left = 8
        Top = 24
        Width = 64
        Height = 13
        Caption = 'Maximum (m):'
      end
      object Label5: TLabel
        Left = 8
        Top = 48
        Width = 60
        Height = 13
        Caption = 'Average (m):'
      end
      object Label6: TLabel
        Left = 8
        Top = 72
        Width = 122
        Height = 13
        Caption = 'Standard deviation, S (m):'
      end
      object m_pMax: TEdit
        Left = 352
        Top = 20
        Width = 65
        Height = 21
        ReadOnly = True
        TabOrder = 0
      end
      object m_pAvg: TEdit
        Left = 352
        Top = 46
        Width = 65
        Height = 21
        ReadOnly = True
        TabOrder = 1
      end
      object m_pS: TEdit
        Left = 352
        Top = 70
        Width = 65
        Height = 21
        ReadOnly = True
        TabOrder = 2
      end
    end
    object UpperTh: TCSpinButton
      Left = 424
      Top = 432
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
      FocusControl = m_pUpperThereshould
      TabOrder = 4
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
      OnDownClick = UpperThDownClick
      OnUpClick = UpperThUpClick
    end
    object m_pUpperThereshould: TEdit
      Left = 376
      Top = 432
      Width = 49
      Height = 21
      TabOrder = 5
      OnExit = UpperThereshouldExit
      OnKeyPress = UpperThereshouldKeyPress
    end
    object GroupBox2: TGroupBox
      Left = 16
      Top = 456
      Width = 425
      Height = 81
      Caption = ' Set Upper Threshold '
      TabOrder = 6
      object Label8: TLabel
        Left = 8
        Top = 24
        Width = 63
        Height = 13
        Caption = 'According to '
      end
      object Label9: TLabel
        Left = 104
        Top = 24
        Width = 5
        Height = 13
        Caption = '/'
      end
      object Label10: TLabel
        Left = 144
        Top = 24
        Width = 17
        Height = 13
        Caption = 'rule'
      end
      object Label11: TLabel
        Left = 8
        Top = 54
        Width = 63
        Height = 13
        Caption = 'According to '
      end
      object Label12: TLabel
        Left = 112
        Top = 54
        Width = 8
        Height = 13
        Caption = '%'
      end
      object m_p2S: TButton
        Left = 72
        Top = 24
        Width = 30
        Height = 16
        Caption = '2S'
        TabOrder = 0
        OnClick = S2Click
      end
      object m_p3S: TButton
        Left = 112
        Top = 24
        Width = 30
        Height = 16
        Caption = '3S'
        TabOrder = 1
        OnClick = S3Click
      end
      object m_pConfIntVal: TCSpinEdit
        Left = 72
        Top = 50
        Width = 41
        Height = 22
        MaxValue = 99
        MinValue = 1
        TabOrder = 2
        Value = 95
      end
      object m_pConfidence: TButton
        Left = 128
        Top = 52
        Width = 113
        Height = 16
        Caption = 'Confidence Interval'
        TabOrder = 3
        OnClick = ConfidenceClick
      end
    end
    object m_pReplace: TCheckBox
      Left = 16
      Top = 544
      Width = 209
      Height = 17
      Caption = 'Replace "bad" points from trend'
      TabOrder = 7
    end
  end
  object m_pProgress: TProgressBar
    Left = 8
    Top = 583
    Width = 551
    Height = 16
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 1
  end
  object m_pGo: TButton
    Left = 480
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = GoClick
  end
  object Button1: TButton
    Left = 480
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 3
  end
end
