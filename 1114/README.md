# Unity 기본 정리

## 1. Unity 설치 과정
- Unity Hub 다운로드 후 설치.
- Personal 또는 Student 라이선스로 로그인.
- Unity Editor 버전 설치 후 New Project 생성.

## 2. RollingBall 간단 예제
- Cube로 바닥(Floor) 생성.
- 여러 Cube로 벽(Wall01~03) 구성.
- 카메라/라이트 위치 조정.
- 경사면(Slope01~05) 생성.
- Sphere(Ball) 추가 후 Rigidbody 적용.
- Physic Material(Bounce)로 탄성 설정.
- Material 생성하여 Ball 색상 변경.

## 3. 캐릭터 이동 예제 (QueryChan)
- 패키지 임포트: Assets → Import Package → Custom Package.
- CharacterController + Animator 사용.
- 방향키 입력 받아 이동/회전 처리.
- Spacebar 입력 시 Jump 트리거.
- Terrain 생성 후 텍스처(Grass, Sand) 페인트.
- 높은 곳 생성(Raise/Lower Terrain).
- 캐릭터(PQchan) 배치 후 위치 조정.
