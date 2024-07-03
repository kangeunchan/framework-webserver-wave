# Design Document

## Introduction
이 문서는 프로젝트의 설계 원칙과 방법론을 설명합니다.

## Core
### Application
- 애플리케이션의 초기화와 실행을 담당합니다.
- 주요 역할: 초기 설정 로드, 서버 시작.

### Controller
- HTTP 요청을 처리하는 컨트롤러 계층입니다.
- 주요 역할: 라우트 매핑, 요청 처리.

### Dispatcher
- HTTP 요청을 적절한 컨트롤러에 전달합니다.
- 주요 역할: 요청 디스패치, 미들웨어 관리.

### Request/Response
- HTTP 요청과 응답을 처리합니다.
- 주요 역할: 요청 파싱, 응답 생성.

### Router
- URL 라우팅을 담당합니다.
- 주요 역할: 경로 매핑, 경로 해석.

### Server
- HTTP 서버를 실행합니다.
- 주요 역할: 서버 설정, 요청 수신.

## Utils
### Logger
- 로그를 관리합니다.
- 주요 역할: 로그 생성, 로그 레벨 관리.

### String Utils
- 문자열 처리 유틸리티입니다.
- 주요 역할: 문자열 변환, 조작.

### File Utils
- 파일 처리 유틸리티입니다.
- 주요 역할: 파일 읽기, 쓰기.

## Config
### Config
- 설정 파일을 처리합니다.
- 주요 역할: 설정 로드, 설정 값 관리.

## Controllers
### Home Controller
- 기본 컨트롤러 예제입니다.
- 주요 역할: 홈 경로 처리.

## Models
### User Model
- 사용자 데이터 모델 예제입니다.
- 주요 역할: 사용자 데이터 구조 정의.

## Views
### Home View
- 홈 뷰 예제입니다.
- 주요 역할: 홈 페이지 렌더링.
