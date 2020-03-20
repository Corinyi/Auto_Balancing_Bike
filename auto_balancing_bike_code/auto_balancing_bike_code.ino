#include <Wire.h>
#include <Servo.h>  
#include <SoftwareSerial.h>
SoftwareSerial bt(7,6);
#define mpu_add 0x68  //mpu6050 address

class kalman {
  public :
    double getkalman(double acc, double gyro, double dt) {
      //project the state ahead
      angle += dt * (gyro - bias) ;

      //Project the error covariance ahead
      P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle) ;
      P[0][1] -= dt * P[1][1] ;
      P[1][0] -= dt * P[1][1] ;
      P[1][1] += Q_gyro * dt ;

      //Compute the Kalman gain
      double S = P[0][0] + R_measure ;
      K[0] = P[0][0] / S ;
      K[1] = P[1][0] / S ;

      //Update estimate with measurement z
      double y = acc - angle ;
      angle += K[0] * y ;
      bias += K[1] * y ;

      //Update the error covariance
      double P_temp[2] = {P[0][0], P[0][1]} ;
      P[0][0] -= K[0] * P_temp[0] ;
      P[0][1] -= K[0] * P_temp[1] ;
      P[1][0] -= K[1] * P_temp[0] ;
      P[1][1] -= K[1] * P_temp[1] ;

      return angle ;
    } ;
    void init(double angle, double gyro, double measure) {
      Q_angle = angle ;
      Q_gyro = gyro ;
      R_measure = measure ;

      angle = 0 ;
      bias = 0 ;

      P[0][0] = 0 ;
      P[0][1] = 0 ;
      P[1][0] = 0 ;
      P[1][1] = 0 ;
    } ;
    double getvar(int num) {
      switch (num) {
        case 0 :
          return Q_angle ;
          break ;
        case 1 :
          return Q_gyro ;
          break ;
        case 2 :
          return R_measure ;
          break ;
      }
    } ;
  private :
    double Q_angle, Q_gyro, R_measure ;
    double angle, bias ;
    double P[2][2], K[2] ;
} ;

kalman kal ;

long ac_x, ac_y, ac_z, gy_x, gy_y, gy_z ;

double deg, dgz_z ;
double dt ;
uint32_t pasttime ;
Servo sg90;   
Servo sg902;      
int servo_pin = 9;
int servo_pin2 = 8;
int serval=0;


int a;
int b;
int *p=&b;
char bts='x';



void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);       // Motor B 방향설정1
  pinMode(5, OUTPUT);

  Serial.begin(9600) ;
  sg90.attach ( servo_pin );
  sg902.attach(servo_pin2);
  Wire.begin() ;
  bt.begin(9600);
  Wire.beginTransmission(mpu_add) ;
  Wire.write(0x6B) ;
  Wire.write(0) ;
  Wire.endTransmission(true) ;
  kal.init(0.001, 0.003, 0.03) ;  //init kalman filter
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
        
  Wire.beginTransmission(mpu_add) ; //get acc data
  Wire.write(0x3B) ;
  Wire.endTransmission(false) ;
  Wire.requestFrom(mpu_add, 6, true) ;
  ac_x = Wire.read() << 8 | Wire.read() ;
  ac_y = Wire.read() << 8 | Wire.read() ;
  ac_z = Wire.read() << 8 | Wire.read() ;

  Wire.beginTransmission(mpu_add) ; //get gyro data
  Wire.write(0x43) ;
  Wire.endTransmission(false) ;
  Wire.requestFrom(mpu_add, 6, true) ;
  gy_x = Wire.read() << 8 | Wire.read() ;
  gy_y = Wire.read() << 8 | Wire.read() ;
  gy_z = Wire.read() << 8 | Wire.read() ;

  deg = atan2(ac_x, ac_z) * 180 / PI ;  //acc data to degree data
  dgz_z = gy_z / 131. ;  //gyro output to

  dt = 0.3;//((double)(micros() - pasttime) / 1000000)*3;
  pasttime = micros();  //convert output to understandable data

  double val = kal.getkalman(deg, dgz_z, dt) ;  //get kalman data
  /*
  serval = map (val, -50, 50, 0, 80) ;
  Serial.print("kalman degree") ;
  */
  //Serial.println (val);


  bts =bt.read();

  if(bts == 'a'){
    a=0;
    b=250;

    }
  if(bts == 'b'){
    a=-10;
    b=150;
 
    }
  if(bts == 'c'){
    a=+10;
    b=150;

    }
  if(bts == 'd'){
    a=0;
    b=130;
    }
  if(bts == 'x'){
    for(int i = 90; i > 0;i--) {
      analogWrite(4,1.5*i);
      sg902.write(100-i);
      delay (10);
    }
    b=0;
    }
  if(bts == 'o'){
    a=0;

    for(int i=0;i<150;i++){
      analogWrite(4,i);
      delay(20);
    }
        sg902.write (10);
    delay(40);
    b=150;
   }



  if(val>100){
    sg90.write (65+a);
    delay(50);
    }
  else if(val<80){
    sg90.write (25+a);
    delay(50);
    }
  else if(val>=80 && val<=100){
    sg90.write(45+a);
    delay(50);
  }

  analogWrite(4, 0+b);     
  analogWrite(5, 0);   

}
