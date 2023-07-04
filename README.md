# UnnamedOS (uOS)
## 개요

uOS는 서울시립대학교 컴퓨터과학부의 행사 중 하나인 셈틀제를 위해 제가 제작하고 있는 운영체제입니다. 8086의 16비트 리얼모드에서 구동하며, 라운드 로빈 알고리즘을 통한 CPU 자원 관리, FAT16과 유사한 파일시스템 구축을 목표로 하고 있습니다.

## 설계

### 부팅
<img src="https://github.com/NatsciT/UnnamedOS/assets/97524957/d8b56368-d401-41a5-b7c0-ca68533c10f4" width="600">

8086은 부팅 가능한 저장매체의 첫 섹터를 물리주소 0x7C00에 적재합니다. 이때, 적재되는 섹터가 바로 `bootloader.asm`입니다. 적재된 코드는 화면을 초기화하고 디스크에서 커널을 읽어 물리주소 0x1000(0100h:0000h)에 적재합니다.

### 인터럽트 08H 핸들러
커널이 적재되면 처음으로 `process_init()` 함수를 호출합니다. 이 함수는 PCB 리스트를 초기화하고 스케줄러 함수를 인터럽트 08H의 핸들러로 설정합니다. 여기서 인터럽트 08H는 55밀리초마다 호출되는 타이머 인터럽트입니다. 그러므로 이 인터럽트를 이용해 라운드 로빈 스케줄링을 구현할 수 있습니다.

### 대기 큐
대기 큐는 프로세스 제어 블록(PCB)를 양방향 연결 리스트로 구현했습니다. 메모리를 동적으로 할당할 수 없는 특성상 연결 리스트를 배열로 구현해야 했습니다.

### PCB
```c
typedef struct __pcb_t
{
        int pid;
        char* name;
        pstate_t state;

        int ip;
        // ax, bx, cx, dx
        // bp, sp, di, si
        // eflags
        int registers[9];

        struct __pcb_t* next;
        struct __pcb_t* prev;
} pcb_t;
```
#### `pid`
pid는 프로세스 식별자로서, uOS에서는 대기 큐 배열에서의 인덱스를 pid로 사용했습니다.
#### `name`
프로세스의 이름입니다.
#### `state`
PCB의 상태입니다. `PCB_UNUSED`, `PCB_WAITING`, 그리고 `PCB_RUNNING` 총 세 가지 상태를 가집니다.
#### `ip`
프로세스의 컨텍스트 스위칭 직전의 Instruction Pointer입니다.
#### `registers`
프로세스의 컨텍스트 스위칭 직전의 레지스터 상태입니다.
#### `next`
다음 PCB를 가리킵니다.
#### `prev`
이전 PCB를 가리킵니다.

## 결과
