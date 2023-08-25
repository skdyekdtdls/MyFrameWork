
/*
0. ANIM_ENUM에 해당하는 열거형을 Cline_Enum.h에 정의한다.
1. StateMachine<OWNER, ANIM_ENUM>를 로더에 Create한다(상태 헤더파일도 포함하기).
2. 원하는 상태(스니펫 활용)를 정의하고 Add_State로 StateMachine의 umap에 넣는다.
3. 게임오브젝트에 typedef StateContext<OWNER, ANIM_ENUM> "X + State"로 정의한다.; 
4. 컴포넌트 추가 과정을 거친다.
	4-1. 변수 선언
	4-2. Add_Component에 추가
	4-3. Tick함수 호출
	4-4. Safe_Release
5. Initialize에서 컴포넌트의 TransitonTo()를 호출한다.
*/

#pragma once
#include "StateMachine.h"
