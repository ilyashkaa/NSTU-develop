Attribute VB_Name = "NewMacros1"
Sub таблица()
 Dim tableName As String
    Dim numRows As Integer
    Dim numCols As Integer
    Dim tableNumber As Integer
    Dim doc As Document
    Dim rng As Range
    Dim tbl As table

    ' Получить активный документ
    Set doc = ActiveDocument

    ' Запросить у пользователя название таблицы, количество строк и столбцов
    tableName = InputBox("Введите название таблицы:", "Название таблицы")
    numRows = InputBox("Введите количество строк:", "Количество строк")
    numCols = InputBox("Введите количество столбцов:", "Количество столбцов")

    ' Определить номер таблицы
    tableNumber = doc.Tables.Count + 1

    ' Вставить заголовок таблицы
    Set rng = Selection.Range
    rng.InsertBefore "Таблица " & tableNumber & ". " & tableName & vbCr
    rng.Collapse Direction:=wdCollapseEnd

    ' Вставить таблицу
    Set tbl = doc.Tables.Add(Range:=rng, numRows:=numRows, NumColumns:=numCols)

    ' Установить цвет границ
    tbl.Borders.InsideLineStyle = wdLineStyleSingle
    tbl.Borders.OutsideLineStyle = wdLineStyleSingle

    ' Автоматическая настройка ширины столбцов
    tbl.AutoFitBehavior wdAutoFitWindow

    ' Нумерация первого столбца
    Dim i As Integer
    For i = 1 To numRows
        tbl.Cell(i, 1).Range.Text = i
    Next i

    ' Переместить курсор за таблицу
    tbl.Range.Collapse Direction:=wdCollapseEnd
    tbl.Range.Select

End Sub

Sub UpdateTableNumbering(tbl As table)
    Dim i As Integer
    Dim cellRange As Range

    ' Пронумеровать строки в первом столбце
    For i = 1 To tbl.Rows.Count
        Set cellRange = tbl.Cell(i, 1).Range
        cellRange.Text = i
    Next i
End Sub

Sub UpdateAllTablesNumbering()
    Dim tbl As table

    ' Обновить нумерацию во всех таблицах документа
    For Each tbl In ActiveDocument.Tables
        Call UpdateTableNumbering(tbl)
    Next tbl
End Sub

