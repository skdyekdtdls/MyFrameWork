attrib -R .\Reference\Headers\*.*
xcopy	/y /s	.\Engine\Public\*.*	.\Reference\Headers\
attrib +R .\Reference\Headers\*.*

attrib -R .\Client\Bin\ShaderFiles\*.hlsl
xcopy   /y /s   .\Engine\Bin\ShaderFiles\*.hlsl  .\Client\Bin\ShaderFiles\
attrib +R .\Client\Bin\ShaderFiles\*.hlsl

xcopy	/y  .\Engine\Bin\Engine.lib	.\Reference\Librarys\
xcopy	/y	.\Engine\Bin\Engine.dll	.\Client\Bin\
xcopy	/y	.\Engine\Bin\Engine.dll	.\IMGUI\examples\Bin\
xcopy	/y	.\Engine\Bin\Engine.dll	.\AssimpPaser\Bin\
