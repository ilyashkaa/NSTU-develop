Attribute VB_Name = "NewMacros"
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
    tableNumber = doc.tables.Count + 1

    ' Вставить заголовок таблицы
    Set rng = Selection.Range
    rng.InsertBefore "Таблица " & tableNumber & ". " & tableName & vbCr
    rng.Collapse Direction:=wdCollapseEnd

    ' Вставить таблицу
    Set tbl = doc.tables.Add(Range:=rng, numRows:=numRows, NumColumns:=numCols)

    ' Установить цвет границ
    tbl.Borders.InsideLineStyle = wdLineStyleSingle
    tbl.Borders.OutsideLineStyle = wdLineStyleSingle

    ' Автоматическая настройка ширины столбцов
    tbl.AutoFitBehavior wdAutoFitWindow

    ' Нумерация первого столбца
    Dim i As Integer
    For i = 1 To numRows
        tbl.cell(i, 1).Range.Text = i
    Next i

    ' Переместить курсор за таблицу
    tbl.Range.Collapse Direction:=wdCollapseEnd
    tbl.Range.Select

End Sub
