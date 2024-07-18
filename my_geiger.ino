//Arduino Geiger counter

#include <avr/delay.h>

#define GEIGER_TIME 75 //время измерения, 75 секунд
#define ALARM_LEVEL 40 //уровень тревоги
#define TIMER1_PRELOAD 64910 //65535-64910=625, 15625/625=25Гц
#define TIMER2_PRELOAD 0
#define HVGEN_FACT 5 // 25/5=5Гц частота подкачки преобразователя
#define TIME_FACT 25 // 25Гц/25=1Гц секундные интервалы

uint16_t rad_buff[GEIGER_TIME]; //массив секундных замеров для расчета фона

uint16_t adc_key_val[5] = { 50, 200, 400, 600, 800 }; //значения АЦП для обработки кнопок


uint32_t rad_sum; //сумма импульсов за все время
uint32_t rad_back; //текущий фон
uint8_t time_sec; //секунды //счетчики времени
uint8_t time_min; //минуты
uint8_t time_hrs; //часы
uint8_t scr_mode; //режим
uint8_t alarm; //флаг тревоги
uint8_t alarm_disable; //флаг запрета тревоги


char str_buff[17];


void conv_pump(void);
void alarm_warning(void);


void conv_pump(void)

{  
  
  bitSet(PORTD, 3);
  _delay_us(10);
  bitClear(PORTD, 3);
  
}


void setup() {
  // put your setup code here, to run once:
  
//настраиваем Timer 1
TIMSK1=0; //отключить таймер
TCCR1A=0; //OC1A/OC1B disconnected
TCCR1B=0b00000101; //предделитель 16M/1024=15625кГц
TCNT1=TIMER1_PRELOAD;


//настраиваем Timer 2
TIMSK2=0;
TCCR2A=0;
TCCR2B=0;
TCNT2=TIMER2_PRELOAD;


//функция подкачки напряжения


for(uint16_t k=2000; k>0; k--) //преднакачка преобразователя

	{
	conv_pump();
	_delay_ms(1);

	}

}

void loop() {
// внешнее прирывавание котрое считает импульсы от счетчика
ISR(INT0_vect){  
  if(rad_buff[0] != 65535) rad_buff[0]++; //нулевой элемент-текущее измерение
  if (++rad_sum>999999UL*3600/GEIGER_TIME) rad_sum = rad_sum = 999999UL*3600/GEOIGER_TIME; //общая сумма импульсов
 
 conv_pump();
}


//прерывание по переполнению Timer 1
ISR(TIMER1_OVF_vect){
 
 static unit8_t cnt1;
 static unit8_t cnt2;
 
 TCNT1 = TIMER1_PRELOAD;

//подкачка преобразователя с заданной частотой
if(++cnt2 >= HVGEN_FACT){
 
 cnt2 = 0;
 conv_pomp();
   
}
 
 if(++cnt1 >= TIME_FACT){
 
 cnt1 = 0;
 unit32_t tmp_buff = 0;
 for (unir8_t i = 0; i < GEIGER_TIME; i++) tmp_buff+=rad_buff[i]; //расчет фона мкР/ч
 if (tmp_buff > 999999) tmp_buff = 999999;
 rad_back = tmp_buff;
 
 if (rad_back > rad_max) rad_max = rad
 }
}
}
