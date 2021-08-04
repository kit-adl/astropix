set now [ clock seconds ]
set nowdate [ clock format $now -format "%y%m%d_%H%M%S" ]
set newfile [ concat "main_top_$nowdate.bit" ]
file copy main_top.bit $newfile

