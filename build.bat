@echo off

if not exist out mkdir out
cl main.cpp /JMC /GS /analyze- /W4 /wd"4365" /wd"4514" /wd"4820" /Zc:wchar_t /ZI /Gm- /Od /sdl /Fd"out\vc142.pdb" /Zc:inline /fp:precise /errorReport:prompt /WX /Zc:forScope /RTC1 /Gd /Oy- /MDd /std:c++17 /FC /Fa"out\\" /nologo /Fo"out\\" /Fp"out\\clipboard.pch" /diagnostics:column /EHsc