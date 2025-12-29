#define PGL_EXCLUDE_STUBS
#define PORTABLEGL_IMPLEMENTATION
#include <portablegl.h>

#include <SDL.h>

#include <stdbool.h>
#include <stdlib.h>

typedef struct custom_uniforms
{
  int width;
  int height;
  float time;
} custom_uniforms;

void
vs(float* vs_out,
   vec4* vertex_attribs,
   Shader_Builtins* builtins,
   void* uniforms)
{
  builtins->gl_Position = vertex_attribs[0];
}

const vec4 white = { 1.0, 1.0, 1.0, 1.0 };
const vec4 pastelRed = { 0.78, 0.27, 0.25, 1.0 };
const vec4 lightPurple = { 0.49, 0.43, 0.70, 1.0 };
const vec4 softPurple = { 0.40, 0.37, 0.58, 1.0 };
const vec4 darkPurple = { 0.05, 0.05, 0.13, 1.0 };

// clang-format off
const float g[100] = {2.2596650169485044, 3.8819625262265145, 1.8071505524340659, 4.153109035464635, 1.6264745219354748, 3.7155079891953036, 3.0685501309939474, 1.0412453800231158, 5.3947116127988055, 4.2703797490641024, 0.016293776359206716, 2.7178409403110764, 2.9364169677424856, 6.192275931563852, 1.6520508485962895, 0.7839160529596241, 5.806377999043719, 3.3562203896765532, 6.247108620665236, 5.21880102452988, 5.174871579023225, 0.4372343707214709, 5.525898968499804, 0.7406854245869584, 5.170524954741027, 3.5207650629597045, 3.531002163366498, 0.03950630959846201, 3.8168222397036007, 0.6721860974311158, 1.163196142871406, 2.4914666569174195, 3.041182933947242, 0.9643962383381693, 2.7687814106508726, 1.418866626305463, 1.5859747118151395, 5.439685057626139, 4.707469707757047, 1.742048755418391, 1.3293700453661443, 0.7075715329372869, 1.7762876522625266, 4.956361433140513, 0.09524276744392365, 5.952328697521576, 2.52568069381271, 2.1771605739409403, 2.705258299297956, 2.6182696754710597, 4.387648043087818, 1.0793274091995555, 0.7385755877132956, 3.608540961528483, 0.9440887978089865, 2.566348196263037, 0.9425644578232383, 5.440153818458312, 3.36909526516092, 3.2168746566104316, 4.028916441509708, 0.9530763618036039, 1.3345918268678674, 3.4369483438675115, 5.5442417643941875, 6.112406145192881, 1.0424579608278413, 4.506812394807002, 0.8556497512385945, 5.5953598156962645, 4.803847365952192, 4.433989325180428, 1.5217214780440316, 4.461725674271762, 0.042445403085478436, 3.0956458415568107, 4.114563526746859, 2.691968765765688, 3.9990240338857834, 4.801902521442798, 1.5848566663554526, 5.295969832396129, 2.7751011760096493, 2.025201374982168, 3.8993076851907027, 2.5182023058271903, 3.7831696086589464, 0.17666140929255822, 1.4696143469423943, 0.255509145191265, 3.699653419268577, 1.027291827734274, 0.15986432236444773, 3.843365873759821, 0.7236937653854696, 4.441824727287969, 4.206854057702628, 5.504976648657879, 5.639413519036437, 6.234272087900348};
// clang-format on

float
S1(float v)
{
  return sin(0.5 + v * 10.0) * 0.1;
}

float
S2(float v)
{
  return sin(v * 10.0) * 0.1;
}

float
t(float x)
{
  return 6.0 * pow(x, 5.0) - 15.0 * pow(x, 4.0) + 10.0 * pow(x, 3.0);
}

float
P(float x, float y)
{
  const float s = 10.0;

  float X = fmod(s * x, 1.0);
  float Y = fmod(s * y, 1.0);

  int i1 = (int)fmod(floor(s * x), s) + s * (int)fmod(floor(s * y), s) + 1.0;
  int i2 =
    (int)fmod(floor(s * x) + 1.0, s) + s * (int)fmod(floor(s * y), s) + 1.0;
  int i3 =
    (int)fmod(floor(s * x), s) + s * (int)fmod(floor(s * y) + 1.0, s) + 1.0;
  int i4 = (int)fmod(floor(s * x) + 1.0, s) +
           s * (int)fmod(floor(s * y) + 1.0, s) + 1.0;

  float d1 = X * cos(g[i1]) + Y * sin(g[i1]);
  float d2 = (X - 1.0) * cos(g[i2]) + Y * sin(g[i2]);
  float d3 = X * cos(g[i3]) + (Y - 1.0) * sin(g[i3]);
  float d4 = (X - 1.0) * cos(g[i4]) + (Y - 1.0) * sin(g[i4]);

  return (d1 * (1.0 - t(X)) + d2 * t(X)) * (1.0 - t(Y)) +
         (d3 * (1.0 - t(X)) + d4 * t(X)) * t(Y);
}

