# Megabonky
Unreal Engine 5 기반의 멀티플레이어 액션 게임으로, Gameplay Ability System(GAS)을 활용한 능력 기반 전투 시스템을 갖춘 프로젝트입니다.

### 주요 특징
- **Gameplay Ability System (GAS) 기반 전투 시스템**
  - 플레이어와 무기가 각각 독립적인 Ability System Component 보유
  - 35개 이상의 세밀한 캐릭터 속성 관리
  - 크리티컬, 넉백, 바운스 등 다양한 전투 메커니즘

- **네트워크 멀티플레이어 지원**
  - Server Authority 기반 치팅 방지
  - 완전한 속성 및 상태 리플리케이션
  - RPC를 통한 효율적인 네트워크 통신

- **무기 시스템**
  - 다중 무기 장착 가능
  - 무기별 독립적인 능력 및 속성
  - 데이터 테이블 기반 밸런싱

- **성장 시스템**
  - 레벨업을 통한 캐릭터 강화
  - 무기 업그레이드 시스템
  - 비전서(Tomes)를 통한 능력 강화

## 📁 프로젝트 구조
Megabonky/
├── Source/Megabonky/
│   ├── Core/                          # 핵심 게임 프레임워크
│   │   ├── MgbGameModeBase.h/cpp      # 게임 모드
│   │   ├── MgbGameStateBase.h/cpp     # 게임 상태 (시간, 레벨, XP)
│   │   ├── MgbPlayerController.h/cpp  # 플레이어 컨트롤러
│   │   ├── MgbCharacter.h/cpp         # 캐릭터 베이스 클래스
│   │   ├── MgbWeapon.h/cpp            # 무기 베이스 클래스
│   │   ├── Characters/                # 캐릭터 구현
│   │   │   ├── MgbPlayerCharacter     # 플레이어
│   │   │   └── MgbEnemyCharacter      # 적
│   │   ├── AbilitySystem/             # GAS 구현
│   │   │   ├── MgbAbilitySystemComponent
│   │   │   ├── Abilities/             # 게임플레이 어빌리티
│   │   │   ├── AttributeSet/          # 속성 세트
│   │   │   └── MgbEffectExecutionCalculation
│   │   └── Data/                      # 데이터 구조체
│   ├── Actors/                        # 액터 클래스
│   │   ├── MgbProjectileActor         # 발사체
│   │   ├── MgbEffectActor             # GameplayEffect 적용 액터
│   │   ├── MgbItemActor               # 아이템
│   │   └── DamageTextActor            # 데미지 텍스트
│   ├── UI/                            # UI 위젯
│   └── MgbSubsystem.h/cpp             # 게임 인스턴스 서브시스템

## 🎮 주요 시스템

### Gameplay Ability System

프로젝트의 핵심은 GAS를 활용한 이중 ASC 아키텍처입니다:

- **플레이어 ASC**: 캐릭터의 기본 속성 및 능력 관리
- **무기 ASC**: 각 무기가 독립적인 ASC를 보유하여 무기별 능력 및 속성 관리

### 네트워크 아키텍처
- **Server Authority**: 적 생성, 데미지 계산, XP/레벨 관리
- **Replication**: 모든 중요 속성 및 상태 동기화
- **RPC 활용**:
  - Server RPC: 클라이언트 → 서버 (XP 추가, 업그레이드 적용)
  - Client RPC: 서버 → 클라이언트 (데미지 텍스트)
  - Multicast RPC: 서버 → 모든 클라이언트 (UI 업데이트)


**마지막 업데이트**: 2026-02-13