
/*Motor*/
/*L9110 connection
B-IA: Motor B Input A -> pin 9
B-2A: Motor B Input B -> pin 8

VCC: supply voltage -> (+)
GND: ground         -> GND (-) 

A-IA: Motor A Input A -> pin 7
A-IB: Motor B Input A -> pin 6
*/

/*
수정한 부분
-sonar 핀 미선언
-거리 계산용 변수 전역변수로 전환
-snoar low>high>low 로 해야 오류 
-distance를 main loop에서 계산
*/

const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 8;
const int IN4 = 9;

const int sonar_Trig_right = 4;
const int sonar_echo_right = 3;
const int sonar_Trig_left = 11;
const int sonar_echo_left = 10;

/* 세번째 sonar 센서: 아직 설치 안함, 연결 핀 결정 안함
const int sonar_Trig_front = ??;
const int sonar_echo_front = ??;
 */


long duration_right = 0, right_distance = 0;
long duration_front = 0, front_distance = 0;
long duration_left = 0, left_distance = 0;


void setup(){
  Serial.begin(9600);
    while (!Serial){
              ;
              }
  Serial.println("Hellow World!");
  //모터 선언
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //초음파센서 선언
  pinMode(sonar_Trig_right,OUTPUT);
  pinMode(sonar_echo_right,INPUT);
  pinMode(sonar_Trig_left,OUTPUT);
  pinMode(sonar_echo_left,INPUT);
}

void forward(){ //직진
  digitalWrite(IN1 ,HIGH);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,HIGH);
  digitalWrite(IN4 ,LOW);
}

void backward(){  //후진
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void dontMove(){ //정지, stop() 함수는 이미 존재해서 dontMove로 설정
  digitalWrite(IN1 ,LOW);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,LOW);
  digitalWrite(IN4 ,LOW);
}


void rightward(){ //좌회전
  digitalWrite(IN1 ,LOW);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,HIGH);
  digitalWrite(IN4 ,LOW);
}

void leftward(){ //우회전
  digitalWrite(IN1 ,HIGH);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,LOW);
  digitalWrite(IN4 ,LOW);
}

long DistanceMeasure_right(){
  digitalWrite(sonar_Trig_right, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_Trig_right, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_Trig_right, LOW);
  duration_right = pulseIn(sonar_echo_right, HIGH);
  right_distance = duration_right * 340/1000/2;   // 단위 mm

}

long DistanceMeasure_left(){
  digitalWrite(sonar_Trig_left, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_Trig_left, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_Trig_left, LOW);
  duration_left = pulseIn(sonar_echo_left, HIGH);
  left_distance = duration_left * 340/1000/2;   // 단위 mm

}

void loop() {
  Serial.print("forward\n");
  forward();
  delay(5000);   //delay for 5 seconds
  Serial.print("backward\n");
  backward();
  delay(5000);
  Serial.print("stop\n");
  dontMove();
  delay(5000);
  Serial.print("rightward\n");
  rightward();
  delay(5000);
  Serial.print("leftward\n");
  leftward();
  delay(5000);

  DistanceMeasure_right();
  DistanceMeasure_left();
  Serial.print("right distance : ");
  Serial.print(right_distance);

  Serial.print("\n"); //줄바꿈
  
  Serial.print("left distance : ");
  Serial.print(left_distance);
  
  Serial.println("mm");
  delay(100);
  
}
