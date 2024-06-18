NASM = ./tools/nasm
GOLINK = ./tools/GoLink

DLLS = kernel32.dll user32.dll

all: source.exe
	@del source.obj

source.obj: src/source.asm
	$(NASM) -fwin64 src/source.asm -o source.obj

source.exe: source.obj
	$(GOLINK) /entry:Start $(DLLS) source.obj -o source.exe

.PHONY: clean
clean:
	del source.exe