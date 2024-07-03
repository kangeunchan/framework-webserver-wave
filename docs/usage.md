# Usage Guide

## Introduction
이 문서는 프로젝트를 사용하는 방법을 설명합니다.

## Setup
### Prerequisites
- C 컴파일러 (예: GCC)
- Make

### Build
프로젝트를 빌드하려면, 루트 디렉토리에서 다음 명령어를 실행합니다:
```
make
```

### Run
애플리케이션을 실행하려면, 빌드 후 다음 명령어를 실행합니다:
```
./my_c_framework
```

## Directory Structure
- `src/`: 소스 코드 디렉토리.
- `include/`: 헤더 파일 디렉토리.
- `tests/`: 테스트 코드 디렉토리.
- `docs/`: 문서 디렉토리.

## Adding a New Controller
1. `src/controllers/` 디렉토리에 새로운 컨트롤러 파일을 생성합니다.
2. `include/controllers/` 디렉토리에 해당 컨트롤러의 헤더 파일을 생성합니다.
3. 새로운 컨트롤러 파일에서 필요한 로직을 구현합니다.
4. `src/core/router.c` 파일에서 새로운 경로를 매핑합니다.

## Adding a New Model
1. `src/models/` 디렉토리에 새로운 모델 파일을 생성합니다.
2. `include/models/` 디렉토리에 해당 모델의 헤더 파일을 생성합니다.
3. 새로운 모델 파일에서 데이터 구조를 정의하고 필요한 로직을 구현합니다.

## Adding a New View
1. `src/views/` 디렉토리에 새로운 뷰 파일을 생성합니다.
2. `include/views/` 디렉토리에 해당 뷰의 헤더 파일을 생성합니다.
3. 새로운 뷰 파일에서 뷰 렌더링 로직을 구현합니다.

## Configuration
- `src/config/config.c` 파일에서 설정 값을 정의하고 로드합니다.
