# API docs
UOS API의 문서입니다.
## 자료형

## 프로세스

## 메모리
| 반환형 | 함수 | 설명 |
| - | - | - |
| KOBJMEM | AllocHeapMem(INT iCount) | iCount 바이트의 메모리를 힙에 할당합니다. 성공시 메모리 커널 오브젝트를, 실패시 INVALID_KOBJMEM을 반환합니다. |
| BOOL | FreeHeapMem(KOBJMEM komem) | 할당된 힙 메모리를 반환합니다. 성공시 TRUE, 실패시 FALSE를 반환합니다. |
| BOOL | WriteHeapMem(KOBJMEM komem, PVOID pBuffer, INT iOffset, INT iCount) | pBuffer에 저장된 내용을 힙 메모리의 iOffset 바이트의 오프셋에 iCount 바이트만큼 씁니다. 성공시 TRUE, 실패시 FALSE를 반환합니다. |
| BOOL | ReadHeapMem(KOBJ komem, PVOID pBuffer, INT iOffset, INT iCount) | 힙 메모리의 iOffset 바이티의 오프셋에 저장된 내용을 iCount 바이트만큼 읽어 pBuffer에 씁니다. 성공시 TRUE, 실패시 FALSE를 반환합니다. |

## 파일
