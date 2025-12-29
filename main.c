#define PGL_EXCLUDE_STUBS
#define PORTABLEGL_IMPLEMENTATION
#include <portablegl.h>

#include <SDL.h>

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

const vec4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
const vec4 pastelRed = { 0.78f, 0.27f, 0.25f, 1.0f };
const vec4 lightPurple = { 0.49f, 0.43f, 0.70f, 1.0f };
const vec4 softPurple = { 0.40f, 0.37f, 0.58f, 1.0f };
const vec4 darkPurple = { 0.05f, 0.05f, 0.13f, 1.0f };

// clang-format off
const float g[100] = {2.2596650169485044f, 3.8819625262265145f, 1.8071505524340659f, 4.153109035464635f, 1.6264745219354748f, 3.7155079891953036f, 3.0685501309939474f, 1.0412453800231158f, 5.3947116127988055f, 4.2703797490641024f, 0.016293776359206716f, 2.7178409403110764f, 2.9364169677424856f, 6.192275931563852f, 1.6520508485962895f, 0.7839160529596241f, 5.806377999043719f, 3.3562203896765532f, 6.247108620665236f, 5.21880102452988f, 5.174871579023225f, 0.4372343707214709f, 5.525898968499804f, 0.7406854245869584f, 5.170524954741027f, 3.5207650629597045f, 3.531002163366498f, 0.03950630959846201f, 3.8168222397036007f, 0.6721860974311158f, 1.163196142871406f, 2.4914666569174195f, 3.041182933947242f, 0.9643962383381693f, 2.7687814106508726f, 1.418866626305463f, 1.5859747118151395f, 5.439685057626139f, 4.707469707757047f, 1.742048755418391f, 1.3293700453661443f, 0.7075715329372869f, 1.7762876522625266f, 4.956361433140513f, 0.09524276744392365f, 5.952328697521576f, 2.52568069381271f, 2.1771605739409403f, 2.705258299297956f, 2.6182696754710597f, 4.387648043087818f, 1.0793274091995555f, 0.7385755877132956f, 3.608540961528483f, 0.9440887978089865f, 2.566348196263037f, 0.9425644578232383f, 5.440153818458312f, 3.36909526516092f, 3.2168746566104316f, 4.028916441509708f, 0.9530763618036039f, 1.3345918268678674f, 3.4369483438675115f, 5.5442417643941875f, 6.112406145192881f, 1.0424579608278413f, 4.506812394807002f, 0.8556497512385945f, 5.5953598156962645f, 4.803847365952192f, 4.433989325180428f, 1.5217214780440316f, 4.461725674271762f, 0.042445403085478436f, 3.0956458415568107f, 4.114563526746859f, 2.691968765765688f, 3.9990240338857834f, 4.801902521442798f, 1.5848566663554526f, 5.295969832396129f, 2.7751011760096493f, 2.025201374982168f, 3.8993076851907027f, 2.5182023058271903f, 3.7831696086589464f, 0.17666140929255822f, 1.4696143469423943f, 0.255509145191265f, 3.699653419268577f, 1.027291827734274f, 0.15986432236444773f, 3.843365873759821f, 0.7236937653854696f, 4.441824727287969f, 4.206854057702628f, 5.504976648657879f, 5.639413519036437f, 6.234272087900348f};
// clang-format on

float
S1(float v)
{
  return sin(0.5f + v * 10.0f) * 0.1f;
}

float
S2(float v)
{
  return sin(v * 10.0f) * 0.1f;
}

float
t(float x)
{
  return 6.0f * pow(x, 5.0f) - 15.0f * pow(x, 4.0f) + 10.0f * pow(x, 3.0f);
}

float
P(float x, float y)
{
  const float s = 10.0f;

  float X = fmod(s * x, 1.0f);
  float Y = fmod(s * y, 1.0f);

  int i1 = (int)fmod(floor(s * x), s) + s * (int)fmod(floor(s * y), s) + 1.0f;
  int i2 =
    (int)fmod(floor(s * x) + 1.0f, s) + s * (int)fmod(floor(s * y), s) + 1.0f;
  int i3 =
    (int)fmod(floor(s * x), s) + s * (int)fmod(floor(s * y) + 1.0f, s) + 1.0f;
  int i4 = (int)fmod(floor(s * x) + 1.0f, s) +
           s * (int)fmod(floor(s * y) + 1.0f, s) + 1.0f;

  float d1 = X * cos(g[i1]) + Y * sin(g[i1]);
  float d2 = (X - 1.0f) * cos(g[i2]) + Y * sin(g[i2]);
  float d3 = X * cos(g[i3]) + (Y - 1.0f) * sin(g[i3]);
  float d4 = (X - 1.0f) * cos(g[i4]) + (Y - 1.0f) * sin(g[i4]);

  return (d1 * (1.0f - t(X)) + d2 * t(X)) * (1.0f - t(Y)) +
         (d3 * (1.0f - t(X)) + d4 * t(X)) * t(Y);
}

