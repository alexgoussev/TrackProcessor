object SettingsForm: TSettingsForm
  Left = 310
  Top = 145
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Settings'
  ClientHeight = 443
  ClientWidth = 508
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
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 409
    Height = 105
    Caption = ' Path '
    TabOrder = 0
    object HGTPath: TLabeledEdit
      Left = 8
      Top = 32
      Width = 369
      Height = 21
      EditLabel.Width = 79
      EditLabel.Height = 13
      EditLabel.Caption = 'HGT Files Folder'
      ReadOnly = True
      TabOrder = 0
    end
    object CachePath: TLabeledEdit
      Left = 8
      Top = 72
      Width = 369
      Height = 21
      EditLabel.Width = 87
      EditLabel.Height = 13
      EditLabel.Caption = 'Map Cache Folder'
      ReadOnly = True
      TabOrder = 1
    end
    object HGTPathButton: TButton
      Left = 378
      Top = 32
      Width = 21
      Height = 21
      Caption = '...'
      TabOrder = 2
      OnClick = HGTPathButtonClick
    end
    object CachePathButton: TButton
      Left = 378
      Top = 72
      Width = 21
      Height = 21
      Caption = '...'
      TabOrder = 3
      OnClick = CachePathButtonClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 120
    Width = 409
    Height = 57
    Caption = ' Map Cache '
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 113
      Height = 13
      Caption = 'Maximal Tile age (days):'
    end
    object TileAge: TEdit
      Left = 344
      Top = 20
      Width = 41
      Height = 21
      TabOrder = 0
      Text = '30'
    end
    object FileAgeUpDown: TUpDown
      Left = 385
      Top = 20
      Width = 15
      Height = 21
      Associate = TileAge
      Max = 32767
      Position = 30
      TabOrder = 1
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 184
    Width = 409
    Height = 249
    Caption = ' Open File '
    TabOrder = 2
    object Label2: TLabel
      Left = 8
      Top = 24
      Width = 192
      Height = 13
      Caption = 'Speed Calculation Averaging Time (sec):'
    end
    object Label3: TLabel
      Left = 8
      Top = 56
      Width = 234
      Height = 13
      Caption = 'Climb Calculation Minimum Altitude Difference (m):'
    end
    object Label4: TLabel
      Left = 8
      Top = 88
      Width = 189
      Height = 13
      Caption = 'Climb Calculation Minimum Distance (m):'
    end
    object Label5: TLabel
      Left = 8
      Top = 120
      Width = 195
      Height = 13
      Caption = 'Climb Calculation Minimum Speed (km/h):'
    end
    object Label6: TLabel
      Left = 8
      Top = 152
      Width = 157
      Height = 13
      Caption = 'Columbus CSV HDOP Factor (m):'
    end
    object Label7: TLabel
      Left = 8
      Top = 184
      Width = 258
      Height = 13
      Caption = 'Colubus CSV Speed Complimentary  Filter Factor (0..1):'
    end
    object Label8: TLabel
      Left = 8
      Top = 216
      Width = 177
      Height = 13
      Caption = 'Restore Columbus Speed and Course'
    end
    object AvgTime: TEdit
      Left = 344
      Top = 20
      Width = 41
      Height = 21
      TabOrder = 0
      Text = '5'
    end
    object AvgTimeUpDown: TUpDown
      Left = 385
      Top = 20
      Width = 15
      Height = 21
      Associate = AvgTime
      Min = 1
      Max = 600
      Position = 5
      TabOrder = 1
    end
    object ClimbAltDiff: TEdit
      Left = 344
      Top = 52
      Width = 41
      Height = 21
      TabOrder = 2
      Text = '1'
    end
    object ClimbAltDiffUpDown: TUpDown
      Left = 385
      Top = 52
      Width = 15
      Height = 21
      Associate = ClimbAltDiff
      Min = 1
      Max = 10
      Position = 1
      TabOrder = 3
    end
    object ClimbMinDist: TEdit
      Left = 344
      Top = 84
      Width = 41
      Height = 21
      TabOrder = 4
      Text = '1'
    end
    object ClimbDistUpDown: TUpDown
      Left = 385
      Top = 84
      Width = 15
      Height = 21
      Associate = ClimbMinDist
      Min = 1
      Position = 1
      TabOrder = 5
    end
    object ClimbMinSpeed: TEdit
      Left = 344
      Top = 116
      Width = 41
      Height = 21
      TabOrder = 6
      Text = '1'
    end
    object ClimbSpeedUpDown: TUpDown
      Left = 385
      Top = 116
      Width = 15
      Height = 21
      Associate = ClimbMinSpeed
      Min = 1
      Max = 25
      Position = 1
      TabOrder = 7
    end
    object HDOPFactor: TEdit
      Left = 344
      Top = 148
      Width = 41
      Height = 21
      TabOrder = 8
      Text = '5'
    end
    object HDOPFactprUpDown: TUpDown
      Left = 385
      Top = 148
      Width = 15
      Height = 21
      Associate = HDOPFactor
      Min = 1
      Max = 25
      Position = 5
      TabOrder = 9
    end
    object ComplimentaryFactor: TEdit
      Left = 344
      Top = 180
      Width = 54
      Height = 21
      TabOrder = 10
      Text = '0.9'
      OnKeyPress = ComplimentaryFactorKeyPress
    end
    object ResoreSpeed: TCheckBox
      Left = 366
      Top = 216
      Width = 33
      Height = 17
      Alignment = taLeftJustify
      Checked = True
      State = cbChecked
      TabOrder = 11
    end
  end
  object SaveButton: TButton
    Left = 424
    Top = 16
    Width = 75
    Height = 25
    Action = SaveAction
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object CancelButton: TButton
    Left = 424
    Top = 48
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object ActionList1: TActionList
    Left = 472
    Top = 408
    object SaveAction: TAction
      Caption = 'Save'
      OnExecute = SaveActionExecute
      OnUpdate = SaveActionUpdate
    end
  end
end
