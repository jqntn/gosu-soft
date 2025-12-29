#define PGL_EXCLUDE_STUBS
#define PORTABLEGL_IMPLEMENTATION
#include <portablegl.h>

#include <SDL.h>

#include <stdbool.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
  int width = 800;
  int height = 800;

  SDL_Window* p_window = NULL;
  SDL_Renderer* p_renderer = NULL;
  SDL_Texture* p_texture = NULL;
  SDL_Event event = { 0 };

  glContext glcontext = { 0 };
  uint32_t* p_backbuffer = NULL;

  SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_ShowSimpleMessageBox(
      SDL_MESSAGEBOX_ERROR, "SDL_GetError", SDL_GetError(), NULL);
    return EXIT_FAILURE;
  }

  p_window = SDL_CreateWindow(u8"ゴース",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width,
                              height,
                              SDL_WINDOW_SHOWN);
  p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_SOFTWARE);
  p_texture = SDL_CreateTexture(p_renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                width,
                                height);

  if (init_glContext(&glcontext,
                     &p_backbuffer,
                     width,
                     height,
                     32,
                     0xFF000000,
                     0x00FF0000,
                     0x0000FF00,
                     0x000000FF) == GL_FALSE) {
    char e[CHAR_MAX];
    sprintf(e, "%d", glGetError());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "glGetError", e, NULL);
    return EXIT_FAILURE;
  }

  float vertices[] = { -0.5, -0.5, 0, 0.5, -0.5, 0, 0, 0.5, 0 };

  GLuint triangle;
  glGenBuffers(1, &triangle);
  glBindBuffer(GL_ARRAY_BUFFER, triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(PGL_ATTR_VERT);
  glVertexAttribPointer(PGL_ATTR_VERT, 3, GL_FLOAT, GL_FALSE, 0, 0);

  GLuint shaders[PGL_NUM_SHADERS];
  pgl_init_std_shaders(shaders);
  glUseProgram(shaders[PGL_SHADER_IDENTITY]);

  pgl_uniforms uniforms = { 0 };
  pglSetUniform(&uniforms);
  uniforms.color = (vec4){ 1.0f, 0.0f, 0.0f, 1.0f };

  while (true) {
    while (SDL_PollEvent(&event) > 0) {
      if (event.type == SDL_QUIT) {
        return EXIT_SUCCESS;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
          return EXIT_SUCCESS;
      }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_UpdateTexture(p_texture, NULL, p_backbuffer, width * sizeof(uint32_t));
    SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
    SDL_RenderPresent(p_renderer);
  }

  free_glContext(&glcontext);
  SDL_DestroyTexture(p_texture);
  SDL_DestroyRenderer(p_renderer);
  SDL_DestroyWindow(p_window);
  SDL_Quit();

  return EXIT_SUCCESS;
}