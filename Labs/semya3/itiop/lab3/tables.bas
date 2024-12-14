Attribute VB_Name = "NewMacros1"
Sub �������()
 Dim tableName As String
    Dim numRows As Integer
    Dim numCols As Integer
    Dim tableNumber As Integer
    Dim doc As Document
    Dim rng As Range
    Dim tbl As table

    ' �������� �������� ��������
    Set doc = ActiveDocument

    ' ��������� � ������������ �������� �������, ���������� ����� � ��������
    tableName = InputBox("������� �������� �������:", "�������� �������")
    numRows = InputBox("������� ���������� �����:", "���������� �����")
    numCols = InputBox("������� ���������� ��������:", "���������� ��������")

    ' ���������� ����� �������
    tableNumber = doc.Tables.Count + 1

    ' �������� ��������� �������
    Set rng = Selection.Range
    rng.InsertBefore "������� " & tableNumber & ". " & tableName & vbCr
    rng.Collapse Direction:=wdCollapseEnd

    ' �������� �������
    Set tbl = doc.Tables.Add(Range:=rng, numRows:=numRows, NumColumns:=numCols)

    ' ���������� ���� ������
    tbl.Borders.InsideLineStyle = wdLineStyleSingle
    tbl.Borders.OutsideLineStyle = wdLineStyleSingle

    ' �������������� ��������� ������ ��������
    tbl.AutoFitBehavior wdAutoFitWindow

    ' ��������� ������� �������
    Dim i As Integer
    For i = 1 To numRows
        tbl.Cell(i, 1).Range.Text = i
    Next i

    ' ����������� ������ �� �������
    tbl.Range.Collapse Direction:=wdCollapseEnd
    tbl.Range.Select

End Sub

Sub UpdateTableNumbering(tbl As table)
    Dim i As Integer
    Dim cellRange As Range

    ' ������������� ������ � ������ �������
    For i = 1 To tbl.Rows.Count
        Set cellRange = tbl.Cell(i, 1).Range
        cellRange.Text = i
    Next i
End Sub

Sub UpdateAllTablesNumbering()
    Dim tbl As table

    ' �������� ��������� �� ���� �������� ���������
    For Each tbl In ActiveDocument.Tables
        Call UpdateTableNumbering(tbl)
    Next tbl
End Sub

