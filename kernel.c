#define VGA_Width 25 // inverti os nomes kakakakaka
#define VGA_Height 80

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

static inline void outb(uint16_t port, uint8_t val){
	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
	return;
}

static int singleHexToInt(char hex){
	if (hex >= 'A' && hex <= 'Z') return hex - 55;
	if (hex >= 'a' && hex <= 'z') return hex - 87;
	if (hex >= '0' && hex <= '9') return hex - 48;
	return 0;
}

void loadVGAmemory(char data, int x, int y, char color){
	volatile char* vga = (volatile char*) 0xB8000;
	int index = (y * VGA_Height + x) * 2;
	vga[index] = data;
	index ++;
	vga[index] = color;
	return;
}

void scrollUp(){
	volatile char* vga = (volatile char*) 0xB8000;

	// move it all a line up
	int lineSize = VGA_Height * 2;
	for (int i = 0; i < (VGA_Width) * lineSize; i ++){
		vga[i] = vga[i + lineSize];
	}
	
	// clear the last line
	int lastLine = (VGA_Width-1) * lineSize; 
	for  (int i  = 0; i < lineSize; i += 2){
		vga[i + lastLine] = ' ';
		vga[i + 1 + lastLine] = 0x0F;
	} 
	return;
}

void updateCursor(int x, int y){
	uint16_t pos = (y * VGA_Height + x);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (pos >> 8) & 0xFF);
	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF); 
	return;
}

void dumpWait(int loop){
	for ( volatile int i = 0; i < loop; i++);
}

void print(char* address, int size){
	static int x = 0, y = 0;
	int i = 0;
	char data;
	char bgcolor;
	char fgcolor;
	char color = 0x0F;
	for(i = 0; i < size; i++){
		data = *(address + i);
		// auto newline
		if (x >= VGA_Height){
			x = 0;		
			y ++;
		}
		
		if (y >= VGA_Width) {
			scrollUp();
			y = VGA_Width - 1;
		}	

		// special chars
		if (data == '|'){
			i ++;
			if (i >= size) return;
			data = *(address + i);
			switch (data) {
			case 'n': // next line
				if (y >= VGA_Width) break; //add scrool
				y ++;
				break;
			case 'm': // unnext line lol
				if (y <= 0) break;
				y --;
				break;
			case '0': // go to beggin of the line
				x = 0;
				break;
			case 'r':
				x = 0;
				y ++;
				break;
			case '|':
				loadVGAmemory(data, x, y, 0x0F);
				break;
			case '[':
				if (i + 2 >= size) return;
				i ++;
				bgcolor = *(address + i); // background color
				i ++;
				fgcolor = *(address + i); // foreground color
				//set the color, bg color and fg color
				color = (singleHexToInt(bgcolor) << 4 | singleHexToInt(fgcolor));
				break; 	
			}
			continue;
		}
		
		// print
		loadVGAmemory(data, x, y, color);
		x ++;
	}
	updateCursor(x, y);
	return;
}

void kernel_main(){
	print("a|rb|rc|rd|re|rf|rg|rh|ri|rj|rk|rl|rm|rn|ro|rp|rq|rr|rs|rt|ru|rv|rw|rx|ry|rz", 76);
	print("|rAopa", 6);
	while (1);
}