void
fs(float* fs_in, Shader_Builtins* builtins, void* uniforms)
{
  custom_uniforms* p_uniforms = ((custom_uniforms*)uniforms);

  vec4 color = lightPurple;

  float x = builtins->gl_FragCoord.x / p_uniforms->width;
  float y = builtins->gl_FragCoord.y / p_uniforms->height;

  float a = p_uniforms->time;
  a = 1.0;

  float x1 = x;
  float y1 = y + S1(a);
  float y2 = y + S2(a);

  float smoke = 0.25 * P(x1 - a, y - a) + (x1 - 0.5) * (x1 - 0.5) +
                  (y2 - 0.5) * (y2 - 0.5) <
                0.14;
  if (smoke) {
    // color = softPurple;
  }

  float body = (x1 - 0.47) * (x1 - 0.47) + (y1 - 0.48) * (y1 - 0.48) < 0.06;
  if (body) {
    color = darkPurple;
  }

  float leftEye =
    ((x1 - 0.3) * (x1 - 0.3)) / (0.6 * 0.6) + ((y1 - 0.54) * (y1 - 0.54)) <
      0.022 &&
    y1 < 0.2 * S2(a) + 1.0 - 1.5 * x1;
  if (leftEye) {
    color = white;
  }

  float rightEye = ((x1 - 0.54) * (x1 - 0.54)) / (0.9 * 0.9) +
                       ((y1 - 0.54) * (y1 - 0.54)) / (1.0 * 1.0) <
                     0.022 &&
                   (y1 - 0.02 - S2(a) * 0.2) < x1;
  if (rightEye) {
    color = white;
  }

  float leftPupil = ((x1 - 0.305) * (x1 - 0.305)) / (1.0 * 1.0) +
                      ((y1 - 0.48) * (y1 - 0.48)) / (4.0 * 4.0) <
                    max(0.00002, S2(a) * 0.0004);
  if (leftPupil) {
    color = darkPurple;
  }

  float rightPupil = ((x1 - 0.516) * (x1 - 0.516)) / (1.0 * 1.0) +
                       ((y1 - 0.48) * (y1 - 0.48)) / (4.0 * 4.0) <
                     max(0.00002, S2(a) * 0.0004);
  if (rightPupil) {
    color = darkPurple;
  }

  float mouth = ((x1 - 0.45) * (x1 - 0.45)) / (1.1 * 1.1) +
                    ((y1 - 0.49) * (y1 - 0.49)) / (1.0 * 1.0) <
                  0.04 &&
                ((x1 - 0.45) * (x1 - 0.45)) / (1.5 * 1.5) +
                    ((y1 - 0.54) * (y1 - 0.54)) / (1.0 * 1.0) >
                  0.04 &&
                y1 - 0.32 < abs(3.0 * x1 - 0.91) &&
                y1 - 0.29 < abs(2.5 * x1 - 1.4);
  if (mouth) {
    color = pastelRed;
  }

  float leftTooth = y1 - 0.32 > abs(3.0 * x1 - 0.91) &&
                    ((x1 - 0.45) * (x1 - 0.45)) / (1.5 * 1.5) +
                        ((y1 - 0.54) * (y1 - 0.54)) / (1.0 * 1.0) >
                      0.04 &&
                    y1 < 0.5;
  if (leftTooth) {
    // color = white;
  }

  float rightTooth = y1 - 0.29 > abs(2.5 * x1 - 1.4) &&
                     ((x1 - 0.45) * (x1 - 0.45)) / (1.5 * 1.5) +
                         ((y1 - 0.54) * (y1 - 0.54)) / (1.0 * 1.0) >
                       0.04 &&
                     y1 < 0.5;
  if (rightTooth) {
    // color = white;
  }

  float rightEyeCircle =
    (x1 - 0.545) * (x1 - 0.545) + (y1 - 0.525) * (y1 - 0.525) < 0.022 &&
    (x1 - 0.54) * (x1 - 0.54) + (y1 - 0.545) * (y1 - 0.545) > 0.027;
  if (rightEyeCircle) {
    color = pastelRed;
  }

  builtins->gl_FragColor = color;
}

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

  float vertices[] = { -1.0, -1.0, 0.0, 1.0,  -1.0, 0.0,
                       1.0,  1.0,  0.0, -1.0, 1.0,  0.0 };

  GLuint triangle;
  glGenBuffers(1, &triangle);
  glBindBuffer(GL_ARRAY_BUFFER, triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(PGL_ATTR_VERT);
  glVertexAttribPointer(PGL_ATTR_VERT, 3, GL_FLOAT, GL_FALSE, 0, 0);

  GLuint shader = pglCreateProgram(vs, fs, 0, NULL, GL_FALSE);
  glUseProgram(shader);

  custom_uniforms uniforms = { 0 };
  pglSetUniform(&uniforms);
  uniforms.width = width;
  uniforms.height = height;

  while (true) {
    while (SDL_PollEvent(&event) > 0) {
      if (event.type == SDL_QUIT) {
        return EXIT_SUCCESS;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
          return EXIT_SUCCESS;
      }
    }

    uniforms.time = 0.15f * SDL_GetTicks();

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

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