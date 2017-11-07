object EditMetadata: TEditMetadata
  Left = 340
  Top = 177
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Track Metadata'
  ClientHeight = 404
  ClientWidth = 482
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
    Width = 465
    Height = 353
    BevelOuter = bvLowered
    TabOrder = 0
    object Label1: TLabel
      Left = 14
      Top = 12
      Width = 59
      Height = 13
      Caption = 'Track Name'
    end
    object Label2: TLabel
      Left = 14
      Top = 72
      Width = 84
      Height = 13
      Caption = 'Track Description'
    end
    object Label3: TLabel
      Left = 14
      Top = 168
      Width = 75
      Height = 13
      Caption = 'Track Comment'
    end
    object Label4: TLabel
      Left = 14
      Top = 264
      Width = 65
      Height = 13
      Caption = 'Track Source'
    end
    object Name: TEdit
      Left = 16
      Top = 32
      Width = 433
      Height = 21
      TabOrder = 0
    end
    object Description: TMemo
      Left = 16
      Top = 96
      Width = 433
      Height = 49
      TabOrder = 1
    end
    object Comment: TMemo
      Left = 16
      Top = 192
      Width = 433
      Height = 49
      TabOrder = 2
    end
    object Source: TMemo
      Left = 16
      Top = 288
      Width = 433
      Height = 49
      TabOrder = 3
    end
  end
  object Set: TButton
    Left = 320
    Top = 370
    Width = 75
    Height = 25
    Action = SetAction
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object Cancel: TButton
    Left = 400
    Top = 370
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object EtitMetadataActions: TActionList
    Left = 8
    Top = 368
    object SetAction: TAction
      Caption = 'Set'
      OnExecute = SetActionExecute
      OnUpdate = SetActionUpdate
    end
  end
end
