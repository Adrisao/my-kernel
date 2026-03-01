#define VGA_Width 25 // inverti os nomes kakakakaka
#define VGA_Height 80

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;



static inline void outb(uint16_t port, uint8_t val){
	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
	return;
}

void loadVGAmemory(char data, int x, int y, char color){
	volatile char* vga = (volatile char*) 0xB8000;
	int index = (y * VGA_Height + x) * 2;
	vga[index] = data;
	index ++;
	vga[index] = color;
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

void print(char* address, int size){
	static int x = 0, y = 0;
	int i = 0;
	char data;
	for(i = 0; i < size; i++){
		data = *(address + i);
		// auto scrool
		if (y > VGA_Width) y --; //not added yet

		// auto newline
		if (x > VGA_Height){
			x = 0;		
			y ++;
		}

		// special chars
		if (data == '|'){
			i ++;
			if (i >= size) return;
			data = *(address + i);
			switch (data) {
			case 'n': // next line
				if (y >= VGA_Width) break; //add scrool
				y += 1;
				break;
			case 'm': // unnext line lol
				if (x <= 0) break;
				y --;
				break;
			case '0': // go to beggin of the line
				x = 0;
				break;
			case 'r':
				x = 0;
				y++;
				break;
			case '|':
				loadVGAmemory(data, x, y, 0x0F);
				break;
			}
			continue;
		}
		
		// print
		loadVGAmemory(data, x, y, 0x0F);
		x ++;
	}
	updateCursor(x, y);
	return;
}

void kernel_main(){
	// first message
	char* str = "Hello World!|n";
	char* str2 = " I am the Steve!|rBom dia e cia :)";
	int size = 0;
	for (size = 0; str[size] != '\0'; size ++);
	print(str, size);
	for (size = 0; str2[size] != '\0'; size ++);
	print(str2, size);
	while (1);
}
