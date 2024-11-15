#include <iostream>
#include <raylib.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;

// TIMER
typedef struct Timer
{
  float Lifetime;
};

void StartTimer(Timer* timer, float lifetime) {
  if(timer != nullptr) {
    timer->Lifetime = lifetime;
  }
}

void UpdateTimer(Timer* timer) {
  if(timer != nullptr && timer->Lifetime > 0) {
    timer->Lifetime = timer->Lifetime - GetFrameTime();
  }
}

bool TimerDone(Timer* timer) {
  if(timer != nullptr) {
    return timer->Lifetime <= 0;
  }
}


class Ball {
  public:
    float pos_x;
    float pos_y;
    int speed_x;
    int speed_y;
    int radius;
  
    void Draw(){
      DrawCircle(pos_x, pos_y, radius, WHITE); 
    };

    // Ball Collision
    void Update() {
      pos_x += speed_x;
      pos_y += speed_y;

      if(pos_y + radius >= GetScreenHeight() || pos_y - radius <= 0) {
        speed_y *= -1;
      }

      if (pos_x - radius <= 0) {
        player_score += 1;
        ResetBall(); 
  
      }
      else if (pos_x + radius > GetScreenWidth())
      {
        cpu_score += 1;
        ResetBall();
      }
      
    };

    void ResetBall() {
      
      pos_x = GetScreenWidth() / 2;
      pos_y = GetScreenHeight() / 2;
      int ball_speed[2] = {-1, 1};
      speed_x *= ball_speed[GetRandomValue(0,1)];
      speed_y *= ball_speed[GetRandomValue(0,1)];
    }
};

class Paddle {
  protected:
      
    // Check collision with screen
    void CheckCollisionWithScreen(){
      if (pos_y <= 0) {
        pos_y = 0;
      }

      if (pos_y + height >= GetScreenHeight()) {
        pos_y = GetScreenHeight() - height;
      }
    };

  public:
    float pos_x;
    float pos_y;
    float width;
    float height;
    int speed;

    void Draw() {
      DrawRectangle(pos_x, pos_y, width, height, WHITE);
    };

    void Update() {
      if(IsKeyDown(KEY_UP)) {
        pos_y = pos_y - speed;
      }
      else if(IsKeyDown(KEY_W)) {
        pos_y = pos_y - speed;
      }

      if(IsKeyDown(KEY_DOWN)) {
        pos_y = pos_y + speed;
      }
      else if (IsKeyDown(KEY_S)) {
        pos_y = pos_y + speed;
      }

      CheckCollisionWithScreen();
    }
};

class CPUPaddle : public Paddle {
  public:
    
    void Update(int ball_y) {
      if (pos_y + height / 2  > ball_y) {
        pos_y = pos_y - speed;
      }
      if (pos_y + height / 2 <= ball_y) {
        pos_y = pos_y + speed;
      }

      CheckCollisionWithScreen();  
    }
};

int main () {

    cout << "Game Started" << endl;

    // Initialization
    Ball ball;
    Paddle player;
    CPUPaddle cpu;
    
    // 1. Create a Blank Screen
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong!");
    SetTargetFPS(60);

    // Ball information
    ball.pos_x = screen_width / 2;
    ball.pos_y = screen_height / 2;
    ball.speed_x = 10;
    ball.speed_y = 10;
    ball.radius = 20;

    // Player information
    player.width = 20;
    player.height = 120;
    player.pos_x = screen_width - player.width - 10;
    player.pos_y = screen_height / 2 - (player.height / 2);
    player.speed = 9;

    // CPU information
    cpu.width = 20; 
    cpu.height = 120;
    cpu.pos_x = 10;
    cpu.pos_y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 9;
  

    // Game Loop
    while (WindowShouldClose() == false) {
      BeginDrawing();

      // Updating
      ball.Update();
      player.Update();
      cpu.Update(ball.pos_y);

      // Checking Collision for Ball to Paddles
      if (CheckCollisionCircleRec(Vector2{ball.pos_x, ball.pos_y}, ball.radius, Rectangle{player.pos_x, player.pos_y, player.width, player.height})) {
        ball.speed_x *= -1;
      }
      if (CheckCollisionCircleRec(Vector2{ball.pos_x, ball.pos_y}, ball.radius, Rectangle{cpu.pos_x, cpu.pos_y, cpu.width, cpu.height})) {
        ball.speed_x *= -1;
      }
      
      // Drawing
      ball.Draw();
      player.Draw();
      cpu.Draw();
      DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

      // Scoring
      DrawText(TextFormat("%i", cpu_score), screen_width / 2 - 200, 20, 80, LIGHTGRAY);
      DrawText(TextFormat("%i", player_score), screen_width / 2 + 200, 20, 80, LIGHTGRAY);

      ClearBackground(BLACK); // Reset gameloop so multiple images isn't being draw
      EndDrawing();
    }

    CloseWindow();
  return 0; 
}