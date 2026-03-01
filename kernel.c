#define VGA_Width 25 // inverti os nomes kakakakaka
#define VGA_Height 80

void loadVGAmemory(char data, int x, int y, char color){
	volatile char* vga = (volatile char*) 0xB8000;
	int index = (y * VGA_Width + x) * 2;
	vga[index] = data;
	index ++;
	vga[index] = color;
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
				x=0;
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
	return;
}

void kernel_main(){
	// first message
	char* str = "Hello World!";
	char* str2 = " I am the Steve!";
	int size = 0;
	for (size = 0; str[size] != '\0'; size ++);
	print(str, size);
	for (size = 0; str2[size] != '\0'; size ++);
	print(str2, size);
	while (1);
}
