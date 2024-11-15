Attribute VB_Name = "Module1"
Sub ReplaceRussianVowels()

  Dim strText As String
  Dim i As Long
  Dim strVowels As String
  Dim strReplacement As String

  ' ������ ������� �������
  strVowels = "����������Ũ�������"
  strReplacement = "@"

  ' �������� ���� ����� ���������
  strText = ActiveDocument.Content.Text

  ' ���� ��� ������ ���� �������
  For i = 1 To Len(strText)
    If InStr(1, strVowels, Mid(strText, i, 1)) > 0 Then
      strText = Left(strText, i - 1) & strReplacement & Mid(strText, i + 1)
    End If
  Next i

  ' ���������� ���������� ����� � ��������
  ActiveDocument.Content.Text = strText

End Sub
