# API docs
UOS API의 문서입니다.
## 자료형
| 자료형 | typedef | 크기 (bytes) | 설명 |
| - | - | - | - |
| BOOL | char | 1 | boolean |
| BYTE | char | 1 | byte |
| INT | int | 2 | int |
| KOBJIO | int | 2 | I/O 커널 오브젝트 |
| KOBJMEM | int | 2 | 메모리 커널 오브젝트 |
| STRING | char* | 2 | 문자열 |
| WORD | short | 2 | word |

## 프로세스

## 메모리
| 반환형 | 함수 | 설명 |
| - | - | - |
| KOBJMEM | AllocHeapMem(INT iCount) | `iCount` 바이트의 메모리를 힙에 할당합니다. 성공시 메모리 커널 오브젝트를, 실패시 `INVALID_KOBJMEM`을 반환합니다. |
| BOOL | FreeHeapMem(KOBJMEM koMem) | 할당된 힙 메모리를 반환합니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다. |
| BOOL | WriteHeapMem(KOBJMEM koMem, PVOID pBuffer, INT iOffset, INT iCount) | `pBuffer`에 저장된 내용을 힙 메모리의 `iOffset` 바이트의 오프셋에 `iCount` 바이트만큼 씁니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다. |
| BOOL | ReadHeapMem(KOBJMEM koMem, PVOID pBuffer, INT iOffset, INT iCount) | 힙 메모리의 `iOffset` 바이티의 오프셋에 저장된 내용을 `iCount` 바이트만큼 읽어 `pBuffer`에 씁니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다. |

## 파일
| 반환형 | 함수 | 설명 |
| - | - | - |
| KOBJIO | OpenFile(STRING strName, STRING strExt, WORD wOpenMode) | 파일 시스템에서 이름이 `strName`이고 확장자명이 `strExt`인 파일을 엽니다. 성공시 I/O 커널 오브젝트를, 실패시 `INVALID_KOBJIO`를 반환합니다.<br/>`wOpenMode`의 값으로는 다음이 가능합니다.<br/>* `FILE_OPEN_WRITE`<br/>* `FILE_OPEN_READ`
| BOOL | CloseFile(KOBJIO koIo) | 열려있는 파일을 닫고 커널 오브젝트를 반환합니다. 성공시 `TRUE`, 실패시 `FALSE`를 반환합니다.
