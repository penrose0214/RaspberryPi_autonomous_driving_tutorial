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
const int ENA = 1;
const int ENB = 2;      //!! in 1 2 3 4 ena enb 핀 번호 확인해야함!!

int front;      //front right left는 const int 쓰면 안됨
int right;
int left;

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
const int speed_front = 100;
const int speed_back = 100;
const int speed_right = 20;
const int speed_left = 20;


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

void setspeed(int s){
  analogWrite(ENA,s);
  analogWrite(ENB,s);
}

void forward(){ //직진
  setspeed(speed_front);
  digitalWrite(IN1 ,HIGH);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,HIGH);
  digitalWrite(IN4 ,LOW);
}

void backward(){  //후진
  setspeed(speed_back);
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

void rightward(){ //우회전
  setspeed(speed_right);
  digitalWrite(IN1 ,LOW);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,HIGH);
  digitalWrite(IN4 ,LOW);
}

void leftward(){ //좌회전
  setspeed(speed_left);
  digitalWrite(IN1 ,HIGH);
  digitalWrite(IN2 ,LOW);
  digitalWrite(IN3 ,LOW);
  digitalWrite(IN4 ,LOW);
}
////////////////////////////////////////////////

long DistanceMeasure_all(){
  digitalWrite(sonar_Trig_left, LOW);
  digitalWrite(sonar_Trig_right, LOW);
  digitalWrite(sonar_Trig_front, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_Trig_left, HIGH);
  digitalWrite(sonar_Trig_right, HIGH);
  digitalWrite(sonar_Trig_front, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_Trig_left, LOW);
  digitalWrite(sonar_Trig_right, LOW);
  digitalWrite(sonar_Trig_front, LOW);  
  duration_left = pulseIn(sonar_echo_left, HIGH);
  duration_right = pulseIn(sonar_echo_right, HIGH);
  duration_front = pulseIn(sonar_echo_front, HIGH);
  left_distance = duration_left * 340/1000/2;   // 단위 mm
  right_distance = duration_right * 340/1000/2;
  front_distance = duration_front * 340/1000/2;
  Serial.print("front distance(mm) : ");
  Serial.print(front_distance);
  Serial.print("\n"); 
  Serial.print("right distance(mm) : ");
  Serial.print(right_distance);
  Serial.print("\n"); 
  Serial.print("left distance(mm) : ");
  Serial.print(left_distance);
  Serial.print("\n");

      if(front_distance>=150){ 
        front=true;
      }
        else front=false;
      if(right_distance>=150){ 
        right=true;
      }
        else right=false;
      if(left_distance>=150){ 
        left=true; 
      }
        else left=false;
}
///////////////////////////////////////////////

void move_test(){
  Serial.print("forward\n");
  forward();
  delay(2000);   //delay for 5 seconds
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

void loop() {
  int start_count = 0;
  int i=0;
  if (start_count==0){
          move_test();
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
    else forward();       // 아무것도 없을 경우 전진

}
