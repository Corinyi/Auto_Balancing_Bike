# Auto Balancing Bike
-------------------------
##스스로 균형을 잡고 컨트롤 할수 있는 자전거 로봇

<img src="/image&video/IMG_4947_2.jpeg" width="60%" height="60%" title="Auto Balancing Bike"></img>

완성된 모습

[전시회 판넬 자전거팀.pdf](https://github.com/Corinyi/Auto_Balancing_Bike/files/8103481/default.pdf)

## 하드웨어 구성

*  사 용  부 품
>       아두이노 : Arduino Nano V3 (328)
>       기울기 및 자이로 센서 : MPU6050
>       블루투스 모듈 : HC-05
>       서보 모터 : SG90
>       DC 모터 : RoboRobo 200rpm  DC 모터(모터 RPM 높은 것으로 변경 요망)
>       전원 : 6V
>       프레임, 휠 : 3D 프린터로 제작
>       바퀴 : 실리콘 도포로 마찰력 극대화


## 소프트 웨어 구성

### 칼만 필터(Kalman Filter)
MPU6050의 결과값을 통해 기울어진 값을 받아온다. 이 때 충격이나 오차 등의 원인으로 결과값이 불균일하게 도출되는 경우가 있다. 소위 결과값이 '튄다'라고 한다.
갑작스러운 결과값의 방지하기 위해 칼만필터를 사용한다. 쉽게 말해 결과값 변화를 완충한다. 
위 프로젝트에서는 기울기 결과값의 변화를 완충하기 위해 아래 프로젝트를 참고하였다.

[MPU6050 칼만필터 적용](https://m.blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=220421114302&proxyReferer=https%3A%2F%2Fwww.google.com%2F)

### P.I.D. 제어(Proportional-Integral-Differential control)
PID 제어는 출력값과 목표값의 차이를 계산하여서 피드백 제어하는 제어 기법 중 하나이다.
위 프로젝트에서는 서보모터를 이용하여서 자전거의 조향장치인 앞바퀴 핸들을 조정한다.
기준값인 수평 기울기 0도를 목표으로 하여서 출력값인 서보모터 기울기의 오차를 측정하고 서보 제어값을 변화시킨다.

[PID 제어](https://ko.wikipedia.org/wiki/PID_제어기)

> 첨부한 코드에는 PID 제어 항목이 들어가 있지 않다. 실제 전시에는 PID 제어가 적용된 코드를 사용하였지만 제작자의 불찰로 인해 코드가 유실되어, PID 제어를 제외한 코드로 출력된다.

이 PID 제어를 거치게 되면, 자전거 로봇이 수평 기울기 0도를 최대한 유지하며 이동할 수 있다.

### 블루투스 컨트롤(Bluetooth Control)
블루투스 컨트롤은 아두이노에 외부 장치를 이용해서 신호를 주는 방식의 일환이다.
위 프로젝트에서는 스마트폰을 이용하여서 자전거 로봇의 이동과 속도 조절을 제어하였다.
스마트폰 어플리케이션으로는 
[Arduino Bluetooth Controller](https://play.google.com/store/apps/details?id=com.giumig.apps.bluetoothserialmonitor&hl=en_US) 를 사용하였다.

콘솔모드에서 상하좌우에 해당하는 character를 설정할 수 있다. 이 설정된 문자가 아두이노에 들어갔을 때 모드의 역할을 하게 된다. 
이 모드 결정변수를 switch문에 적용하여 모드(이동 방향, 속도)를 결정하였다.
특히 좌우 운행은 PID 제어 기준값의 변경을 이용하였다.(ex. 좌측 운행은 기울기 기준값을 -15도로 설정하였다.)



## 개발 환경

MacOS Catalina, Version 10.15.1
Arduino Application, 1.8.1.1 



## 제작자

Corinyi and SIOR

Copyright @ 2019 Sungkyunkwan Institute of Robotics
