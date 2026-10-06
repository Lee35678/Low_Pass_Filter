# Low_Pass_Filter

ESP32 밝기 센서 값에 지수이동평균(EMA) 저역통과 필터를 적용해 릴레이를 제어하는 PlatformIO 예제

## 개요

Brightness_Controlled_Relay와 같은 회로에서, 센서 원시값 대신 지수이동평균(EMA)으로 걸러낸 값을 기준으로 릴레이를 켜고 끕니다. 센서 값의 순간적인 흔들림을 완화해 임계값 근처에서 릴레이가 자주 바뀌는 현상을 줄이는 것이 목적입니다. 작성 시기는 2024년 9월입니다(커밋 기록 기준).

## 하드웨어

- 보드: DOIT ESP32 DevKit V1 (`board = esp32doit-devkit-v1`)
- 입력: 아날로그 출력형 밝기 센서 1개
- 출력: 릴레이 모듈 1개

| 신호 | GPIO | 설정 |
|------|------|------|
| 릴레이 (`RELAY`) | 23 | `OUTPUT` |
| 밝기 센서 (`LDR_PIN`) | 15 | `analogRead` |

## 동작 방식

1. `setup()`
   - 시리얼을 115200 bps로 열고 `Starting`을 출력한 뒤 릴레이 핀을 출력으로 설정합니다.
   - 센서 값을 한 번 읽어 필터 초기값(`filtered_val`)으로 씁니다.
2. `loop()`: 500 ms마다 다음을 반복합니다.
   - 센서 값을 읽고 EMA를 적용합니다.

     ```
     filtered_val = alpha * light_val + (1 - alpha) * filtered_val    // alpha = 0.1
     ```

     `alpha`가 작을수록 새 값의 반영 비율이 낮아져 출력이 더 부드럽고 느리게 변합니다.
   - 필터링된 값을 시리얼에 출력합니다.
   - 필터링된 값이 임계값(`threshold = 500`)보다 작으면 릴레이 HIGH(켜짐), 아니면 LOW(꺼짐)입니다.

| 파라미터 | 값 | 의미 |
|----------|----|------|
| `alpha` | 0.1 | EMA 가중치 (0~1) |
| `threshold` | 500 | 릴레이 판단 기준 (ADC 값) |
| 주기 | 500 ms | 측정/제어 간격 |

## 개발 환경

| 항목 | 값 |
|------|----|
| 도구 | PlatformIO |
| 플랫폼 | `espressif32` |
| 프레임워크 | `arduino` |
| 외부 라이브러리 | 없음 |
| 모니터 속도 | `monitor_speed = 115200` |

## 빌드 및 업로드

```bash
pio run -t upload
pio device monitor
```

## 폴더 구조

```
Low_Pass_Filter/
├── platformio.ini
└── src/
    └── main.cpp
```

## 참고

- 필터는 흔들림을 줄여 줄 뿐 히스테리시스는 없으므로, 필터링된 값이 오래 500 근처에 머무르면 릴레이가 여전히 바뀔 수 있습니다.
- `alpha = 0.1`, 500 ms 주기에서는 밝기가 갑자기 바뀌어도 필터 출력이 변화량의 약 63%를 따라가는 데 약 10회 측정(약 5초)이 걸립니다. 반응 속도와 안정성은 `alpha`로 조절합니다.
