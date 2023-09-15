# UnnamedOS (uOS)
## 개요

uOS는 서울시립대학교 컴퓨터과학부의 행사 중 하나인 셈틀제를 위해 제가 제작하고 있는 8086용 운영체제입니다. FAT16과 유사한 파일시스템 구축과 single-tasking operating system 구현을 목표로 하고 있습니다.

## API 문서
[API 문서](API%20docs.md)

## Devlog
Devlog

## 설계

### 부팅
<img src="https://github.com/NatsciT/UnnamedOS/assets/97524957/d8b56368-d401-41a5-b7c0-ca68533c10f4" width="600">

8086은 부팅 가능한 저장매체의 첫 섹터를 물리주소 0x7C00에 적재합니다. 이때, 적재되는 섹터가 `bootloader.asm`입니다. 적재된 코드는 화면을 초기화하고 디스크에서 커널을 읽어 물리주소 0x1000(0100h:0000h)에 적재합니다.

### 시스템 호출
커널은 `syscall_init()` 함수를 통해 IVT의 0x20번째 항목부터를 핸들러로 초기화합니다. 핸들러의 형태는 다음과 같습니다.
```c
return_type handler(int return_cs, int flags, arg0, arg1, ...);
```
초기화하는 내용은 다음과 같습니다.
| 인터럽트 번호 | 핸들러 | 설명 |
| --- | --- | --- |
| 0x20 | process_start | 프로세스를 생성합니다. |
| 0x21 | process_kill | 프로세스를 종료합니다. |
| 0x22 | memory_alloc | 힙 메모리를 할당합니다. |
| 0x23 | memory_free | 힙 메모리를 반환합니다. |
| 0x24 | memory_write | 힙 메모리에 데이터를 씁니다. |
| 0x25 | memory_read | 힙 메모리로부터 데이터를 읽습니다. |
| 0x26 | io_open | 파일을 엽니다. |
| 0x27 | io_close | 파일을 닫습니다. |
| 0x28 | io_write | 파일에 데이터를 씁니다. |
| 0x29 | io_read | 파일로부터 데이터를 읽습니다. |
| 0x2A | create_file | 파일 또는 디렉토리를 생성합니다. |
| 0x2B | delete_file | 파일 또는 디렉토리를 지웁니다. |

### 메모리 관리
커널은 프로세스의 메모리를 3가지로 나눕니다. code와 data 영역 4KiB, stack 영역 2KiB, 그리고 heap 영역 6KiB입니다. heap 영역은 16바이트, 32바이트, 64바이트, 128바이트, 그리고 256바이트 영역으로 다시 나뉩니다.

### 프로세스 관리

## 결과
