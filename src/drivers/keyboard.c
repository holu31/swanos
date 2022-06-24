#include <keyboard.h>
#include <ports.h>

int kb_getkey(){
    if(inb(0x64) & 1) return inb(KB_DATA_PORT);
    return 0;
}

char kb_getchar(){
    return kb_sctochar(kb_getkey());
}

char kb_sctochar(int scancode){     // scancode to char
    if(scancode == 0x1C) return '\n';
	if(scancode == 0x39) return ' ';
	if(scancode == 0xE) return '\r';
	if(scancode == 0xB4) return '.';
	if(scancode == 0xB5) return '/';
	if(scancode == 0x29) return '0';
	if(scancode >= 0x2 && scancode <= 0xA)
		return "123456789"[scancode - 0x2];
	if(scancode >= 0x10 && scancode <= 0x1C)
	{
		return "qwertyuiop"[scancode - 0x10];
	} else if(scancode >= 0x1E && scancode <= 0x26)
	{
		return "asdfghjkl"[scancode - 0x1E];
	} else if(scancode >= 0x2C && scancode <= 0x32)
	{
		return "zxcvbnm"[scancode - 0x2C];
	}
	return 0;
}