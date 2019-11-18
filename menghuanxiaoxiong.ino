const uint8_t input_table[12] = {30,31,32,33,34,35,36,37,38,39,40,41};//循迹模块引脚号
const uint8_t  left_motor_pin1=44,left_motor_pin2=45,right_motor_pin1=47,right_motor_pin2=46;  //TB6612电机驱动引脚号
int setup1=48;//TB6612电机驱动使能引脚
const uint8_t pwmpin1 = 9,pwmpin2 = 8;//TB6612电机控速PWM引脚
unsigned long lastTime;//程序上次记录时间
double Input, Output, Setpoint=0;//输入，输出，期望值
double errSum, lastErr;//积分段需要的误差累计值，微分段需要 保存的上个周期误差值
double kp=10, ki=0.01, kd=50; //PID系数
int SampleTime=100;
int x_last;//上次检测到的偏差值

double ITerm,lastInput; //ITerm为PID计算的I项输出，lastInput为上个周期的PID输入值
struct PWMVALUE //电机速度结构体
{
  int Left = 0;
  int Right = 0;
  int base = 80;//速度基量
  int inc = 0; //速度增量
};
PWMVALUE PWM;

int x = 0,x1=0,x2=0,x3=0,x4=0,count=0,b=0;//第X个循迹模块检测到黑线  b为第b次检测到黑线（即出发区和十字路口）count为同时读取到黑线的传感器数量
                                          //X1为从左向右读取传感器  X2为从右向左读取传感器  两种区别为 当识别到黑线时 停止向后读取传感器的值 
                                          //这样处理 当同时检测到两条黑线时  就可以控制小车向左还是向右
                                          //x3，x4 分别为x1，x2的绝对值 用来比较大小
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(left_motor_pin1,OUTPUT);
pinMode(left_motor_pin2,OUTPUT);
pinMode(right_motor_pin1,OUTPUT);
pinMode(right_motor_pin2,OUTPUT);
pinMode(pwmpin1,OUTPUT);
pinMode(pwmpin2,OUTPUT);
digitalWrite(48,HIGH);
  for (int i=0;i<12;i++)
  {
    pinMode(input_table[i],INPUT);
  }
}

void loop() {
  if(b==7)
  {
digitalWrite(setup1,LOW);//第七次遇到黑线 停车
  }
    //巡线
    for (int i=0;i<12;i++)//从左向右读取传感器的值
    {
      if (digitalRead(input_table[i])==HIGH)//依次读取传感器的值
      {
        //找到黑线x即误差=>用于比例计算
        x1= i - 6;
        count++;
      }
      //break
    };
    for(int k=11;k>=0;k--)//从右向左读取传感器的值
    {
     if (digitalRead(input_table[k])==HIGH)
      {
        //找到黑线x即误差=>用于比例计算
        x2= k - 6;
      }
      //break;
    };
    if(count>=6)//当有六个以上的传感器检测到黑线，视为遇到黑色横线
    {
    b++;
    delay(100);
    }
    x3=abs(x1);
    x4=abs(x2);
    if(x3<x4)
      x=x2;
      else
      x=x1;
      if(count==0)
      x=x_last;
  if(x>2||x<-2)//  误差值大时 选用大的PID参数 更快调节系统
   {
    kp=15;
      PWM.base=80;
        }
   if(x>=-2&&x<=2)//误差值小时 选用小的PID参数 微调系统
    {
      kp=10;
       PWM.base=120;
      }
  Input=x;//将误差传入PID计算中
  Compute();//进行PID计算
  PWM.inc=Output;//PID的输出值赋给电机速度差值
    PWM.Left = PWM.base + PWM.inc;//左加右减
    PWM.Right = PWM.base - PWM.inc;
PWM.Left=constrain( PWM.Left,0,255);//系统可能超调  超过ArduinoPWM的输出值  所以把PID输出值限制在0到255内
PWM.Right=constrain(PWM.Right,0,255);
    Serial.print("X:");
    Serial.print(x);
    Serial.print("left_motor:");
    Serial.print(PWM.Left);
    Serial.print("right_motor:");
    Serial.println(PWM.Right);  
    Serial.print("PWM.INC=");
    Serial.print(PWM.inc);
    Serial.print("count=");
    Serial.print(count);
    Serial.print("b=");
    Serial.print(b);
  motor(PWM.Left,left_motor_pin1,left_motor_pin2,pwmpin1);
  motor(PWM.Right,right_motor_pin1,right_motor_pin2,pwmpin2);
  count=0;
  x_last=x;
    }
/*working variables*/
void Compute()
{
   /*How long since we last calculated*/
   unsigned long now = millis();//millis（）为计算当前运行时间函数  保存在now变量中  函数输出为微秒
   if(now%100==0)//每过100ms进行一次PID计算
   // ITerm=0;
    //if(now%30==0)
    lastInput=Input;
   double error =Input;
   ITerm+=(ki*error);//误差值累加
   double dInput  = (Input - lastInput);//微分计算
   /*Compute PID Output*/
   Output = kp * error + ITerm + kd * dInput;//位置型PID计算公式
   /*Remember some variables for next time*/
   Serial.print("Dinput");
   Serial.print(dInput);
   lastInput=Input;
   
}
void motor(int _speed,int in1,int in2,int PWMpin)
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(PWMpin,_speed);
}
