#define VGA_ADDRESS 0xB8000   /* video memory begins here. */

#include <sys/io.h>

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
char   keyboard_map[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\n', 'a', 's',
     'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'' ,'\n', ' ' ,'\\', 'z', 'x', 'c',  'v', 'b', 'n', 'm', ',', '.', '/', ' ', ' ', 114, '0', '5', '6', '7', '8', '9', '0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 114, 'p', '[', 114 , 0, 0, 114};


void clear_screen(void)
{
    int index = 0;
    /* there are 25 lines each of 80 columns;
       each element takes 2 bytes */
    while (index < 80 * 25 * 2) {
            terminal_buffer[index] = ' ';
            index += 1;
    }
}

void remove_char(void)
{
    if (vga_index > 0) {
        vga_index -= 1;
        terminal_buffer[vga_index] = ' ';
    }
    if (vga_index >= 0) {
        vga_index -= 1;
    }
}


// convert int keyboard code to char
char get_char(int code)
{
    return keyboard_map[code];
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

// print keyboard pressed key
void print_key(char key, enum vga_color *color)
{
    char c;
    if (keyboard_map[key] == '\n')
    {
        vga_index += 80;
        c = ' ';
    }
    else if ( keyboard_map[key] == 114)
    {
        *color = (*color + 1) % 16;
        return ;
    }
    else if ( keyboard_map[key] == 0)
    {
        if (color == 0)
            *color = 15;
        else
            *color -= 1;
        return ;
    }
    else if (keyboard_map[key] == '\b')
    {
        remove_char();
        return ;
    }
    else
        c = keyboard_map[key];
    
    terminal_buffer[vga_index] = (unsigned short)c|(unsigned short)*color << 8;
}



//catch key pressed
void keyboard_handler_main(enum vga_color *color)
{
    unsigned char status;
    char keycode;

    //write EOI
    outb(0x20, 0x20);

    status = inb(0x64);
    // Lowest bit of status will be set if buffer is not empty
    if (status & 0x01) {
        keycode = inb(0x60);
        if(keycode < 0)
            return;

        print_key(keycode, color);
        vga_index++;
    }
}

void main(void)
{
    terminal_buffer = (unsigned short *)VGA_ADDRESS;
    vga_index = 0;

    clear_screen();

    enum vga_color color = COLOR_LIGHT_GREEN;
    print_string("Hello, world!", COLOR_LIGHT_GREEN);
    print_string("42", COLOR_LIGHT_GREEN);

    while (1) 
    {
        keyboard_handler_main(&color);
    }
  
 
    return;
}