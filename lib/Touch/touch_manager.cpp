#include "touch_manager.h"
/*******************************************************************************
 * Touch libraries:
 * GT911: https://github.com/TAMCTec/gt911-arduino.git
 ******************************************************************************/

#include <Wire.h>
#include <TAMC_GT911.h>

#define TOUCH_GT911
#define TOUCH_GT911_SCL 45
#define TOUCH_GT911_SDA 19
#define TOUCH_GT911_INT 255
#define TOUCH_GT911_RST 255
#define TOUCH_GT911_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

int touch_last_x = 0, touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));
void touch_init_lvgl()
{
  Serial.println("Inicializando touch LVGL...");
  delay(1000);
  touch_init();
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touch_read;
  lv_indev_t *touch_input = lv_indev_drv_register(&indev_drv);

  if (touch_input == NULL)
  {
    Serial.println("Erro: Falha ao registrar driver de touch!");
  }
  else
  {
    Serial.println("Driver de touch registrado com sucesso!");
  }
}

/* void touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_touched())
  {
    data->point.x = touch_last_x;
    data->point.y = touch_last_y;
    data->state = LV_INDEV_STATE_PR;
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
} */

void touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  // Serial.println("Executando touch_read()...");  // Depuração
  if (touch_has_signal())
  { // Verifica se há um toque detectado
    if (touch_touched())
    { // Confirma que o toque está pressionado
      //   Serial.println("Touch detectado!");
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
      data->state = LV_INDEV_STATE_PR; // Define o estado como pressionado
    }
    else if (touch_released())
    {                                   // Confirma que o toque foi liberado
                                        //   Serial.println("Touch liberado!");
      data->state = LV_INDEV_STATE_REL; // Define o estado como liberado
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL; // Se não houver sinal, mantém como liberado
  }
}

void touch_init()
{
  Wire.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);
  ts.begin();
  // ts.setRotation(TOUCH_GT911_ROTATION);
}

bool touch_has_signal()
{
  return true;
}

bool touch_touched()
{

  ts.read();
  if (ts.isTouched)
  {
#if defined(TOUCH_SWAP_XY)
    touch_last_x = map(ts.points[0].y, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, 480 - 1);
    touch_last_y = map(ts.points[0].x, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, 480 - 1);
#else
    touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, 480 - 1);
    touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, 480 - 1);
#endif
    return true;
  }
  else
  {
    return false;
  }
}

bool touch_released()
{
  return true;
}
