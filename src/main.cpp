#include <Arduino.h>

int RELAY = 23;         // 릴레이가 연결된 핀
int LDR_PIN = 15;       // 밝기 센서(아날로그 핀)가 연결된 핀
int threshold = 500;    // 밝기 임계값
int light_val = 0;      // 밝기 값을 저장할 변수
float alpha = 0.1;      // EMA의 알파 값 (필터의 민감도, 0 ~ 1)
float filtered_val = 0; // 필터링된 밝기 값을 저장할 변수

void setup() {
  Serial.begin(115200);       // 시리얼 통신 시작
  delay(500);                 // 안정화 시간
  Serial.println("Starting");

  pinMode(RELAY, OUTPUT);     // 릴레이 핀을 출력 모드로 설정

  filtered_val = analogRead(LDR_PIN); // 첫 번째 값을 초기화
}

void loop() {
  light_val = analogRead(LDR_PIN);    // 밝기 센서에서 값을 읽음
  filtered_val = alpha * light_val + (1 - alpha) * filtered_val; // EMA 적용
  Serial.println(filtered_val);       // 필터링된 값을 시리얼 모니터에 출력

  // 필터링된 값에 따라 릴레이 제어
  if (filtered_val < threshold) {     // 밝기가 임계값보다 낮으면 (어두울 때)
    digitalWrite(RELAY, HIGH);        // 릴레이 ON (전기기구 켬)
  } else {                            // 밝기가 임계값보다 높으면 (밝을 때)
    digitalWrite(RELAY, LOW);         // 릴레이 OFF (전기기구 끔)
  }

  delay(500);  // 0.5초마다 값을 읽고 릴레이 상태 업데이트
}
