#include "Display.h"


Display::Display(){}

void Display::init(){
    // PCD8544-compatible display resolution...
    lcd.begin(84, 48);

    // backlight is off by default
    pinMode(BL, OUTPUT);
    digitalWrite(BL, HIGH);

    // Add the custom symbols to the ASCII table...
    lcd.createChar(0, glyph_temperature);
    lcd.createChar(1, glyph_house);
    lcd.createChar(2, glyph_cloud);
    lcd.createChar(3, glyph_degree);
    lcd.createChar(4, glyph_pressure);
    lcd.createChar(5, glyph_drop);
    lcd.createChar(6, glyph_light);
    lcd.createChar(7, glyph_soil_moisture);
    lcd.createChar(8, glyph_1);
    lcd.createChar(9, glyph_2);
    
    lcd.createChar(10, glyph_c);
    lcd.createChar(11, glyph_a);
    lcd.createChar(12, glyph_t);
    lcd.createChar(13, glyph_i);
    lcd.createChar(14, glyph_defis);
    lcd.createChar(15, glyph_e);
    lcd.createChar(16, glyph_n);
    lcd.createChar(17, glyph_v);
    lcd.createChar(18, glyph__);
    lcd.createChar(19, glyph_cacti);

    lcd.createChar(20, glyph_loding_bar);
}

void Display::turn_backlight_on(){
    digitalWrite(BL, LOW);
    backlight = true;
}

void Display::turn_backlight_off(){
    digitalWrite(BL, HIGH);
    backlight = false;
}

void Display::switch_backlight(){
    if (backlight){
        Display::turn_backlight_off();
    }else{
        Display::turn_backlight_on();
    }
}

bool Display::get_backlight_status(){
    return backlight;
}

void Display::show_msg_in_title(String title){
    lcd.setCursor(0, 0);
    lcd.clearLine();
    lcd.print(title);
}

void Display::show_title(){
    lcd.setCursor(0, 0);
    lcd.clearLine();
    
    lcd.write(18); // space
    lcd.write(19); // cacti
    lcd.write(18); // space
    lcd.write(10); // c
    lcd.write(11); // a
    lcd.write(10); // c    
    lcd.write(12); // t
    lcd.write(13); // i
    lcd.write(14); // -
    lcd.write(15); // e
    lcd.write(16); // n
    lcd.write(17); // v
    lcd.write(18); // space
    lcd.write(19); // cacti
    lcd.write(18); // space
}

void Display::show_welcome_screen(){
    lcd.clear();
    lcd.drawBitmap(Welcome_Bitmap, 84, 48);
}

void Display::show_main_page(float t_out, float t_in, int p, int h, int l1, int l2, int s1, int s2){

    // Display Line 2
    lcd.setCursor(0, 1);
    lcd.clearLine();
    lcd.write(0); // temperature symbol   
    lcd.write(2); // cloud symbol   
    lcd.print(" ");
    if (t_out > 0)     { lcd.print("+");      }
    if (t_out != -100) { lcd.print(t_out, 1); }else{ lcd.print(nodata); }
    
    lcd.setCursor(54, 1);
    lcd.write(3); // degree symbol   
    lcd.print("C");
    
    // Display Line 3
    lcd.setCursor(0, 2);
    lcd.clearLine();
    lcd.write(0); // temperature symbol   
    lcd.write(1); // house symbol   
    lcd.print(" ");
    if (t_in > 0){lcd.print("+");}
    if (t_in != -100) { lcd.print(t_in, 1); }else{ lcd.print(nodata); }
    lcd.setCursor(54, 2);
    lcd.write(3); // degree symbol   
    lcd.print("C");

    // Display Line 4
    lcd.setCursor(0, 3);
    lcd.clearLine();
    lcd.write(4); // pressure symbol 
    lcd.print(" ");
    if (p != -100) { lcd.print(p); }else{ lcd.print(nodata); }
    lcd.print("mm ");
    lcd.write(5); // humidity symbol
    lcd.print(" ");
    if (h != -100) { lcd.print(h); }else{ lcd.print(nodata); }
    lcd.print("%");

    // Display Line 5
    lcd.setCursor(0, 4);
    lcd.clearLine();
    lcd.write(6); // light 1 symbol 
    lcd.write(8); // symbol I
    if (l1 != -100) { lcd.print(l1); }else{ lcd.print(nodata); }
    lcd.setCursor(48, 4);
    lcd.write(7); // soli moisture symbol 
    lcd.write(8); // symbol I
    if (s1 == -100) {
        lcd.print(nodata);
    }else if (s1 < 0){
        lcd.print(0);    
    } else if (s1 > 99){
        lcd.print(100);    
    } else {
        lcd.print(s1);    
    }
    lcd.print("%");

    // Display Line 6
    lcd.setCursor(0, 5);
    lcd.clearLine();

    lcd.write(6);
    lcd.write(9); // symbol II
    if (l2 != -100) { lcd.print(l2); }else{ lcd.print(nodata); }
    
    lcd.setCursor(48, 5);
    lcd.write(7);
    lcd.write(9); // symbol II
    if (s2 == -100) {
        lcd.print(nodata);
    }else if (s2 < 0){
        lcd.print(0);    
    } else if (s2 > 99){
        lcd.print(100);    
    } else {
        lcd.print(s2);    
    }
    lcd.print("%");

}

void Display::show_loading_line(String task, String status, int line){
    // show task name
    lcd.setCursor(0, line);
    lcd.clearLine();
    lcd.print(task);
    int num_points = 14 - task.length() - status.length();
    for (int i = 0; i < num_points; i++){
        lcd.print(".");
        delay(random(200, 500));
    }
    lcd.print(status);
    delay(700);
}

void Display::show_msg_in_line(String msg, int line){
    lcd.setCursor(0, line);
    lcd.clearLine();
    lcd.print(msg);
}

void Display::clear(){
    lcd.clear();
}

