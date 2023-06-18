# UnnamedOS (uOS)
## 개요

uOS는 서울시립대학교 컴퓨터과학부의 행사 중 하나인 셈틀제를 위해 제가 제작하고 있는 운영체제입니다. 8086의 16비트 리얼모드에서 구동하며, 라운드 로빈 알고리즘을 통한 CPU 자원 관리, FAT16과 유사한 파일시스템 구축을 목표로 하고 있습니다.

## 설계 및 제작

### 부팅

8086은 부팅 가능한 저장매체의 첫 섹터를 물리주소 0x7C00에 적재합니다. 적재되는 섹터가 `bootloader.asm`입니다.

### process_init()

## 결과
