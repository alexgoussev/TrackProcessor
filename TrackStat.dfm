object TrackStatForm: TTrackStatForm
  Left = 384
  Top = 19
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsDialog
  Caption = 'Track Statistics'
  ClientHeight = 557
  ClientWidth = 647
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 647
    Height = 557
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object GroupBox6: TGroupBox
      Left = 328
      Top = 264
      Width = 305
      Height = 97
      Caption = ' Indices '
      TabOrder = 0
      object Label25: TLabel
        Left = 16
        Top = 24
        Width = 38
        Height = 13
        Caption = 'GPSies:'
      end
      object Label26: TLabel
        Left = 16
        Top = 48
        Width = 61
        Height = 13
        Caption = 'ClimbByBike:'
      end
      object Label27: TLabel
        Left = 16
        Top = 72
        Width = 33
        Height = 13
        Caption = 'FIETS:'
      end
      object IndexGPSies: TLabel
        Left = 176
        Top = 24
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object IndexClimbByBike: TLabel
        Left = 176
        Top = 48
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object IndexFIETS: TLabel
        Left = 176
        Top = 72
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
    end
    object GroupBox5: TGroupBox
      Left = 328
      Top = 184
      Width = 305
      Height = 73
      Caption = ' Climb '
      TabOrder = 1
      object Label23: TLabel
        Left = 16
        Top = 24
        Width = 89
        Height = 13
        Caption = 'Maximum Uphill ('#176'):'
      end
      object Label24: TLabel
        Left = 16
        Top = 48
        Width = 103
        Height = 13
        Caption = 'Maximum Downhill ('#176'):'
      end
      object ClimbUphill: TLabel
        Left = 176
        Top = 24
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object ClimbDownhill: TLabel
        Left = 176
        Top = 48
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
    end
    object GroupBox4: TGroupBox
      Left = 328
      Top = 8
      Width = 305
      Height = 169
      Caption = ' Speed '
      TabOrder = 2
      object Label18: TLabel
        Left = 16
        Top = 24
        Width = 81
        Height = 13
        Caption = 'Maximum (km/h):'
      end
      object Label19: TLabel
        Left = 16
        Top = 48
        Width = 77
        Height = 13
        Caption = 'Average (km/h):'
      end
      object Label20: TLabel
        Left = 16
        Top = 72
        Width = 130
        Height = 13
        Caption = 'Movement Average (km/h):'
      end
      object Label21: TLabel
        Left = 16
        Top = 96
        Width = 106
        Height = 13
        Caption = 'Average Uphill (km/h):'
      end
      object Label22: TLabel
        Left = 16
        Top = 120
        Width = 120
        Height = 13
        Caption = 'Average Downhill (km/h):'
      end
      object SpdMax: TLabel
        Left = 176
        Top = 24
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object SpdAvg: TLabel
        Left = 176
        Top = 48
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object SpdMovAvg: TLabel
        Left = 176
        Top = 72
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object SpdAvgUp: TLabel
        Left = 176
        Top = 96
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object SpdAvgDn: TLabel
        Left = 176
        Top = 120
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object Label31: TLabel
        Left = 16
        Top = 144
        Width = 97
        Height = 13
        Caption = 'Average Flat (km/h):'
      end
      object SpdAvgFlat: TLabel
        Left = 176
        Top = 144
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
    end
    object GroupBox3: TGroupBox
      Left = 16
      Top = 312
      Width = 305
      Height = 193
      Caption = ' Altitude '
      TabOrder = 3
      object Label11: TLabel
        Left = 16
        Top = 24
        Width = 64
        Height = 13
        Caption = 'Maximum (m):'
      end
      object Label12: TLabel
        Left = 16
        Top = 48
        Width = 61
        Height = 13
        Caption = 'Minimum (m):'
      end
      object Label13: TLabel
        Left = 16
        Top = 72
        Width = 69
        Height = 13
        Caption = 'Difference (m):'
      end
      object Label14: TLabel
        Left = 16
        Top = 96
        Width = 54
        Height = 13
        Caption = 'Accent (m):'
      end
      object Label15: TLabel
        Left = 16
        Top = 120
        Width = 60
        Height = 13
        Caption = 'Descent (m):'
      end
      object Label16: TLabel
        Left = 16
        Top = 144
        Width = 115
        Height = 13
        Caption = 'Relative Accent (m/km):'
      end
      object Label17: TLabel
        Left = 16
        Top = 168
        Width = 121
        Height = 13
        Caption = 'Relative Descent (m/km):'
      end
      object AltMax: TLabel
        Left = 176
        Top = 24
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object AltMin: TLabel
        Left = 176
        Top = 48
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object AltDiff: TLabel
        Left = 176
        Top = 72
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object AltAccent: TLabel
        Left = 176
        Top = 96
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object AltDescent: TLabel
        Left = 176
        Top = 120
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object AltRelAcc: TLabel
        Left = 176
        Top = 144
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object AltRelDesc: TLabel
        Left = 176
        Top = 168
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
    end
    object GroupBox2: TGroupBox
      Left = 16
      Top = 184
      Width = 305
      Height = 121
      Caption = ' Distance '
      TabOrder = 4
      object Label7: TLabel
        Left = 16
        Top = 24
        Width = 50
        Height = 13
        Caption = 'Total (km):'
      end
      object Label8: TLabel
        Left = 16
        Top = 48
        Width = 52
        Height = 13
        Caption = 'Uphill (km):'
      end
      object Label9: TLabel
        Left = 16
        Top = 72
        Width = 66
        Height = 13
        Caption = 'Downhill (km):'
      end
      object Label10: TLabel
        Left = 16
        Top = 96
        Width = 43
        Height = 13
        Caption = 'Flat (km):'
      end
      object DistTotal: TLabel
        Left = 176
        Top = 24
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object DistUphill: TLabel
        Left = 176
        Top = 48
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object DistDownhill: TLabel
        Left = 176
        Top = 72
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object DistFlat: TLabel
        Left = 176
        Top = 96
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 8
      Width = 305
      Height = 169
      Caption = ' Time '
      TabOrder = 5
      object Label1: TLabel
        Left = 16
        Top = 24
        Width = 27
        Height = 13
        Caption = 'Total:'
      end
      object Label2: TLabel
        Left = 16
        Top = 48
        Width = 53
        Height = 13
        Caption = 'Movement:'
      end
      object Label3: TLabel
        Left = 16
        Top = 72
        Width = 25
        Height = 13
        Caption = 'Stop:'
      end
      object Label4: TLabel
        Left = 16
        Top = 96
        Width = 29
        Height = 13
        Caption = 'Uphill:'
      end
      object Label5: TLabel
        Left = 16
        Top = 120
        Width = 43
        Height = 13
        Caption = 'Downhill:'
      end
      object Label6: TLabel
        Left = 16
        Top = 144
        Width = 20
        Height = 13
        Caption = 'Flat:'
      end
      object TimeTotal: TLabel
        Left = 176
        Top = 24
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
      end
      object TimeMovement: TLabel
        Left = 176
        Top = 48
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        BiDiMode = bdRightToLeft
        ParentBiDiMode = False
      end
      object TimeStop: TLabel
        Left = 176
        Top = 72
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        BiDiMode = bdRightToLeft
        ParentBiDiMode = False
      end
      object TimeUphill: TLabel
        Left = 176
        Top = 96
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        BiDiMode = bdRightToLeft
        ParentBiDiMode = False
      end
      object TimeDownhill: TLabel
        Left = 176
        Top = 120
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        BiDiMode = bdRightToLeft
        ParentBiDiMode = False
      end
      object TimeFlat: TLabel
        Left = 176
        Top = 144
        Width = 110
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        BiDiMode = bdRightToLeft
        ParentBiDiMode = False
      end
    end
    object Save: TButton
      Left = 542
      Top = 520
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 6
      OnClick = SaveClick
    end
    object GroupBox7: TGroupBox
      Left = 328
      Top = 368
      Width = 305
      Height = 145
      Caption = ' Settings '
      TabOrder = 7
      object Label28: TLabel
        Left = 16
        Top = 20
        Width = 165
        Height = 13
        Caption = 'Minimum Movement Speed (km/h):'
      end
      object Label29: TLabel
        Left = 16
        Top = 50
        Width = 85
        Height = 13
        Caption = 'Minimum Climb ('#176'):'
      end
      object Label30: TLabel
        Left = 16
        Top = 80
        Width = 146
        Height = 13
        Caption = 'Minimum Accend/Descent (m):'
      end
      object Set: TButton
        Left = 216
        Top = 112
        Width = 75
        Height = 25
        Caption = 'Set'
        TabOrder = 0
        OnClick = SetClick
      end
      object SetMinSpeed: TEdit
        Left = 240
        Top = 18
        Width = 49
        Height = 21
        TabOrder = 1
        OnKeyPress = SetMinSpeedKeyPress
      end
      object SetMinClimb: TEdit
        Left = 240
        Top = 48
        Width = 49
        Height = 21
        TabOrder = 2
        OnKeyPress = SetMinClimbKeyPress
      end
      object SetMinAccend: TEdit
        Left = 240
        Top = 78
        Width = 49
        Height = 21
        TabOrder = 3
        OnKeyPress = SetMinSpeedKeyPress
      end
    end
  end
  object SaveStatDialog: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Text Files|*.txt|All Files|*.*'
    FilterIndex = 0
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 512
    Top = 520
  end
end