void
fs(float* fs_in, Shader_Builtins* builtins, void* uniforms)
{
  custom_uniforms* p_uniforms = ((custom_uniforms*)uniforms);

  vec4 color = lightPurple;

  float x = builtins->gl_FragCoord.x / p_uniforms->width;
  float y = builtins->gl_FragCoord.y / p_uniforms->height;

  printf("%f, %f\n", x, y);

  float a = p_uniforms->time;

  float x1 = x;
  float y1 = y + S1(a);
  float y2 = y + S2(a);

  int smoke = 0.25f * P(x1 - a, y - a) + (x1 - 0.5f) * (x1 - 0.5f) +
                (y2 - 0.5f) * (y2 - 0.5f) <
              0.14f;
  if (smoke) {
    // color = softPurple;
  }

  int body = (x1 - 0.47f) * (x1 - 0.47f) + (y1 - 0.48f) * (y1 - 0.48f) < 0.06f;
  if (body) {
    color = darkPurple;
  }

  int leftEye = ((x1 - 0.3f) * (x1 - 0.3f)) / (0.6f * 0.6f) +
                    ((y1 - 0.54f) * (y1 - 0.54f)) <
                  0.022f &&
                y1 < 0.2f * S2(a) + 1.0f - 1.5f * x1;
  if (leftEye) {
    color = white;
  }

  int rightEye = ((x1 - 0.54f) * (x1 - 0.54f)) / (0.9f * 0.9f) +
                     ((y1 - 0.54f) * (y1 - 0.54f)) / (1.0f * 1.0f) <
                   0.022f &&
                 (y1 - 0.02f - S2(a) * 0.2f) < x1;
  if (rightEye) {
    color = white;
  }

  int leftPupil = ((x1 - 0.305f) * (x1 - 0.305f)) / (1.0f * 1.0f) +
                    ((y1 - 0.48f) * (y1 - 0.48f)) / (4.0f * 4.0f) <
                  max(0.00002f, S2(a) * 0.0004f);
  if (leftPupil) {
    color = darkPurple;
  }

  int rightPupil = ((x1 - 0.516f) * (x1 - 0.516f)) / (1.0f * 1.0f) +
                     ((y1 - 0.48f) * (y1 - 0.48f)) / (4.0f * 4.0f) <
                   max(0.00002f, S2(a) * 0.0004f);
  if (rightPupil) {
    color = darkPurple;
  }

  int mouth = ((x1 - 0.45f) * (x1 - 0.45f)) / (1.1f * 1.1f) +
                  ((y1 - 0.49f) * (y1 - 0.49f)) / (1.0f * 1.0f) <
                0.04f &&
              ((x1 - 0.45f) * (x1 - 0.45f)) / (1.5f * 1.5f) +
                  ((y1 - 0.54f) * (y1 - 0.54f)) / (1.0f * 1.0f) >
                0.04f &&
              y1 - 0.32f < abs(3.0f * x1 - 0.91f) &&
              y1 - 0.29f < abs(2.5f * x1 - 1.4f);
  if (mouth) {
    color = pastelRed;
  }

  int leftTooth = y1 - 0.32f > abs(3.0f * x1 - 0.91f) &&
                  ((x1 - 0.45f) * (x1 - 0.45f)) / (1.5f * 1.5f) +
                      ((y1 - 0.54f) * (y1 - 0.54f)) / (1.0f * 1.0f) >
                    0.04f &&
                  y1 < 0.5f;
  if (leftTooth) {
    // color = white;
  }

  int rightTooth = y1 - 0.29f > abs(2.5f * x1 - 1.4f) &&
                   ((x1 - 0.45f) * (x1 - 0.45f)) / (1.5f * 1.5f) +
                       ((y1 - 0.54f) * (y1 - 0.54f)) / (1.0f * 1.0f) >
                     0.04f &&
                   y1 < 0.5f;
  if (rightTooth) {
    // color = white;
  }

  int rightEyeCircle =
    (x1 - 0.545f) * (x1 - 0.545f) + (y1 - 0.525f) * (y1 - 0.525f) < 0.022f &&
    (x1 - 0.54f) * (x1 - 0.54f) + (y1 - 0.545f) * (y1 - 0.545f) > 0.027f;
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

  float vertices[] = { -1.0f, -1.0f, 0.0f, 1.0f,  -1.0f, 0.0f,
                       1.0f,  1.0f,  0.0f, -1.0f, 1.0f,  0.0f };

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

  for (;;) {
    while (SDL_PollEvent(&event) > 0) {
      if (event.type == SDL_QUIT) {
        return EXIT_SUCCESS;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
          return EXIT_SUCCESS;
      }
    }

    uniforms.time = 0.15f * (SDL_GetTicks() / 1000.0f);

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