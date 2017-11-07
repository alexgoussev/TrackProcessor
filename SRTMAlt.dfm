object STRMAltitude: TSTRMAltitude
  Left = 382
  Top = 241
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'SRTM Data Apply'
  ClientHeight = 129
  ClientWidth = 350
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
    Top = 80
    Width = 329
    Height = 13
    AutoSize = False
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 104
    Width = 329
    Height = 16
    TabOrder = 0
  end
  object m_Ok: TButton
    Left = 264
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    ModalResult = 1
    TabOrder = 1
    OnClick = m_OkClick
  end
  object m_Cancel: TButton
    Left = 264
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 249
    Height = 65
    BevelOuter = bvLowered
    TabOrder = 3
    object Download: TCheckBox
      Left = 8
      Top = 8
      Width = 201
      Height = 17
      Caption = 'Automatically download missing files'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object AltSmoothFactor: TLabeledEdit
      Left = 120
      Top = 36
      Width = 33
      Height = 21
      EditLabel.Width = 108
      EditLabel.Height = 13
      EditLabel.Caption = 'Altitude smooth factor: '
      LabelPosition = lpLeft
      TabOrder = 1
    end
  end
  object IdHTTP: TIdHTTP
    IOHandler = IdSSLIOHandlerSocketOpenSSL
    OnWork = HTTPGetSRTMWork
    OnWorkBegin = HTTPGetSRTMWorkBegin
    AllowCookies = True
    HandleRedirects = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.ContentRangeEnd = -1
    Request.ContentRangeStart = -1
    Request.ContentRangeInstanceLength = -1
    Request.Accept = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/5.0 (Windows NT 6.3; Trident/7.0; rv:11.0) like Gecko'
    Request.Ranges.Units = 'bytes'
    Request.Ranges = <>
    HTTPOptions = [hoForceEncodeParams]
    Left = 64
    Top = 64
  end
  object IdSSLIOHandlerSocketOpenSSL: TIdSSLIOHandlerSocketOpenSSL
    MaxLineAction = maException
    Port = 0
    DefaultPort = 0
    SSLOptions.Method = sslvTLSv1_2
    SSLOptions.SSLVersions = [sslvTLSv1_2]
    SSLOptions.Mode = sslmUnassigned
    SSLOptions.VerifyMode = []
    SSLOptions.VerifyDepth = 0
    Left = 184
    Top = 64
  end
end
