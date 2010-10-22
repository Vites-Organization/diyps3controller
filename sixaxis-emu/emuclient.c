/* Sixaxis emulator

   Copyright (c) 2010 Mathieu Laurendeau

   Copyright (c) 2009 Jim Paris <jim@jtan.com>
   License: GPLv3
*/

#ifndef WIN32
#include <termio.h>
#include <sys/ioctl.h>
#include <err.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include "sixaxis.h"
#include "dump.h"
#include "macros.h"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define DEFAULT_DEAD_ZONE 18
#define DEFAULT_MULTIPLIER 4
#define MULTIPLIER_STEP 0.25
#define REFRESH_PERIOD 10000 //=10ms
#define EVENT_BUFFER_SIZE 32

#define POSTPONE_COUNT 3

static int debug = 0;
static int done = 0;
static double multiplier = DEFAULT_MULTIPLIER;
static int dead_zone = DEFAULT_DEAD_ZONE;
static int dead_zone_calibration = 0;

SDL_Surface *screen = NULL;

#ifdef WIN32
static void err(int eval, const char *fmt)
{
    fprintf(stderr, fmt);
    exit(eval);
}
#endif

static int clamp(int min, int val, int max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

int initialize(int width, int height, const char *title)
{
    /* Init SDL */
    if (SDL_Init(SDL_INIT_VIDEO
#ifdef JOYSTICK
    |SDL_INIT_JOYSTICK
#endif
    ) < 0)
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 0;
    }

#ifdef WIN32
    /* enable stdout and stderr */
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );
#endif

    SDL_WM_SetCaption(title, title);

    /* Init video */
    screen = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        fprintf(stderr, "Unable to create video surface: %s\n", SDL_GetError());
        return 0;
    }

    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);

#ifdef JOYSTICK
    SDL_JoystickOpen(0);
#endif

    return 1;
}

#define TCP_PORT 21313
static int device_number = 0;

int tcpconnect(void)
{
    int fd;
    struct sockaddr_in addr;

#ifdef WIN32
    WSADATA wsadata;

    if (WSAStartup(MAKEWORD(1,1), &wsadata) == SOCKET_ERROR)
        err(1, "WSAStartup");

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err(1, "socket");
#else
    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        err(1, "socket");
#endif
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(TCP_PORT+device_number);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //inet_addr("192.168.56.101");
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        err(1, "connect");

#ifdef WIN32
    // Set the socket I/O mode; iMode = 0 for blocking; iMode != 0 for non-blocking
    int iMode = 1;
    ioctlsocket(fd, FIONBIO, (u_long FAR*) &iMode);
#endif

    return fd;
}

struct sixaxis_state state;
int (*assemble)(uint8_t *buf, int len, struct sixaxis_state *state);
int sockfd;

void move_x(int x)
{
    state.user.axis[0].x = x;
}

void move_y(int y)
{
    state.user.axis[0].y = y;
}

void key(int sym, int down)
{
    int index = -1;

    switch (sym) {
    case SDLK_RSHIFT:    index = sb_ps;        break;

    case SDLK_e:        index = sb_triangle;    break;
    case SDLK_SPACE:    index = sb_cross;    break;
    case SDLK_LCTRL:    index = sb_circle;    break;
    case SDLK_r:        index = sb_square;    break;

    case SDLK_TAB:         index = sb_select;    break;
    case SDLK_BACKSPACE:    index = sb_start;    break;

    case SDLK_t:            index = sb_l2;        break;
    case SDLK_k:            index = sb_l1;        break;
    case SDLK_l:            index = sb_r1;        break;
    case SDLK_g:            index = sb_r2;        break;
    case SDLK_LSHIFT:       index = sb_l3;        break;
    case SDLK_f:            index = sb_r3;        break;

    case SDLK_AMPERSAND:    index = sb_up;        break;
    case SDLK_WORLD_73:         index = sb_down;    break;
    case SDLK_QUOTEDBL:         index = sb_left;    break;
    case SDLK_QUOTE:         index = sb_right;    break;

#ifndef WIN32
    case SDLK_q:        down?move_x(-127):move_x(0);    break;
    case SDLK_z:         down?move_y(-127):move_y(0);    break;
#else
    case SDLK_a:        down?move_x(-127):move_x(0);    break;
    case SDLK_w:         down?move_y(-127):move_y(0);    break;
#endif
    case SDLK_s:         down?move_y(127):move_y(0);    break;
    case SDLK_d:         down?move_x(127):move_x(0);    break;

    case SDLK_ESCAPE:    if(down) done = 1; break;
    case SDLK_KP_MINUS:    if(down) { multiplier -= MULTIPLIER_STEP; printf("multiplier: %e\n", multiplier); } break;
    case SDLK_KP_PLUS:     if(down) { multiplier += MULTIPLIER_STEP; printf("multiplier: %e\n", multiplier); } break;
    case SDLK_KP_DIVIDE:    if(down && dead_zone_calibration) { dead_zone -= 1; printf("dead_zone: %d\n", dead_zone); } break;
    case SDLK_KP_MULTIPLY:     if(down && dead_zone_calibration) { dead_zone += 1; printf("dead_zone: %d\n", dead_zone); } break;
    case SDLK_KP0:
        if(down)
        {
            if(dead_zone_calibration)
            {
                dead_zone_calibration = 0;
                printf("dead zone calibration mode disabled\n");
            }
            else
            {
                dead_zone_calibration = 1;
                printf("dead zone calibration mode enabled\n");
            }
        }
        break;
    }

    if(down) macro_lookup(sym);

    if (index >= 0) {
        state.user.button[index].pressed = down ? 1 : 0;
        state.user.button[index].value = down ? 255 : 0;
    }
}

