object ImgSize: TImgSize
  Left = 331
  Top = 170
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Image size'
  ClientHeight = 92
  ClientWidth = 214
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
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 194
    Height = 41
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 10
      Width = 55
      Height = 13
      Caption = 'Image Size:'
    end
    object Label2: TLabel
      Left = 108
      Top = 12
      Width = 5
      Height = 13
      Caption = 'x'
    end
    object Label3: TLabel
      Left = 158
      Top = 10
      Width = 26
      Height = 13
      Caption = 'pixels'
    end
    object Width: TEdit
      Left = 72
      Top = 8
      Width = 33
      Height = 21
      TabOrder = 0
      Text = '800'
    end
    object Height: TEdit
      Left = 118
      Top = 8
      Width = 33
      Height = 21
      TabOrder = 1
      Text = '300'
    end
  end
  object Go: TButton
    Left = 48
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Save'
    ModalResult = 1
    TabOrder = 1
    OnClick = GoClick
  end
  object Exit: TButton
    Left = 128
    Top = 56
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 2
  end
end
