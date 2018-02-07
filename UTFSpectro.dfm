object FSpectro: TFSpectro
  Left = 0
  Top = 0
  Caption = #1057#1087#1077#1082#1090#1088#1072#1083#1100#1085#1099#1081' '#1092#1080#1083#1100#1090#1088
  ClientHeight = 576
  ClientWidth = 643
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnMouseWheel = FormMouseWheel
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Ch: TChart
    Left = 5
    Top = 15
    Width = 512
    Height = 264
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 20
    AllowPanning = pmNone
    BackWall.Color = clWhite
    Legend.Visible = False
    Title.Shadow.Visible = False
    Title.Text.Strings = (
      #1057#1087#1077#1082#1090#1088' ('#1052#1099#1096#1100' - '#1082#1085#1086#1087#1082#1080', '#1082#1086#1083#1077#1089#1086', '#1085#1072#1078#1072#1090#1086#1077' '#1082#1086#1083#1077#1089#1086')')
    Title.VertMargin = 0
    OnClickBackground = ChClickBackground
    SeriesGroups = <
      item
        Name = 'Group1'
      end>
    View3D = False
    TabOrder = 0
    ColorPaletteIndex = 13
    object SSpectro: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      OnClick = SSpectroClick
      LinePen.Color = 10708548
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object SBorders: TArrowSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Frame.Visible = False
      Marks.Transparent = True
      Marks.Visible = False
      ClickableLine = False
      Pointer.Brush.Gradient.EndColor = 3513587
      Pointer.Gradient.EndColor = 3513587
      Pointer.HorizSize = 1
      Pointer.InflateMargins = False
      Pointer.Pen.Width = 2
      Pointer.Style = psRectangle
      Pointer.VertSize = 1
      Pointer.Visible = True
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      EndXValues.Name = 'EndX'
      EndXValues.Order = loNone
      EndYValues.Name = 'EndY'
      EndYValues.Order = loNone
      StartXValues.Name = 'X'
      StartXValues.Order = loAscending
      StartYValues.Name = 'Y'
      StartYValues.Order = loNone
    end
  end
  object ChAcf: TChart
    Left = 5
    Top = 307
    Width = 512
    Height = 264
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 20
    AllowPanning = pmNone
    BackWall.Color = clWhite
    Legend.Visible = False
    Title.Text.Strings = (
      #1050#1086#1088#1088#1077#1083#1103#1094#1080#1103)
    Title.VertMargin = 0
    SeriesGroups = <
      item
        Name = 'Group1'
      end>
    View3D = False
    TabOrder = 1
    ColorPaletteIndex = 13
    object SAcf: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      OnClick = SSpectroClick
      LinePen.Color = 10708548
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object STSpectro: TStaticText
    Left = 8
    Top = 284
    Width = 589
    Height = 17
    AutoSize = False
    Caption = 'STSpectro'
    TabOrder = 2
  end
end
