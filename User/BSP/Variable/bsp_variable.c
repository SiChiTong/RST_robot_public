#include "bsp_variable.h"


/*初始化所有变量*/

struct battery Battery = {0,0,0,0,0,0,0,0,0};
struct modbustx Modbustx = {0,0,0,0};
struct robot Robot = {0,0,1,10,0,0,0,0};
struct usart Usart485 = {{0},0};
struct adc ADC_V = {0,0,0};
struct wifi WIFI={{0},{0},{0},{0},0,0,0};


/*网络状态结构体*/


