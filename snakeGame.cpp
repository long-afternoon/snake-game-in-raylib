#include <iostream>
#include "include\raylib.h"
#include <deque>
#include "include\raymath.h"


using namespace std;

Color blue = { 189, 182, 186, 2};
Color black = {43, 56, 24, 255};

int cs = 30;
int cc = 25;
int Runing = 1;

double updateTime = 0;

int intervel(double time) {
    double current = GetTime();

    if(current - updateTime >= time) {
        updateTime = current;
        return 1;
    }
    return 0;
}


class Snake {
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1, 0}; //default


    void Draw() {

        for(int i = 0; i < body.size(); i++){
            
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x*cs, y*cs, (float)cs, (float)cs};
            DrawRectangleRounded(segment, 0.5, 6, BLACK);
            
          
        }
    }
    
    void Update() {
        if(Runing) {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
    }

    void Grow() {
        body.push_front(Vector2Add(body[0], direction));
    }
    

};




class Food {
public:
    Texture2D texture;
    Vector2 pos;


    Food() {
        Image image = LoadImage("img/food1.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        pos = RandomPos();

    }
    ~Food() {
        UnloadTexture(texture);
    }

    Vector2 RandomPos(){
        float x = GetRandomValue(2, cc-2);
        float y = GetRandomValue(0, cc-2);
        return Vector2{x, y};
    }
 

    void Draw() {
        DrawTexture( texture, pos.x*cs, pos.y*cs, WHITE);
    }


};

class Game {
public:
    Snake snake;
    Food food;
    int score;
    void Draw() {
        snake.Draw();
        food.Draw();
    }
    void Reset() {
        food.pos = food.RandomPos();

        snake.body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        snake.direction = {1, 0};
    }
    void GameOver() {
        Reset();
        cout << "Game Over!" << endl;
        Runing = 0;
        score = 0;
    }
    void Update() {
        snake.Update();
        collision();
        snakeCollision();
        edgeCollision();

    }
    void collision() { // if food and snake head collides increases score and body;!
        if(Vector2Equals( food.pos, snake.body[0])) {
            cout << "hit!" << endl;
            food.pos = food.RandomPos();
            snake.Grow();
            score++;
        }
    }
    void edgeCollision() {
        if(snake.body[0].x == 24) {
            snake.body[0].x = 23;
            GameOver();
        }
        if(snake.body[0].y == 24) {
            snake.body[0].y = 23;
            GameOver();
        }
        if(snake.body[0].x == -1) {
            snake.body[0].x = 0;
            GameOver();
        }
        if(snake.body[0].y == -1) {
            snake.body[0].y = 0;
            GameOver(); 
        }

    }
    void snakeCollision() { //if snake head and body collides calls gameover;
        deque<Vector2> body = snake.body;
        body.pop_front();

        for(int i = 0; i < body.size(); i++) {
            if(Vector2Equals(snake.body[0], body[i])) {
                GameOver();
                
            }
        }
    }


};





int main() {
    InitWindow(720, 720, "fuck yu");
    SetTargetFPS(60);
    Game game;

    while(WindowShouldClose() == 0){

        BeginDrawing();
        DrawText(TextFormat("Score : %i", game.score), 20, 20, 20, BLACK);
        int simulPress = 1;
        if(intervel(0.1)){
            game.Update();
            simulPress = 1;

        }
        if(IsKeyPressed(0x57) && game.snake.direction.y != 1 && simulPress){ // W
            game.snake.direction = {0, -1};
            simulPress = 0;
        }
        else if(IsKeyPressed(0x44) && game.snake.direction.x != -1 && simulPress){ // D
            game.snake.direction = {1, 0};
            simulPress = 0;
        }
        else if(IsKeyPressed(0x53) && game.snake.direction.y != -1 && simulPress){ // s
            game.snake.direction = {0, 1};
            simulPress = 0;
        }
        else if(IsKeyPressed(0x41) && game.snake.direction.x != 1 && simulPress){ // A
            game.snake.direction = {-1, 0};
            simulPress = 0;
        }
        else if(IsKeyPressed(KEY_SPACE)) {
            switch(Runing){
                case 0:
                    Runing = 1;
                    break;
                case 1:
                    Runing = 0;
                    break;
            }
        }

        ClearBackground(blue);

        game.Draw();

        EndDrawing();
       
    }
    CloseWindow();


}


// g++ text.cpp -o a.exe -I include -L lib -lraylib -lopengl32 -lwinmm -lgdi32
