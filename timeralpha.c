#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIN_NAME "Timer"
#define WIDTH 1366
#define HEIGHT 735

struct display {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  SDL_Texture *texture;
  SDL_Texture *penger;
  int counter;
};

void initialize(struct display *pD);
void rectangle(struct display *pD);
SDL_FRect *numbers(struct display *pD, int c, int m);
SDL_FRect *penger(struct display *pD, int c, int m);
void displayer(struct display *pD, int h, int m, int s);
void timer(struct display *pD, int s);
void draw(struct display *pD, int s);
bool isthisclose(struct display *pD);
void quit(struct display *pD);

int main(int argc, char *argv[]) {
  struct display *Display = calloc(1, sizeof(struct display));

  initialize(Display);

  int seconds = atoi(argv[1]);

  draw(Display, seconds);

  quit(Display);

  return 0;
}

void initialize(struct display *pD) {
  SDL_Init(SDL_INIT_VIDEO);

  pD->window = SDL_CreateWindow(WIN_NAME, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
  if (pD->window == NULL) {
    printf("Window couldn't be created\n");
    return;
  }

  pD->renderer = SDL_CreateRenderer(pD->window, NULL);
  if (pD->renderer == NULL) {
    printf("Renderer couldn't be created\n");
    return;
  }

  pD->texture = IMG_LoadTexture(pD->renderer, "assets/digits_brown.png");
  if (pD->texture == NULL) {
    printf("Texture couldn't be created\n");
    return;
  }
}

bool isthisclose(struct display *pD) {
  while (SDL_PollEvent(&pD->event)) {
    if (pD->event.type == SDL_EVENT_QUIT) {
      printf("Quit pressed\n");
      return true;
    }

    if (pD->event.type == SDL_EVENT_KEY_DOWN) {
      printf("Key pressed\n");
      if (pD->event.key.scancode == SDL_SCANCODE_Q) {
        printf("Q pressed\n");
        return true;
      }
    }
  }
  return false;
}

SDL_FRect *numbers(struct display *pD, int c, int m) {
  static SDL_FRect rectangle;

  float x = 0.0f;
  float y = 0.0f;
  float w = 150.0f;
  float h = 190.0f;

  switch (m) {
  case 1:
    y = 0.0f;
    break;
  case 2:
    y = 190.0f;
    break;
  case 3:
    y = 380.0f;
    break;
  }

  x = c * 150.0f;

  rectangle = (SDL_FRect){x, y, w, h};

  return &rectangle;
}

void displayer(struct display *pD, int h, int m, int s) {
  int h1 = h / 10;
  int h2 = h % 10;
  int m1 = m / 10;
  int m2 = m % 10;
  int s1 = s / 10;
  int s2 = s % 10;

  SDL_FRect H1 = {83.0f, 300.0f, 150.0f, 190.0f};
  SDL_FRect H2 = {83.0f + 150.0f, 300.0f, 150.0f, 190.0f};
  SDL_FRect C1 = {83.0f + 300.0f, 300.0f, 150.0f, 190.0f};
  SDL_FRect M1 = {83.0f + 450.0f, 300.0f, 150.0f, 190.0f};
  SDL_FRect M2 = {83.0f + 600.0f, 300.0f, 150.0f, 190.0f};
  SDL_FRect C2 = {83.0f + 750.0f, 300.0f, 150.0f, 190.0f};
  SDL_FRect S1 = {83.0f + 900.0f, 300.0f, 150.0f, 190.0f};
  SDL_FRect S2 = {83.0f + 1050.0f, 300.0f, 150.0f, 190.0f};

  for (int i = 0; i < 2; i++) {
    for (int i = 1; i < 4; i++) {
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, h1, i), &H1);
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, h2, i), &H2);
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, 10, i), &C1);
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, m1, i), &M1);
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, m2, i), &M2);
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, 10, i), &C2);
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, s1, i), &S1);
      SDL_RenderTexture(pD->renderer, pD->texture, numbers(pD, s2, i), &S2);

      SDL_RenderPresent(pD->renderer);

      SDL_RenderClear(pD->renderer);

      if (i == 3) {
        SDL_Delay(168);
      } else {
        SDL_Delay(166);
      }
    }
  }
}

void timer(struct display *pD, int s) {
  while (s > 0 && !isthisclose(pD)) {
    int ho = s / 3600;
    int mi = (s % 3600) / 60;
    int se = ((s % 3600) % 60);

    displayer(pD, ho, mi, se);

    s--;
  }
}

void draw(struct display *pD, int s) {
  while (!isthisclose(pD)) {
    SDL_SetRenderDrawColor(pD->renderer, 255, 235, 207, 255);

    timer(pD, s);

    break;
  }
}

void quit(struct display *pD) {
  if (pD->texture) {
    SDL_DestroyTexture(pD->texture);
    pD->texture = NULL;
  }

  if (pD->renderer) {
    SDL_DestroyRenderer(pD->renderer);
    pD->renderer = NULL;
  }

  if (pD->window) {
    SDL_DestroyWindow(pD->window);
    pD->window = NULL;
  }

  SDL_Quit();
}
