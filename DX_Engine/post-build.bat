set ProjectDir=%1

rmdir "%ProjectDir%\Include\"
xcopy "%ProjectDir%\Code\*.h" "%ProjectDir%\Include\" /exclude:exclude.txt /sy
