for %%f in (.\test_pictures\*.bmp) do .\bmp2c\bmp2c.exe "copy_test.ini" "%%f" "out\%%f.c"
pause


