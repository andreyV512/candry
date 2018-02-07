object SignalsStateForm: TSignalsStateForm
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  Caption = #1057#1048#1043#1053#1040#1051#1067' '#1054#1041#1052#1045#1053#1040' '#1057' '#1064#1059' '#1058#1057
  ClientHeight = 427
  ClientWidth = 323
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object InOutTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = InOutTimerTimer
    Left = 24
    Top = 24
  end
end
