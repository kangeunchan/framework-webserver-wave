# Architecture Document

## Overview
이 문서는 프로젝트의 아키텍처를 설명합니다.

## Layers
### Core Layer
- 애플리케이션의 핵심 로직을 포함합니다.
- 구성 요소: Application, Controller, Dispatcher, Request/Response, Router, Server.

### Utility Layer
- 유틸리티 함수들을 포함합니다.
- 구성 요소: Logger, String Utils, File Utils.

### Configuration Layer
- 설정 파일을 처리합니다.
- 구성 요소: Config.

### Controller Layer
- 사용자 정의 컨트롤러를 포함합니다.
- 구성 요소: Home Controller.

### Model Layer
- 데이터 모델을 포함합니다.
- 구성 요소: User Model.

### View Layer
- 뷰 파일을 포함합니다.
- 구성 요소: Home View.

## Data Flow
1. 클라이언트 요청을 서버가 수신합니다.
2. 서버가 요청을 디스패처로 전달합니다.
3. 디스패처가 요청을 라우터로 전달하여 적절한 컨트롤러를 찾습니다.
4. 컨트롤러가 요청을 처리하고 응답을 생성합니다.
5. 응답이 클라이언트로 반환됩니다.
