#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define SCREEN_W 960
#define SCREEN_H 590
bool done;
int state = 0;
ALLEGRO_SAMPLE_INSTANCE *songInstance;
ALLEGRO_SAMPLE *soundeffect;
ALLEGRO_SAMPLE *ballsound;
ALLEGRO_SAMPLE *firstsong;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *ball = NULL;
ALLEGRO_BITMAP *ballshadow = NULL;
ALLEGRO_BITMAP *shadow1 = NULL;
ALLEGRO_BITMAP *shadow2 = NULL;
ALLEGRO_BITMAP *screen = NULL;
ALLEGRO_BITMAP *endbackground = NULL;
ALLEGRO_BITMAP *psyduckwin= NULL;
ALLEGRO_BITMAP *pausebackground = NULL;
ALLEGRO_BITMAP *image0 = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *image2 = NULL;
ALLEGRO_BITMAP *back  = NULL;
ALLEGRO_FONT *title = NULL;
ALLEGRO_FONT *endlist = NULL;
ALLEGRO_FONT *endlist2 = NULL;
ALLEGRO_FONT *endlist3 = NULL;
ALLEGRO_FONT *pause = NULL;
ALLEGRO_FONT *pauselist = NULL;
ALLEGRO_FONT *num1 = NULL;
ALLEGRO_FONT *num2 = NULL;
double x1,y1;
double x2,y2;
double ballx,bally,r;
double speed_y,speed_x,speed_x2,speed_y2,speed_bx,speed_by;

void abort_game(const char* message)
{
    printf("%s\n", message);
    exit(1);
}

void game_init(void)
{
    if (!al_init())
        abort_game("Failed to initialize allegro");

    if (!al_install_keyboard())
        abort_game("Failed to install keyboard");

    timer = al_create_timer(1.0 / 30);
    if (!timer)
        abort_game("Failed to create timer");

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display)
        abort_game("Failed to create display");

    event_queue = al_create_event_queue();
    if (!event_queue)
        abort_game("Failed to create event queue");

    al_register_event_source(event_queue, al_get_keyboard_event_source());
   // al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_install_mouse();
    al_register_event_source(event_queue, al_get_mouse_event_source());
    //al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();

    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon
    done = false;
}

void game_destroy(void)
{
    if (timer)
        al_destroy_timer(timer);

    if (display)
        al_destroy_display(display);

    if (event_queue)
        al_destroy_event_queue(event_queue);
    if(ball)
        al_destroy_bitmap(ball);
    if(screen)
        al_destroy_bitmap(screen);
    if(image0)
        al_destroy_bitmap(image0);
    if(endbackground)
        al_destroy_bitmap(endbackground);
    if (psyduckwin)
        al_destroy_bitmap(psyduckwin);
    if(pausebackground)
        al_destroy_bitmap(pausebackground);
    if (image)
        al_destroy_bitmap(image);
    if (image2)
        al_destroy_bitmap(image2);
    if (back)
        al_destroy_bitmap(back);
    if(ballshadow)
        al_destroy_bitmap(ballshadow);
    if(shadow1)
        al_destroy_bitmap(shadow1);
    if(shadow2)
        al_destroy_bitmap(shadow2);
    if (title)
        al_destroy_font(title);
    if (endlist)
        al_destroy_font(endlist);
    if (endlist2)
        al_destroy_font(endlist2);
    if (endlist3)
        al_destroy_font(endlist3);
    if (pause)
        al_destroy_font(pause);
    if (pauselist)
        al_destroy_font(pauselist);
    if(num1)
        al_destroy_font(num1);
    if(num2)
        al_destroy_font(num2);
    if(soundeffect)
        al_destroy_sample(soundeffect);
    if(ballsound)
        al_destroy_sample(ballsound);
    if(firstsong)
        al_destroy_sample(firstsong);
    if(songInstance)
        al_destroy_sample_instance(songInstance);
}
ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h)
{
    ALLEGRO_BITMAP *resized_bmp, *loaded_bmp, *prev_target;

    resized_bmp = al_create_bitmap(w, h);
    if (!resized_bmp) return NULL;

    loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp)
    {
        al_destroy_bitmap(resized_bmp);
        return NULL;
    }

    prev_target = al_get_target_bitmap();
    al_set_target_bitmap(resized_bmp);

    al_draw_scaled_bitmap(loaded_bmp,
                          0, 0,
                          al_get_bitmap_width(loaded_bmp),
                          al_get_bitmap_height(loaded_bmp),
                          0, 0,
                          w, h, 0 );

    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    return resized_bmp;
}

