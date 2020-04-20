set OutDir=%1
set ProjectDir=%2

if not exist "%OutDir%\Data\Shaders\" md "%OutDir%\Data\Shaders\"
xcopy /y "%ProjectDir%\..\DX_Engine\Shaders\Compiled\*" "%OutDir%\Data\Shaders\"