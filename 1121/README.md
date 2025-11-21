# 🎮 Unity Game Programming & Event Methods 정리

## 1) 기본 3D 게임 제작 흐름 (공 굴리기 / 장애물 피하기 기반)

### ■ 씬 구성 기본 흐름
- 3D Object로 Ground 생성, Position/Rotation/Scale 조정
- Sphere → Ball 생성 후 Rigidbody 부착
- Directional Light, Camera 배치

### ■ Material 적용
- Create → Material → Albedo 색 선택
- Ground·Coin 등에 Drag & Drop

### ■ 스크립트 기초
- Start(), Update() 중심
- PrtPosition: 이동 거리 출력
- if문 + bool로 특정 상황 1회만 출력
- CameraFollow: Ball 따라 카메라 이동
- GroundMove: 키보드/마우스로 Stage 기울이기
- BallJump: Space 입력 시 AddForce
- ObstacleMove: 좌우 왕복 이동
- OnCollisionEnter(): 충돌 시 Force 적용

### ■ Trigger 활용
- FailZone: 떨어지면 씬 재시작
- CoinTrigger: Ball 닿으면 코인 제거
- Tag 활용: GameObject.FindGameObjectsWithTag()
- RedCoinItem: 레드 코인 먹으면 장애물 전체 제거

### ■ GameManager 패턴
- 씬 재시작, 장애물 제거 등 전역 로직 관리
- SendMessage로 GameManager 메소드 호출


## 2) 장애물 런(RUN) 게임 제작 흐름

### ■ 프로젝트 준비
- Unity Hub → 3D Core
- Standard Assets Import (Characters, Cameras 등)

### ■ 캐릭터 배치
- ThirdPersonController Prefab을 Player로
- Tag = Player
- MultipurposeCameraRig으로 카메라 구성

### ■ 스테이지 구축
- FloorPrefab 조합해 길 생성
- BlockPrototype으로 장애물 배치
- Wall, Pillar 배경 구성
- Metal Texture Pack으로 텍스처 적용

### ■ 조명 구성
- Directional Light 약하게
- Spotlight, Pointlight 배치로 연출 강화

### ■ 트리거 기반 기능
- OutArea: 플레이어 낙하 감지 후 씬 재시작
- GoalArea: 목표 지점 도달 감지

### ■ UI
- Canvas → Text로 타이머(TimeText) 표시


## 3) Unity 이벤트 함수(Event Methods)

### ■ 이벤트 함수 개념
Unity가 특정 상황에서 자동 호출하는 메소드  
MonoBehaviour 상속 스크립트 안에서 구현

### ■ 실행 순서 핵심 흐름
Awake → OnEnable → Start →  
FixedUpdate → OnTrigger/OnCollision → Update → LateUpdate →  
OnDisable → OnDestroy → OnApplicationQuit

### ■ 초기화 관련
- Awake(): 씬 로드 즉시 1회, 컴포넌트 비활성이어도 실행
- Start(): 첫 Update 직전 1회, 활성 상태에서만 실행
- 차이: Awake가 더 빠르고 오브젝트 활성화만 되어도 실행됨

### ■ 업데이트 관련
- Update(): 프레임마다 실행 (물리 X)
- FixedUpdate(): 일정한 시간 간격으로 실행 (물리 처리)
- LateUpdate(): Update 이후 실행 (카메라 추적용)

### ■ 충돌/트리거
- OnTriggerEnter(Collider): 한쪽에 Trigger 필요
- OnCollisionEnter(Collision): Rigidbody 필요, 물리 충돌

### ■ 종료/해체
- OnDisable(), OnDestroy(), OnApplicationQuit()

### ■ Gizmo
- SceneView 디버깅용 그래픽 출력 (OnDrawGizmos)

### ■ 사용자 정의 이벤트
- UnityEvent + Invoke() 사용해 커스텀 이벤트 실행


## 📘 총 요약
세 PDF는 Unity 3D게임 제작의 기본 흐름(씬 구성, 스크립트, 트리거·충돌 처리, 코인, 장애물, GameManager)과  
3D 러너 게임 제작 과정, 그리고 Unity 이벤트 함수 라이프사이클을 정리하고 있다.  
입문자가 Unity로 게임 구조를 이해하는 데 필요한 핵심 개념들이 포함되어 있다.
