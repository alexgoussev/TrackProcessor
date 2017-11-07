object Profile: TProfile
  Left = 210
  Top = 0
  Caption = 'Profiles'
  ClientHeight = 562
  ClientWidth = 784
  Color = clBtnFace
  Constraints.MinHeight = 480
  Constraints.MinWidth = 640
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PopupMenu = PopupMenu
  Position = poMainFormCenter
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object AltPanel: TPanel
    Left = 0
    Top = 0
    Width = 784
    Height = 286
    Align = alTop
    BevelOuter = bvLowered
    TabOrder = 0
    object AltImage: TPaintBox
      Left = 1
      Top = 1
      Width = 782
      Height = 284
      Align = alClient
      OnPaint = AltImagePaint
      ExplicitWidth = 790
    end
  end
  object SpdPanel: TPanel
    Left = 0
    Top = 286
    Width = 784
    Height = 286
    Align = alTop
    BevelOuter = bvLowered
    TabOrder = 1
    object SpdImage: TPaintBox
      Left = 1
      Top = 1
      Width = 782
      Height = 284
      Align = alClient
      OnPaint = SpdImagePaint
      ExplicitWidth = 790
    end
  end
  object PopupMenu: TPopupMenu
    Left = 752
    Top = 528
    object SaveAltitudeProfile1: TMenuItem
      Action = SaveAltAction
    end
    object SaveSpeedProfile1: TMenuItem
      Action = SaveSpdAction
    end
    object SaveBothProfiles1: TMenuItem
      Action = SaveBothAction
    end
  end
  object ActionList: TActionList
    Left = 720
    Top = 528
    object SaveAltAction: TAction
      Caption = 'Save Altitude Profile'
      OnExecute = SaveAltActionExecute
      OnUpdate = SaveAltActionUpdate
    end
    object SaveSpdAction: TAction
      Caption = 'Save Speed Profile'
      OnExecute = SaveSpdActionExecute
      OnUpdate = SaveSpdActionUpdate
    end
    object SaveBothAction: TAction
      Caption = 'Save Both Profiles'
      OnExecute = SaveBothActionExecute
      OnUpdate = SaveBothActionUpdate
    end
  end
  object SaveProfile: TSavePictureDialog
    DefaultExt = 'gif'
    Filter = 'GIF Image File|*.gif'
    FilterIndex = 0
    Title = 'Save Profile Image'
    Left = 688
    Top = 528
  end
end