void move_z_rz(int z, int rz)
{
    if(z > 0) state.user.axis[1].x = dead_zone + z;
        else if(z < 0) state.user.axis[1].x = z - dead_zone;
    else state.user.axis[1].x = 0;
    if(rz > 0) state.user.axis[1].y = dead_zone + rz;
        else if(rz < 0) state.user.axis[1].y = rz - dead_zone;
    else state.user.axis[1].y = 0;
}

void clic(int button, int down)
{
    int index = -1;

    switch(button)
    {
    case SDL_BUTTON_LEFT:         index = sb_r1;             break;
    case SDL_BUTTON_RIGHT:         index = sb_l1;             break;
    case SDL_BUTTON_MIDDLE:     index = sb_triangle;     break;
    case SDL_BUTTON_WHEELUP:     index = sb_r2;             break;
    case SDL_BUTTON_WHEELDOWN:     index = sb_l2;             break;
    case SDL_BUTTON_X1:            index = sb_square;         break;
    case SDL_BUTTON_X2:         index = sb_r3;             break;
    case 8:                     index = sb_triangle;     break; //Logitech G5 side button
    }

    if (index >= 0) {
        state.user.button[index].pressed = down ? 1 : 0;
        state.user.button[index].value = down ? 255 : 0;
    }
}

#ifdef JOYSTICK
void process_joystick_event(SDL_Event* event)
{
    int index;

    switch(event->type)
    {
        case SDL_JOYAXISMOTION:

            if(event->jaxis.axis == 0)
            {
                state.user.axis[0].x = event->jaxis.value/96;
            }
            else if(event->jaxis.axis == 1)
            {
                if(event->jaxis.value/128 > 16)
                {
                    state.user.button[sb_l2].pressed = 1;
                    state.user.button[sb_l2].value = clamp(0, event->jaxis.value/96, 255);
                }
                else if(event->jaxis.value/128 < -16)
                {
                    state.user.button[sb_r2].pressed = 1;
                    state.user.button[sb_r2].value = clamp(0, -event->jaxis.value/96, 255);
                }
                else
                {
                    state.user.button[sb_l2].pressed = 0;
                    state.user.button[sb_l2].value = 0;
                    state.user.button[sb_r2].pressed = 0;
                    state.user.button[sb_r2].value = 0;
                }
            }
            break;

        case SDL_JOYBUTTONDOWN:

            index = -1;
            switch(event->jbutton.button)
            {
            case 0:
                index = sb_l1;
                break;
            case 1:
                index = sb_circle;
                break;
            case 2:
                index = sb_triangle;
                break;
            case 3:
                index = sb_cross;
                break;
            case 4:
                index = sb_square;
                break;
            case 5:
                index = sb_up;
                break;
            }
            if(index >= 0)
            {
                state.user.button[index].pressed = 1;
                state.user.button[index].value = 255;
            }
            break;

        case SDL_JOYBUTTONUP:

            index = -1;
            switch(event->jbutton.button)
            {
            case 0:
                index = sb_l1;
                break;
            case 1:
                index = sb_circle;
                break;
            case 2:
                index = sb_triangle;
                break;
            case 3:
                index = sb_cross;
                break;
            case 4:
                index = sb_square;
                break;
            case 5:
                index = sb_up;
                break;
            }
            if(index >= 0)
            {
                state.user.button[index].pressed = 0;
                state.user.button[index].value = 0;
            }
            break;
    }
}
#endif

