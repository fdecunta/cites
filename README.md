# CITES(1)

## NAME
**cites** — extract parenthetical citations from plain text

## SYNOPSIS
**cites** \[**-n**\] \[*file*\]

## DESCRIPTION
**cites** scans a plain text file for parenthetical citations, such as `(Smith 2020)`, and prints each one found.

If *file* is omitted, **cites** reads from standard input.

## OPTIONS
**-n**
&nbsp;&nbsp;&nbsp;&nbsp;Prefix each match with the line number where it starts.

## EXAMPLES
Scan a text file:
```
cites paper.txt
```

Convert a Word document with pandoc and scan the result, showing line numbers:
```
pandoc -t plain paper.docx | cites -n
```

## EXIT STATUS
**0** on success, **1** if the file cannot be opened or arguments are invalid.

## AUTHOR
Written for personal citation-checking workflows.
# cites
