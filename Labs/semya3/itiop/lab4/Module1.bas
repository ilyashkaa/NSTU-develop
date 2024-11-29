Attribute VB_Name = "Module11"
Sub CreateSurfacePlot()

  Dim x1 As Double, x2 As Double, y1 As Double, y2 As Double
  Dim nx As Long, ny As Long
  Dim chartTitle As String
  Dim xValues() As Double, yValues() As Double, zValues() As Double
  Dim i As Long, j As Long
  Dim cht As Chart
  Dim inputString As String
  Dim inputLines() As String

  ' Ввод параметров пользователем
    x1 = InputBox("Input x1 value:")
    x2 = InputBox("Input x2 value:")
    y1 = InputBox("Input y1 value:")
    y2 = InputBox("Input y2 value:")
    nx = InputBox("Input nx value:")
    ny = InputBox("Input ny value:")
    chartTitle = InputBox("Input Title value:")


  ' Проверка на корректность ввода
  
  If nx < 2 Or ny < 2 Then
    MsgBox "Количество шагов по x и y должно быть не меньше 2.", vbCritical
    Exit Sub
  End If
  If x1 >= x2 Or y1 >= y2 Then
    MsgBox "x1 < x2 ? y1 < y2", vbCritical
    Exit Sub
  End If

  ' Создание массивов значений
  ReDim xValues(1 To nx)
  ReDim yValues(1 To ny)
  ReDim zValues(1 To nx, 1 To ny)

  ' Заполнение массивов
  For i = 1 To nx
    xValues(i) = x1 + (x2 - x1) * (i - 1) / (nx - 1)
  Next i
  For j = 1 To ny
    yValues(j) = y1 + (y2 - y1) * (j - 1) / (ny - 1)
  Next j

  For i = 1 To nx
    For j = 1 To ny
      zValues(i, j) = xValues(i) * yValues(j)
    Next j
  Next i


  ' Создание таблицы
  Worksheets.Add
  Range("A1").Resize(nx, ny).Value = zValues
  Range("A1").CurrentRegion.Name = "Data"

  ' Создание диаграммы поверхности
  Set cht = Charts.Add
  With cht
    .ChartType = xlSurface
    .SetSourceData Source:=Range("Data")
    .HasLegend = False

    ' Добавление заголовка диаграммы
    With .chartTitle
        .Text = chartTitle
    End With

  End With

End Sub
