object ViewTubeDetailsForm: TViewTubeDetailsForm
  Left = 0
  Top = 0
  Caption = #1044#1077#1090#1072#1083#1080#1088#1086#1074#1072#1085#1085#1099#1081' '#1087#1088#1086#1089#1084#1086#1090#1088' '#1090#1088#1091#1073#1099
  ClientHeight = 688
  ClientWidth = 1200
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
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 185
    Width = 1200
    Height = 16
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 1
  end
  object Splitter2: TSplitter
    Left = 489
    Top = 201
    Width = 20
    Height = 487
    Align = alRight
    ExplicitLeft = 593
    ExplicitTop = 190
  end
  object MeasureChart: TChart
    Left = 0
    Top = 0
    Width = 1200
    Height = 185
    AllowPanning = pmNone
    Legend.Visible = False
    MarginLeft = 1
    MarginRight = 1
    Title.Text.Strings = (
      #1048#1079#1084#1077#1088#1077#1085#1080#1103' '#1087#1086' '#1079#1086#1085#1077)
    View3D = False
    Zoom.Allow = False
    Align = alTop
    TabOrder = 0
    TabStop = True
    OnClick = MeasureChartClick
    OnKeyDown = MeasureChartKeyDown
    OnMouseWheelDown = MeasureChartMouseWheelDown
    OnMouseWheelUp = MeasureChartMouseWheelUp
    ColorPaletteIndex = 13
    object SBSF: TSpeedButton
      Left = 135
      Top = 6
      Width = 30
      Height = 22
      Caption = #1057#1060
      OnClick = SBSFClick
    end
    object sbMeasure: TStatusBar
      Left = 1
      Top = 165
      Width = 1198
      Height = 19
      Panels = <
        item
          Width = 150
        end
        item
          Width = 150
        end
        item
          Width = 150
        end
        item
          Width = 150
        end
        item
          Width = 150
        end>
    end
    object cbMedianFilter: TCheckBox
      Left = 8
      Top = 6
      Width = 129
      Height = 17
      Caption = #1052#1077#1076#1080#1072#1085#1085#1099#1081' '#1092#1080#1083#1100#1090#1088
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = cbMedianFilterClick
      OnKeyDown = FormKeyDown
    end
    object Series1: TBarSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = True
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
  end
  object SourceSignalChart: TChart
    Left = 0
    Top = 201
    Width = 489
    Height = 487
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 20
    AllowPanning = pmNone
    BackWall.Color = clWhite
    Title.Text.Strings = (
      #1055#1077#1088#1074#1080#1095#1085#1099#1081' '#1089#1080#1075#1085#1072#1083)
    OnClickBackground = SourceSignalChartClickBackground
    View3D = False
    Align = alClient
    TabOrder = 1
    OnKeyDown = SourceSignalChartKeyDown
    OnMouseWheelDown = SourceSignalChartMouseWheelDown
    OnMouseWheelUp = SourceSignalChartMouseWheelUp
    ColorPaletteIndex = 13
    object sbSource: TStatusBar
      Left = 1
      Top = 467
      Width = 487
      Height = 19
      Panels = <
        item
          Width = 90
        end
        item
          Width = 90
        end
        item
          Width = 110
        end
        item
          Width = 110
        end
        item
          Width = 50
        end>
    end
  end
  object ACFChart: TChart
    Left = 509
    Top = 201
    Width = 571
    Height = 487
    AllowPanning = pmNone
    BackImage.Inside = True
    Title.Text.Strings = (
      #1054#1075#1080#1073#1072#1102#1097#1072#1103' '#1072#1074#1090#1086#1082#1086#1088#1088#1077#1083#1103#1094#1080#1080)
    OnClickBackground = ACFChartClickBackground
    OnClickSeries = ACFChartClickSeries
    SeriesGroups = <
      item
        Name = 'Group1'
      end>
    View3D = False
    Align = alRight
    PopupMenu = popupACF
    TabOrder = 2
    TabStop = True
    OnKeyDown = ACFChartKeyDown
    OnMouseWheelDown = ACFChartMouseWheelDown
    OnMouseWheelUp = ACFChartMouseWheelUp
    ColorPaletteIndex = 13
    object sbACF: TStatusBar
      Left = 1
      Top = 467
      Width = 569
      Height = 19
      Panels = <
        item
          Width = 150
        end
        item
          Width = 150
        end
        item
          Width = 150
        end>
    end
    object bShowRePeaks: TBitBtn
      Left = 493
      Top = 6
      Width = 40
      Height = 27
      DoubleBuffered = True
      Kind = bkRetry
      NumGlyphs = 2
      ParentDoubleBuffered = False
      TabOrder = 1
      OnClick = bShowRePeaksClick
      OnKeyDown = FormKeyDown
    end
  end
  object rePeaks: TRichEdit
    Left = 1080
    Top = 201
    Width = 120
    Height = 487
    Align = alRight
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnKeyDown = FormKeyDown
  end
  object popupACF: TPopupMenu
    Left = 576
    Top = 216
    object pmDrawMinMaxThickness: TMenuItem
      Caption = #1056#1080#1089#1086#1074#1072#1090#1100' '#1075#1088#1072#1085#1080#1094#1099' '#1087#1086#1080#1089#1082#1072
      OnClick = pmDrawMinMaxThicknessClick
    end
  end
end
