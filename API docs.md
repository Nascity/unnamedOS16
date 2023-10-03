# API docs
UOS API의 문서입니다.
## 자료형
| 자료형 | typedef | 크기 (bytes) | 설명 |
| - | - | - | - |
| ARGS | typedef struct | 4 | Entrypoint의 전달인자 배열 구조체<br/>* `uiArgCount`: 전달인자의 수<br/>* `pArgs`: `ARGS_ENTRY` 배열의 주소
| ARGS_ENTRY | typedef struct | 4 | Entrypoint의 전달인자 구조체<br/>* `uiArgLength`: 전달인자의 길이(null문자 포함)<br/>* `strArg`: 전달인자의 주소
| BOOL | char | 1 | boolean |
| CHAR | char | 1 | char |
| BYTE | unsigned char | 1 | byte |
| INT | int | 2 | int |
| KOBJIO | int | 2 | I/O 커널 오브젝트 |
| PBOOL | char* | 2 | BOOL형 포인터 |
| PINT | int* | 2 | INT형 포인터 |
| PTR | void* | 2 | 포인터 |
| PWORD | unsigned short* | 2 | WORD형 포인터
| STRING | char* | 2 | 문자열 |
| TIME_ENTRY | typedef struct | 6 | 시간 정보 구조체<br/>* `bYear`<br/>* `bMonth`<br/>* `bDay`<br/>* `bHour`<br/>* `bMinute`<br/>* `bSecond` |
| UINT | unsigned int | 2 | 부호 없는 INT형 |
| WORD | unsigned short | 2 | word |
| VOID | void | 0 | void |

## 프로그램의 Entrypoint
| 반환형 | 함수 | 설명 |
| - | - | - |
| INT | UosMain(PTR pCmdLine, ARGS args) | 프로그램의 entrypoint입니다. 전달된 문자열의 주소가 `pCmdLine`으로 전달되고, 띄어쓰기를 기준으로 분리된 전달인자 배열은 `args`를 통해 전달됩니다. |

## 화면 출력
| 반환형 | 함수 | 설명 |
| - | - | - |
| VOID | PrintChar(CHAR chCharacter) | `chCharacter`를 현재 커서의 위치에 출력합니다. |
| VOID | PrintFormat(STRING strFormat, ...) | `strFormat`을 `...`의 내용으로 포맷해서 출력합니다. |
| VOID | PrintHex(UINT iNumber, BOOL bCapitalize, BOOL bPrefix) | `iNumber`를 16진수로 출력합니다. `bCapitalize`가 `TRUE`이면 대문자로 출력되고, `bPrefix`가 `TRUE`이면 16진수 숫자 앞에 `0x` 접두어가 붙습니다. 출력할 수 있는 값의 범위는 0000 ~ FFFF입니다. |
| VOID | PrintNumber(INT iNumber) | `iNumber`를 10진수로 출력합니다. 출력할 수 있는 값의 범위는 -32768 ~ 32767입니다. |

## 메모리
| 반환형 | 함수 | 설명 |
| - | - | - |
| PTR | AllocHeapMem(INT iCount) | `iCount` 바이트의 메모리를 힙에 할당합니다. 성공시 힙 메모리의 주소를, 실패시 `NULL`을 반환합니다. |
| BOOL | FreeHeapMem(PTR pAddr) | 주소가 `pAddr`인 힙 메모리를 반환합니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다. |
| VOID | Memcpy(PTR pTarget, PTR pSource, INT iCount) | `pTarget`으로 `pSource`의 데이터를 `iCount` 바이트만큼 복사합니다. |

## 파일
| 반환형 | 함수 | 설명 |
| - | - | - |
| KOBJIO | OpenFile(STRING strName, STRING strExt, WORD wOpenMode) | 파일 시스템에서 이름이 `strName`이고 확장자명이 `strExt`인 파일을 엽니다. 성공시 I/O 커널 오브젝트를, 실패시 `INVALID_KOBJIO`를 반환합니다.<br/>`wOpenMode`의 값으로는 다음이 가능합니다.<br/>* `FILE_OPEN_WRITE`: 파일을 쓰기 전용으로 엽니다.<br/>* `FILE_OPEN_READ`: 파일을 읽기 전용으로 엽니다. |
| BOOL | CloseFile(KOBJIO koIo) | 열려있는 파일을 닫고 커널 오브젝트를 반환합니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다. |
| KOBJIO | GetWorkingDirectory(VOID) | 작업 디렉토리의 `KOBJIO`를 반환합니다. |
| VOID | SetWorkingDirectory(KOBJIO koIo) | 작업 디렉토리를 `koIo`로 변경합니다. |
| BOOL | WriteFile(KOBJIO koIoFile, PTR pBuffer, INT iOffset, INT iCount) | `koIoFile`에 해당하는 파일의 `iOffset` 바이트 오프셋에 `pBuffer`의 데이터를 `iCount` 바이트만큼 씁니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다. |
| BOOL | ReadFile(KOBJIO koIoFile, PTR pBuffer, INT iOffset, INT iCount) | `koIoFile`에 해당하는 파일의 `iOffset` 바이트 오프셋에 있는 데이터를 `iCount` 바이트만큼 `pBuffer`에 읽습니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다. |
| BOOL | CreateFile(STRING strName, STRING strExt, BYTE bAttrib) | 현재 디렉토리에 이름이 `strName`이고 확장자명이 `strExt`인 파일을 생성합니다. 성공시 `TRUE`를, 실패시 `FALSE`를 반환합니다.<br/>`bAttrib`의 값으로는 다음이 가능합니다.<br/>* `FILE_CREATE_READONLY`: 읽기 전용 파일을 생성합니다.<br/>* `FILE_CREATE_HIDDEN`: 숨김파일을 생성합니다.<br/>* `FILE_CREATE_SYSTEM`: 시스템 파일을 생성합니다.<br/>* `FILE_CREATE_SUBDIR`: 서브 디렉토리를 생성합니다. |
| BOOL | DeleteFile(STRING strName, STRING strExt) | 현재 디렉토리에 이름이 `strName`이고 확장자명이 `strExt`인 파일을 제거합니다. 성공시 `TRUE`를, 실패시 `FALSE`를 반환합니다. |

## 프로세스
| 반환형 | 함수 | 설명 |
| - | - | - |
| INT | CreateProcess(STRING strName, STRING strExt, STRING strArgs, PBOOL pbIsSuccessful) | 파일 시스템에서 이름이 `strName`이고 확장자명이 `strExt`인 파일을 메모리에 적재해 실행합니다. 이때, `strArgs`를 전달인자로 전달합니다. 프로세스가 종료한 후 리턴하는 값이 리턴되며, 함수의 성공 여부는 `pbIsSuccessful`을 통해 전달됩니다.<br/>* 주의사항: 실행 가능하지 않은 파일을 실행함으로써 발생하는 문제의 책임은 사용자에 있습니다. |
