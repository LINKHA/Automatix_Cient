@echo off
REM 切换到命令所在的相对目录
cd /d "%~dp0"

REM 执行 protoc 命令
..\tool\protoc\bin\protoc --cpp_out=. .\protobuf\*.proto

pause