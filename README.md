# UnnamedOS16 (uOS16)
## 개요

uOS는 서울시립대학교 컴퓨터과학부의 행사 중 하나인 셈틀제를 위해 제가 제작하고 있는 8086용 운영체제입니다. FAT16과 유사한 파일시스템 구축과 single-tasking operating system 구현을 목표로 하고 있습니다.

## API 문서
[API 문서](API%20docs.md)

## Devlog
Devlog

## 시스템 호출 (추후에 옮겨질 섹션)
초기화하는 내용은 다음과 같습니다.
| 인터럽트 번호 | 핸들러 | 설명 |
| --- | --- | --- |
| 0x20 | process_start | 프로세스를 생성합니다. |
| 0x21 |  |  |
| 0x22 | memory_alloc | 힙 메모리를 할당합니다. |
| 0x23 | memory_free | 힙 메모리를 반환합니다. |
| 0x24 |  |  |
| 0x25 |  |  |
| 0x26 | io_open | 파일을 엽니다. |
| 0x27 | io_close | 파일을 닫습니다. |
| 0x28 | io_write | 파일에 데이터를 씁니다. |
| 0x29 | io_read | 파일로부터 데이터를 읽습니다. |
| 0x2A | create_file | 파일 또는 디렉토리를 생성합니다. |
| 0x2B | delete_file | 파일 또는 디렉토리를 지웁니다. |

## 결과
