#define VGA_ADDRESS 0xB8000   /* video memory begins here. */

enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14, // yellow
    COLOR_WHITE = 15,
};

unsigned short *terminal_buffer;
unsigned int vga_index;

void clear_screen(void)
{
    int index = 0;
    /* there are 25 lines each of 80 columns;
       each element takes 2 bytes */
    while (index < 80 * 25 * 2) {
            terminal_buffer[index] = ' ';
            index += 2;
    }
}

void print_string(char *str, unsigned char color)
{
    int index = 0;
    while (str[index]) {
            terminal_buffer[vga_index] = (unsigned short)str[index]|(unsigned short)color << 8;
            index++;
            vga_index++;
    }
}

void main(void)
{
    terminal_buffer = (unsigned short *)VGA_ADDRESS;
    vga_index = 0;

    clear_screen();

    enum vga_color color = 0;
    print_string("Hello 42", color);
    vga_index = 80;
    print_string("This is a test.", color++);
    vga_index = 160;
    print_string("This is a test.", color++);
    vga_index = 240;
    print_string("This is a test.", color++);
 
    return;
}