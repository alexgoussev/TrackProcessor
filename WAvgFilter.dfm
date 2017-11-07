object WAvgForm: TWAvgForm
  Left = 370
  Top = 175
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Averaging Smooth Filter'
  ClientHeight = 385
  ClientWidth = 379
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
  object Status: TLabel
    Left = 8
    Top = 344
    Width = 273
    Height = 13
    AutoSize = False
    Caption = 'Ready'
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 273
    Height = 329
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 136
      Height = 13
      Caption = 'Averaging Distance (meters):'
    end
    object Label5: TLabel
      Left = 8
      Top = 48
      Width = 103
      Height = 13
      Caption = 'Averaging Time (sec):'
    end
    object Label6: TLabel
      Left = 8
      Top = 208
      Width = 96
      Height = 13
      Caption = 'Smooth Factor (0-1):'
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 72
      Width = 249
      Height = 121
      Caption = ' Turn detection '
      TabOrder = 0
      object Label2: TLabel
        Left = 8
        Top = 24
        Width = 165
        Height = 13
        Caption = 'Minimum time since last point (sec):'
      end
      object Label3: TLabel
        Left = 8
        Top = 56
        Width = 110
        Height = 13
        Caption = 'Minimum speed (km/h):'
      end
      object Label4: TLabel
        Left = 8
        Top = 88
        Width = 141
        Height = 13
        Caption = 'Minimum turn angle (degrees):'
      end
      object Time: TEdit
        Left = 208
        Top = 22
        Width = 33
        Height = 21
        TabOrder = 0
        Text = '2'
      end
      object Speed: TEdit
        Left = 208
        Top = 54
        Width = 33
        Height = 21
        TabOrder = 1
        Text = '1'
      end
      object Angle: TEdit
        Left = 208
        Top = 86
        Width = 33
        Height = 21
        TabOrder = 2
        Text = '30'
      end
    end
    object Distance: TEdit
      Left = 216
      Top = 14
      Width = 33
      Height = 21
      TabOrder = 1
      Text = '10'
    end
    object AvgTime: TEdit
      Left = 216
      Top = 46
      Width = 33
      Height = 21
      TabOrder = 2
      Text = '25'
    end
    object SmoothFactor: TEdit
      Left = 216
      Top = 202
      Width = 33
      Height = 21
      TabOrder = 3
      Text = '0.25'
    end
    object GroupBox2: TGroupBox
      Left = 8
      Top = 232
      Width = 249
      Height = 81
      Caption = ' Additionally apply Filter to: '
      TabOrder = 4
      object Altitude: TCheckBox
        Left = 16
        Top = 24
        Width = 97
        Height = 17
        Caption = 'Altitude data'
        TabOrder = 0
      end
      object SpeedCourse: TCheckBox
        Left = 16
        Top = 48
        Width = 153
        Height = 17
        Caption = 'Speed and Course data'
        TabOrder = 1
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
    Top = 360
    Width = 361
    Height = 16
    TabOrder = 3
  end
end
