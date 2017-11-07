object GridSettings: TGridSettings
  Left = 363
  Top = 214
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Settings'
  ClientHeight = 101
  ClientWidth = 412
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
  object TimeFormat: TRadioGroup
    Left = 8
    Top = 8
    Width = 105
    Height = 81
    Caption = ' Time '
    ItemIndex = 0
    Items.Strings = (
      'UTC'
      'Local')
    TabOrder = 0
  end
  object CoordFormat: TRadioGroup
    Left = 128
    Top = 8
    Width = 185
    Height = 81
    Caption = 'Coordinates Format'
    ItemIndex = 0
    Items.Strings = (
      'DDD'#176' MM'#39' SS.SS"'
      'DDD'#176'.DDDDD')
    TabOrder = 1
  end
  object Ok: TButton
    Left = 328
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 2
  end
  object Cancel: TButton
    Left = 328
    Top = 48
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
