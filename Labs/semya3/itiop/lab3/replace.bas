Attribute VB_Name = "Module1"
Sub ReplaceRussianVowels()

  Dim strText As String
  Dim i As Long
  Dim strVowels As String
  Dim strReplacement As String

  ' —писок русских гласных
  strVowels = "аеЄиоуыэю€ј≈®»ќ”џЁёя"
  strReplacement = "@"

  ' ѕолучаем весь текст документа
  strText = ActiveDocument.Content.Text

  ' ÷икл дл€ замены всех гласных
  For i = 1 To Len(strText)
    If InStr(1, strVowels, Mid(strText, i, 1)) > 0 Then
      strText = Left(strText, i - 1) & strReplacement & Mid(strText, i + 1)
    End If
  Next i

  ' «аписываем измененный текст в документ
  ActiveDocument.Content.Text = strText

End Sub
