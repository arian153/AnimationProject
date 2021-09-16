echo Clear
@echo off

for %%i in (*.vcxproj) do (
 del *.vcxproj
)

for %%i in (*.filters) do (
 del *.filters
)

for %%i in (*.sln) do (
 del *.sln
)

for %%i in (*.user) do (
 del *.user
)

if exist .vs (
rmdir .vs /s /q
)

if exist Obj (
rmdir Obj /s /q
)

echo Done