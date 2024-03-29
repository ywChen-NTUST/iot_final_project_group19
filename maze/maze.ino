#include "command.h"
#include "control.h"
#include "parameters.h"
#include "user.h"
#include "display.h"
#include "global.h"

Command command;
Control control(UP_PIN, DOWN_PIN, LEFT_PIN, RIGHT_PIN, ENTER_PIN);
Display display(
    I2C_SDA_PIN, I2C_SCL_PIN, 
    (u_char[]){LIFE1_PIN, LIFE2_PIN, LIFE3_PIN, LIFE4_PIN, LIFE5_PIN}, 
    (u_char[]){KEY1_PIN, KEY2_PIN, KEY3_PIN, KEY4_PIN, KEY5_PIN}
);
User users[3]; // 0 is never used
u_char myuid;
u_char keynum = 0;
maze_animation_type animeId = MAZE_ANIMATION_NONE;

static SemaphoreHandle_t mutex_user;
static SemaphoreHandle_t mutex_maze;
static SemaphoreHandle_t mutex_network;

void task_display(void* pvParameters)
{
    int pos_x[3], pos_y[3], life[3], keynum_copy;
    while (1)
    {
        for(int uid = 1; uid <= 2; uid++)
        {
            xSemaphoreTake(mutex_user, portMAX_DELAY);
            users[uid].getPos(pos_x[uid], pos_y[uid]);
            life[uid] = users[uid].getLife();
            keynum_copy = keynum;
            xSemaphoreGive(mutex_user);
        }

        display.showLife(life[myuid]);
        display.showKey(keynum_copy);
        
        if(keynum_copy == 5)
        {
            display.showEvent(MAZE_ANIMATION_WIN);
            vTaskDelay(100000);
        }
        if(life[myuid] == 0 || life[3-myuid] == 0)
        {
            display.showEvent(MAZE_ANIMATION_LOSS);
            vTaskDelay(100000);
        }
        if(animeId != MAZE_ANIMATION_NONE)
        {
            display.showEvent(animeId);
            animeId = MAZE_ANIMATION_NONE;
            vTaskDelay(5000);
        }

        xSemaphoreTake(mutex_maze, portMAX_DELAY);
        display.renderMaze(myuid, pos_x, pos_y);
        xSemaphoreGive(mutex_maze);

        vTaskDelay(200);
    }
}

void task_input(void* pvParameters)
{
    int curr_x, curr_y;
    int life[3], keynum_copy;
    while (1)
    {

        xSemaphoreTake(mutex_user, portMAX_DELAY);
        users[myuid].getPos(curr_x, curr_y);
        life[myuid] = users[myuid].getLife();
        life[3-myuid] = users[3-myuid].getLife();
        keynum_copy = keynum;
        xSemaphoreGive(mutex_user);

        if(keynum_copy == 5 || life[myuid] == 0 || life[3-myuid] == 0)
        {
            vTaskDelay(100000);
        }

        maze_control_type control_state = control.readControl();
        if(control_state != MAZE_CONTROL_NONE)
        {
            bool checks[4] = {true, true, true, true};
            xSemaphoreTake(mutex_maze, portMAX_DELAY);
            checks[0] = control.checkPos(curr_x, curr_y-1);
            checks[1] = control.checkPos(curr_x, curr_y+1);
            checks[2] = control.checkPos(curr_x-1, curr_y);
            checks[3] = control.checkPos(curr_x+1, curr_y);
            xSemaphoreGive(mutex_maze);

            if(control_state == MAZE_CONTROL_UP && checks[0])
            {
                Serial.println("Control: up");
                curr_y--;
            }
            else if(control_state == MAZE_CONTROL_DOWN && checks[1])
            {
                Serial.println("Control: down");
                curr_y++;
            }
            else if(control_state == MAZE_CONTROL_LEFT && checks[2])
            {
                Serial.println("Control: left");
                curr_x--;
            }
            else if(control_state == MAZE_CONTROL_RIGHT && checks[3])
            {
                Serial.println("Control: right");
                curr_x++;
            }

            if(control_state != MAZE_CONTROL_ENTER)
            {
                xSemaphoreTake(mutex_user, portMAX_DELAY);
                users[myuid].moveto(curr_x, curr_y);
                xSemaphoreGive(mutex_user);
            }

            maze_entity_type entity;
            xSemaphoreTake(mutex_maze, portMAX_DELAY);
            if(control_state == MAZE_CONTROL_ENTER)
            {
                Serial.println("Control: enter");
                entity = control.interact(curr_x, curr_y);
            }
            else
                entity = control.checkEntity(curr_x, curr_y);
            xSemaphoreGive(mutex_maze);

            if (entity != MAZE_ENTITY_NONE)
            {
                //Serial.printf("Entity %d\n", entity);
                animeId = (maze_animation_type)entity;
                xSemaphoreTake(mutex_network, portMAX_DELAY);
                command.doEntity(entity, curr_x, curr_y);
                xSemaphoreGive(mutex_network);
                vTaskDelay(5000);
            }
        }

        vTaskDelay(200);
    }
}

