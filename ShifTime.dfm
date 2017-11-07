object TimeShift: TTimeShift
  Left = 344
  Top = 200
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Shift TrackPoints Time'
  ClientHeight = 160
  ClientWidth = 396
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
    Width = 289
    Height = 121
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 18
      Width = 75
      Height = 13
      Caption = 'First Point Time:'
    end
    object Label2: TLabel
      Left = 16
      Top = 42
      Width = 76
      Height = 13
      Caption = 'Last Point Time:'
    end
    object Label3: TLabel
      Left = 16
      Top = 84
      Width = 24
      Height = 13
      Caption = 'Shift:'
    end
    object Label4: TLabel
      Left = 148
      Top = 84
      Width = 3
      Height = 13
      Caption = ':'
    end
    object Label5: TLabel
      Left = 208
      Top = 84
      Width = 3
      Height = 13
      Caption = ':'
    end
    object FirstPtTime: TEdit
      Left = 104
      Top = 16
      Width = 161
      Height = 21
      ReadOnly = True
      TabOrder = 0
    end
    object LastPtTime: TEdit
      Left = 104
      Top = 40
      Width = 161
      Height = 21
      ReadOnly = True
      TabOrder = 1
    end
    object ShiftH: TCSpinEdit
      Left = 96
      Top = 80
      Width = 49
      Height = 22
      MaxValue = 99999999
      TabOrder = 2
    end
    object ShiftM: TCSpinEdit
      Left = 156
      Top = 80
      Width = 49
      Height = 22
      MaxValue = 59
      TabOrder = 3
    end
    object ShiftS: TCSpinEdit
      Left = 216
      Top = 80
      Width = 49
      Height = 22
      MaxValue = 59
      TabOrder = 4
    end
    object ShiftDir: TComboBox
      Left = 64
      Top = 80
      Width = 33
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 5
      Text = '+'
      Items.Strings = (
        '+'
        '-')
    end
  end
  object Go: TButton
    Left = 312
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 312
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
    Top = 136
    Width = 377
    Height = 16
    TabOrder = 3
  end
end
