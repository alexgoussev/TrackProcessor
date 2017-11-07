object SplineSmoothForm: TSplineSmoothForm
  Left = 368
  Top = 134
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Spline Smooth'
  ClientHeight = 467
  ClientWidth = 798
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 689
    Height = 433
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 18
      Width = 102
      Height = 13
      Caption = 'Base Smooth Factor: '
    end
    object Label2: TLabel
      Left = 176
      Top = 18
      Width = 28
      Height = 13
      Caption = 'points'
    end
    object BaseSmoothFactor: TEdit
      Left = 128
      Top = 16
      Width = 41
      Height = 21
      TabOrder = 0
      Text = '0.1'
      OnKeyPress = OnKeyPress
    end
    object GroupBox1: TGroupBox
      Left = 16
      Top = 48
      Width = 657
      Height = 305
      Caption = ' Increase Smooth Factor Criteria: '
      TabOrder = 1
      object GroupBox2: TGroupBox
        Left = 8
        Top = 24
        Width = 641
        Height = 49
        Caption = ' On HDOP '
        TabOrder = 0
        object Label3: TLabel
          Left = 8
          Top = 20
          Width = 127
          Height = 13
          Caption = 'Increase Smooth Factor by'
        end
        object Label4: TLabel
          Left = 192
          Top = 20
          Width = 75
          Height = 13
          Caption = 'points on every '
        end
        object Label5: TLabel
          Left = 320
          Top = 20
          Width = 164
          Height = 13
          Caption = 'HDOP points if HDOP greater than'
        end
        object HDOPIncr: TEdit
          Left = 144
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 0
          Text = '2.5'
          OnKeyPress = OnKeyPress
        end
        object HDOPStep: TEdit
          Left = 272
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 1
          Text = '0.2'
          OnKeyPress = OnKeyPress
        end
        object HDOPLevel: TEdit
          Left = 496
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 2
          Text = '1.5'
          OnKeyPress = OnKeyPress
        end
      end
      object GroupBox3: TGroupBox
        Left = 8
        Top = 80
        Width = 641
        Height = 49
        Caption = ' On Speed '
        TabOrder = 1
        object Label6: TLabel
          Left = 8
          Top = 20
          Width = 127
          Height = 13
          Caption = 'Increase Smooth Factor by'
        end
        object Label7: TLabel
          Left = 192
          Top = 20
          Width = 72
          Height = 13
          Caption = 'points on every'
        end
        object Label8: TLabel
          Left = 320
          Top = 20
          Width = 112
          Height = 13
          Caption = 'km/h if Speed less than'
        end
        object Label9: TLabel
          Left = 488
          Top = 20
          Width = 25
          Height = 13
          Caption = 'km/h'
        end
        object SpeedIncr: TEdit
          Left = 144
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 0
          Text = '2.5'
          OnKeyPress = OnKeyPress
        end
        object SpeedStep: TEdit
          Left = 272
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 1
          Text = '1'
          OnKeyPress = OnKeyPress
        end
        object SpeedLevel: TEdit
          Left = 440
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 2
          Text = '5'
          OnKeyPress = OnKeyPress
        end
      end
      object GroupBox4: TGroupBox
        Left = 8
        Top = 136
        Width = 641
        Height = 73
        Caption = ' On Course changing '
        TabOrder = 2
        object Label10: TLabel
          Left = 8
          Top = 20
          Width = 127
          Height = 13
          Caption = 'Increase Smooth Factor by'
        end
        object Label11: TLabel
          Left = 192
          Top = 20
          Width = 72
          Height = 13
          Caption = 'points on every'
        end
        object Label12: TLabel
          Left = 320
          Top = 20
          Width = 187
          Height = 13
          Caption = 'deg/s speed of direction change above'
        end
        object Label13: TLabel
          Left = 568
          Top = 20
          Width = 28
          Height = 13
          Caption = 'deg/s'
        end
        object CourseIncr: TEdit
          Left = 144
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 0
          Text = '2.5'
          OnKeyPress = OnKeyPress
        end
        object CourseStep: TEdit
          Left = 272
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 1
          Text = '15'
          OnKeyPress = OnKeyPress
        end
        object CourseLevel: TEdit
          Left = 520
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 2
          Text = '30'
          OnKeyPress = OnKeyPress
        end
        object InstantCourse: TCheckBox
          Left = 8
          Top = 48
          Width = 265
          Height = 17
          Caption = 'Use calculated by coordinates instatnt course'
          Checked = True
          State = cbChecked
          TabOrder = 3
        end
      end
      object GroupBox5: TGroupBox
        Left = 8
        Top = 216
        Width = 641
        Height = 73
        Caption = ' On Speed changing '
        TabOrder = 3
        object Label14: TLabel
          Left = 8
          Top = 20
          Width = 127
          Height = 13
          Caption = 'Increase Smooth Factor by'
        end
        object Label15: TLabel
          Left = 192
          Top = 20
          Width = 72
          Height = 13
          Caption = 'points on every'
        end
        object Label16: TLabel
          Left = 320
          Top = 20
          Width = 181
          Height = 13
          Caption = 'km/h per sec of speed chaging above'
        end
        object Label17: TLabel
          Left = 560
          Top = 20
          Width = 63
          Height = 13
          Caption = 'km/h per sec'
        end
        object AccIncr: TEdit
          Left = 144
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 0
          Text = '2.5'
          OnKeyPress = OnKeyPress
        end
        object AccStep: TEdit
          Left = 272
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 1
          Text = '5'
          OnKeyPress = OnKeyPress
        end
        object AccLevel: TEdit
          Left = 512
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 2
          Text = '5'
          OnKeyPress = OnKeyPress
        end
        object InstatntSpeed: TCheckBox
          Left = 8
          Top = 48
          Width = 241
          Height = 17
          Caption = 'Use calculated by coordinates instatnt speed'
          Checked = True
          State = cbChecked
          TabOrder = 3
        end
      end
    end
    object GroupBox6: TGroupBox
      Left = 16
      Top = 360
      Width = 657
      Height = 57
      Caption = ' Parameters to smooth '
      TabOrder = 2
      object SmoothCoordinates: TCheckBox
        Left = 16
        Top = 24
        Width = 97
        Height = 17
        Caption = 'Coordinates'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object SmoothAltitude: TCheckBox
        Left = 120
        Top = 24
        Width = 97
        Height = 17
        Caption = 'Altitude'
        TabOrder = 1
      end
      object SmoothSpeed: TCheckBox
        Left = 200
        Top = 24
        Width = 129
        Height = 17
        Caption = 'Speed and Course'
        TabOrder = 2
      end
    end
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 448
    Width = 689
    Height = 16
    TabOrder = 1
  end
  object Button1: TButton
    Left = 712
    Top = 8
    Width = 75
    Height = 25
    Action = StartAction
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 712
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object SmoothActionList: TActionList
    Left = 712
    Top = 80
    object StartAction: TAction
      Caption = 'Start'
      OnExecute = StartActionExecute
      OnUpdate = StartActionUpdate
    end
  end
end
