xcopy	/y /s	.\Engine\Public\*.*	.\Reference\Headers\
xcopy   /y /y   .\Engine\Bin\ShaderFiles\*.hlsl  .\Client\Bin\ShaderFiles\

xcopy	/y  .\Engine\Bin\Engine.lib	.\Reference\Librarys\
xcopy	/y	.\Engine\Bin\Engine.dll	.\Client\Bin\
xcopy	/y	.\Engine\Bin\Engine.dll	.\IMGUI\examples\Bin\
xcopy	/y	.\Engine\Bin\Engine.dll	.\AssimpPaser\Bin\
