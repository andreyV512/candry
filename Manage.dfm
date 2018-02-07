object ManageForm: TManageForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1059#1087#1088#1072#1074#1083#1077#1085#1080#1077' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072#1084#1080
  ClientHeight = 288
  ClientWidth = 590
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object gbThickness: TGroupBox
    Left = 14
    Top = 30
    Width = 563
    Height = 195
    Caption = #1052#1086#1076#1091#1083#1100' '#1090#1086#1083#1097#1080#1085#1099' '#1089#1090#1077#1085#1082#1080
    TabOrder = 0
    object Label4: TLabel
      Left = 160
      Top = 40
      Width = 72
      Height = 13
      Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1074' '#1043#1094
    end
    object bRotation: TBitBtn
      Left = 41
      Top = 35
      Width = 113
      Height = 49
      Caption = #1042#1088#1072#1097#1072#1090#1100
      DoubleBuffered = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentDoubleBuffered = False
      ParentFont = False
      TabOrder = 0
      OnClick = bRotationClick
      OnKeyPress = FormKeyPress
    end
    object bPowerSU: TCheckBox
      Left = 284
      Top = 28
      Width = 87
      Height = 17
      Caption = #1057#1086#1083#1077#1085#1086#1080#1076
      TabOrder = 1
      OnClick = bPowerSUClick
      OnKeyPress = FormKeyPress
    end
    object pSolenoid: TPanel
      Left = 361
      Top = 18
      Width = 185
      Height = 111
      BevelInner = bvRaised
      TabOrder = 2
      object Label1: TLabel
        Left = 16
        Top = 13
        Width = 52
        Height = 13
        Caption = #1057#1080#1083#1072' '#1090#1086#1082#1072
      end
      object Label2: TLabel
        Left = 16
        Top = 34
        Width = 84
        Height = 13
        Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077', ('#1042')'
      end
      object Label3: TLabel
        Left = 16
        Top = 57
        Width = 105
        Height = 13
        Caption = #1057#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077'('#1054#1084'.)'
      end
      object Label5: TLabel
        Left = 15
        Top = 82
        Width = 83
        Height = 13
        Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' ('#1057')'
      end
      object eAmperage: TEdit
        Left = 122
        Top = 8
        Width = 55
        Height = 21
        TabOrder = 0
      end
      object eVoltage: TEdit
        Left = 122
        Top = 32
        Width = 55
        Height = 21
        TabOrder = 1
      end
      object eTemperature: TEdit
        Left = 122
        Top = 81
        Width = 55
        Height = 21
        TabOrder = 2
      end
      object eResist: TEdit
        Left = 123
        Top = 57
        Width = 55
        Height = 21
        TabOrder = 3
      end
    end
    object eFriquencyRot: TEdit
      Left = 160
      Top = 66
      Width = 107
      Height = 21
      NumbersOnly = True
      TabOrder = 3
    end
  end
  object StatusBarBottom: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 254
    Width = 584
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Width = 600
      end
      item
        Width = 400
      end>
    ParentColor = True
    UseSystemFont = False
  end
  object InputTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = InputTimerTimer
    Left = 160
    Top = 8
  end
end
