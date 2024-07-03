# Project Structure

## src/
소스 코드 디렉토리. 프레임워크의 모든 핵심 기능이 여기에 포함됩니다.

### core/
프레임워크의 핵심 로직을 포함합니다.
- `application.c/h`: 애플리케이션 초기화 및 실행을 담당합니다.
- `controller.c/h`: 컨트롤러 관련 로직을 포함합니다.
- `dispatcher.c/h`: HTTP 요청을 디스패치하는 로직을 포함합니다.
- `request.c/h`: HTTP 요청을 처리하는 로직을 포함합니다.
- `response.c/h`: HTTP 응답을 처리하는 로직을 포함합니다.
- `router.c/h`: 요청 라우팅 로직을 포함합니다.
- `server.c/h`: 서버를 실행하는 로직을 포함합니다.

### utils/
유틸리티 함수들을 포함합니다.
- `logger.c/h`: 로깅 유틸리티.
- `string_utils.c/h`: 문자열 처리 유틸리티.
- `file_utils.c/h`: 파일 처리 유틸리티.

### config/
설정 파일을 포함합니다.
- `config.c/h`: 설정 처리 로직.

### controllers/
사용자 정의 컨트롤러를 포함합니다.
- `home_controller.c/h`: 예제 홈 컨트롤러.

### models/
데이터 모델을 포함합니다.
- `user_model.c/h`: 사용자 모델.

### views/
뷰 파일을 포함합니다.
- `home_view.c/h`: 예제 홈 뷰.

## include/
헤더 파일들을 포함합니다. 각 소스 파일에 대응하는 헤더 파일들이 여기에 포함됩니다.

## tests/
테스트 코드들을 포함합니다.
- `core/`: 핵심 로직 테스트 파일.
- `utils/`: 유틸리티 테스트 파일.
- `config/`: 설정 테스트 파일.
- `controllers/`: 컨트롤러 테스트 파일.
- `models/`: 데이터 모델 테스트 파일.
- `views/`: 뷰 테스트 파일.

## docs/
문서화 파일을 포함합니다.
- `design.md`: 설계 문서.
- `architecture.md`: 아키텍처 문서.
- `usage.md`: 사용법 문서.

## 기타 파일
- `Makefile`: 빌드 설정 파일.
- `README.md`: 프로젝트 개요.
- `LICENSE`: 라이선스 파일.
