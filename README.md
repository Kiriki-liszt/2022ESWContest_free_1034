# Embedded software Contest 2022

2022년 임베디드 소프트웨어 경진대회 참가 관리용 git

## 시스템 설정

* 보드 매니저
  * <https://arduino.esp8266.com/stable/package_esp8266com_index.json>  
  * <https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json>  

* 라이브러리
  * RF24  
  * <https://github.com/Cesar-S/DFRobot_TFmini>  

## 내가 한 것들

* 두 보드 간 RF 통신
  * STM Nucleo F103RB <-> NodeMCU
* 하나의 보드에서 여러 보드로의 브로드캐스팅
* 데이터 전송 유효거리
  * 7미터 이상 성공
* 중간 장애물 여부
  * 철문 + 콘크리트 벽 바깥에서 문 닫고 성공
* Station이 다가오는 자동차 Node의 존재를 판단할 수 있다.
* 그것을 어떤 포맷으로 전달할 것인가
  * 하나의 문자를 하나의 상태를 표현하기에는 데이터 낭비
  * 문자보다는 이진으로 봐서 00 01 10 11 네 상태 어떨까
  * 2바이트만 되어도 8가지의 상황에 4가지의 상태 전달 가능
* I2C 데이터 통신
  * Arduino Nano <-> STM Nucleo F103RB

## 할 것

* 어떠한 정보를 전달할 것인가
* lidar==sensor 에서 station으로 전달할 때 -> station의 request 이후 한 스텝 돌릴 것인가, 둘은 독립적으로 돌 것인가.  
