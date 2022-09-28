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

//left wheel control
const int IN1 = 7;
const int IN2 = 2;
const int ENA = 6;

//right wheel control
const int IN3 = 8;
const int IN4 = 9;
const int ENB = 5;

//front right left는 const int 쓰면 안됨, flag 변수
int front;
int right;
int left;
int start_count = 0;

const int sonar_Trig_left = 4;
const int sonar_echo_left = 3;

const int sonar_Trig_right = 11;
const int sonar_echo_right = 10;

const int sonar_Trig_front = 13;
const int sonar_echo_front = 12;

long duration_front = 0, front_distance = 0;
long duration_right = 0, right_distance = 0;
long duration_left = 0, left_distance = 0;

//speed setting
const int speed_right = 230;
const int speed_left = 200;

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
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //초기값 설정 (ENA, ENB 모두 일단 시작은 0으로)
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
 
  
  //초음파센서 선언
  pinMode(sonar_Trig_right,OUTPUT);
  pinMode(sonar_echo_right,INPUT);
  pinMode(sonar_Trig_left,OUTPUT);
  pinMode(sonar_echo_left,INPUT);
  pinMode(sonar_Trig_front, OUTPUT);
  pinMode(sonar_echo_front, INPUT);
}

void setspeed_left(int s){
  analogWrite(ENA,s);
}

void setspeed_right(int s){
  analogWrite(ENB,s);
}

void forward(){ //직진
  setspeed_right(speed_right);
  setspeed_left(speed_left);  
  digitalWrite(IN1 ,HIGH);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,HIGH);
  digitalWrite(IN4 ,LOW);
}

void backward(){  //후진
  setspeed_right(speed_right);
  setspeed_left(speed_left);
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

void leftward(){ //좌회전 >> 본체 좌측 본체 길이의 2배 지점에 순간회전중심 생성
  setspeed_right(speed_right*0.75);
  setspeed_left(speed_left*0.375);
  digitalWrite(IN1 ,LOW);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,HIGH);
  digitalWrite(IN4 ,LOW);
}

void rightward(){ //우회전
  setspeed_right(speed_right*0.375);
  setspeed_left(speed_left*0.75);
  digitalWrite(IN1 ,HIGH);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,LOW);
  digitalWrite(IN4 ,LOW);
}
////////////////////////////////////////////////

long DistanceMeasure_front(){
  digitalWrite(sonar_Trig_front, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_Trig_front, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_Trig_front, LOW);
  delayMicroseconds(10);  
  duration_front = pulseIn(sonar_echo_front, HIGH);
  front_distance = duration_front * 340/1000/2;
  Serial.print("front distance(mm) : ");
  Serial.print(front_distance);
      if(front_distance<=250){ 
        front=true;
        Serial.print("   앞에 장애물\n");
      }
      else{  
        front=false;
        Serial.print("\n");
       }
}

long DistanceMeasure_left(){
  digitalWrite(sonar_Trig_left, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_Trig_left, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_Trig_left, LOW);
  delayMicroseconds(10);
  duration_left = pulseIn(sonar_echo_left, HIGH);
  left_distance = duration_left * 340/1000/2;   // 단위 mm
  Serial.print("left distance(mm) : ");
  Serial.print(left_distance);
      if(left_distance<=250){ 
        left=true;
        Serial.print("   왼쪽에 장애물\n");
      }
      else{  
        left=false;
        Serial.print("\n");
       }
}

long DistanceMeasure_right(){
  digitalWrite(sonar_Trig_right, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_Trig_right, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_Trig_right, LOW);
  delayMicroseconds(10);
  duration_right = pulseIn(sonar_echo_right, HIGH);
  right_distance = duration_right * 340/1000/2;
  Serial.print("right distance(mm) : ");
  Serial.print(right_distance);
      if(right_distance<=250){ 
        right=true;
        Serial.print("   오른쪽에 장애물\n");
      }
      else{  
        right=false;
        Serial.print("\n");
       }
}

long DistanceMeasure_all(){
  DistanceMeasure_front();
  DistanceMeasure_right();
  DistanceMeasure_left();
}
///////////////////////////////////////////////

void loop() {

  int i=0;
  if (start_count==0){
//          move_test();
          start_count++;
     }
    
    delay(100);         // 0.1초마다 메인루프 1회
    DistanceMeasure_all();
 
          ////장애물 발견시 전진 없이 회전만 함!
    if(front==true){  //정면에 장애물. 좌우는 고려하지 않음
        if(right==true && left==false)              //좌측이 비었으면 좌측으로, 우측이 비었으면 우측으로, 모두 막혔으면 일단 우측으로.
            leftward();
        else rightward();
    }
    else if(left==true && right==false){ //좌측에만 장애물 있을 경우. 정면은 고려하지 않아도 될듯?
        rightward();
    }
    else if(left==false && right==true){  //우측에만 장애물 있을 경우
        leftward();
    }
    else if(left==true && right==true){   //정면에 관계 없이 좌우가 막혔을 경우, 일단 우측으로 회피 
        rightward();                      //조금 돌면 정면만 막히게 되며, 더 돌면 좌측만 막히게 되고, 결국 우측으로 완전히 회피 가능
    }
    else forward();       // 아무것도 없을 경우 
}





////////////////////////////////////////////

void move_test(){
  Serial.print("forward\n");
  forward();
  delay(2000);   //delay for 2 seconds
  Serial.print("backward\n");
  backward();
  delay(2000);
  Serial.print("stop\n");
  dontMove();
  delay(2000);
  Serial.print("rightward\n");
  rightward();
  delay(2000);
  Serial.print("leftward\n");
  leftward();
  delay(2000);
}
