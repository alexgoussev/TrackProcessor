object CellEqualizer: TCellEqualizer
  Left = 351
  Top = 187
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Cell Equalizer'
  ClientHeight = 250
  ClientWidth = 392
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  DesignSize = (
    392
    250)
  PixelsPerInch = 96
  TextHeight = 13
  object m_pProgressBar: TProgressBar
    Left = 8
    Top = 230
    Width = 377
    Height = 16
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 283
    Height = 207
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelOuter = bvLowered
    TabOrder = 1
    DesignSize = (
      283
      207)
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 58
      Height = 13
      Caption = 'Cell size (m):'
    end
    object Label2: TLabel
      Left = 8
      Top = 48
      Width = 90
      Height = 13
      Caption = 'Averaging Method:'
    end
    object Label3: TLabel
      Left = 26
      Top = 100
      Width = 181
      Height = 13
      Caption = 'Maximum Course Diifference, degrees:'
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 126
      Width = 267
      Height = 73
      Anchors = [akLeft, akRight, akBottom]
      Caption = ' Apply Filter to: '
      TabOrder = 0
      object m_pCoordinates: TCheckBox
        Left = 8
        Top = 24
        Width = 97
        Height = 17
        Caption = 'Coordinates'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object m_pAltitude: TCheckBox
        Left = 8
        Top = 48
        Width = 97
        Height = 17
        Caption = 'Altitude'
        TabOrder = 1
      end
    end
    object m_pCellSize: TEdit
      Left = 212
      Top = 14
      Width = 57
      Height = 21
      Anchors = [akTop, akRight]
      TabOrder = 1
      Text = '15'
      OnKeyPress = CellSizeKeyPress
    end
    object m_pKType: TComboBox
      Left = 132
      Top = 46
      Width = 137
      Height = 21
      Style = csDropDownList
      Anchors = [akTop, akRight]
      ItemIndex = 0
      TabOrder = 2
      Text = 'Simple (Nonweighted)'
      Items.Strings = (
        'Simple (Nonweighted)'
        'Bi-Square Weights'
        'Tri-Cube Weights'
        'Gauss Weights')
    end
    object m_pMovDir: TCheckBox
      Left = 8
      Top = 80
      Width = 257
      Height = 17
      Caption = 'Consider Movement Direction'
      Checked = True
      State = cbChecked
      TabOrder = 3
      OnClick = MovDirClick
    end
    object m_pMaxCourseDiff: TEdit
      Left = 236
      Top = 96
      Width = 33
      Height = 21
      Anchors = [akTop, akRight]
      TabOrder = 4
      Text = '60'
      OnKeyPress = MaxCourseDiffKeyPress
    end
  end
  object Button1: TButton
    Left = 306
    Top = 8
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Start'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = GoClick
  end
  object Button2: TButton
    Left = 306
    Top = 40
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 3
  end
end
