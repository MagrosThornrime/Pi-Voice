.PHONY: all

all: copy_module pack copy_resources

setup: *
	npm install
	npx electron-rebuild

ifeq ($(OS),Windows_NT)
copy_resources:
	powershell -Command "$$dest = '../app/MIDIApp-win32-x64/res'; if (Test-Path $$dest) { Remove-Item $$dest -Recurse -Force }; Copy-Item '../res' $$dest -Recurse -Force"
	
copy_module: *
	powershell -Command "if (!(Test-Path './lib')) { New-Item -ItemType Directory -Path './lib' } ; Copy-Item '../backend/build/Release/*' -Destination './lib' -Recurse -Force"

pack: *
	npx electron-packager . MIDIApp --out="../app" --platform=win32 --arch=x64 --overwrite
else
copy_resources:
	rm -rf ../app/MIDIApp-linux-$$(node -p "process.arch")/res
	cp -r ../res ../app/MIDIApp-linux-$$(node -p "process.arch")/res

copy_module: *
	rm -rf lib
	mkdir lib
	cp -f -r ../backend/build/Release/* ./lib

pack: *
	npx electron-packager . MIDIApp --out="../app" --platform=linux --arch=$$(node -p "process.arch") --overwrite
endif
