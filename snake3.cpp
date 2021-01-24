#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <time.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> 
#include <stdio.h>
#include <string.h>
#include<windows.h>

using namespace std;

int width = 800;
int height = 600;

int main() {

    ALLEGRO_DISPLAY* display;
    ALLEGRO_SAMPLE* MUZ = NULL;
    ALLEGRO_SAMPLE* MUZ_2 = NULL;
    ALLEGRO_SAMPLE* bg_MUZ = NULL;
    const float FPS = 60.0;
    const float frameFPS = 15.0;
    float FPSrate = 10;


    if (!al_init()) return -1;
    display = al_create_display(width, height);
    if (!display) return -1;
    al_set_window_position(display, 200, 200);
    if (!al_install_audio()) {
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }


    bool done = false, active = false;
    bool draw = true;
    int x = 0, y = 0, moveSpeed = 5;


    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    
    al_reserve_samples(3);
    MUZ = al_load_sample("music/sword.mp3");
    MUZ_2 = al_load_sample("music/beep.mp3");
    bg_MUZ = al_load_sample("music/background.mp3");
    

    if (!MUZ) {
        printf("Audio clip sample not loaded!\n");
        return -1;
    }
    
    al_play_sample(bg_MUZ, 1.0, 0.0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

    char background_image[100] = "assets/background/background.jpg";
    char head[100] = "assets/head/creeper.png";
    char tail[100] = "assets/tail/tnt.png";

    start:
    ALLEGRO_BITMAP* head_s = al_load_bitmap(head);
    ALLEGRO_BITMAP* tail_s = al_load_bitmap(tail);

    ALLEGRO_FONT* font1 = al_load_font("Minecrafter_3.ttf", 15, 0);
    ALLEGRO_FONT* mcfont = al_load_font("Minecrafter_3.ttf", 30, 0);
    ALLEGRO_BITMAP* coin1 = al_load_bitmap("assets/coin/coin.png");
    ALLEGRO_BITMAP* sword = al_load_bitmap("assets/coin/sword.png");


    ALLEGRO_BITMAP* background = al_load_bitmap(background_image);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPSrate);
    ALLEGRO_TIMER* frameTimer = al_create_timer(1.0 / frameFPS);
    ALLEGRO_TIMER* VREME = al_create_timer(1);
    ALLEGRO_KEYBOARD_STATE keyState;

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(frameTimer));
    al_register_event_source(event_queue, al_get_timer_event_source(VREME));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_start_timer(timer);
    al_start_timer(frameTimer);
    al_start_timer(VREME);
    srand(time(NULL));

    const int maxF = 8;
    int curF = 0;
    int frameC = 0;
    int frameD = 2;
    int frameW = 40;
    int frameH = 40;
    int timeS = 0;
    int timeF = 0;

    enum Direction { DOWN, LEFT, RIGHT, UP };

    bool sdasda;
    int dir = DOWN;
    int score = 1;
    int diamonds = 1;
    int swords = 1;
    int lastX;
    int lastY;

    int coinX = 40 * (rand() % 20);
    int coinY = 40 * (rand() % 15);

    int swordX = -100;
    int swordY = -100;
    
    int z = 1;

    int snakeT[50];
    for (int i = 0; i <= 50; i++) {
        snakeT[i] = 0;
    }
    int snakeX[50], snakeY[50];

    bool menu = true;
    bool options = false;
    bool options_1 = false;
    bool dead = false;

    while (!done) {

        lastX = x;
        lastY = y;


        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);


        if (events.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (events.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_ENTER:
                if (menu) menu = false, score = 1, diamonds = 1, swords = 1, timeS = 0, x = 0, y = 0;
                break;
            }
        }
        else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            done = true;

        if (events.type == ALLEGRO_EVENT_TIMER) {
            if (events.timer.source == VREME) timeS++;
            if (events.timer.source == timer) {

                al_get_keyboard_state(&keyState);
                if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && dir != LEFT)
                    dir = RIGHT;
                else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && dir != RIGHT)
                    dir = LEFT;
                else if (al_key_down(&keyState, ALLEGRO_KEY_UP) && dir != DOWN)
                    dir = UP;
                else if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) && dir != UP)
                    dir = DOWN;
                else if (al_key_down(&keyState, ALLEGRO_KEY_A))
                    score++;
                else if (al_key_down(&keyState, ALLEGRO_KEY_ENTER) && menu == true)
                    menu = false, options = false, score = 1, timeS = 0, x = 0, y = 0;
                else if (al_key_down(&keyState, ALLEGRO_KEY_1) && menu == true) {
                    menu = false;
                    options = true;
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_1) && options == true) {
                    options = false;
                    options_1 = true;
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_BACKSPACE) && options == true) {
                    menu = true;
                    options = false;
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_BACKSPACE) && options_1 == true) {
                    options_1 = false;
                    options = true;
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_3) && options == true) {
                    goto start;
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_1) && options_1 == true) {
                    strcpy_s(head, "assets/head/steve.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_2) && options_1 == true) {
                    strcpy_s(head, "assets/head/creeper.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_3) && options_1 == true) {
                    strcpy_s(head, "assets/head/pig.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_4) && options_1 == true) {
                    strcpy_s(head, "assets/head/cow.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_5) && options_1 == true) {
                    strcpy_s(head, "assets/head/trolley.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_6) && options_1 == true) {
                    strcpy_s(head, "assets/head/blaze.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_7) && options_1 == true) {
                    strcpy_s(tail, "assets/tail/tnt.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_8) && options_1 == true) {
                    strcpy_s(tail, "assets/tail/cobblestone.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_9) && options_1 == true) {
                    strcpy_s(tail, "assets/tail/sponge.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_0) && options_1 == true) {
                    strcpy_s(tail, "assets/tail/tracks.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_Q) && options_1 == true) {
                    strcpy_s(background_image, "assets/background/background.jpg");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_W) && options_1 == true) {
                    strcpy_s(background_image, "assets/background/nether.png");
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_E) && options_1 == true) {
                    strcpy_s(background_image, "assets/background/the_end.png");
                }
                if (menu == false) {
                    if (score != 0) {
                        for (int i = score; i > 0; i--) {
                            snakeX[i] = snakeX[i - 1];
                            snakeY[i] = snakeY[i - 1];
                        }
                        snakeX[0] = lastX;
                        snakeY[0] = lastY;
                    }
                }

                switch (dir) {
                case RIGHT: x = x + 40;
                    break;
                case LEFT: x = x - 40;
                    break;
                case UP: y = y - 40;
                    break;
                case DOWN: y = y + 40;
                    break;
                }

                if (x == coinX && y == coinY) {
                    score++;
                    diamonds++;
                    al_play_sample(MUZ, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    coinX = 40 * (rand() % 20);
                    coinY = 40 * (rand() % 15);
                    snakeT[score] = 1;
                }

                

                if (x == swordX && y == swordY) {
                    score += 2;
                    swords += 1;
                    al_play_sample(MUZ_2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    swordX = -100;
                    swordY = -100;
                    
                    snakeT[score] = 2;
                }
                
                if (z % 100 == 0) 
                {
                    swordX = 40 * (rand() % 20);
                    swordY = 40 * (rand() % 15);
                }
                z++;
                

                if (menu == false) {
                    for (int i = 0; i < score; i++) {
                        if (x == snakeX[i] && y == snakeY[i] && menu == false) dead = true;
                    }
                    if (x < 0 || x >= 800 || y < 0 || y >= 600 && menu == false) dead = true;
                }

                draw = true;

            }
        }
        if (++frameC >= frameD) {
            if (++curF >= maxF)
                curF = 0;

            frameC = 0;
        }

        if (dead && menu == false) {
            menu = true;
            timeF = timeS;
            x = 0, y = 0;
            for (int i = 0; i <= 50; i++) {
                snakeT[i] = 0;
            }
            dead = false;
            dir = DOWN;
        }



        if (draw == true) {
            draw = false;
            if (menu) {
                x = 750, y = 550;
                for (int i = 0; i <= 50; i++) {
                    snakeT[i] = 0;
                }
                al_draw_text(mcfont, al_map_rgb(100, 50, 250), 170, 100, 0,
                    "Press ENTER to start");
                al_draw_text(mcfont, al_map_rgb(100, 50, 250), 170, 200, 0,
                    "1.Options");
                al_draw_text(mcfont, al_map_rgb(100, 50, 250), 170, 300, 0,
                    "Press ESC to exit");
                al_draw_textf(mcfont, al_map_rgb(250, 0, 250), 120, 450, 0,
                    "Score: %i", score - 1);
                al_draw_textf(mcfont, al_map_rgb(250, 0, 250), 470, 450, 0,
                    "Time: %i sec", timeF);
            }
            else {
                al_draw_bitmap(coin1, coinX, coinY, NULL);
                
                al_draw_bitmap(sword, swordX, swordY, NULL);                 
                

                //al_draw_bitmap_region(coin1, curF * frameW, 0, frameW, frameH, coinX, coinY, 0);
                for (int i = 0; i < score; i++) {
                    al_draw_bitmap(tail_s, snakeX[i], snakeY[i], NULL);
                }

                al_draw_bitmap(head_s, x, y, NULL);
                al_draw_textf(font1, al_map_rgb(250, 0, 250), 5, 5, 0,
                    "Score: %i", score - 1);
                al_draw_textf(font1, al_map_rgb(250, 0, 250), 5, 30, 0,
                    "Diamonds: %i", diamonds - 1);
                al_draw_textf(font1, al_map_rgb(250, 0, 250), 5, 55, 0,
                    "Swords: %i", swords - 1);
                al_draw_textf(font1, al_map_rgb(250, 0, 250), 705, 5, 0,
                    "Time: %i", timeS);
            }

            if (options) {
                x = 750, y = 550;
                for (int i = 0; i <= 50; i++) {
                    snakeT[i] = 0;
                }
                al_draw_text(mcfont, al_map_rgb(100, 50, 250), 170, 100, 0,
                    "1.SKINS");
                al_draw_textf(mcfont, al_map_rgb(100, 50, 250), 170, 200, 0,
                    "2.GAME SPEED(a/d): %f", FPSrate);
                al_draw_textf(mcfont, al_map_rgb(100, 50, 250), 170, 300, 0,
                    "3.SAVE CHANGES", FPSrate);

               // if (al_key_down(&keyState, ALLEGRO_KEY_A))
               //     FPSrate += 1;
               // else if (al_key_down(&keyState, ALLEGRO_KEY_D))
                 //   FPSrate -= 1;
               
                if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
                    FPSrate -= 1;
                }
                else if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
                    FPSrate += 1;
                }
                    
            }

            if (options_1) {
                x = 750, y = 550;
                for (int i = 0; i <= 50; i++) {
                    snakeT[i] = 0;
                }
                al_draw_text(mcfont, al_map_rgb(0, 0, 0), 50, 50, 0,
                    "Head");
                al_draw_text(mcfont, al_map_rgb(198, 204, 16), 50, 100, 0,
                    "1.Steve");
                al_draw_text(mcfont, al_map_rgb(198, 204, 16), 50, 150, 0,
                    "2.Creeper");
                al_draw_text(mcfont, al_map_rgb(198, 204, 16), 50, 200, 0,
                    "3.Pig");
                al_draw_text(mcfont, al_map_rgb(198, 204, 16), 50, 250, 0,
                    "4.Cow");
                al_draw_text(mcfont, al_map_rgb(198, 204, 16), 50, 300, 0,
                    "5.Trolley");
                al_draw_text(mcfont, al_map_rgb(198, 204, 16), 50, 350, 0,
                    "6.Blaze");

                al_draw_text(mcfont, al_map_rgb(0, 0, 0), 400, 50, 0,
                    "Tail");
                al_draw_text(mcfont, al_map_rgb(43, 181, 80), 400, 100, 0,
                    "7.TNT");
                al_draw_text(mcfont, al_map_rgb(43, 181, 80), 400, 150, 0,
                    "8.Cobblestone");
                al_draw_text(mcfont, al_map_rgb(43, 181, 80), 400, 200, 0,
                    "9.Sponge");
                al_draw_text(mcfont, al_map_rgb(43, 181, 80), 400, 250, 0,
                    "0.Tracks");

                al_draw_text(mcfont, al_map_rgb(0, 0, 0), 250, 400, 0,
                    "Background");
                al_draw_text(mcfont, al_map_rgb(189, 8, 35), 250, 450, 0,
                    "Q.Standard");
                al_draw_text(mcfont, al_map_rgb(189, 8, 35), 250, 500, 0,
                    "W.Nether");
                al_draw_text(mcfont, al_map_rgb(189, 8, 35), 250, 550, 0,
                    "E.The End");
            }

            al_flip_display();
            //al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, -40, -40, -40);
        
           
        }




    }//while


    al_destroy_sample(MUZ);
    al_destroy_sample(MUZ_2);
    al_destroy_sample(bg_MUZ);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    return 0;
}
