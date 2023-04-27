# DMU Project [2-1]

## PASI ROBOT(피킹 로봇)

### 작품 동기

새벽 배송 경쟁 격화, 온라인 쇼핑 증가

### 컨셉

물건을 담아 옮겨주는 로봇, 물건을 내려주는 선반

### 프로그램

ROS, C++, Kotlin

### 보드

RaspberryPi 4, OpenCR 1.0

### 운영체제

Ubuntu 18.04, ROS Melodic

### 부품

LDS-01 (Lidar Sensor)

HC-SR04(Ultrasonic Sensor)

MX-106 (Dynamixel Motor)

103h5210-0424(Step Motor)

### Using ROS package

Slam :: Gmapping

Navigation :: AutoClearMap / Movebase

### 수행 절차

1. 앱을 통해 물건 주문
2. 주문을 받으면 로봇이 선반으로 이동
3. 선반이 로봇에게 물건을 내려줌
4. 물건을 다 받은 로봇은 포장 장소로 이동
5. 사람이 물건 포장 후 로봇을 원위치로 복귀
