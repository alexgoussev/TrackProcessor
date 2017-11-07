object SplitForm: TSplitForm
  Left = 359
  Top = 156
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Split Track'
  ClientHeight = 168
  ClientWidth = 420
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
  object Go: TButton
    Left = 336
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 0
    OnClick = GoClick
  end
  object Cancel: TButton
    Left = 336
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 1
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 144
    Width = 401
    Height = 16
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 313
    Height = 129
    BevelOuter = bvLowered
    TabOrder = 3
    object Label1: TLabel
      Left = 218
      Top = 14
      Width = 5
      Height = 13
      Caption = ':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 258
      Top = 14
      Width = 5
      Height = 13
      Caption = ':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 218
      Top = 42
      Width = 5
      Height = 13
      Caption = ':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label4: TLabel
      Left = 258
      Top = 42
      Width = 5
      Height = 13
      Caption = ':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object PointsInterval: TRadioButton
      Left = 16
      Top = 38
      Width = 153
      Height = 17
      Caption = 'Track Points Interval'
      TabOrder = 0
      OnClick = CriterionClick
    end
    object TrackLength: TRadioButton
      Left = 16
      Top = 66
      Width = 113
      Height = 17
      Caption = 'Track Length (m)'
      TabOrder = 1
      OnClick = CriterionClick
    end
    object TrackDuration: TRadioButton
      Left = 16
      Top = 10
      Width = 113
      Height = 17
      Caption = 'Track Duration'
      Checked = True
      TabOrder = 2
      TabStop = True
      OnClick = CriterionClick
    end
    object SecL: TEdit
      Left = 264
      Top = 38
      Width = 33
      Height = 21
      TabOrder = 3
    end
    object SecD: TEdit
      Left = 264
      Top = 10
      Width = 33
      Height = 21
      TabOrder = 4
    end
    object PointsDistance: TRadioButton
      Left = 16
      Top = 94
      Width = 153
      Height = 17
      Caption = 'Track Points Distance (m)'
      TabOrder = 5
      OnClick = CriterionClick
    end
    object MinL: TEdit
      Left = 224
      Top = 38
      Width = 33
      Height = 21
      TabOrder = 6
    end
    object MinD: TEdit
      Left = 224
      Top = 10
      Width = 33
      Height = 21
      TabOrder = 7
    end
    object Length: TEdit
      Left = 224
      Top = 66
      Width = 73
      Height = 21
      TabOrder = 8
    end
    object HourL: TEdit
      Left = 184
      Top = 38
      Width = 33
      Height = 21
      TabOrder = 9
    end
    object HourD: TEdit
      Left = 184
      Top = 10
      Width = 33
      Height = 21
      TabOrder = 10
    end
    object Distance: TEdit
      Left = 224
      Top = 94
      Width = 73
      Height = 21
      TabOrder = 11
    end
  end
end
