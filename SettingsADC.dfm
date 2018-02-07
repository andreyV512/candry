object FormSettADC: TFormSettADC
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
  ClientHeight = 412
  ClientWidth = 920
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Times New Roman'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 19
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 920
    Height = 365
    ActivePage = TabSheet3
    Align = alTop
    MultiLine = True
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1088#1072#1089#1095#1105#1090#1072
      DoubleBuffered = False
      ParentDoubleBuffered = False
      object gbSensors: TGroupBox
        Left = 11
        Top = 36
        Width = 286
        Height = 265
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1072#1085#1072#1083#1086#1074
        TabOrder = 0
        object lParameterA: TLabel
          Left = 88
          Top = 25
          Width = 80
          Height = 19
          Caption = #1055#1072#1088#1072#1084#1077#1090#1088' '#1040
        end
        object lParameterB: TLabel
          Left = 184
          Top = 25
          Width = 79
          Height = 19
          Caption = #1055#1072#1088#1072#1084#1077#1090#1088' '#1042
        end
      end
      object pcMathLogics: TPageControl
        Left = 303
        Top = 13
        Width = 598
        Height = 288
        ActivePage = tabSheetGAK
        TabOrder = 1
        object tabSheetGAK: TTabSheet
          Caption = #1051#1086#1075#1080#1082#1072' 1'
          object lInaccuracy: TLabel
            Left = 79
            Top = 16
            Width = 166
            Height = 19
            Caption = #1055#1086#1075#1088#1077#1096#1085#1086#1089#1090#1100' '#1074' '#1086#1090#1089#1095#1077#1090#1072#1093':'
          end
          object lPeaksCount: TLabel
            Left = 116
            Top = 58
            Width = 127
            Height = 19
            Caption = #1063#1080#1089#1083#1086' '#1087#1080#1082#1086#1074' '#1040#1050#1060':'
          end
          object lMaxToMin: TLabel
            Left = 96
            Top = 96
            Width = 149
            Height = 19
            Caption = #1054#1090#1085#1086#1096#1077#1085#1080#1077' '#1084#1072#1082#1089'/'#1084#1080#1085':'
          end
          object cbSmartAnalysis: TCheckBox
            Left = 58
            Top = 131
            Width = 187
            Height = 17
            Alignment = taLeftJustify
            Caption = #1056#1072#1089#1096#1080#1088#1077#1085#1085#1099#1081' '#1072#1085#1072#1083#1080#1079':'
            TabOrder = 0
          end
          object tbSmartBegin: TTrackBar
            Left = 58
            Top = 159
            Width = 187
            Height = 45
            Max = 40
            Min = -40
            PositionToolTip = ptBottom
            TabOrder = 1
          end
          object tbSmartEnd: TTrackBar
            Left = 58
            Top = 197
            Width = 187
            Height = 45
            Max = 40
            Min = -40
            PositionToolTip = ptBottom
            TabOrder = 2
          end
          object eMaxToMin: TEdit
            Left = 270
            Top = 93
            Width = 79
            Height = 27
            TabOrder = 3
          end
          object cbPeaksCount: TComboBox
            Tag = 7
            Left = 270
            Top = 54
            Width = 79
            Height = 23
            Style = csDropDownList
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Times New Roman'
            Font.Style = []
            ItemIndex = 0
            ParentFont = False
            TabOrder = 4
            Text = '1'
            Items.Strings = (
              '1'
              '2'
              '3')
          end
          object cbInaccuracy: TComboBox
            Tag = 7
            Left = 270
            Top = 16
            Width = 79
            Height = 23
            Style = csDropDownList
            Font.Charset = RUSSIAN_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Times New Roman'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            Items.Strings = (
              '1'
              '2'
              '3'
              '4'
              '5'
              '6'
              '7'
              '8'
              '9'
              '10')
          end
        end
        object tabSheetEKE: TTabSheet
          Caption = #1051#1086#1075#1080#1082#1072' 2'
          ImageIndex = 1
          object lSensorsMaxToMin: TLabel
            Left = 198
            Top = 18
            Width = 75
            Height = 19
            Caption = #1052#1072#1082#1089' / '#1084#1080#1085
          end
          object lMaxEnergy: TLabel
            Left = 100
            Top = 18
            Width = 92
            Height = 19
            Caption = #1052#1072#1082#1089' '#1101#1085#1077#1088#1075#1080#1103
          end
          object lMinEnergy: TLabel
            Left = 5
            Top = 18
            Width = 89
            Height = 19
            Caption = #1052#1080#1085' '#1101#1085#1077#1088#1075#1080#1103
          end
          object lEnergyToMax: TLabel
            Left = 279
            Top = 18
            Width = 96
            Height = 19
            Caption = #1052#1072#1082#1089'/'#1069#1085#1077#1088#1075#1080#1103
          end
          object lMaxPeaks: TLabel
            Left = 381
            Top = 18
            Width = 64
            Height = 19
            Caption = #1052#1072#1082#1089' '#1087#1080#1082
          end
          object lDJProcent: TLabel
            Left = 451
            Top = 18
            Width = 34
            Height = 19
            Caption = 'DJ %'
          end
          object lDJIndex: TLabel
            Left = 517
            Top = 18
            Width = 70
            Height = 19
            Caption = 'DJ '#1048#1085#1076#1077#1082#1089
          end
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1040#1062#1055
      ImageIndex = 2
      ParentShowHint = False
      ShowHint = True
      object LSizeBuf: TLabel
        Left = 16
        Top = 123
        Width = 102
        Height = 19
        Caption = #1059#1089#1080#1083#1077#1085#1080#1077' '#1040#1062#1055
      end
      object LRate: TLabel
        Left = 16
        Top = 168
        Width = 134
        Height = 19
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1040#1062#1055' , '#1052#1043#1094
        WordWrap = True
      end
      object lSynchroCaption: TLabel
        Left = 397
        Top = 66
        Width = 101
        Height = 38
        Caption = #1059#1088#1086#1074#1077#1085#1100' '#1089#1080#1085#1093#1088#1086#1089#1080#1075#1085#1072#1083#1072
        WordWrap = True
      end
      object lSynchroLevel: TLabel
        Left = 698
        Top = 66
        Width = 47
        Height = 19
        AutoSize = False
        Caption = 'q'
      end
      object lStartDelay: TLabel
        Left = 397
        Top = 171
        Width = 110
        Height = 19
        Caption = #1047#1072#1076#1077#1088#1078#1082#1072' '#1089#1090#1072#1088#1090#1072
        WordWrap = True
      end
      object lMeasuresPacket: TLabel
        Left = 16
        Top = 71
        Width = 129
        Height = 19
        Caption = #1050#1086#1083'-'#1074#1086' '#1080#1079#1084#1077#1088#1077#1085#1080#1081':'
      end
      object lSensorsCount: TLabel
        Left = 16
        Top = 27
        Width = 116
        Height = 19
        Caption = #1050#1086#1083'-'#1074#1086' '#1076#1072#1090#1095#1080#1082#1086#1074':'
      end
      object lStrobesPerPacket: TLabel
        Left = 397
        Top = 123
        Width = 118
        Height = 19
        Caption = #1057#1090#1088#1086#1073#1086#1074' '#1074' '#1087#1072#1082#1077#1090#1077
        WordWrap = True
      end
      object lChannelAmperage: TLabel
        Left = 16
        Top = 215
        Width = 100
        Height = 19
        Caption = #1058#1086#1082#1086#1074#1099#1081' '#1082#1072#1085#1072#1083
      end
      object lVoltage: TLabel
        Left = 16
        Top = 255
        Width = 85
        Height = 19
        Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077
      end
      object lThresholdSolenoid: TLabel
        Left = 16
        Top = 280
        Width = 174
        Height = 36
        AutoSize = False
        Caption = #1041#1083#1086#1082#1080#1088#1086#1074#1082#1072' '#1087#1088#1080' '#1089#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1080' '#1073#1086#1083#1077#1077
        WordWrap = True
      end
      object lACFDivider: TLabel
        Left = 397
        Top = 223
        Width = 102
        Height = 19
        Caption = #1044#1077#1083#1080#1090#1077#1083#1100' '#1040#1050#1060
      end
      object cbGain: TComboBox
        Tag = 7
        Left = 196
        Top = 121
        Width = 79
        Height = 23
        Style = csDropDownList
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        ItemIndex = 5
        ParentFont = False
        TabOrder = 2
        Text = '40'
        OnChange = cbSensorsNumberChange
        Items.Strings = (
          '1'
          '2'
          '5'
          '10'
          '20'
          '40'
          '100'
          '200')
      end
      object cbFrequency: TComboBox
        Tag = 7
        Left = 196
        Top = 168
        Width = 79
        Height = 23
        Style = csDropDownList
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        ItemIndex = 2
        ParentFont = False
        TabOrder = 3
        Text = '25'
        Items.Strings = (
          '6,25'
          '12,5'
          '25'
          '50'
          '100')
      end
      object tbSynchroLevel: TTrackBar
        Left = 556
        Top = 71
        Width = 115
        Height = 33
        Max = 128
        Min = -128
        Position = 30
        PositionToolTip = ptRight
        TabOrder = 5
        TickStyle = tsNone
        OnChange = tbSynchroLevelChange
      end
      object eStartDelay: TEdit
        Left = 585
        Top = 168
        Width = 79
        Height = 27
        NumbersOnly = True
        TabOrder = 6
      end
      object cbSynchroFront: TCheckBox
        Left = 397
        Top = 29
        Width = 209
        Height = 17
        Alignment = taLeftJustify
        Caption = #1057#1080#1085#1093#1088#1086#1085#1080#1079#1072#1094#1080#1103' '#1087#1086' '#1092#1088#1086#1085#1090#1091
        TabOrder = 4
      end
      object cbSamplesCount: TComboBox
        Tag = 7
        Left = 196
        Top = 71
        Width = 79
        Height = 23
        Style = csDropDownList
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Items.Strings = (
          '218'
          '474'
          '986'
          '2010')
      end
      object cbSensorsNumber: TComboBox
        Tag = 7
        Left = 196
        Top = 27
        Width = 79
        Height = 23
        Style = csDropDownList
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        ItemIndex = 2
        ParentFont = False
        TabOrder = 0
        Text = '3'
        OnChange = cbSensorsNumberChange
        Items.Strings = (
          '1'
          '2'
          '3'
          '6')
      end
      object eStrobesPerPacket: TEdit
        Left = 585
        Top = 115
        Width = 79
        Height = 27
        NumbersOnly = True
        TabOrder = 7
      end
      object csAmperage: TCSpinEdit
        Left = 196
        Top = 210
        Width = 79
        Height = 24
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 8
      end
      object csVoltage: TCSpinEdit
        Left = 196
        Top = 253
        Width = 79
        Height = 24
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 9
      end
      object eResistSolenoid: TEdit
        Left = 196
        Top = 292
        Width = 79
        Height = 23
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
        OnKeyPress = eResistSolenoidKeyPress
      end
      object cbACFDivider: TComboBox
        Tag = 7
        Left = 585
        Top = 223
        Width = 79
        Height = 23
        Style = csDropDownList
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Times New Roman'
        Font.Style = []
        ItemIndex = 0
        ParentFont = False
        TabOrder = 11
        Text = '1'
        OnChange = cbSensorsNumberChange
        Items.Strings = (
          '1'
          '2'
          '4'
          '8')
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1057#1080#1089#1090#1077#1084#1085#1099#1077
      ImageIndex = 2
      object lTestTime: TLabel
        Left = 11
        Top = 11
        Width = 180
        Height = 19
        Caption = #1058#1077#1089#1090#1086#1074#1099#1081' '#1089#1073#1086#1088' '#1076#1072#1085#1085#1099#1093', '#1084#1089
      end
      object eTestTime: TEdit
        Left = 232
        Top = 8
        Width = 89
        Height = 27
        NumbersOnly = True
        TabOrder = 0
      end
    end
  end
  object bExit: TButton
    Left = 529
    Top = 371
    Width = 150
    Height = 33
    Caption = #1042#1099#1081#1090#1080
    TabOrder = 3
    OnClick = bExitClick
  end
  object bSave: TButton
    Left = 355
    Top = 371
    Width = 150
    Height = 33
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 2
    OnClick = bSaveClick
  end
  object bRecalculateTube: TButton
    Left = 185
    Top = 371
    Width = 150
    Height = 33
    Caption = #1055#1077#1088#1077#1089#1095#1080#1090#1072#1090#1100' '#1090#1088#1091#1073#1091
    TabOrder = 1
    OnClick = bRecalculateTubeClick
  end
  object ApplicationEvents1: TApplicationEvents
    OnMessage = ApplicationEvents1Message
    Left = 760
  end
end