int main(int argc, char *argv[])
{
    SDL_Event events[EVENT_BUFFER_SIZE];
    SDL_Event* event;
    int i;
    int cpt = 0;
    int num_evt;
    unsigned char buf[48];
    int merge_x, merge_y;
    int nb_motion;
    int postpone_wheel_up = 0, postpone_wheel_down = 0, postpone_button_x1 = 0, postpone_button_x2 = 0;
    int change;
    int send_command;

    if(argc > 1)
        device_number = atoi(argv[1]);
    else
        printf("default bt device number 0 is used\n");

    if(device_number < 0)
    {
        fprintf(stderr, "bad bt device number\n");
        fprintf(stderr, "usage: %s <bt device number>\n", *argv);
        return 1;
    }

    initialize_macros();

    sixaxis_init(&state);
    for (i = 0; sixaxis_assemble[i].func; i++)
        if (sixaxis_assemble[i].type == HID_TYPE_INPUT &&
            sixaxis_assemble[i].report == 0x01) {
            assemble = sixaxis_assemble[i].func;
            break;
        }
    if (!sixaxis_assemble[i].func)
        err(1, "can't find assemble function");

    if (!initialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Sixaxis Control"))
        err(1, "can't init sdl");

    sockfd = tcpconnect();

    done = 0;
    while(!done)
    {
        SDL_PumpEvents();
        num_evt = SDL_PeepEvents(events, sizeof(events)/sizeof(events[0]), SDL_GETEVENT, SDL_ALLEVENTS);
        if(num_evt > 0)
        {
            change = 1;
            send_command = 1;
            merge_x = 0;
            merge_y = 0;
            nb_motion = 0;
            //if(num_evt>1) printf("%d\n", num_evt);
            for(event=events; event<events+num_evt; ++event)
            {
                switch( event->type ) {
#ifdef JOYSTICK
                case SDL_JOYAXISMOTION:
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                    process_joystick_event(event);
                    break;
#endif
                case SDL_QUIT:
                    done = 1;
                    break;
                case SDL_KEYDOWN:
                    key(event->key.keysym.sym, 1);
                    break;
                case SDL_KEYUP:
                    key(event->key.keysym.sym, 0);
                    break;
                case SDL_MOUSEMOTION:
                    merge_x += multiplier*event->motion.xrel;
                    merge_y += multiplier*event->motion.yrel;
                    nb_motion++;
                    cpt = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    clic(event->button.button, 1);
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event->button.button == SDL_BUTTON_WHEELUP)
                    {
                        if(postpone_wheel_up < POSTPONE_COUNT)
                        {
                            SDL_PushEvent(event);
                            postpone_wheel_up++;
                        }
                        else
                        {
                            postpone_wheel_up = 0;
                            clic(event->button.button, 0);
                        }
                    }
                    else if(event->button.button == SDL_BUTTON_WHEELDOWN)
                    {
                        if(postpone_wheel_down < POSTPONE_COUNT)
                        {
                            SDL_PushEvent(event);
                            postpone_wheel_down++;
                        }
                        else
                        {
                            postpone_wheel_down = 0;
                            clic(event->button.button, 0);
                        }
                    }
                    else if(event->button.button == SDL_BUTTON_X1)
                    {
                        if(postpone_button_x1 < POSTPONE_COUNT)
                        {
                            SDL_PushEvent(event);
                            postpone_button_x1++;
                        }
                        else
                        {
                            postpone_button_x1 = 0;
                            clic(event->button.button, 0);
                        }
                    }
                    else if(event->button.button == SDL_BUTTON_X2)
                    {
                        if(postpone_button_x2 < POSTPONE_COUNT)
                        {
                            SDL_PushEvent(event);
                            postpone_button_x2++;
                        }
                        else
                        {
                            postpone_button_x2 = 0;
                            clic(event->button.button, 0);
                        }
                    }
                    else
                    {
                        clic(event->button.button, 0);
                    }
                    break;
                }

            }
            if(nb_motion) move_z_rz(merge_x/nb_motion, merge_y/nb_motion);
            else move_z_rz(0, 0);
        }
        else
        {
            if(change)
            {
                move_z_rz(0, 0);
                change = 0;
                send_command = 1;
            }
            if(dead_zone_calibration)
            {
                move_z_rz(dead_zone_calibration, 0);
                send_command = 1;
            }
        }
        if(send_command)
        {
            if (assemble(buf, sizeof(buf), &state) < 0)
                printf("can't assemble\n");
#ifdef WIN32
            send(sockfd, buf, 48, 0);
#else
            send(sockfd, buf, 48, MSG_DONTWAIT);
#endif
            if(debug) sixaxis_dump_state(&state);
            send_command = 0;
        }
        usleep(REFRESH_PERIOD);
    }

    printf("Exiting\n");
    SDL_Quit();

    free_macros();

    return 0;
}
