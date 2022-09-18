# Rx-Tx Idea

Rx : 아주 짧은 간격으로 계속 송신, ack on
Tx : 일반 수신 세팅 -> 수신 성공 시 500ms정도 정지 -> 그리고 다시 수신 시작
일반적으로 하나의 송신-수신 세트에 5ms 정도 걸린다 함

Rx, Tx1, Tx2, Tx3

1. Rx가 계속 신호를 뿌린다.
2. Tx1이 먼저 받는다. Tx2, Tx3는 늦게 받았다.  
3. Tx1은 ack를 반환한다. 그리고 수신을 잠시 멈춘다.
4. Rx는 ack를 받았으니 송신-수신이 성공했음을 return한다.
5. Rx는 이제 적어도 하나의 차가 있음을 안다.
6. Rx가 다시 송신을 시작한다.
7. Tx1은 잠시 수신을 쉬고 있으니 받지 못한다.
8. Tx2가 먼저 받는다. Tx3는 늦게 받았다.
9. Tx2는 ack를 반환하고 잠시 쉰다.
10. Rx는 다시 ack를 받았으니 적어도 하나의 차량이 근처에 있음을 안다.
11. Rx가 다시 송신을 시작한다. 이번에는 Tx3가 받을 것이다.

* 만약 차가 한 대 있다. 그러면 Tx가 쉬는 동안 Rx는 차가 있는지 없는지 모른다.
  -> 그러면 Rx는 차의 전재 유무를 타이머 처리하자. 한번 flag가 올라가면 내려갈 때 까지의 시간은 Tx가 쉬는 시간의 두 배 정도로 하자.

* 만약 차가 너무 많아서 받지 못한다면?
  -> 그러면 차의 존재 여부 flag가 올라가면 boradcasting할까?
                                                                 -> 아니면 ack를 받지 못해 꼬일 뿐 Rx의 신호를 받기는 하나?

1. Rx가 일반 송신을 뿌린다. re-trans는 0번으로.
2. Tx1이 먼저 받고 Tx2, 3는 늦게 받는다.
3. Tx1이 ack 반환하고 Rx는 ack를 받는다.
4. Rx는 flag를 켜고 타이머를 시작한다.
5. Rx는 flag가 켜져 있는 동안 Broadcast한다.
6. 그러면 Tx2, 3 둘 다 신호를 정상적으로 받고 ack를 반환하고 잠시 쉴 것이다.
7. Rx는 ack 관계 없이 타이머 끝날 때 까지 계속 broadcast한다.
8. Rx의 타이머가 내려가면 다시 일반 송신으로 바꾸고 ack를 기다린다. 이 때 차는 없다고 간주된다.
9. Tx들 중 하나가 있다면 수신하고 다시 3번으로, 없다면 아무 일도 일어나지 않는다.

타이머 : broadcast timer, car exist timer;

flag은 0;
singlecast 성공하면 시작시간 기록, flag는 1;  
if ( milis() - timer_bct > TIME_BCT ) singlecast;

flag -> 1 : singlecast 성공  
flag -> 0 : timer_car - milis > TIME_CAR  
