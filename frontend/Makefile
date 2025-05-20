.PHONY: all

all: copy_module pack

ifeq ($(OS),Windows_NT)
copy_module:
	powershell -Command "if (!(Test-Path './lib')) { New-Item -ItemType Directory -Path './lib' } ; Copy-Item '../backend/build/Release/*' -Destination './lib' -Recurse -Force"

pack:
	npx electron-packager . MIDIApp --out="../app" --platform=win32 --arch=x64 --overwrite
else 
copy_module:
	mkdir lib
	cp -f -r ../backend/build/Release ./lib

pack:
	npx electron-packager . MIDIApp --out="../app" --platform=linux --arch=$$(node -p "process.arch") --overwrite
endif