void task_upload(void* pvParameters)
{
    int pos_x_prev = 1, pos_y_prev = 1;
    int pos_x, pos_y;
    int life[3], keynum_copy;
    while (1)
    {
        xSemaphoreTake(mutex_user, portMAX_DELAY);
        users[myuid].getPos(pos_x, pos_y);
        life[myuid] = users[myuid].getLife();
        life[3-myuid] = users[3-myuid].getLife();
        keynum_copy = keynum;
        xSemaphoreGive(mutex_user);

        if(keynum_copy == 5 || life[myuid] == 0 || life[3-myuid] == 0)
        {
            vTaskDelay(100000);
        }

        if (pos_x != pos_x_prev || pos_y != pos_y_prev)
        {
            xSemaphoreTake(mutex_network, portMAX_DELAY);
            command.move(pos_x, pos_y);
            xSemaphoreGive(mutex_network);
            pos_x_prev = pos_x;
            pos_y_prev = pos_y;
        }

        vTaskDelay(100);
    }
}

void task_download(void *pvParameters)
{
    while (1)
    {

        xSemaphoreTake(mutex_network, portMAX_DELAY);
        xSemaphoreTake(mutex_user, portMAX_DELAY);
        u_char keynum_temp = command.getKeyNum();
        if (keynum_temp != (u_char)-1)
            keynum = keynum_temp;
        //Serial.printf("server Key: %d\n", keynum);
        xSemaphoreGive(mutex_user);
        xSemaphoreGive(mutex_network);

        int life_copy[3];
        xSemaphoreTake(mutex_user, portMAX_DELAY);
        life_copy[myuid] = users[myuid].getLife();
        life_copy[3-myuid] = users[3-myuid].getLife();
        xSemaphoreGive(mutex_user);
        if(keynum == 5 || life_copy[myuid] == 0 || life_copy[3-myuid] == 0)
        {
            vTaskDelay(100000);
        }

        xSemaphoreTake(mutex_network, portMAX_DELAY);
        xSemaphoreTake(mutex_maze, portMAX_DELAY);
        command.syncMaze();
        xSemaphoreGive(mutex_maze);
        xSemaphoreGive(mutex_network);

        int pos_x_curr, pos_y_curr, life_curr;
        int pos_x, pos_y, life;
        for(int uid = 1; uid <= 2; uid++)
        {
            xSemaphoreTake(mutex_network, portMAX_DELAY);
            bool success = command.getUserAttribute(uid, pos_x, pos_y, life);
            xSemaphoreGive(mutex_network);

            if (success)
            {
                xSemaphoreTake(mutex_user, portMAX_DELAY);
                users[uid].getPos(pos_x_curr, pos_y_curr);
                life_curr = users[uid].getLife();
                if (pos_x != pos_x_curr || pos_y != pos_y_curr || life != life_curr)
                {
                    if(uid != myuid)
                        users[uid].moveto(pos_x, pos_y);
                    users[uid].setLife(life);
                }
                xSemaphoreGive(mutex_user);
            }

            // if (!success)
            //     Serial.printf("Failed to get user %d attribute\n", uid);
        }

        vTaskDelay(50);
    }
}

void setup() {
    Serial.begin(115200);
    maze_init();
    control.init();
    
    display.init(20, 4);
    display.showLife(0);
    display.showKey(0);

    command.init(SSID, PASSWORD, SERVER, PORT);

    display.showEvent(MAZE_ANIMATION_COVERPAGE);
    while(true)
    {
        // wait for user input enter btn
        maze_control_type control_state = control.readControl();
        if(control_state == MAZE_CONTROL_ENTER)
            break;
        delay(50);
    }
    display.lcdClear();
    display.lcdMsg("Creating user...", 2, 1);

    bool success = command.createUser(myuid);
    if (!success) error(command._socket, "Create user failed");

    mutex_user = xSemaphoreCreateMutex();
    mutex_network = xSemaphoreCreateMutex();
    mutex_maze = xSemaphoreCreateMutex();

    xTaskCreate(task_display, "task_display", 4000, NULL, 1, NULL);
    xTaskCreate(task_download, "task_download", 4000, NULL, 2, NULL);
    xTaskCreate(task_input, "task_input", 4000, NULL, 3, NULL);
    xTaskCreate(task_upload, "task_upload", 4000, NULL, 4, NULL);
}

void loop() {}