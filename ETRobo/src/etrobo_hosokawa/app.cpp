#include "ev3api.h"
#include "app.h"
#include "balancer.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/**
 * センサー、モーターの接続を定義します
 */
static const sensor_port_t
    touch_sensor    = EV3_PORT_1,
    sonar_sensor    = EV3_PORT_2,
    color_sensor    = EV3_PORT_3,
    gyro_sensor     = EV3_PORT_4;

static const motor_port_t
    left_motor      = EV3_PORT_C,
    right_motor     = EV3_PORT_B,
    tail_motor      = EV3_PORT_A;

static int      bt_cmd = 0;     /* Bluetoothコマンド 1:リモートスタート */
static FILE     *bt = NULL;     /* Bluetoothファイルハンドル */

#define GYRO_OFFSET  0          /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define LIGHT_WHITE  0         /* 白色の光センサ値 */
#define LIGHT_BLACK  0          /* 黒色の光センサ値 */
#define LIGHT_GRAY  0          /* 灰色の光センサ値 */
#define KP 0.38


#define TAIL_ANGLE_STAND_UP  93 /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */

//#define DEVICE_NAME     "ET0"  /* Bluetooth名 hrp2/target/ev3.h BLUETOOTH_LOCAL_NAMEで設定 */
//#define PASS_KEY        "1234" /* パスキー    hrp2/target/ev3.h BLUETOOTH_PIN_CODEで設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

/* 関数プロトタイプ宣言 */
static void tail_control(signed int angle);

#include "BalancerCpp.h"        // <1>
Balancer balancer;              // <1>

/* メインタスク */
void main_task(intptr_t unused)
{
    signed char forward;      /* 前後進命令 */
    signed char turn;         /* 旋回命令 */
    signed char pwm_L, pwm_R; /* 左右モータPWM出力 */

    /* LCD画面表示 */
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    ev3_lcd_draw_string("ミヤウチ", 0, CALIB_FONT_HEIGHT*1);

    /* センサー入力ポートの設定 */
    ev3_sensor_config(sonar_sensor, ULTRASONIC_SENSOR);
    ev3_sensor_config(color_sensor, COLOR_SENSOR);
    ev3_color_sensor_get_reflect(color_sensor); /* 反射率モード */
    ev3_sensor_config(touch_sensor, TOUCH_SENSOR);
    ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
    /* モーター出力ポートの設定 */
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_motor_config(tail_motor, LARGE_MOTOR);
    ev3_motor_reset_counts(tail_motor);
    
     //キャリブレイト 白取得
    printf("Press the touch sensor to measure light intensity of WHITE.\n");
    while(!ev3_touch_sensor_is_pressed(touch_sensor));
    while(ev3_touch_sensor_is_pressed(touch_sensor));
    int white = ev3_color_sensor_get_reflect(color_sensor);
    printf("WHITE light intensity: %d.\n", white);

    //キャリブレイト 黒取得
    printf("Press the touch sensor to measure light intensity of BLACK.\n");
    while(!ev3_touch_sensor_is_pressed(touch_sensor));
    while(ev3_touch_sensor_is_pressed(touch_sensor));
    int black = ev3_color_sensor_get_reflect(color_sensor);
    printf("BLACK light intensity: %d.\n", black);

    //PID制御
    float lasterror = 0, integral = 0;
    float midpoint = (white - black) / 2 + black;

    /* Open Bluetooth file */
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);

    /* Bluetooth通信タスクの起動 */
    act_tsk(BT_TASK);

    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */

    /* スタート待機 */
    while(1)
    {
    //尻尾さげる
        tail_control(TAIL_ANGLE_STAND_UP); 

        if (bt_cmd == 1)
        {
            //リモートスタート
            break; 
        }

        if (ev3_touch_sensor_is_pressed(touch_sensor) == 1)
        {
            //タッチセンサが押された
            break; 
        }

        tslp_tsk(1);
    }

    /* 走行モーターエンコーダーリセット */
    ev3_motor_reset_counts(left_motor);
    ev3_motor_reset_counts(right_motor);

    /* ジャイロセンサーリセット */
    ev3_gyro_sensor_reset(gyro_sensor);
    balancer.init(GYRO_OFFSET);                // <1>

	/* スタート（LED緑色） */
    ev3_led_set_color(LED_GREEN); 

    while(1)
	{
        int32_t motor_ang_l, motor_ang_r;
        int gyro, volt;

        if (ev3_button_is_pressed(BACK_BUTTON)) break;

        tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */

        float error = midpoint - ev3_color_sensor_get_reflect(color_sensor);
        integral = error + integral * 0.5;
        turn = 0.07 * error + 0.3 * integral + 1 * (error - lasterror);
           // float steer = 0.07 * error + 0.3 * integral + 1 * (error - lasterror);
           // ev3_motor_steer(left_motor, right_motor, 10, steer);
        lasterror = error;
        tslp_tsk(1);

        /* 倒立振子制御API に渡すパラメータを取得する */
        motor_ang_l = ev3_motor_get_counts(left_motor);
        motor_ang_r = ev3_motor_get_counts(right_motor);
        gyro = ev3_gyro_sensor_get_rate(gyro_sensor);
        volt = ev3_battery_voltage_mV();

        /* 倒立振子制御APIを呼び出し、倒立走行するための */
        /* 左右モータ出力値を得る */
        balancer.setCommand(forward, turn);   // <1>
        balancer.update(gyro, motor_ang_r, motor_ang_l, volt); // <2>
        pwm_L = balancer.getPwmRight();       // <3>
        pwm_R = balancer.getPwmLeft();        // <3>

        /* EV3ではモーター停止時のブレーキ設定が事前にできないため */
        /* 出力0時に、その都度設定する */
        if (pwm_L == 0)
        {
             ev3_motor_stop(left_motor, true);
        }
        else
        {
            ev3_motor_set_power(left_motor, (int)pwm_L);
        }

        if (pwm_R == 0)
        {
             ev3_motor_stop(right_motor, true);
        }
        else
        {
         ev3_motor_set_power(right_motor, (int)pwm_R);
        }
	

        tslp_tsk(4); /* 4msec周期起動 */
	}
    ev3_motor_stop(left_motor, false);
    ev3_motor_stop(right_motor, false);

    ter_tsk(BT_TASK);
    fclose(bt);

    ext_tsk();
}

//尻尾制御
static void tail_control(signed int angle)
{
    float pwm = (float)(angle - ev3_motor_get_counts(tail_motor))*P_GAIN; 
    /* PWM出力飽和処理 */
    if (pwm > PWM_ABS_MAX)
    {
        pwm = PWM_ABS_MAX;
    }
    else if (pwm < -PWM_ABS_MAX)
    {
        pwm = -PWM_ABS_MAX;
    }

    if (pwm == 0)
    {
        ev3_motor_stop(tail_motor, true);
    }
    else
    {
        ev3_motor_set_power(tail_motor, (signed char)pwm);
    }
}

//リモートスタート( TeraTermからASCIIコードで1を送信すると、リモートスタートする。)
void bt_task(intptr_t unused)
{
    while(1)
    {
        uint8_t c = fgetc(bt); /* 受信 */
        switch(c)
        {
        case '1':
            bt_cmd = 1;
            break;
        default:
            break;
        }
        fputc(c, bt);
    }
}