Attribute VB_Name = "NewMacros"
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
    tableNumber = doc.tables.Count + 1

    ' �������� ��������� �������
    Set rng = Selection.Range
    rng.InsertBefore "������� " & tableNumber & ". " & tableName & vbCr
    rng.Collapse Direction:=wdCollapseEnd

    ' �������� �������
    Set tbl = doc.tables.Add(Range:=rng, numRows:=numRows, NumColumns:=numCols)

    ' ���������� ���� ������
    tbl.Borders.InsideLineStyle = wdLineStyleSingle
    tbl.Borders.OutsideLineStyle = wdLineStyleSingle

    ' �������������� ��������� ������ ��������
    tbl.AutoFitBehavior wdAutoFitWindow

    ' ��������� ������� �������
    Dim i As Integer
    For i = 1 To numRows
        tbl.cell(i, 1).Range.Text = i
    Next i

    ' ����������� ������ �� �������
    tbl.Range.Collapse Direction:=wdCollapseEnd
    tbl.Range.Select

End Sub