void game_run(void)
{
    bool redraw = true;
    al_start_timer(timer);

    ballshadow = load_bitmap_at_size("ballshadow.png",50,10);
    ball = load_bitmap_at_size("ball.png",50,50);
    screen = load_bitmap_at_size("screen.png",50,300);
    image0 = load_bitmap_at_size("images0.png", 960, 590);
    endbackground = load_bitmap_at_size("endbackground.png",960,590);
    psyduckwin = load_bitmap_at_size("psyduckwin.png",960,590);
    pausebackground = load_bitmap_at_size("pausebackground.png",960,590);
    image = load_bitmap_at_size("image.png", 100, 100);
    shadow1 = load_bitmap_at_size("shadow1.png",100,10);
    image2 = load_bitmap_at_size("pika.png", 100, 100);
    shadow2 = load_bitmap_at_size("shadow2.png",100,10);
    back  = load_bitmap_at_size("background.png",960,590);

    num1 = al_load_font("pirulen.ttf",64,NULL);
    num2 = al_load_font("pirulen.ttf",64,NULL);
    title = al_load_font("SHREK.ttf", 64, NULL);
    endlist = al_load_font("pirulen.ttf",32,NULL);
    endlist2 = al_load_font("pirulen.ttf",32,NULL);
    endlist3 = al_load_font("pirulen.ttf",32,NULL);
    pauselist = al_load_font("pirulen.ttf",64,NULL);
    pause = al_load_font("pirulen.ttf",32,NULL);

    al_reserve_samples(2);
    soundeffect = al_load_sample("soundeffect.wav");
    ballsound = al_load_sample("ballsound.wav");

    firstsong = al_load_sample("firstsong.ogg");
    songInstance = al_create_sample_instance(firstsong);
    al_set_sample_instance_playmode(songInstance,ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(songInstance,al_get_default_mixer());

    r=25;
    ballx=SCREEN_W-8*r;//4
    bally=SCREEN_H - al_get_bitmap_height(image)-8*r;//2
    x1=SCREEN_W;
    y1=SCREEN_H - al_get_bitmap_height(image);
    x2=0;
    y2=SCREEN_H - al_get_bitmap_height(image2);

    int net_x[2] = { 430 , 480};
    int net_y[2] = { 330, SCREEN_H};
    /*int net_x[2] = { SCREEN_W/2-20 , SCREEN_W/2+20 };
    int net_y[2] = { SCREEN_H -250, SCREEN_H};*/
    speed_x=30;
    speed_y=0;
    speed_x2=30;
    speed_y2=0;
    speed_bx=0;
    speed_by=0;

    const double Gravity=5;
    const double Jump_speed=-50;
    const double ballx_speed=-200;
    const double bally_speed=-120;
    const double dt=0.1;

    int touch = 0;
    int jump = 0;
    int jump2=0;
    bool L1 = false;
    bool R1 = false;
    bool L2 = false;
    bool R2 = false;
    int count1=0;
    int count2=0;
    int wins = 0;

    while (!done) {

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
            break;
        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                done = true;
            }
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
            break;
        }

        if(state == 0 ) {

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    state = 1;
                }
            }

            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;

                al_play_sample_instance(songInstance);
                //al_clear_to_color(al_map_rgb(102, 194, 165));
                al_draw_bitmap(image0, 0, 0, 0);
                al_draw_text(title, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2-200, ALLEGRO_ALIGN_CENTER, "Pikachu Volleyball");
                al_flip_display();
            }

        }
        else if(state == 1) {


            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        if (jump<2){
                            jump++;
                            speed_y=Jump_speed;
                            al_play_sample(soundeffect,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
                        }
                        break;
                    case ALLEGRO_KEY_LEFT:
                        L1=true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        R1=true;
                        break;
                    case ALLEGRO_KEY_W:
                        if (jump2<2){
                            jump2++;
                            speed_y2=Jump_speed;
                            al_play_sample(soundeffect,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
                        }
                        break;
                    case ALLEGRO_KEY_A:
                        L2=true;
                        break;
                    case ALLEGRO_KEY_D:
                        R2=true;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        done=true;
                        break;
                    default:
                        break;
                }
            }
            if (event.type == ALLEGRO_EVENT_KEY_UP) {
                switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_LEFT:
                        L1=false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        R1=false;
                        break;
                    case ALLEGRO_KEY_A:
                        L2=false;
                        break;
                    case ALLEGRO_KEY_D:
                        R2=false;
                        break;
                }
            }
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.mouse.button == 1 && event.mouse.x<=580&&event.mouse.x>=400) {
                    if(event.mouse.y>=25&&event.mouse.y<=75){
                        state = 3;
                    }
                }
            }

            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;

                al_draw_bitmap(back, 0, 0, 0);
                al_draw_bitmap(screen,net_x[0],net_y[0],0);

                if(y1+al_get_bitmap_height(image)>=SCREEN_H && speed_y>0){
                speed_y=-speed_y;
                }
                if(x1+al_get_bitmap_height(image)>=SCREEN_W){
                    R1=false;
                    x1=SCREEN_W-al_get_bitmap_height(image);
                }
                if(x1<=net_x[1]){
                    L1=false;
                    x1=net_x[1];
                }
                if(jump)speed_y+=Gravity;
                y1+=speed_y;
                x1+=(R1-L1)*speed_x;
                if(y1+al_get_bitmap_height(image)>=SCREEN_H){
                    y1=SCREEN_H-al_get_bitmap_height(image);
                    jump=0;
                    speed_y=0;
                }

                if(y2+al_get_bitmap_height(image2)>=SCREEN_H && speed_y2>0){
                speed_y2=-speed_y2;
                }
                if(x2+al_get_bitmap_height(image2)>=net_x[0]){
                    R2=false;
                    x2=net_x[0]-al_get_bitmap_height(image2);
                }
                if(x2<=0){
                    L2=false;
                    x2=0;
                }
                if(jump2)speed_y2+=Gravity;
                y2+=speed_y2;
                x2+=(R2-L2)*speed_x2;

                if(y2+al_get_bitmap_height(image2)>=SCREEN_H){
                    y2=SCREEN_H-al_get_bitmap_height(image2);
                    jump2=0;
                    speed_y2=0;
                }

                if(ballx>=(x1-1.5*r)&&ballx<=(x1+r)){ //鴨子 左
                    if(bally>=(y1-1.5*r)&&bally<=(y1+r)){
                        speed_bx=ballx_speed;
                        speed_by=bally_speed;
                        touch=1;
                    }
                }
                if(ballx>(x1+r)&&ballx<=(x1+100+r)){ //鴨子 右
                    if(bally>=(y1-1.5*r)&&bally<=(y1+r)){
                        speed_bx=-ballx_speed;
                        speed_by=bally_speed;
                        touch=1;
                    }
                }
                if(touch){  //初速度
                    ballx=ballx+speed_bx*dt;
                    bally=bally+speed_by*dt-0.5*Gravity*dt*dt;
                }

                if(ballx+2*r<=net_x[0]+5&&ballx+2*r>=net_x[0]){ //網子 左
                    if(bally+2*r>=(double)net_y[0] && bally+2*r<=SCREEN_H){
                        speed_bx=-speed_bx;
                        al_play_sample(ballsound,1.0,0.0,2.0,ALLEGRO_PLAYMODE_ONCE,0);
                    }
                }
                if(ballx>=net_x[1]-5&& ballx<=net_x[1]){ //網子 右
                    if(bally+2*r>=(double)net_y[0] && bally+2*r<=SCREEN_H){
                        speed_bx=-speed_bx;
                        al_play_sample(ballsound,1.0,0.0,2.0,ALLEGRO_PLAYMODE_ONCE,0);
                    }
                }

                if(ballx<=(x2+100+r)&&ballx>=x2+50){ //皮卡丘
                        if(bally>=(y2-1.5*r)&&bally<=(y2+r)){
                            speed_bx=-ballx_speed;
                            speed_by=bally_speed;
                        }
                }
                if(ballx<(x2+50)&&ballx>=x2-r){
                        if(bally>=(y2-1.5*r)&&bally<=(y2+r)){
                            speed_bx=ballx_speed;
                            speed_by=bally_speed;
                        }
                }
                if(ballx+2*r>SCREEN_W ){//最頂
                    speed_bx=-speed_bx;
                    al_play_sample(ballsound,1.0,0.0,2.0,ALLEGRO_PLAYMODE_ONCE,0);
                }
                if(ballx<0){
                speed_bx=-speed_bx;
                al_play_sample(ballsound,1.0,0.0,2.0,ALLEGRO_PLAYMODE_ONCE,0);
                }
                if(bally<0){
                    speed_by=-speed_by;
                    al_play_sample(ballsound,1.0,0.0,2.0,ALLEGRO_PLAYMODE_ONCE,0);
                }
                if(bally+2*r>SCREEN_H){ //計分
                   if(ballx<SCREEN_W/2){
                        ballx=SCREEN_W-8*r;//4
                        bally=SCREEN_H - al_get_bitmap_height(image)-8*r;
                        speed_bx=0;
                        speed_by=0;
                        count1++;
                   }
                   else{
                        ballx=SCREEN_W-8*r;//4
                        bally=SCREEN_H - al_get_bitmap_height(image)-8*r;
                        speed_bx=0;
                        speed_by=0;
                        count2++;
                   }
                }
                //if(ballx==)
                /*if(y1+al_get_bitmap_height(image)>=SCREEN_H && speed_y>0){
                    speed_y=-speed_y;
                }
                if(x1+al_get_bitmap_height(image)>=SCREEN_W){
                    R1=false;
                    x1=SCREEN_W;
                }
                if(x1-al_get_bitmap_height(image)<=net_x[1]){
                    L1=false;
                    x1=net_x[1];
                }
                if(jump)speed_y+=Gravity;
                y1+=speed_y;
                x1+=(R1-L1)*speed_x;

                if(y1>=SCREEN_H){
                    y1=SCREEN_H;
                    jump=0;
                    speed_y=0;
                }*/

                al_draw_bitmap(image , x1 ,y1/*SCREEN_H - al_get_bitmap_height(image)*/ , 0);
                al_draw_bitmap(image2, x2, y2/*SCREEN_H - al_get_bitmap_height(image2)*/, 0);
                al_draw_bitmap(ball,ballx,bally,0);
                al_draw_bitmap(ballshadow,ballx,SCREEN_H-10,0);
                al_draw_bitmap(shadow1,x1,SCREEN_H-10,0);
                al_draw_bitmap(shadow2,x2,SCREEN_H-10,0);
                al_draw_textf(num2, al_map_rgb(255,255,255), 40, 25, ALLEGRO_ALIGN_CENTER,"%d",count2);
                al_draw_textf(num1, al_map_rgb(255,255,255), 920, 25, ALLEGRO_ALIGN_CENTER,"%d", count1);
                al_draw_text(pause, al_map_rgb(255,255,255), SCREEN_W/2,50, ALLEGRO_ALIGN_CENTER, "PAUSE");
                if(count2==3||count1==3){
                    if(count2==3){
                        count1=0;
                        count2=0;
                        x1=SCREEN_W;
                        y1=SCREEN_H - al_get_bitmap_height(image);
                        x2=0;
                        y2=SCREEN_H - al_get_bitmap_height(image2);
                        state=2;
                    }
                    if(count1==3){
                        count1=0;
                        count2=0;
                        x1=SCREEN_W;
                        y1=SCREEN_H - al_get_bitmap_height(image);
                        x2=0;
                        y2=SCREEN_H - al_get_bitmap_height(image2);
                        state=4;
                    }
                }
                al_flip_display();

            }

        }
        else if(state == 2)
        {
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.mouse.button == 1 && event.mouse.x<=530&&event.mouse.x>=430) {
                    if(event.mouse.y>=250&&event.mouse.y<=285){
                        state = 1;
                    }
                    else if(event.mouse.y>=385&&event.mouse.y<=415){
                        break;
                    }
                }
            }
            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;

                al_draw_bitmap(endbackground, 0, 0, 0);

                al_draw_text(endlist, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2-200, ALLEGRO_ALIGN_CENTER, "PIKA WINS");

                /*else if(wins==1){
                al_draw_text(endlist, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2-200, ALLEGRO_ALIGN_CENTER, "Psyduck WINS");
                }*/
                al_draw_text(endlist2, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2-50, ALLEGRO_ALIGN_CENTER, "PLAY AGAIN");
                al_draw_text(endlist3, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2+100, ALLEGRO_ALIGN_CENTER, "Exit");
                al_flip_display();
            }
        }
        else if (state == 3)
        {
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.mouse.button == 1 && event.mouse.x<=600&&event.mouse.x>=300) {
                    if(event.mouse.y>=200&&event.mouse.y<=285){
                        state = 1;
                    }
                }
            }
            if (redraw && al_is_event_queue_empty(event_queue)){
                redraw = false;

                al_draw_bitmap(pausebackground, 0, 0, 0);
                al_draw_text(pauselist, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2-100, ALLEGRO_ALIGN_CENTER, "Return");
                al_flip_display();
            }
        }
        else if(state == 4)
        {
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.mouse.button == 1 && event.mouse.x<=530&&event.mouse.x>=430) {
                    if(event.mouse.y>=250&&event.mouse.y<=285){
                        state = 1;
                    }
                    else if(event.mouse.y>=385&&event.mouse.y<=415){
                        break;
                    }
                }
            }
            if (redraw && al_is_event_queue_empty(event_queue)) {
                redraw = false;

                al_draw_bitmap(psyduckwin, 0, 0, 0);

                al_draw_text(endlist, al_map_rgb(0,255,255), SCREEN_W/2, SCREEN_H/2-200, ALLEGRO_ALIGN_CENTER, "PSYDUCK WINS");

                /*else if(wins==1){
                al_draw_text(endlist, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2-200, ALLEGRO_ALIGN_CENTER, "Psyduck WINS");
                }*/
                al_draw_text(endlist2, al_map_rgb(0,255,255), SCREEN_W/2, SCREEN_H/2-50, ALLEGRO_ALIGN_CENTER, "PLAY AGAIN");
                al_draw_text(endlist3, al_map_rgb(0,255,255), SCREEN_W/2, SCREEN_H/2+100, ALLEGRO_ALIGN_CENTER, "Exit");
                al_flip_display();
            }
        }
    }
}

int main(int argc, char* argv[]) {

    game_init();
    game_run();
    game_destroy();

    return 0;
}
