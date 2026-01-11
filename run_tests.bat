@echo off
setlocal EnableDelayedExpansion

set failed=0
set total=0

for %%t in (backend\build\*Test.exe) do (
    set /a total+=1
    echo Running %%t
    %%t
    if errorlevel 1 set /a failed+=1
)

set /a passed=total-failed

if %total% gtr 0 (
    set /a percent=100*failed/total
) else (
    set percent=0
)

echo -----------------------------
echo Total test binaries: %total%
echo Passed: %passed%
echo Failed: %failed%
echo Failure rate: %percent%%

if %failed% neq 0 exit /b 1
