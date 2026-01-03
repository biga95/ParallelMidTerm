@echo off
setlocal

REM cartella risultati
if not exist results mkdir results

REM input
set SEQ=results\seq_times.txt
set PAR=results\par_times.txt

REM output
set OUTXLSX=results\speedup_report.xlsx
set OUTPNG=results\speedup_report.png


python make_excel.py %SEQ% %PAR% %OUTXLSX% %OUTPNG%

echo.
echo Creati
echo   %OUTXLSX%
echo   %OUTPNG%
pause
